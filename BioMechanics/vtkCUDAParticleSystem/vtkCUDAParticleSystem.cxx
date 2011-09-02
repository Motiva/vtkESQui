
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkCell.h"
#include "vtkMath.h"

#include "vtkCUDAParticleSystem.h"
#include "vtkCUDAMotionEquationSolver.h"
#include "vtkCUDAEulerSolver.h"
#include "vtkCUDAVelocityVerletSolver.h"
#include "vtkCUDAMidPointSolver.h"
#include "vtkCUDARK4Solver.h"

vtkCxxRevisionMacro(vtkCUDAParticleSystem, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCUDAParticleSystem);

//----------------------------------------------------------------------------
vtkCUDAParticleSystem::vtkCUDAParticleSystem()
{
	this->Initialized = false;
	this->SpringCoefficient = 0.0;
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->DeltaTime = 0;
	this->Mass = 1;
	this->Residual = 1e-6;
	this->Gravity[0] = this->Gravity[1] = this->Gravity[2] = 0;
	this->SolverType = vtkCUDAMotionEquationSolver::Euler;
	this->CollisionIds = NULL;
	this->CollisionDisplacements = NULL;
}

//----------------------------------------------------------------------------
vtkCUDAParticleSystem::~vtkCUDAParticleSystem()
{
	if(this->CollisionIds) this->CollisionIds->Delete();
	if(this->CollisionDisplacements) this->CollisionDisplacements->Delete();

	// delete host arrays
	delete [] this->hPos;
	delete [] this->hVel;
	delete [] this->hAcc;
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::Init()
{
	vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());
	//Ensure mesh is at its last state
	input->Update();

	if(!this->Initialized)
	{
		// Initialize Particle System
		this->NumberOfParticles = input->GetNumberOfPoints();
		this->NumberOfSprings = input->GetNumberOfCells();

		unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

		// Allocate host memory
		this->hPos = new float[this->NumberOfParticles*4];
		this->hVel = new float[this->NumberOfParticles*4];
		this->hAcc = new float[this->NumberOfParticles*4];
		this->hFor = new float[this->NumberOfParticles*4];
		memset(this->hPos, 0 , memSize);
		memset(this->hVel, 0 , memSize);
		memset(this->hAcc, 0 , memSize);
		memset(this->hFor, 0 , memSize);

		this->hIds = new int[this->NumberOfSprings*2];
		memset(this->hIds, 0 , this->NumberOfSprings*2*sizeof(int));
		this->hLength = new float[this->NumberOfSprings];
		memset(this->hLength, 0 , this->NumberOfSprings*sizeof(float));

		// Set particle parameters
		for(int id = 0; id < this->NumberOfParticles; id++)
		{
			double * point = input->GetPoint(id);

			// float array position
			int i = id*4;
			//One particle per input point
			this->hPos[i] = point[0];
			this->hPos[i+1] = point[1];
			this->hPos[i+2] = point[2];
			this->hPos[i+3] = 0.0f;

		}

		// Set spring particle ids
		vtkIdList * cellPointIds = vtkIdList::New();

		//For each cell get its points
		for(vtkIdType jd=0; jd<this->NumberOfSprings;jd++)
		{
			vtkIdType cellId = jd;
			//Cell points
			cellPointIds->Reset();
			input->GetCellPoints(cellId, cellPointIds);

			//Spring particle ids
			this->hIds[2*jd] = cellPointIds->GetId(0);
			this->hIds[2*jd+1] = cellPointIds->GetId(1);

			float * ap0 = GetParticleVector(Position, cellPointIds->GetId(0));
			float * ap1 = GetParticleVector(Position, cellPointIds->GetId(1));
			this->hLength[jd] = sqrt(vtkMath::Distance2BetweenPoints(ap0,ap1));
		}
	}

	//Set system parameters

	this->Initialized = true;

	switch(this->SolverType)
	{
	case vtkCUDAMotionEquationSolver::Euler:
		this->Solver = vtkCUDAEulerSolver::New();
		break;
	case vtkCUDAMotionEquationSolver::VelocityVerlet:
		this->Solver = vtkCUDAVelocityVerletSolver::New();
		break;
	case vtkCUDAMotionEquationSolver::MidPoint:
		this->Solver = vtkCUDAMidPointSolver::New();
		break;
	case vtkCUDAMotionEquationSolver::RungeKutta4:
		this->Solver = vtkCUDARK4Solver::New();
		break;
	default:
		break;
	}

	//Initialize motion equation solver
	this->Solver->SetDeformationModel(this);
	this->Solver->SetNumberOfParticles(this->NumberOfParticles);
	this->Solver->SetDeltaTime(this->DeltaTime);
	this->Solver->SetResidual(this->Residual);
	this->Solver->Init();

	//Initialize contact objects
	this->CollisionIds = vtkIdList::New();
	this->CollisionDisplacements = vtkDoubleArray::New();
	this->CollisionDisplacements->SetNumberOfComponents(3);

	//Raise update event
	this->Modified();
}

//----------------------------------------------------------------------------
// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkCUDAParticleSystem::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));


	//Compute Collisions
	this->ComputeCollisions();

	this->Solver->ComputeNextStep(this->hPos, this->hVel, this->hAcc);

	//DisplayParticleVectors();

	output->DeepCopy(input);

	//Update output points
	vtkPoints * points = output->GetPoints();

	for (int i=0; i < output->GetNumberOfPoints(); i++)
	{
		float *p = GetParticleVector(Position, i);
		points->SetPoint(i, p[0], p[1], p[2]);
	}

	return 1;
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::InsertCollision(vtkIdType id, double * displacement)
{
	this->CollisionIds->InsertNextId(id);
	this->CollisionDisplacements->InsertNextTuple(displacement);
	this->Modified();
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::SetCollisions(vtkIdList * ids, vtkDoubleArray * displacements)
{
	if(ids->GetNumberOfIds() != displacements->GetNumberOfTuples())
	{
		vtkDebugMacro("Not the same number of contact ids and displacements")
												return;
	}

	this->CollisionIds->Reset();
	this->CollisionDisplacements->Reset();

	this->CollisionIds->DeepCopy(ids);
	this->CollisionDisplacements->DeepCopy(displacements);

	this->Modified();
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::ComputeCollisions()
{
	if(this->CollisionIds && this->CollisionIds->GetNumberOfIds() != 0)
	{
		for (vtkIdType i = 0; i < this->CollisionIds->GetNumberOfIds(); i++)
		{
			//
			vtkIdType id = this->CollisionIds->GetId(i);
			double * d = this->CollisionDisplacements->GetTuple(i);

			// Update particle position
			float * p = GetParticleVector(Position, id);
			p[0] += d[0];
			p[1] += d[1];
			p[2] += d[2];
			//Set particle as contacted
			p[3] = 1;
		}

		//Reset contact state
		this->CollisionIds->Reset();
		this->CollisionDisplacements->Reset();
	}
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::ComputeForces()
{
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

	//Reset Forces
	memset(this->hFor, 0, memSize);

	for(int i=0; i<this->NumberOfParticles; i++)
	{
		float * p = GetParticleVector(Force, i);
		//Gravitational Force
		p[0] = this->Gravity[0];
		p[1] = this->Gravity[1];
		p[2] = this->Gravity[2];
	}

	float d[3];
	float v[3];
	float F[3];
	float Fs[3];
	float Fd[3];

	//Process springs
	for(int i=0; i<this->NumberOfSprings; i++)
	{
		//Spring particle ids
		vtkIdType p0 = this->hIds[2*i];
		vtkIdType p1 = this->hIds[2*i+1];

		float * pp0 = GetParticleVector(Position, p0);
		float * pp1 = GetParticleVector(Position, p1);
		float * pv0 = GetParticleVector(Velocity, p0);
		float * pv1 = GetParticleVector(Velocity, p1);
		float * pf0 = GetParticleVector(Force, p0);
		float * pf1 = GetParticleVector(Force, p1);

		//Obtain distance between particles
		d[0] = pp0[0] - pp1[0];
		d[1] = pp0[1] - pp1[1];
		d[2] = pp0[2] - pp1[2];

		//Obtain velocity difference between particles
		v[0] = pv0[0] - pv1[0];
		v[1] = pv0[1] - pv1[1];
		v[2] = pv0[2] - pv1[2];

		float L = this->hLength[i];
		double dNorm = vtkMath::Norm(d);
		double K = this->SpringCoefficient;
		double damping = this->DampingCoefficient;
		double Ad = (dNorm-L);

		// Measure Spring/Damping Force
		for(int j=0; j<3; j++)
		{
			double dD = (d[j]/dNorm);
			//Spring Force
			Fs[j] = -(K*Ad)*dD; //OK
			//Damping Force
			Fd[j] = -(damping*(v[j]*d[j])/dNorm)*dD;//OK

			//Add forces
			F[j] = Fs[j] + Fd[j];
		}

		//Add up all forces
		pf0[0] += F[0];
		pf0[1] += F[1];
		pf0[2] += F[2];
		pf1[0] += -F[0];
		pf1[1] += -F[1];
		pf1[2] += -F[2];
	}

	//Update particle acceleration
	float iMass = 1/this->Mass;
	for(int i = 0; i<this->NumberOfParticles;i++)
	{
		float *a = GetParticleVector(Acceleration, i);
		float *f = GetParticleVector(Force, i);
		a[0] = f[0]*iMass;
		a[1] = f[1]*iMass;
		a[2] = f[2]*iMass;
	}
}

//----------------------------------------------------------------------------
float * vtkCUDAParticleSystem::GetParticleVector(VectorType t, vtkIdType id)
{
	int dim = 4;
	int p = dim *id;
	switch(t)
	{
	case Position:
		return &this->hPos[p];
	case Velocity:
		return &this->hVel[p];
	case Acceleration:
		return &this->hAcc[p];
	case Force:
		return &this->hFor[p];
	}
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::DisplayParticleVectors()
{
	std::cout << "----------------------------------------------------------------------\n";
	for (int i=0; i<this->NumberOfParticles; i++){
		float *p = GetParticleVector(Position, i);
		float *v = GetParticleVector(Velocity, i);
		float *a = GetParticleVector(Acceleration, i);
		float *f = GetParticleVector(Force, i);
		std::cout <<"Id: " << i << "\tp:"<< p[0] << "," << p[1] << "," << p[2] <<
				"\tv: "<< v[0] << "," << v[1] << "," << v[2] <<
				"\n\ta:"<< a[0] << "," << a[1] << "," << a[2] <<
				"\tf:"<< f[0] << "," << f[1] << "," << f[2] << endl;
	}
}

//----------------------------------------------------------------------------
void vtkCUDAParticleSystem::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);

	os << indent << "NumberOfParticles: " << this->NumberOfParticles << endl;
	os << indent << "NumberOfSprings: " << this->NumberOfSprings << endl;
	os << indent << "SpringCoefficient: " << this->SpringCoefficient << endl;
	os << indent << "DistanceCoefficient: " << this->DistanceCoefficient << endl;
	os << indent << "DampingCoefficient: " << this->DampingCoefficient << endl;
	os << indent << "DeltaT: " << this->DeltaTime << endl;
	os << indent << "Mass: " << this->Mass << endl;
	os << indent << "Residual: " << this->Residual << endl;
}
