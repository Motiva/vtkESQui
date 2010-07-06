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
	organ->SetId(this->GetNumberOfOrgans());
	organ->SetRenderWindow(this->RenderWindow);
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
	tool->SetId(this->GetNumberOfTools());
	tool->SetRenderWindow(this->RenderWindow);
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

