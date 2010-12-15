#include "vtkVelocityVerletSolver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkParticle.h"
#include "vtkParticleCollection.h"

vtkCxxRevisionMacro(vtkVelocityVerletSolver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkVelocityVerletSolver);

//----------------------------------------------------------------------------
vtkVelocityVerletSolver::vtkVelocityVerletSolver()
{
}

//----------------------------------------------------------------------------
vtkVelocityVerletSolver::~vtkVelocityVerletSolver()
{
}


//----------------------------------------------------------------------------
void vtkVelocityVerletSolver::Init()
{
	for(int i=0; i< NumberOfParticles; i++)
	{
		this->dv->InsertNextTuple3(0.0, 0.0, 0.0);
	}
}

//----------------------------------------------------------------------------
void vtkVelocityVerletSolver::Reset()
{
}

//----------------------------------------------------------------------------
void vtkVelocityVerletSolver::ComputeNextStep(vtkParticleCollection * particles, double dt)
{

	double dt05 = 0.5*dt;

	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);
		double * vel = part->GetVelocity();
		double * pos = part->GetPosition();
		double * dvi = this->dv->GetTuple(id);

		vel[0] += dt05 * dvi[0];
		vel[1] += dt05 * dvi[1];
		vel[2] += dt05 * dvi[2];

		//this->v1_2->SetTuple(id, vel);

		pos[0] += dt*vel[0];
		pos[1] += dt*vel[1];
		pos[2] += dt*vel[2];

	}

	this->DeformationModel->ComputeForces();

	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);
		double * vel = part->GetVelocity();
		//double * dvi = this->dv->GetTuple(id);
		double * dvi = part->GetAcceleration();
		vel[0] += dt05 * dvi[0];
		vel[1] += dt05 * dvi[1];
		vel[2] += dt05 * dvi[2];
		this->dv->SetTuple(id, dvi);
	}

}

//----------------------------------------------------------------------------
void vtkVelocityVerletSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
