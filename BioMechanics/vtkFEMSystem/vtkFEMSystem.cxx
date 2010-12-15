#include "vtkFEMSystem.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include "vtkDoubleArray.h"
#include "vtkIdList.h"

vtkCxxRevisionMacro(vtkFEMSystem, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkFEMSystem);

//----------------------------------------------------------------------------
vtkFEMSystem::vtkFEMSystem()
{
	this->SpringCoefficient = 0.0;
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->DeltaT = 0;
	this->Mass = 0;
	this->RigidityFactor = 0;
	this->ContactIds = NULL;
	this->ContactDisplacements = NULL;
}

//----------------------------------------------------------------------------
vtkFEMSystem::~vtkFEMSystem()
{

}

//----------------------------------------------------------------------------
// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkFEMSystem::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	return 1;
}

void vtkFEMSystem::Step()
{
}

//----------------------------------------------------------------------------
void vtkFEMSystem::Init()
{
	//this->Mesh.

}


//----------------------------------------------------------------------------
void vtkFEMSystem::SetContacts(vtkIdList * ids, vtkDoubleArray * directions)
{
	this->ContactIds->DeepCopy(ids);
	this->ContactDisplacements->DeepCopy(directions);
	this->Modified();
}

//----------------------------------------------------------------------------
void vtkFEMSystem::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
