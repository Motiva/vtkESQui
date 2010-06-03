
#include "vtkScenario.h"

vtkCxxRevisionMacro(vtkScenario, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenario);

//----------------------------------------------------------------------------
vtkScenario::vtkScenario() {

	this->Organs = vtkOrganCollection::New();
	this->Tools = vtkToolCollection::New();

}

//----------------------------------------------------------------------------
vtkScenario::~vtkScenario(){
	this->Organs->RemoveAllItems();
	this->Tools->RemoveAllItems();
	this->Organs->Delete();
	this->Tools->Delete();

}

//----------------------------------------------------------------------------
//Set the Collection of organs of the scenario
void vtkScenario::SetOrganCollection(vtkOrganCollection * collection)
{
	this->Organs->RemoveAllItems();

	vtkOrgan * organ;
	collection->InitTraversal();
	organ = collection->GetNextOrgan();
	while(organ != 0)
	{
		this->InsertNextOrgan(organ);
		organ = collection->GetNextOrgan();
	}

}

//----------------------------------------------------------------------------
//Get the collection of organs of the Scenario
vtkOrganCollection * vtkScenario::GetOrganCollection()
{
	return this->Organs;
}

//----------------------------------------------------------------------------
//Add Organs to the Scenario
void vtkScenario::InsertNextOrgan(vtkOrgan* organ)
{
	organ->SetRenderWindow(this->RenderWindow);
	organ->SetId(this->GetNumberOfOrgans());
	organ->Init();
	this->Organs->InsertNextOrgan(organ);
}

//----------------------------------------------------------------------------
// Insert an organ in the specified position
void vtkScenario::InsertOrgan(vtkIdType index,vtkOrgan* organ)
{
	organ->SetId(index);
	this->Organs->InsertOrgan(index, organ);
}

//----------------------------------------------------------------------------
// Delete the organ at the specified id from the scenario
void vtkScenario::RemoveOrgan(vtkIdType index)
{
	this->Organs->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkScenario::GetNumberOfOrgans()
{
	return this->Organs->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkOrgan * vtkScenario::GetOrgan(vtkIdType id)
{
	return this->Organs->GetOrgan(id);	
}

//----------------------------------------------------------------------------
//Set the Collection of tools of the scenario
void vtkScenario::SetToolCollection(vtkToolCollection * collection)
{
	this->Tools->RemoveAllItems();

	vtkTool * tool;
	collection->InitTraversal();
	tool = collection->GetNextTool();
	while(tool != 0)
	{
		this->InsertNextTool(tool);
		tool = collection->GetNextTool();
	}

}

//----------------------------------------------------------------------------
//Get the collection of tools of the Scenario
vtkToolCollection * vtkScenario::GetToolCollection()
{
	return this->Tools;
}

//----------------------------------------------------------------------------
//Add Tools to the Scenario
void vtkScenario::InsertNextTool(vtkTool* tool)
{
	tool->SetRenderWindow(this->RenderWindow);
	tool->SetId(this->GetNumberOfTools());
	tool->Init();
	this->Tools->InsertNextTool(tool);
}

//----------------------------------------------------------------------------
//Insert in a given position in the vector the tool for collisions detection purposes
void vtkScenario::InsertTool(vtkIdType index, vtkTool* tool)
{
	tool->SetId(index);
	this->Tools->InsertTool(index, tool);
}

//----------------------------------------------------------------------------
//Delete the tool at the specified id from the scenario
void vtkScenario::RemoveTool(vtkIdType index)
{
	this->Tools->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkScenario::GetNumberOfTools()
{
	return this->Tools->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkTool * vtkScenario::GetTool(vtkIdType id)
{
	return this->Tools->GetTool(id);	
}

//----------------------------------------------------------------------------
void vtkScenario::SetRenderWindow(vtkRenderWindow *Renderer) {
	this->RenderWindow = Renderer;
	this->Renderer= RenderWindow->GetRenderers()->GetFirstRenderer();
}

//----------------------------------------------------------------------------
vtkRenderWindow* vtkScenario::GetRenderWindow() {
	return this->RenderWindow;
}

//----------------------------------------------------------------------------
void vtkScenario::Update()
{
	//Process every scenario item and perform an update on it
	vtkOrgan * organ;
	vtkTool * tool;

	this->Organs->InitTraversal();
	organ = this->Organs->GetNextOrgan();
	while(organ)
	{
		organ->Update();
		organ = this->Organs->GetNextOrgan();
	}

	this->Tools->InitTraversal();
	tool = this->Tools->GetNextTool();
	while(tool)
	{
		tool->Update();
		tool = this->Tools->GetNextTool();
	}
}

