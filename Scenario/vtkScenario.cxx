/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM),
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/

#include "vtkScenario.h"

#include "vtkObjectFactory.h"
#include "vtkRendererCollection.h"
#include "vtkPoints.h"

#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

#include "vtkRenderWindow.h"
#include "vtkTextProperty.h"
#include "vtkTextActor.h"
#include "vtkActor2DCollection.h"

#include <sstream>

vtkCxxRevisionMacro(vtkScenario, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenario);

//----------------------------------------------------------------------------
vtkScenario::vtkScenario() {

	this->Organs = vtkOrganCollection::New();
	this->Tools = vtkToolCollection::New();
	this->Contacts = NULL;

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
void vtkScenario::SetOrgans(vtkOrganCollection * collection)
{
	this->Organs->RemoveAllItems();

	vtkOrgan * organ;
	collection->InitTraversal();
	organ = collection->GetNextOrgan();
	while(organ != 0)
	{
		this->AddOrgan(organ);
		organ = collection->GetNextOrgan();
	}

}

//----------------------------------------------------------------------------
//Get the collection of organs of the Scenario
vtkOrganCollection * vtkScenario::GetOrgans()
{
	return this->Organs;
}

//----------------------------------------------------------------------------
//Add Organs to the Scenario
void vtkScenario::AddOrgan(vtkOrgan* organ)
{
	organ->SetId(this->GetNumberOfOrgans());
	organ->SetRenderWindow(this->RenderWindow);
	organ->Init();
	this->Organs->AddOrgan(organ);
}

//----------------------------------------------------------------------------
// Insert an organ in the specified position
void vtkScenario::ReplaceOrgan(vtkIdType index,vtkOrgan* organ)
{
	organ->SetId(index);
	this->Organs->ReplaceOrgan(index, organ);
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
void vtkScenario::SetTools(vtkToolCollection * collection)
{
	this->Tools->RemoveAllItems();

	vtkTool * tool;
	collection->InitTraversal();
	tool = collection->GetNextTool();
	while(tool != 0)
	{
		this->AddTool(tool);
		tool = collection->GetNextTool();
	}

}

//----------------------------------------------------------------------------
//Get the collection of tools of the Scenario
vtkToolCollection * vtkScenario::GetTools()
{
	return this->Tools;
}

//----------------------------------------------------------------------------
//Add Tools to the Scenario
void vtkScenario::AddTool(vtkTool* tool)
{
	tool->SetRenderWindow(this->RenderWindow);
	tool->SetId(this->GetNumberOfTools());
	tool->Init();
	this->Tools->AddTool(tool);
}

//----------------------------------------------------------------------------
//Insert in a given position in the vector the tool for collisions detection purposes
void vtkScenario::ReplaceTool(vtkIdType index, vtkTool* tool)
{
	tool->SetId(index);
	tool->SetRenderWindow(this->RenderWindow);
	tool->Init();
	this->Tools->ReplaceTool(index, tool);
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
//Set the Collection of contacts of the scenario
void vtkScenario::SetContacts(vtkContactCollection * collection)
{
	//this->Contacts->DeepCopy(collection);
	this->Contacts = collection;
}

//----------------------------------------------------------------------------
//Get the collection of contacts of the Scenario
vtkContactCollection * vtkScenario::GetContacts()
{
	return this->Contacts;
}

//----------------------------------------------------------------------------
//Set the Collection of extras of the scenario
/*void vtkScenario::SetExtras(vtkPropCollection * collection)
{
	this->Extras->RemoveAllItems();

	vtkProp * actor;
	collection->InitTraversal();
	actor = collection->GetLastProp();
	while(actor != 0)
	{
		this->InsertNextExtra(actor);
		actor = collection->GetNextProp();
	}

}

//----------------------------------------------------------------------------
//Get the collection of extras of the Scenario
vtkPropCollection * vtkScenario::GetExtras()
{
	return this->Extras;
}

//----------------------------------------------------------------------------
//Add extras to the scenario
void vtkScenario::InsertNextExtra(vtkProp* extra)
{
	this->Extras->AddItem(extra);
}

//----------------------------------------------------------------------------
//Insert in a given position in the vector the tool for collisions detection purposes
void vtkScenario::InsertExtra(vtkIdType index, vtkProp* actor)
{
	this->Extras->InsertItem(index, actor);
}

//----------------------------------------------------------------------------
//Delete the tool at the specified id from the scenario
void vtkScenario::RemoveExtra(vtkIdType index)
{
	this->Extras->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkScenario::GetNumberOfExtras()
{
	return this->Extras->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkActor * vtkScenario::GetExtra(vtkIdType id)
{
	return static_cast<vtkActor *>(this->Extras->GetItemAsObject(id));
}
*/

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
void vtkScenario::Init()
{
}

//----------------------------------------------------------------------------
void vtkScenario::Update()
{
	cout << "vtkScenario::Update()\n";
	//Update organs & tools after collision detection has been performed
	if(this->Contacts && this->Contacts > 0)
	{
		this->Contacts->InitTraversal();
		while(vtkContact * contact = this->Contacts->GetNextContact())
		{
			//Set organ contact point for deformation purposes
			vtkTool * tool = this->GetTool(contact->GetItemId(0));
			vtkOrgan * organ = this->GetOrgan(contact->GetItemId(1));

			tool->InsertNextContact(contact);
			organ->InsertNextContact(contact);
		}
	}
	//Process every scenario item and perform an update on it

	this->Organs->InitTraversal();
	while(vtkOrgan * organ = this->Organs->GetNextOrgan())
	{
		organ->Modified();
		organ->Update();
	}

	this->Tools->InitTraversal();
	while(vtkTool * tool = this->Tools->GetNextTool())
	{
		//tool->Modified();
		tool->Update();
	}
}

