#include "vtkEulerSolver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkParticle.h"
#include "vtkParticleCollection.h"

vtkCxxRevisionMacro(vtkEulerSolver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkEulerSolver);

//----------------------------------------------------------------------------
vtkEulerSolver::vtkEulerSolver()
{
}

//----------------------------------------------------------------------------
vtkEulerSolver::~vtkEulerSolver()
{
}


//----------------------------------------------------------------------------
void vtkEulerSolver::Init()
{
	for(int i=0; i< NumberOfParticles; i++)
	{
		this->dv->InsertNextTuple3(0.0, 0.0, 0.0);
	}
}

//----------------------------------------------------------------------------
void vtkEulerSolver::Reset()
{
}

//----------------------------------------------------------------------------
void vtkEulerSolver::ComputeNextStep(vtkParticleCollection * particles, double dt)
{

	this->DeformationModel->ComputeForces();

	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);
		double * vel = part->GetVelocity();
		double * pos = part->GetPosition();
		double * dvi = part->GetAcceleration();

		//Vn+1 = Vn + An*dt
		vel[0] += dt*dvi[0];
		vel[1] += dt*dvi[1];
		vel[2] += dt*dvi[2];

		//Xn+1 = Xn + Vn+1*dt
		pos[0] += dt*vel[0];
		pos[1] += dt*vel[1];
		pos[2] += dt*vel[2];

		this->dv->SetTuple(id, dvi);
	}
}

//----------------------------------------------------------------------------
void vtkEulerSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
