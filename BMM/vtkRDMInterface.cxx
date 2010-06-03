#include "vtkRDMInterface.h"

vtkCxxRevisionMacro(vtkRDMInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkRDMInterface);

//----------------------------------------------------------------------------
vtkRDMInterface::vtkRDMInterface()
{
	this->RDMMesh = vtkRobustDeformationModel::New();
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->Mass = 0;
	this->DeltaT = 0;
	this->Steps = 0;
}

//----------------------------------------------------------------------------
vtkRDMInterface::~vtkRDMInterface()
{
	this->RDMMesh->Delete();
	this->Mesh->Delete();
}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkRDMInterface::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

    // Get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the input and output
    vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
                                     inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
                                      outInfo->Get(vtkDataObject::DATA_OBJECT()));

    //std::cout << "vtkRDMInterface::RequestData" << endl;

    if (input->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE)
    {
    	//Set input mesh where deformation will be calculated
    	vtkPoints * ps = vtkPoints::New();
    	ps->SetDataTypeToDouble();
    	ps->DeepCopy(input->GetPoints());
    	input->SetPoints(ps);
    	input->Update();
    }

    this->RDMMesh->SetInput(input);
    this->RDMMesh->SetContacts(this->ContactPointIds, this->Directions);

    this->RDMMesh->Update();

    output->DeepCopy(this->RDMMesh->GetOutput());

    this->Clear();

    return 1;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::Init()
{
	//Set Mass-Spring System parameters
	this->RDMMesh->SetDistanceForceCoefficient(this->DistanceCoefficient);
	this->RDMMesh->SetDamping(this->DampingCoefficient);//Friction
	this->RDMMesh->SetSurfaceForceCoefficient(this->SurfaceCoefficient);
	this->RDMMesh->SetVolumeForceCoefficient(this->VolumeCoefficient);
	//this->RDMMesh->SetMass(this->Mass);
	//this->RDMMesh->SetDeltaT(this->DeltaT);
	//this->RDMMesh->SetSteps(this->Steps);

	this->RDMMesh->SetInput(vtkUnstructuredGrid::SafeDownCast(this->GetInput()));
	this->RDMMesh->Init();

	this->Print(cout);
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetDistanceCoefficient(double value)
{
	this->DistanceCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetDampingCoefficient(double value)
{
	this->DampingCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetSurfaceCoefficient(double value)
{
	 this->SurfaceCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetVolumeCoefficient(double value)
{
	this->VolumeCoefficient = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetMass(double value)
{
	this->Mass = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetDeltaT(double value)
{
	this->DeltaT = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::SetSteps(int value)
{
	this->Steps = value;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent <<  "DistanceCoefficient: " << this->DistanceCoefficient << endl;
  os << indent <<  "DampingCoefficient: " << this->DampingCoefficient << endl;
  os << indent <<  "Mass: " << this->Mass << endl;
  os << indent <<  "DeltaT: " << this->DeltaT << endl;
  os << indent <<  "Steps: " << this->Steps << endl;
}
