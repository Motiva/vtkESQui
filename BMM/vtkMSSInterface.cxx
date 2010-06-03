#include "vtkMSSInterface.h"

vtkCxxRevisionMacro(vtkMSSInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkMSSInterface);

//----------------------------------------------------------------------------
vtkMSSInterface::vtkMSSInterface()
{
	this->MSSMesh = vtkMSS::New();
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->Mass = 0;
	this->DeltaT = 0;
	this->Steps = 0;
}

//----------------------------------------------------------------------------
vtkMSSInterface::~vtkMSSInterface()
{
	this->MSSMesh->Delete();
	this->Mesh->Delete();
}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkMSSInterface::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

    // Get the info objects
    //vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the input and output
   //vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    //std::cout << "vtkMSSInterface::RequestData" << endl;

    this->MSSMesh->SetContacts(this->ContactPointIds, this->Directions);
    this->MSSMesh->Update();

    output->DeepCopy(this->MSSMesh->GetOutput());

    this->Clear();

    return 1;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::Init()
{
	//Set Mass-Spring System parameters
	this->MSSMesh->SetDistanceCoefficient(this->DistanceCoefficient);
	this->MSSMesh->SetDampingCoefficient(this->DampingCoefficient);//Friction
	this->MSSMesh->SetMass(this->Mass);
	this->MSSMesh->SetDeltaT(this->DeltaT);
	this->MSSMesh->SetSteps(this->Steps);

	this->MSSMesh->SetInput(vtkUnstructuredGrid::SafeDownCast(this->GetInput()));

	//this->Print(cout);
}

//--------------------------------------------------------------------------
void vtkMSSInterface::SetDistanceCoefficient(double value)
{
	this->DistanceCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::SetDampingCoefficient(double value)
{
	this->DampingCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::SetMass(double value)
{
	this->Mass = value;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::SetDeltaT(double value)
{
	this->DeltaT = value;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::SetSteps(int value)
{
	this->Steps = value;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent <<  "DistanceCoefficient: " << this->DistanceCoefficient << endl;
  os << indent <<  "DampingCoefficient: " << this->DampingCoefficient << endl;
  os << indent <<  "Mass: " << this->Mass << endl;
  os << indent <<  "DeltaT: " << this->DeltaT << endl;
  os << indent <<  "Steps: " << this->Steps << endl;
}
