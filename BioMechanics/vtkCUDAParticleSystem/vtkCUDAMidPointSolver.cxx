#include "vtkCUDAMidPointSolver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkCUDAParticleSystem.h"
#include "vtkCUDAParticleSystem.cuh"

vtkCxxRevisionMacro(vtkCUDAMidPointSolver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCUDAMidPointSolver);

//----------------------------------------------------------------------------
vtkCUDAMidPointSolver::vtkCUDAMidPointSolver()
{
}

//----------------------------------------------------------------------------
vtkCUDAMidPointSolver::~vtkCUDAMidPointSolver()
{
	//Free CUDA arrays
	freeArray(this->dPos);
	freeArray(this->dVel);
	freeArray(this->dAcc);
}

//----------------------------------------------------------------------------
void vtkCUDAMidPointSolver::Init()
{
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

	//Initialize CUDA device
	cudaInit();

	//Allocate device vectors memory
	allocateArray((void **)&this->dPos, memSize);
	allocateArray((void **)&this->dVel, memSize);
	allocateArray((void **)&this->dAcc, memSize);

	//Allocate derivative vector memory
	this->p1 = new float[this->NumberOfParticles*4];
	this->v1 = new float[this->NumberOfParticles*4];
	memset(this->p1, 0 , memSize);
	memset(this->v1, 0 , memSize);

}

//----------------------------------------------------------------------------
void vtkCUDAMidPointSolver::ComputeNextStep(float *p, float *v, float *a)
{
	double dt05 = this->DeltaTime*0.5;
	double dt = this->DeltaTime;

	// MidPoint Equations
	// * Xn+1 = Xn + K2x ; K1x = Vn*dt, K2x = K1x + 0.5*K1v, K1v = An*dt ;
	// * Xn+1 = Xn + Vn*dt * 0.5*An*dt²
	// * Vn+1 = Vn + K2v ; K2v = An+0.5 * dt

	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

	memcpy(p1, p, memSize);
	memcpy(v1, v, memSize);

	// Copy host -> device
	copyArrayToDevice(this->dPos, p, 0, memSize);
	copyArrayToDevice(this->dVel, v, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	//CUDA procedure
	//Full Euler step
	integrateSystem(this->dVel, this->dAcc, dt05, this->NumberOfParticles);
	integrateSystem(this->dPos, this->dVel, dt, this->NumberOfParticles);

	// Copy Device -> host
	//copyArrayFromDevice(v, this->dVel, 0, memSize);//Vn+0.5 = Vn + 0.5*dt
	copyArrayFromDevice(p, this->dPos, 0, memSize);//Xn+1 = Xn + Vn*dt + 0.5*An*dt²

	//midpoint
	this->DeformationModel->ComputeForces();
	// Copy host -> device
	copyArrayToDevice(this->dPos, p1, 0, memSize);
	copyArrayToDevice(this->dVel, v1, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	//Vn+1 = Vn + dt*An+1
	integrateSystem(this->dVel, this->dAcc, dt05, this->NumberOfParticles);
	integrateSystem(this->dPos, this->dVel, dt05, this->NumberOfParticles);

	// Copy Device -> host
	copyArrayFromDevice(v, this->dVel, 0, memSize);

}

//----------------------------------------------------------------------------
void vtkCUDAMidPointSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
