#include "vtkParticleSpringSystem.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkMassProperties.h"
#include "vtkCell.h"
#include "vtkMath.h"

#include "vtkSpring.h"
#include "vtkSpringCollection.h"
#include "vtkParticle.h"
#include "vtkParticleCollection.h"
#include "vtkMotionEquationSolver.h"
#include "vtkVelocityVerletSolver.h"
#include "vtkRK4Solver.h"
#include "vtkEulerSolver.h"

vtkCxxRevisionMacro(vtkParticleSpringSystem, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkParticleSpringSystem);

//----------------------------------------------------------------------------
vtkParticleSpringSystem::vtkParticleSpringSystem()
{
	this->SpringCoefficient = 0.0;
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->DeltaT = 0;
	this->Mass = 0;
	this->Residual = 1e-6;
	this->RigidityFactor = 1;
	this->Gravity[0] = this->Gravity[1] = this->Gravity[2] = 0;
	this->SolverType = vtkMotionEquationSolver::VelocityVerlet;
	this->CollisionIds = NULL;
	this->CollisionDisplacements = NULL;
	this->Particles = NULL;
	this->Springs = NULL;
}

//----------------------------------------------------------------------------
vtkParticleSpringSystem::~vtkParticleSpringSystem()
{
	if(this->Particles) this->Particles->Delete();
	if(this->Springs) this->Springs->Delete();
	if(this->CollisionIds) this->CollisionIds->Delete();
	if(this->CollisionDisplacements) this->CollisionDisplacements->Delete();
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::Init()
{
	vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());
	//Ensure mesh is at its last state
	input->Update();

	//Initialize Particle System
	this->Particles = vtkParticleCollection::New();
	this->Springs = vtkSpringCollection::New();

	for(int id = 0; id < input->GetNumberOfPoints(); id++)
	{
		//One particle per input point
		double * point = input->GetPoint(id);

		vtkParticle * p0 = vtkParticle::New();
		p0->SetId(id);
		p0->SetPosition(point);
		p0->SetMass(this->Mass);
		p0->SetStatus(1);
		this->Particles->InsertNextParticle(p0);
	}

	vtkIdList * cellPointIds = vtkIdList::New();
	vtkIdList * neighborCellIds = vtkIdList::New();
	vtkIdList * neighborPointIds = vtkIdList::New();

	//For each cell get its points
	for(vtkIdType jd=0; jd<input->GetNumberOfCells();jd++)
	{
		vtkIdType cellId = jd;
		//Cell points
		cellPointIds->Reset();
		input->GetCellPoints(cellId, cellPointIds);
		//Structural springs
		//Default 3 points per cell. 3D cells are unsupported
		for(int kd=0;kd<cellPointIds->GetNumberOfIds();kd++)
		{
			//Connect each pair of cell points with a spring
			//In case cells are lines only the first 2 points are processed
			if (input->GetCellType(cellId) == VTK_LINE)
			{
				if (kd>0) continue;
			}
			vtkIdType nextId = kd+1;
			if(nextId == 3) nextId = 0;
			vtkParticle * p0 = this->Particles->GetParticle(cellPointIds->GetId(kd));
			vtkParticle * p1 = this->Particles->GetParticle(cellPointIds->GetId(nextId));

			this->CreateSpring(p0, p1);

			if(this->RigidityFactor > 1)
			{
				//Shearing springs (Rigidity Factor=2)
				input->GetPointCells(p1->GetId(), neighborCellIds);
				for(vtkIdType ld=0;ld<neighborCellIds->GetNumberOfIds();ld++)
				{
					vtkIdType neighborCellId = neighborCellIds->GetId(ld);
					input->GetCellPoints(neighborCellId, neighborPointIds);
					for(vtkIdType md=0;md<neighborPointIds->GetNumberOfIds();md++)
					{
						vtkIdType neighborPointId = neighborPointIds->GetId(md);
						if(neighborPointId != p0->GetId() ||
								neighborPointId != p1->GetId())
						{
							vtkParticle * pn = this->Particles->GetParticle(neighborPointId);
							this->CreateSpring(p0, pn);
						}
					}
				}
			}
		}
	}

	switch(this->SolverType)
	{
	case vtkMotionEquationSolver::Euler:
		this->Solver = vtkEulerSolver::New();
		break;
	case vtkMotionEquationSolver::VelocityVerlet:
		this->Solver = vtkVelocityVerletSolver::New();
		break;
	case vtkMotionEquationSolver::RungeKutta4:
		this->Solver = vtkRK4Solver::New();
		break;
	default:
		break;
	}

	//TODO: Use vtk based linear solvers. http://www.vtk.org/doc/nightly/html/classvtkInitialValueProblemSolver.html
	//Initialize motion equation solver
	this->Solver->SetDeformationModel(this);
	this->Solver->SetNumberOfParticles(this->Particles->GetNumberOfItems());
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
int vtkParticleSpringSystem::RequestData(
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
	this->ComputeContacts();

	// Solve motion equation
	this->Step();

	output->ShallowCopy(input);

	//Update output points
	vtkPoints * points = output->GetPoints();
	//vtkPoints * points = output->GetPoints();
	for (int i=0; i < output->GetNumberOfPoints(); i++)
	{
		vtkParticle * particle = this->Particles->GetParticle(i);
		points->SetPoint(i, particle->GetPosition());
	}

	return 1;
}

void vtkParticleSpringSystem::Step()
{
	this->Solver->ComputeNextStep(this->Particles, this->DeltaT);
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::SetParticleStatus(vtkIdType id, bool status)
{
	this->Particles->GetParticle(id)->SetStatus(status);
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::CreateSpring(vtkParticle * p0, vtkParticle * p1)
{
	vtkSpring * spring = vtkSpring::New();
	spring->SetId(this->Springs->GetNumberOfItems());
	spring->SetDistanceCoefficient(this->DistanceCoefficient);
	spring->SetDampingCoefficient(this->DampingCoefficient);
	spring->SetSpringCoefficient(this->SpringCoefficient);
	spring->SetDeltaT(this->DeltaT);
	spring->InsertNextParticle(p0);
	spring->InsertNextParticle(p1);

	//Check if particles are interconnected already
	if(!p0->ContainsSpring(spring) && !p1->ContainsSpring(spring))
	{
		spring->Init();
		//Link particles to spring
		p0->InsertNextSpring(spring);
		p1->InsertNextSpring(spring);
		//Add to global spring collection
		this->Springs->InsertNextSpring(spring);
	}
	else
	{
		spring->Delete();
	}
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::InsertCollision(vtkIdType id, double * displacement)
{
	this->CollisionIds->InsertNextId(id);
	this->CollisionDisplacements->InsertNextTuple(displacement);
	this->Modified();
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::SetCollisions(vtkIdList * ids, vtkDoubleArray * displacements)
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
void vtkParticleSpringSystem::ComputeContacts()
{
	double position[3];
	//double distance[3];
	//double dNorm, L, ratio;

	if(this->CollisionIds && this->CollisionIds->GetNumberOfIds() != 0)
	{
		for (vtkIdType i = 0; i < this->CollisionIds->GetNumberOfIds(); i++)
		{
			//
			vtkIdType id = this->CollisionIds->GetId(i);
			double * d = this->CollisionDisplacements->GetTuple(i);

			vtkParticle * p = this->Particles->GetParticle(id);

			//Save original position
			p->GetPosition(position);

			//Add contact displacement
			p->AddPosition(d[0], d[1], d[2]);

			//Set particle as contacted
			p->SetContacted(1);
		}

		//Reset contact state
		this->CollisionIds->Reset();
		this->CollisionDisplacements->Reset();
	}
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::ComputeForces()
{
	for(int i=0; i<this->Particles->GetNumberOfItems(); i++)
	{
		vtkParticle * particle = this->Particles->GetParticle(i);
		//Reset Forces
		particle->SetForce(0, 0, 0);

		//Gravitational Force
		double g[3];
		this->GetGravity(g);
		vtkMath::MultiplyScalar(g, particle->GetMass());
		particle->AddForce(g);
	}

	double d[3];
	double v[3];
	double F[3];
	double Fs[3];
	double Fd[3];

	//Process springs
	for(int i=0; i<this->Springs->GetNumberOfItems(); i++)
	{
		vtkSpring * spring = this->Springs->GetSpring(i);
		vtkParticle * p0 = spring->GetParticle(0);
		vtkParticle * p1 = spring->GetParticle(1);
		vtkMath::Subtract(p0->GetPosition(), p1->GetPosition(), d); // d = p[0]-p[1]
		vtkMath::Subtract(p0->GetVelocity(), spring->GetParticle(1)->GetVelocity(), v); // v = v[0]-v[1]
		double L = spring->GetRestLength();
		double dNorm = vtkMath::Norm(d);
		double K = spring->GetSpringCoefficient();
		double damping = spring->GetDampingCoefficient();
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
		p0->AddForce(F[0], F[1], F[2]);
		p1->AddForce(-F[0], -F[1], -F[2]);
	}

	for(int i = 0; i<this->Particles->GetNumberOfItems();i++)
	{
		this->Particles->GetParticle(i)->Update();
	}
}


//----------------------------------------------------------------------------
void vtkParticleSpringSystem::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);

	os << indent << "SpringCoefficient: " << this->SpringCoefficient << endl;
	os << indent << "DistanceCoefficient: " << this->DistanceCoefficient << endl;
	os << indent << "DampingCoefficient: " << this->DampingCoefficient << endl;
	os << indent << "DeltaT: " << this->DeltaT << endl;
	os << indent << "Mass: " << this->Mass << endl;
	os << indent << "Residual: " << this->Residual << endl;
	os << indent << "RigidityFactor: " << this->RigidityFactor << endl;
}
