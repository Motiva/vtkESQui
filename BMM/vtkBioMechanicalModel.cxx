#include "vtkBioMechanicalModel.h"

vtkCxxRevisionMacro(vtkBioMechanicalModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioMechanicalModel);

//--------------------------------------------------------------------------
vtkBioMechanicalModel::vtkBioMechanicalModel()
{
	this->Mesh = vtkUnstructuredGrid::New();

	this->ContactPoints = vtkPoints::New();
	this->ContactPointIds = vtkIdList::New();
	this->ContactCelltIds = vtkIdList::New();

	this->Directions = vtkDoubleArray::New();
	this->Directions->SetNumberOfComponents(3);

	this->Name = "";

}
//--------------------------------------------------------------------------
vtkBioMechanicalModel::~vtkBioMechanicalModel()
{
	this->Mesh->Delete();

	this->ContactPoints->Delete();
	this->ContactPointIds->Delete();
	this->ContactCelltIds->Delete();
	this->Directions->Delete();
}

//--------------------------------------------------------------------------
int vtkBioMechanicalModel::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
    // get the info objects
    //vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    //vtkInformation *outInfo = outputVector->GetInformationObject(0);
    // get the input and output
    //vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    //vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    std::cout << "vtkBioMechanicalModel::RequestData" << endl;

    //output->DeepCopy(input);

    return 1;
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::Init()
{

}
//--------------------------------------------------------------------------
void vtkBioMechanicalModel::Clear()
{
	this->ContactPoints->Reset();
	this->ContactPointIds->Reset();
	this->ContactCelltIds->Reset();
	this->Directions->Reset();
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::SetName(const char * name)
{
	this->Name = name;
}

//--------------------------------------------------------------------------
const char * vtkBioMechanicalModel::GetName()
{
	return this->Name;
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::InsertNextContact(vtkContact* contact)
{
	//Insert collision point coordinates
	double * point = contact->GetOrganPoint();
	this->ContactPoints->InsertNextPoint(point[0],point[1],point[2]);
	this->ContactPointIds->InsertNextId(contact->GetOrganPointId());
	this->ContactCelltIds->InsertNextId(contact->GetOrganCellId());

	this->Directions->InsertNextTuple(contact->GetDirectionVector());

	this->Modified();
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::InsertContacts(vtkContactCollection * collection)
{
	//Extract data from vtkContactCollection object
	collection->InitTraversal();
	vtkContact * c;
	c = collection->GetNextContact();
	while(c)
	{
		this->InsertNextContact(c);
		c = collection->GetNextContact();
	}
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::DeleteContacts()
{
	this->Clear();
}

//--------------------------------------------------------------------------
vtkPoints * vtkBioMechanicalModel::GetContactPoints()
{
	return this->ContactPoints;
}

//--------------------------------------------------------------------------
vtkCell * vtkBioMechanicalModel::GetMeshCell(vtkIdType id)
{
	return this->Mesh->GetCell(id);
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::PrintSelf(ostream& os, vtkIndent indent)
{
	 os << indent <<  "Name: " << this->Name << endl;
	 this->Superclass::PrintSelf(os, indent);
}
