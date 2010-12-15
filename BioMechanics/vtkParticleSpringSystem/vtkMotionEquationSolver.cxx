#include "vtkMotionEquationSolver.h"

#include "vtkObjectFactory.h"
#include "vtkIdList.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"

#include "vtkParticleSpringSystem.h"
#include "vtkParticleCollection.h"
#include "vtkParticle.h"

vtkCxxRevisionMacro(vtkMotionEquationSolver, "$Revision: 0.1 $");

//----------------------------------------------------------------------------
vtkMotionEquationSolver::vtkMotionEquationSolver()
{
	this->NumberOfParticles = 0;
	this->dv = vtkDoubleArray::New();
	this->dv->SetNumberOfComponents(3);
	this->dx = vtkDoubleArray::New();
	this->dx->SetNumberOfComponents(3);
}

//----------------------------------------------------------------------------
vtkMotionEquationSolver::~vtkMotionEquationSolver()
{
	if(this->dv) this->dv->Delete();
	if(this->dx) this->dx->Delete();
}

//----------------------------------------------------------------------------
void vtkMotionEquationSolver::Init()
{
	for(int i=0; i< this->NumberOfParticles; i++)
	{
		this->dv->InsertNextTuple3(0.0, 0.0, 0.0);
		this->dx->InsertNextTuple3(0.0, 0.0, 0.0);
	}
}

//----------------------------------------------------------------------------
void vtkMotionEquationSolver::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
