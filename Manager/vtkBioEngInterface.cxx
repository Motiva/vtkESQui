#include "vtkBioEngInterface.h"

vtkCxxRevisionMacro(vtkBioEngInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioEngInterface);

//----------------------------------------------------------------------------
vtkBioEngInterface::vtkBioEngInterface(){

	this->DetectionFilter = vtkCollisionDetectionFilter::New();
	this->Matrix0 = vtkMatrix4x4::New();
	this->Matrix1 = vtkMatrix4x4::New();
	//Create contact collection object
	this->Contacts = vtkContactCollection::New();

}

//--------------------------------------------------------------------------
vtkBioEngInterface::~vtkBioEngInterface()
{
	this->Clear();

	this->Matrix0->Delete();
	this->Matrix1->Delete();

	this->Contacts->Delete();

	this->DetectionFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Init()
{
	//Initial setup of the collision detection parameters
	this->DetectionFilter->SetBoxTolerance (0.0);
	this->DetectionFilter->SetCellTolerance (0.0);
	this->DetectionFilter->SetNumberOfCellsPerBucket(2);
	//this->DetectionFilter->SetCollisionModeToFirstContact();
	this->DetectionFilter->SetCollisionModeToHalfContacts();
	//this->DetectionFilter->SetCollisionModeToAllContacts();
	//Debug. Display contact cells.
	this->DetectionFilter->SetGenerateScalars(1);
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Clear()
{
	//Clear contacts from previous executions
	this->Contacts->InitTraversal();
	vtkContact * contact = this->Contacts->GetNextContact();
	while(contact)
	{
		contact->Delete();
		contact = this->Contacts->GetNextContact();
	}
	this->Contacts->RemoveAllItems();

}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Update()
{
	//Get the organs and tools from the collections. Set Inputs of the collision library
	//The organ (converted to vtkPolydata) and a bounding box of the tool. This bounding box (polydata)
	//is returned by a method created in vtkTool

	//Clear from previous executions
	this->Clear();

	for (vtkIdType organId=0; organId < this->Organs->GetNumberOfItems(); organId++)
	{
		vtkOrgan * organ = this->Organs->GetOrgan(organId);
		vtkPolyData * organBox = organ->GetSimpleMesh();

		for (vtkIdType toolId=0; toolId < this->Tools->GetNumberOfItems(); toolId++)
		{
			vtkTool * tool =  this->Tools->GetTool(toolId);
			//Both grasper shall be set as Collision Detection inputs
			vtkPolyData * toolBox = tool->GetSimpleMesh();

			//Each organ bounding box is set as an input of the CDL
			this->DetectionFilter->SetInput(0, organBox);

			//Tool bounding box is set as CDL input
			this->DetectionFilter->SetInput(1, toolBox);

			//Transformation matrixes
			this->DetectionFilter->SetMatrix(0, Matrix0);
			this->DetectionFilter->SetMatrix(1, Matrix1);

			this->DetectionFilter->Update();

			//Detection Filter is set to return the first contact only
			vtkIdType numberOfContacts = this->DetectionFilter->GetNumberOfContacts();

			for(int i =0; i < numberOfContacts; i++)
			{
				//There has been a collision
				//A new contact will be created, filling the info required
				vtkContact *contact = vtkContact::New();

				//Set organ & tool ids
				contact->SetOrganId(organId);
				contact->SetToolId(toolId);

				vtkIdType cellId = this->DetectionFilter->GetContactCells(0)->GetValue(i);
				vtkIdType pointId = organBox->GetCell(cellId)->GetPointId(0);
				double * point = organBox->GetCell(cellId)->GetPoints()->GetPoint(0);

				contact->SetPointId(0, pointId);
				contact->SetPoint(0, point);
				contact->SetCellId(0, cellId);

				cellId = this->DetectionFilter->GetContactCells(1)->GetValue(i);
				pointId = toolBox->GetCell(cellId)->GetPointId(0);
				point = toolBox->GetCell(cellId)->GetPoints()->GetPoint(0);

				contact->SetPointId(1, pointId);
				contact->SetPoint(1, point);
				contact->SetCellId(1, cellId);

				//contact->Print(cout);

				double vector[3];
				tool->GetDirection(vector);
				contact->SetDirectionVector(vector);

				if(!this->Contacts->ContainsContact(contact))
				{
					this->Contacts->InsertNextContact(contact);
				}
				else
				{
					contact->Delete();
				}
			}
		}
	}
}



