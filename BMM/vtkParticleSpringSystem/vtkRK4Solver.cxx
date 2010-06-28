#include "vtkRK4Solver.h"

#include "vtkObjectFactory.h"

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
	this->Reset();

	/*double deltaT_05 = 0.5*deltaT;
	for(int id=0; id<particles->GetNumberOfItems(); id++)
	{
		vtkParticle * part = particles->GetParticle(id);

		this->Evaluate(particle, this->dX->GetTuple3(id), this->dV->GetTuple3(id), deltaT_05);
		this->Evaluate(particle, this->dX1->GetTuple3(id), this->dV1->GetTuple3(id), deltaT_05);
		this->Evaluate(particle, this->dX2->GetTuple3(id), this->dV2->GetTuple3(id), deltaT_05);
		this->Evaluate(particle, this->dX3->GetTuple3(id), this->dV3->GetTuple3(id), deltaT);

	}*/
}

//----------------------------------------------------------------------------
void vtkRK4Solver::Evaluate(vtkParticle * p, double * dX, double * dV, double deltaT)
{
	//Compute Deformation Model Forces
	this->DeformationModel->ComputeForces();
	double pos[3];
	double vel[3];
	p->GetPosition(pos);
	p->GetVelocity(vel);
	for(int j=0;j<2;j++)
	{
		pos[j] += dX[j]*deltaT;
		vel[j] += dV[j]*deltaT;
	}
	dX[0] = vel[0];
	dX[1] = vel[1];
	dX[2] = vel[2];
	dV[0] = vel[0];
	dV[1] = vel[1];
	dV[2] = vel[2];
}

//----------------------------------------------------------------------------
void vtkRK4Solver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent << "Id: "  << "\n";
}
