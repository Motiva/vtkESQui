#include "vtkParticleSpringSystem.h"

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
  this->Initialized = 0;
  this->Spring = 0.0;
  this->Distance = 1;
  this->Damping = 0;
  this->TimeStep = 0;
  this->Mass = 0;
  this->Residual = 1e-6;
  this->Gravity[0] = this->Gravity[1] = this->Gravity[2] = 0;
  this->SolverType = vtkMotionEquationSolver::VelocityVerlet;
  this->Particles = NULL;
  this->Springs = NULL;
}

//----------------------------------------------------------------------------
vtkParticleSpringSystem::~vtkParticleSpringSystem()
{
  if(this->Particles) this->Particles->Delete();
  if(this->Springs) this->Springs->Delete();
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::Initialize()
{
  vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());
  //Ensure mesh is at its last state
  //input->Update();

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

  //For each cell get its points
  for(vtkIdType jd=0; jd<input->GetNumberOfCells();jd++)
  {
    vtkIdType cellId = jd;
    //Cell points
    cellPointIds->Reset();
    input->GetCellPoints(cellId, cellPointIds);
    //Connect each pair of cell points with a spring
    //Input cells must be of VTK_LINE type
    if (input->GetCellType(cellId) == VTK_LINE)
    {
      vtkParticle * p0 = this->Particles->GetParticle(cellPointIds->GetId(0));
      vtkParticle * p1 = this->Particles->GetParticle(cellPointIds->GetId(1));
      this->CreateSpring(p0, p1);
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
  this->Solver->Initialize();

  this->Initialized = 1;

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

  //Initialize System
  if(!this->Initialized) this->Initialize();

  //TODO: Define ROI to refine deformation to a local area

  // Solve motion equation
  this->Solver->ComputeNextStep(this->Particles, this->TimeStep);

  output->ShallowCopy(input);

  //Update output points
  //TODO: Use vtkWarpVector to apply deformation
  vtkPoints * points = output->GetPoints();
  for (int i=0; i < output->GetNumberOfPoints(); i++)
  {
    vtkParticle * particle = this->Particles->GetParticle(i);
    points->SetPoint(i, particle->GetPosition());
  }

  return 1;
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
  spring->SetDistance(this->Distance);
  spring->SetDamping(this->Damping);
  spring->SetK(this->Spring);
  spring->SetTimeStep(this->TimeStep);
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
void vtkParticleSpringSystem::InsertDisplacement(vtkIdType id, double x, double y, double z)
{
  vtkParticle * p = this->Particles->GetParticle(id);

  //Add contact displacement
  p->AddPosition(x, y, z);

  //Set particle as contacted
  p->SetContacted(1);
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::InsertDisplacement(vtkIdType id, double * d)
{
  this->InsertDisplacement(id, d[0], d[1], d[2]);
}

//----------------------------------------------------------------------------
void vtkParticleSpringSystem::SetDisplacements(vtkIdList * ids, vtkDoubleArray * displacements)
{
  if(ids->GetNumberOfIds() != displacements->GetNumberOfTuples())
  {
    vtkDebugMacro("Not the same number of contact ids and displacements")
        return;
  }

  for(int i=0; i<ids->GetNumberOfIds(); i++){
    this->InsertDisplacement(ids->GetId(i), displacements->GetTuple3(i));
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
    vtkMath::Subtract(p0->GetPosition(), p1->GetPosition(), d); // d = (p0[0]-p1[0], p0[1]-p1[1], p0[2]-p1[2])
    vtkMath::Subtract(p0->GetVelocity(), spring->GetParticle(1)->GetVelocity(), v); // v = (v0[0]-v1[0], v0[1]-v1[1], v0[2]-v1[2])
    double L = spring->GetRestLength();
    double dNorm = vtkMath::Norm(d);
    double damping = spring->GetDamping();
    double Ad = (dNorm-L);

    //Dynamic Stiffness (Hyperelasticity)
    double K = spring->GetK();
    double r = dNorm/L;
    if(r > this->Distance)
      K *= r;

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
    vtkParticle * p = this->Particles->GetParticle(i);
    p->Update();
    p->SetContacted(0);
  }
}


//----------------------------------------------------------------------------
void vtkParticleSpringSystem::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "NumberOfParticles: " << this->Particles->GetNumberOfParticles() << endl;
  os << indent << "NumberOfSprings: " << this->Springs->GetNumberOfItems() << endl;
  os << indent << "SpringCoefficient: " << this->Spring << endl;
  os << indent << "DistanceCoefficient: " << this->Distance << endl;
  os << indent << "DampingCoefficient: " << this->Damping << endl;
  os << indent << "DeltaT: " << this->TimeStep << endl;
  os << indent << "Mass: " << this->Mass << endl;
  os << indent << "Residual: " << this->Residual << endl;
}
