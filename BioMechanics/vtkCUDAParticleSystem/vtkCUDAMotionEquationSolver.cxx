#include "vtkCUDAMotionEquationSolver.h"

#include "vtkObjectFactory.h"
#include "vtkIdList.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"

#include "vtkCUDAParticleSystem.h"

vtkCxxRevisionMacro(vtkCUDAMotionEquationSolver, "$Revision: 0.1 $");

//----------------------------------------------------------------------------
vtkCUDAMotionEquationSolver::vtkCUDAMotionEquationSolver()
{
	this->NumberOfParticles = 0;
	this->Residual = 1e-6;
	this->Initialized = 0;
	this->dPos = NULL;
	this->dVel = NULL;
	this->dAcc = NULL;
}


//----------------------------------------------------------------------------
vtkCUDAMotionEquationSolver::~vtkCUDAMotionEquationSolver()
{

}

//----------------------------------------------------------------------------
void vtkCUDAMotionEquationSolver::SetDeformationModel(vtkCUDAParticleSystem * model)
{
	this->DeformationModel = model;
}

//----------------------------------------------------------------------------
void vtkCUDAMotionEquationSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
