#include "vtkParticleSpringSystem.h"

#include "vtkObjectFactory.h"

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
	this->RigidityFactor = 0;
	this->Volume = 0;
	this->SystemProperties = NULL;
	this->SolverType = vtkParticleSpringSystem::VelocityVerlet;
	this->ContactIds = NULL;
	this->ContactDisplacements = NULL;
}

//----------------------------------------------------------------------------
vtkParticleSpringSystem::~vtkParticleSpringSystem()
{
	this->Particles->Delete();
	this->Springs->Delete();
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

	//output->DeepCopy(input);

	//Compute Collisions
	this->ComputeContacts();

	// Solve motion equation
	this->Step();

	//Update output points
	vtkPoints * points = input->GetPoints();
	//vtkPoints * points = output->GetPoints();
	for (int i=0; i < input->GetNumberOfPoints(); i++)
	{
		vtkParticle * particle = this->Particles->GetParticle(i);
		points->SetPoint(i, particle->GetPosition());
	}

	output->ShallowCopy(input);

	//Compute System properties
	//TODO: Check why input is not updated on each step (volume computation)
	vtkPolyData * surface = vtkPolyData::New();
	surface->ShallowCopy(output);

	this->SystemProperties->SetInput(surface);
	this->Volume = this->SystemProperties->GetVolume();

	surface->Delete();
	return 1;
}

void vtkParticleSpringSystem::Step()
{
	this->Solver->ComputeNextStep(this->Particles, this->DeltaT);
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::Init()
{
	vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());
	vtkPolyData * mesh = input;

	//Initialize Particle System
	this->Particles = vtkParticleCollection::New();
	this->Springs = vtkSpringCollection::New();
	this->Links = vtkCellLinks::New();

	for(int id = 0; id < mesh->GetNumberOfPoints(); id++)
	{
		//One particle per mesh point
		double * point = mesh->GetPoint(id);

		vtkParticle * p0 = vtkParticle::New();
		p0->SetId(id);
		p0->SetPosition(point);
		p0->SetMass(this->Mass);
		this->Particles->InsertNextParticle(p0);

		//Create one link entry per particle
		this->Links->InsertNextPoint(0);
	}
	cout << "Total number of particles: " << this->Particles->GetNumberOfItems() << endl;

	vtkIdList * cellPointIds = vtkIdList::New();
	vtkIdList * neighborCellIds = vtkIdList::New();
	vtkIdList * neighborPointIds = vtkIdList::New();

	//For each cell get its points
	for(vtkIdType jd=0; jd<mesh->GetNumberOfCells();jd++)
	{
		vtkIdType cellId = jd;
		//Cell points
		mesh->GetCellPoints(cellId, cellPointIds);

		//Structural springs
		//Default 3 points per cell. 3D cells are unsupported
		for(int kd=0;kd<cellPointIds->GetNumberOfIds();kd++)
		{
			//Connect each pair of cell points with a spring
			//In case cells are lines only the first 2 points are processed
			if (mesh->GetCellType(cellId) == VTK_LINE)
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
				mesh->GetPointCells(p1->GetId(), neighborCellIds);
				for(vtkIdType ld=0;ld<neighborCellIds->GetNumberOfIds();ld++)
				{
					vtkIdType neighborCellId = neighborCellIds->GetId(ld);
					mesh->GetCellPoints(neighborCellId, neighborPointIds);
					//cout << "["<< p0->GetId() << "]["<< p1->GetId() << "] neighborPoint: " ;
					for(vtkIdType md=0;md<neighborPointIds->GetNumberOfIds();md++)
					{
						vtkIdType neighborPointId = neighborPointIds->GetId(md);
						//cout << neighborPointIds->GetId(md) << ", ";
						if(neighborPointId != p0->GetId() ||
							neighborPointId != p1->GetId())
						{
							p1 = this->Particles->GetParticle(neighborPointId);
							this->CreateSpring(p0, p1);
						}
					}
					//cout << endl;
				}

			}
		}
	}
	cout << "Total number of springs: " << this->Springs->GetNumberOfItems() << endl;

	switch(this->SolverType)
	{
	case Euler:
		this->Solver = vtkEulerSolver::New();
		cout << "Euler solver selected\n";
		break;
	case VelocityVerlet:
		this->Solver = vtkVelocityVerletSolver::New();
		cout << "Velocity verlet solver selected\n";
		break;
	case RungeKutta4:
		this->Solver = vtkRK4Solver::New();
		cout << "Runge-Kutta45 solver selected\n";
		break;
	default:
		break;
	}

	//System Properties
	this->SystemProperties = vtkMassProperties::New();

	//Initialize motion equation solver
	this->Solver->SetDeformationModel(this);
	this->Solver->SetNumberOfParticles(this->Particles->GetNumberOfItems());
	this->Solver->Init();

	//Initialize contact objects
	this->ContactIds = vtkIdList::New();
	this->ContactDisplacements = vtkDoubleArray::New();
	this->ContactDisplacements->SetNumberOfComponents(3);

	//Raise update event
	this->Modified();

	cout << this->GetClassName() << " has been init...\n";
}

void vtkParticleSpringSystem::SetParticleStatus(vtkIdType id, bool status)
{
	this->Particles->GetParticle(id)->SetFixed(status);
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
	if(!this->Springs->ContainsSpring(spring))
	{
		spring->Init();
		this->Springs->InsertNextSpring(spring);
		this->Links->ResizeCellList(p0->GetId(),1);
		this->Links->InsertNextCellReference(p0->GetId(),spring->GetId());
		this->Links->ResizeCellList(p1->GetId(),1);
		this->Links->InsertNextCellReference(p1->GetId(),spring->GetId());
	}
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::SetContacts(vtkIdList * ids, vtkDoubleArray * directions)
{
	this->ContactIds->DeepCopy(ids);
	this->ContactDisplacements->DeepCopy(directions);
	this->Modified();
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::ComputeContacts()
{
	double position[3];
	double distance[3];
	double dNorm, L, ratio;

	if(this->ContactIds && this->ContactDisplacements)
	{
		for (vtkIdType i = 0; i < this->ContactIds->GetNumberOfIds(); i++)
		{
			//
			vtkIdType id = this->ContactIds->GetId(i);
			double * d = this->ContactDisplacements->GetTuple3(i);

			vtkParticle * p = this->Particles->GetParticle(id);

			//Distance Coefficient constraint
			//Save original position
			p->GetPosition(position);
			//p->Print(cout);
			//Add contact displacement
			p->AddPosition(d[0], d[1], d[2]);

			//Retrieve springs containing this particle
			vtkIdType * links = this->Links->GetCells(id);
			for(int j = 0; j < this->Links->GetNcells(id); j++)
			{
				//Retrieve pointer component -> spring Id
				vtkIdType springId = *links;

				//Springs containing particle
				vtkSpring * spring = this->Springs->GetSpring(springId);
				vtkParticle * p0 = spring->GetParticle(0);
				vtkParticle * p1 = spring->GetParticle(1);
				//spring->Print(cout);

				vtkMath::Subtract(p0->GetPosition(), p1->GetPosition(), distance); // distance = p[0]-p[1]
				dNorm = vtkMath::Norm(distance);
				L = spring->GetRestLength();
				ratio = abs(100*((L-dNorm)/L));
				/*while(ratio > spring->GetDistanceCoefficient())
				{
					double factor = (1/ratio)*100;
					p->AddPosition(-d[0]/factor, -d[1]/factor, -d[2]/factor);
					//p->Print(cout);
					vtkMath::Subtract(p0->GetPosition(), p1->GetPosition(), distance); // distance = p[0]-p[1]
					dNorm = vtkMath::Norm(distance);
					L = spring->GetRestLength();
					ratio = 100*((dNorm-L)/L);
				}*/
				//Increment link pointer
				links += 1;
			}
			p->SetContacted(1);
		}

		//Reset contact state
		this->ContactIds->Reset();
		this->ContactDisplacements->Reset();
	}
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::ComputeForces()
{
	double drag[3];

	for(int i=0; i<this->Particles->GetNumberOfItems(); i++)
	{
		vtkParticle * particle = this->Particles->GetParticle(i);
		//Reset Forces
		particle->SetForce(0, 0, 0);

		//Gravitational Force
		//TODO: Obtain Gravity from simulation
		//FIXME: Reset gravity force
		/*double gravity = -9.0;
		double gf = gravity*particle->GetMass();
		particle->AddForce(0, gf, 0);*/

		//TODO: Add Viscous drag force
		//Viscous Drag
		particle->GetVelocity(drag);
		vtkMath::Normalize(drag);
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
		//TODO: Implement Internal Pressure Force
		//Sum up all forces
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
}
