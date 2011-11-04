#include "vtkCUDAVelocityVerletSolver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkCUDAParticleSystem.h"
#include "vtkCUDAParticleSystem.cuh"

vtkCxxRevisionMacro(vtkCUDAVelocityVerletSolver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCUDAVelocityVerletSolver);

//----------------------------------------------------------------------------
vtkCUDAVelocityVerletSolver::vtkCUDAVelocityVerletSolver()
{
}

//----------------------------------------------------------------------------
vtkCUDAVelocityVerletSolver::~vtkCUDAVelocityVerletSolver()
{
	//Free CUDA arrays
	freeArray(this->dPos);
	freeArray(this->dVel);
	freeArray(this->dAcc);
}

//----------------------------------------------------------------------------
void vtkCUDAVelocityVerletSolver::Init()
{
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;

	//Initialize CUDA device
	cudaInit();

	//Allocate device vectors memory
	allocateArray((void **)&this->dPos, memSize);
	allocateArray((void **)&this->dVel, memSize);
	allocateArray((void **)&this->dAcc, memSize);

}

//----------------------------------------------------------------------------
void vtkCUDAVelocityVerletSolver::ComputeNextStep(float *p, float *v, float *a)
{
	double dt05 = this->TimeStep*0.5;
	double dt = this->TimeStep;

	// Velocity Verlet Equations
	// * Xn+1 = Xn + Vn*dt + 0.5*An*dt
	// * Vn+1 = Vn + 0.5*dt(An + An+1)

	// Copy host -> device
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;
	copyArrayToDevice(this->dPos, p, 0, memSize);
	copyArrayToDevice(this->dVel, v, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	// CUDA procedure
	integrateSystem(this->dVel, this->dAcc, dt05, this->NumberOfParticles);
	integrateSystem(this->dPos, this->dVel, dt, this->NumberOfParticles);

	// Copy Device -> host
	copyArrayFromDevice(v, this->dVel, 0, memSize);//Vi = Vn + 0.5*An*dt
	copyArrayFromDevice(p, this->dPos, 0, memSize);//Xn+1 = Xn + Vn*dt + 0.5*An*dt

	// Derive An+1
	this->DeformationModel->ComputeForces();
	// Copy host -> device
	copyArrayToDevice(this->dVel, v, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	integrateSystem(this->dVel, this->dAcc, dt05, this->NumberOfParticles);

	// Copy Device -> host
	copyArrayFromDevice(v, this->dVel, 0, memSize);//Vn+1 = Vi + 0.5*dt*An+1 ; Vn+1 = Vn + 1/2*dt*(An + An+1)

}

//----------------------------------------------------------------------------
void vtkCUDAVelocityVerletSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
