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

#include "vtkBioEngInterface.h"
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
	this->Legends = vtkActor2DCollection::New();

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
	tool->SetId(this->GetNumberOfTools());
	tool->SetRenderWindow(this->RenderWindow);
	tool->Init();
	this->Tools->AddTool(tool);
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
	//Init Collision Detection & Scenario
	this->CollisionDetection = vtkBioEngInterface::New();
	this->CollisionDetection->Init();

	this->CollisionDetection->SetTools(this->Tools);
	this->CollisionDetection->SetOrgans(this->Organs);

	//Add
	//FIXME:Set this as param in the srml file
	this->LegendDisplay = 1;

	vtkTextActor * legend;
	if(this->LegendDisplay)
	{
		for(int i = 0; i < this->Tools->GetNumberOfItems(); i++)
		{
			legend = vtkTextActor::New();
			vtkTextProperty * prop = vtkTextProperty::New();
			prop->SetFontFamilyToTimes();
			prop->SetFontSize(9);
			prop->SetColor(1,1,1);
			int * size = this->RenderWindow->GetSize();
			cout << size[0] << "," << size[1] << endl;
			legend->SetTextProperty(prop);
			legend->SetDisplayPosition(size[0]-160, size[1]-(i+1)*50);
			this->Legends->AddItem(legend);
			this->Renderer->AddActor(legend);
		}
	}

}

//----------------------------------------------------------------------------
void vtkScenario::Update()
{
	//Check if any collision between tools & organs has occurred
	this->CollisionDetection->Update();

	//Update organs & tools after collision detection has been performed
	vtkContactCollection * contacts;
	vtkContact * contact;

	contacts = this->CollisionDetection->GetContacts();
	contacts->InitTraversal();

	contact = contacts->GetNextContact();
	while(contact)
	{
		//Set organ contact point for deformation purposes
		vtkOrgan * organ = this->GetOrgan(contact->GetOrganId());
		vtkTool * tool = this->GetTool(contact->GetToolId());

		organ->InsertNextContact(contact);
		tool->InsertNextContact(contact);

		contact = contacts->GetNextContact();
	}

	//Process every scenario item and perform an update on it
	vtkOrgan * organ;
	vtkTool * tool;
	vtkTextActor * legend;

	this->Organs->InitTraversal();
	organ = this->Organs->GetNextOrgan();
	while(organ)
	{
		organ->Update();
		organ = this->Organs->GetNextOrgan();
	}

	this->Tools->InitTraversal();
	this->Legends->InitTraversal();

	tool = this->Tools->GetNextTool();
	legend = static_cast<vtkTextActor*>(this->Legends->GetNextItemAsObject());

	double * o;
	double * d;
	double * v;
	double * p;

	while(tool)
	{
		tool->Update();

		//Update Legend
		char text[120];
		o = tool->GetOrientation();
		d = tool->GetDirection();
		p = tool->GetPosition();
		v = tool->GetVelocity();
		sprintf( text, "O: (%1.3f,  %1.3f,  %1.3f)\nD: (%1.3f,  %1.3f,  %1.3f)\nP: (%1.3f,  %1.3f,  %1.3f)\nV: (%1.3f,  %1.3f,  %1.3f)\n",
				o[0], o[1], o[2], d[0], d[1], d[2], p[0], p[1], p[2], v[0], v[1], v[2]);
		legend->SetInput(text);
		tool = this->Tools->GetNextTool();
		legend = static_cast<vtkTextActor*>(this->Legends->GetNextItemAsObject());
	}
}

