#include "vtkSimulationManager.h"

vtkCxxRevisionMacro(vtkSimulationManager, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulationManager);

//--------------------------------------------------------------------------
vtkSimulationManager::vtkSimulationManager(){
	this->collisionLibrary=0;
	this->Scenario = 0;
}

//--------------------------------------------------------------------------
vtkSimulationManager::~vtkSimulationManager(){
	if (collisionLibrary!=0)
		this->collisionLibrary->Delete();
}

//--------------------------------------------------------------------------
void vtkSimulationManager::Init()
{
	//Scenario & collision detection library must be defined previously
	if(this->Scenario && this->collisionLibrary)
	{
		vtkToolCollection * tools = this->Scenario->GetToolCollection();
		vtkOrganCollection * organs = this->Scenario->GetOrganCollection();

		this->collisionLibrary->SetTools(tools);
		this->collisionLibrary->SetOrgans(organs);
	}
}

//--------------------------------------------------------------------------
void vtkSimulationManager::Update()
{

	//Check if any collision between tools & organs has occurred
	this->collisionLibrary->Update();

	//Update organs & tools after collision detection has been performed
	vtkContactCollection * contacts;
	vtkContact * contact;

	contacts = this->collisionLibrary->GetContacts();
	contacts->InitTraversal();

	contact = contacts->GetNextContact();
	while(contact)
	{
		//Set organ contact point for deformation purposes
		vtkOrgan * organ = this->Scenario->GetOrgan(contact->GetOrganId());
		vtkTool * tool = this->Scenario->GetTool(contact->GetToolId());

		organ->InsertNextContact(contact);
		tool->InsertNextContact(contact);

		contact = contacts->GetNextContact();
	}

	//Update scenario
	this->Scenario->Update();
}

//--------------------------------------------------------------------------
void vtkSimulationManager::SetScenario(vtkScenario * scenario)
{
	this->Scenario = scenario;
}

//--------------------------------------------------------------------------
vtkScenario * vtkSimulationManager::GetScenario()
{
	return this->Scenario;
}

//--------------------------------------------------------------------------
void vtkSimulationManager::SetCollisionDetectionLibrary (const char *library)
{
	if (library)
	{
		this->libraryName = library;
		if (!strcmp(this->libraryName, "vtkbioeng"))
		{
			this->collisionLibrary = vtkBioEngInterface::New();
			this->collisionLibrary->Init();
		}
		else
		{
			//In case vtkbioeng is not selected new libraries will be added here
		}
	}
	else
	{
		this->libraryName = NULL;
	}

}

//--------------------------------------------------------------------------
vtkContactCollection * vtkSimulationManager::GetContacts() {
	return this->collisionLibrary->GetContacts();
}

