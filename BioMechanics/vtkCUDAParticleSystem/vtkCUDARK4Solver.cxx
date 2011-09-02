#include "vtkCUDARK4Solver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkCUDAParticleSystem.h"
#include "vtkCUDAParticleSystem.cuh"

vtkCxxRevisionMacro(vtkCUDARK4Solver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCUDARK4Solver);

//----------------------------------------------------------------------------
vtkCUDARK4Solver::vtkCUDARK4Solver()
{
	this->dx1 = NULL;
	this->dv1 = NULL;
	this->dx2 = NULL;
	this->dv2 = NULL;
	this->dx3 = NULL;
	this->dv4 = NULL;
	this->dx4 = NULL;
	this->dv4 = NULL;
}

//----------------------------------------------------------------------------
vtkCUDARK4Solver::~vtkCUDARK4Solver()
{
	//Free CUDA arrays
	freeArray(this->dPos);
	freeArray(this->dVel);
	freeArray(this->dAcc);

	//Delete derivative arrays
	delete [] this->dx1;
	delete [] this->dv1;
	delete [] this->dx2;
	delete [] this->dv2;
	delete [] this->dx3;
	delete [] this->dv3;
	delete [] this->dx4;
	delete [] this->dv4;
}

//----------------------------------------------------------------------------
void vtkCUDARK4Solver::Init()
{
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

	//Initialize CUDA device
	cudaInit();

	//Allocate device vectors memory
	allocateArray((void **)&this->dPos, memSize);
	allocateArray((void **)&this->dVel, memSize);
	allocateArray((void **)&this->dAcc, memSize);

	//Allocate derivative vector memory
	this->dx1 = new float[this->NumberOfParticles*4];
	this->dv1 = new float[this->NumberOfParticles*4];
	this->dx2 = new float[this->NumberOfParticles*4];
	this->dv2 = new float[this->NumberOfParticles*4];
	this->dx3 = new float[this->NumberOfParticles*4];
	this->dv3 = new float[this->NumberOfParticles*4];
	this->dx4 = new float[this->NumberOfParticles*4];
	this->dv4 = new float[this->NumberOfParticles*4];
	memset(this->dx1, 0 , memSize);
	memset(this->dv1, 0 , memSize);
	memset(this->dx2, 0 , memSize);
	memset(this->dv2, 0 , memSize);
	memset(this->dx3, 0 , memSize);
	memset(this->dv3, 0 , memSize);
	memset(this->dx4, 0 , memSize);
	memset(this->dv4, 0 , memSize);

}

//----------------------------------------------------------------------------
void vtkCUDARK4Solver::ComputeNextStep(float *p, float *v, float *a)
{
	double dt05 = this->DeltaTime*0.5;
	double dt1_6 = this->DeltaTime/6;
	double dt = this->DeltaTime;

	this->Evaluate(p, v, a, dt05, 1);
	this->Evaluate(p, v, a, dt05, 2);
	this->Evaluate(p, v, a, dt, 3);
	//Add up
	this->DeformationModel->ComputeForces();

	for(int i=0; i<this->NumberOfParticles; i++)
	{
		int id = 4*i;
		// dx4 = v & dv4 = a
		// Runge Kutta coefficients
		// Xn+1 = Xn + (dt/6)*(dx1+2*dx2+2*dx3+dx4)
		// Vn+1 = Vn + (dt/6)*(dv1+2*dv2+2*dv3+dv4)

		p[id] += dt1_6 * (dx1[id] + 2*dx2[id] + 2*dx3[id] + v[id]);
		p[id+1] += dt1_6 * (dx1[id+1] + 2*dx2[id+1] + 2*dx3[id+1] + v[id+1]);
		p[id+2] += dt1_6 * (dx1[id+2] + 2*dx2[id+2] + 2*dx3[id+2] + v[id+2]);

		v[id] += dt1_6 * (dv1[id] + 2*dv2[id] + 2*dv3[id] + a[id]);
		v[id+1] += dt1_6 * (dv1[id+1] + 2*dv2[id+1] + 2*dv3[id+1] + a[id+1]);
		v[id+2] += dt1_6 * (dv1[id+2] + 2*dv2[id+2] + 2*dv3[id+2] + a[id+2]);
	}
}

//----------------------------------------------------------------------------
void vtkCUDARK4Solver::Evaluate(float *p, float *v, float *a, double dt, int order)
{
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;
	float * dx;
	float * dv;

	//f(t,x)
	this->DeformationModel->ComputeForces();

	switch(order)
	{
	case 1:
		dx = this->dx1;
		dv = this->dv1;
		break;
	case 2:
		dx = this->dx2;
		dv = this->dv2;
		break;
	case 3:
		dx = this->dx3;
		dv = this->dv3;
		break;
	case 4:
		dx = this->dx4;
		dv = this->dv4;
		break;
	}

	memcpy(dv, a, memSize);
	memcpy(dx, v, memSize);

	// Copy host -> device
	copyArrayToDevice(this->dPos, p, 0, memSize);
	copyArrayToDevice(this->dVel, v, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	//CUDA procedure
	//Xn+1 = Xn + dt*Vn + dt2*An
	integrateSystem(this->dVel, this->dAcc, dt, this->NumberOfParticles);
	integrateSystem(this->dPos, this->dVel, dt, this->NumberOfParticles);

	// Copy Device -> host
	copyArrayFromDevice(p, this->dPos, 0, memSize);
	copyArrayFromDevice(v, this->dVel, 0, memSize);

}

//----------------------------------------------------------------------------
void vtkCUDARK4Solver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
