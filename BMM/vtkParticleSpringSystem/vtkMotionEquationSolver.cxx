#include "vtkMotionEquationSolver.h"

vtkCxxRevisionMacro(vtkMotionEquationSolver, "$Revision: 0.1 $");

//----------------------------------------------------------------------------
vtkMotionEquationSolver::vtkMotionEquationSolver()
{
	this->NumberOfParticles = 0;
	this->dv = vtkDoubleArray::New();
	this->dv->SetNumberOfComponents(3);
}

//----------------------------------------------------------------------------
vtkMotionEquationSolver::~vtkMotionEquationSolver()
{
	if(this->dv) this->dv->Delete();
}

//----------------------------------------------------------------------------
void vtkMotionEquationSolver::SetDeformationModel(vtkParticleSpringSystem * model)
{
	this->DeformationModel = model;
}

//----------------------------------------------------------------------------
void vtkMotionEquationSolver::Init()
{
	for(int i=0; i< NumberOfParticles; i++)
	{
		this->dv->InsertNextTuple3(0.0, 0.0, 0.0);
	}
}

//----------------------------------------------------------------------------
void vtkMotionEquationSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
