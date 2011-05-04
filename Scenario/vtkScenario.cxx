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
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

#include "vtkScenarioObject.h"
#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"
#include "vtkModel.h"
#include "vtkActor.h"
#include "vtkVisualizationModel.h"
#include "vtkModelCollection.h"

vtkCxxRevisionMacro(vtkScenario, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenario);

//----------------------------------------------------------------------------
vtkScenario::vtkScenario() {

	this->Initialized = 0;
	this->Objects = vtkScenarioObjectCollection::New();

}

//----------------------------------------------------------------------------
vtkScenario::~vtkScenario(){
	this->Objects->RemoveAllItems();
	this->Objects->Delete();
}

//----------------------------------------------------------------------------
void vtkScenario::SetObjects(vtkScenarioObjectCollection * collection)
{
	this->Objects->RemoveAllItems();

	collection->InitTraversal();
	while(vtkScenarioObject * object = collection->GetNextObject())
	{
		this->AddObject(object);
	}

}

//----------------------------------------------------------------------------
vtkScenarioObjectCollection * vtkScenario::GetObjects()
{
	return this->Objects;
}

//----------------------------------------------------------------------------
void vtkScenario::AddObject(vtkScenarioObject* object)
{
	object->SetId(this->GetNumberOfObjects());
	object->SetRenderWindow(this->RenderWindow);
	this->Objects->AddObject(object);
}

//----------------------------------------------------------------------------
void vtkScenario::ReplaceObject(vtkIdType index,vtkScenarioObject* object)
{
	object->SetId(index);
	this->Objects->ReplaceObject(index, object);
}

//----------------------------------------------------------------------------
void vtkScenario::RemoveObject(vtkIdType index)
{
	this->Objects->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkScenario::GetNumberOfObjects()
{
	return this->Objects->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkScenarioObject * vtkScenario::GetObject(vtkIdType id)
{
	return this->Objects->GetObject(id);
}

//----------------------------------------------------------------------------
void vtkScenario::SetRenderWindow(vtkRenderWindow *window) {
	this->RenderWindow = window;
	this->Renderer= RenderWindow->GetRenderers()->GetFirstRenderer();
}

//----------------------------------------------------------------------------
vtkRenderWindow* vtkScenario::GetRenderWindow() {
	return this->RenderWindow;
}

//----------------------------------------------------------------------------
void vtkScenario::Init()
{
	if(!this->Initialized && this->RenderWindow)
	{
		this->Objects->InitTraversal();
		int i = 0;
		while(vtkScenarioObject * o = this->Objects->GetNextObject())
		{
			o->SetRenderWindow(this->RenderWindow);
			o->SetRenderer(this->Renderer);
			o->SetId(i);

			if(!o->IsInitialized()) o->Init();

			//Add all actors to the render window
			for(int id = 0; id<o->GetNumberOfElements(); id++)
			{
				vtkScenarioElement * e = o->GetElement(id);

				//Display every visible model. Hiding model is done by vtkModel:Hide()
				vtkModelCollection * models = e->GetModels();
				models->InitTraversal();
				while(vtkModel * m = models->GetNextModel()){
					if(m->GetVisibility()) this->Renderer->AddActor(m->GetActor());
				}

			}
			i++;
		}
		this->Initialized = 1;
	}
}

//----------------------------------------------------------------------------
void vtkScenario::Update()
{
	//cout << "vtkScenario::Update()" << endl;
	//Process every scenario object and perform an update on it
	this->Objects->InitTraversal();
	while(vtkScenarioObject * object = this->Objects->GetNextObject())
	{
		object->Modified();
		object->Update();
	}
	this->RenderWindow->Render();
}

//--------------------------------------------------------------------------
void vtkScenario::PrintSelf(ostream&os, vtkIndent indent)
{
	if(this->Name) os << indent << "Name: " << this->Name << "\n";
	os << indent << "NumberOfObjects: " << this->Objects->GetNumberOfObjects() << endl;
	this->Objects->InitTraversal();
	while(vtkScenarioObject * o = this->Objects->GetNextObject())
	{
		o->Print(os);
	}

}
