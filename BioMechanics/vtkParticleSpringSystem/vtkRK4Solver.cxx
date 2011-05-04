#include "vtkRK4Solver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkParticle.h"
#include "vtkParticleCollection.h"
#include "vtkParticleSpringSystem.h"

vtkCxxRevisionMacro(vtkRK4Solver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkRK4Solver);

//----------------------------------------------------------------------------
vtkRK4Solver::vtkRK4Solver()
{
	this->NumberOfParticles = 0;
	this->dx = vtkDoubleArray::New();
	this->dx->SetNumberOfComponents(3);
	this->dv = vtkDoubleArray::New();
	this->dv->SetNumberOfComponents(3);
	this->dx1  = vtkDoubleArray::New();
	this->dv1  = vtkDoubleArray::New();
	this->dx2  = vtkDoubleArray::New();
	this->dv2  = vtkDoubleArray::New();
	this->dx3  = vtkDoubleArray::New();
	this->dv3  = vtkDoubleArray::New();
	this->dx4  = vtkDoubleArray::New();
	this->dv4  = vtkDoubleArray::New();
}

//----------------------------------------------------------------------------
vtkRK4Solver::~vtkRK4Solver()
{
	if(this->dx) this->dx->Delete();
	if(this->dv) this->dv->Delete();
}

//----------------------------------------------------------------------------
void vtkRK4Solver::Init()
{
	for(int i=0; i< NumberOfParticles; i++)
	{
		this->dx->InsertNextTuple3(0.0, 0.0, 0.0);
		this->dv->InsertNextTuple3(0.0, 0.0, 0.0);
	}
	this->dx1->DeepCopy(this->dx);
	this->dv1->DeepCopy(this->dv);
	this->dx2->DeepCopy(this->dx);
	this->dv2->DeepCopy(this->dv);
	this->dx3->DeepCopy(this->dx);
	this->dv3->DeepCopy(this->dv);
	this->dx4->DeepCopy(this->dx);
	this->dv4->DeepCopy(this->dv);
}

//----------------------------------------------------------------------------
void vtkRK4Solver::Reset()
{
	for(int i=0; i< NumberOfParticles; i++)
	{
		this->dx->SetTuple3(i, 0.0, 0.0, 0.0);
		this->dv->SetTuple3(i, 0.0, 0.0, 0.0);
	}
}


//----------------------------------------------------------------------------
void vtkRK4Solver::ComputeNextStep(vtkParticleCollection * particles, double deltaT)
{
	double dt05 = 0.5*deltaT;
	double dt1_6 = deltaT/6;

	this->Evaluate(particles, this->dx1, this->dv1, dt05);
	this->Evaluate(particles, this->dx2, this->dv2, dt05);
	this->Evaluate(particles, this->dx3, this->dv3, deltaT);
	//Add up
	this->DeformationModel->ComputeForces();
	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);
		double * vel = part->GetVelocity();
		double * pos = part->GetPosition();
		double * acc = part->GetAcceleration();

		this->dv4->SetTuple(id, acc);
		this->dx4->SetTuple(id, vel);

		// Runge Kutta coefficients
		// x(n+1) = xn + (dt/6)*(a+2b+2c+d)
		double * ax = this->dx1->GetTuple(id);
		double * bx = this->dx2->GetTuple(id);
		double * cx = this->dx3->GetTuple(id);
		double * dx = this->dx4->GetTuple(id);
		double * av = this->dv1->GetTuple(id);
		double * bv = this->dv2->GetTuple(id);
		double * cv = this->dv3->GetTuple(id);
		double * dv = this->dv4->GetTuple(id);

		pos[0] += dt1_6 * (ax[0] + 2*bx[0] + 2*cx[0] + dx[0]);
		pos[1] += dt1_6 * (ax[1] + 2*bx[1] + 2*cx[1] + dx[1]);
		pos[2] += dt1_6 * (ax[2] + 2*bx[2] + 2*cx[2] + dx[2]);

		vel[0] += dt1_6 * (av[0] + 2*bv[0] + 2*cv[0] + dv[0]);
		vel[1] += dt1_6 * (av[1] + 2*bv[1] + 2*cv[1] + dv[1]);
		vel[2] += dt1_6 * (av[2] + 2*bv[2] + 2*cv[2] + dv[2]);
	}
}

//----------------------------------------------------------------------------
void vtkRK4Solver::Evaluate(vtkParticleCollection * particles, vtkDoubleArray * dX, vtkDoubleArray * dV, double deltaT)
{
	//Compute Deformation Model Forces
	this->DeformationModel->ComputeForces();
	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);
		double * vel = part->GetVelocity();
		double * pos = part->GetPosition();
		double * acc = part->GetAcceleration();

		dV->SetTuple(id, acc);
		dX->SetTuple(id, vel);

		pos[0] += deltaT*vel[0];
		pos[1] += deltaT*vel[1];
		pos[2] += deltaT*vel[2];

		vel[0] += deltaT*acc[0];
		vel[1] += deltaT*acc[1];
		vel[2] += deltaT*acc[2];
	}
}

//----------------------------------------------------------------------------
void vtkRK4Solver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent << "Id: "  << "\n";
}
