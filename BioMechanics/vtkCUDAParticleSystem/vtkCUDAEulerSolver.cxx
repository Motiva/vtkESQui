#include "vtkCUDAEulerSolver.h"

#include "vtkObjectFactory.h"
#include "vtkDoubleArray.h"

#include "vtkCUDAParticleSystem.h"
#include "vtkCUDAParticleSystem.cuh"

vtkCxxRevisionMacro(vtkCUDAEulerSolver, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCUDAEulerSolver);

//----------------------------------------------------------------------------
vtkCUDAEulerSolver::vtkCUDAEulerSolver()
{
}

//----------------------------------------------------------------------------
vtkCUDAEulerSolver::~vtkCUDAEulerSolver()
{
	//Free CUDA arrays
	freeArray(this->dPos);
	freeArray(this->dVel);
	freeArray(this->dAcc);
}

//----------------------------------------------------------------------------
void vtkCUDAEulerSolver::Init()
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
void vtkCUDAEulerSolver::ComputeNextStep(float *p, float *v, float *a)
{

	this->DeformationModel->ComputeForces();

	// Copy host -> device
	unsigned int memSize = sizeof(float) * 4 * this->NumberOfParticles;
	copyArrayToDevice(this->dPos, p, 0, memSize);
	copyArrayToDevice(this->dVel, v, 0, memSize);
	copyArrayToDevice(this->dAcc, a, 0, memSize);

	//CUDA procedure

	//Vn+1 = Vn + dt*At
	integrateSystem(this->dVel, this->dAcc, this->DeltaTime, this->NumberOfParticles);
	//Xn+1 = Xn + dt*Vn+1
	integrateSystem(this->dPos, this->dVel, this->DeltaTime, this->NumberOfParticles);

	// Copy Device -> host
	copyArrayFromDevice(p, this->dPos, 0, memSize);
	copyArrayFromDevice(v, this->dVel, 0, memSize);

}

//----------------------------------------------------------------------------
void vtkCUDAEulerSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
