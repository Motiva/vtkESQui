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
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"

#include "vtkScenarioObject.h"
#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"
#include "vtkModel.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkDeformationModel.h"
#include "vtkActor.h"
#include "vtkVisualizationModel.h"
#include "vtkModelCollection.h"

vtkCxxRevisionMacro(vtkScenario, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenario);

//----------------------------------------------------------------------------
vtkScenario::vtkScenario() {

  this->Initialized = 0;
  this->Background[0] = 1.0;
  this->Background[1] = 1.0;
  this->Background[2] = 1.0;
  this->WindowSize[0] = 800;
  this->WindowSize[1] = 600;
  this->Name = NULL;
  this->Renderer = NULL;
  this->RenderWindow = NULL;
  this->RenderWindowInteractor = NULL;
  this->Camera = NULL;

  this->Objects = vtkScenarioObjectCollection::New();
  this->Lights = vtkLightCollection::New();

}

//----------------------------------------------------------------------------
vtkScenario::~vtkScenario(){
  this->Objects->RemoveAllItems();
  this->Objects->Delete();
  this->Lights->RemoveAllItems();
  this->Lights->Delete();
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
void vtkScenario::SetLights(vtkLightCollection * collection)
{
  this->Lights->RemoveAllItems();

  collection->InitTraversal();
  while(vtkLight * light = collection->GetNextItem())
  {
    this->AddLight(light);
  }
}

//----------------------------------------------------------------------------
vtkLightCollection * vtkScenario::GetLights()
{
  return this->Lights;
}

//----------------------------------------------------------------------------
void vtkScenario::AddLight(vtkLight* light)
{
  this->Lights->AddItem(light);
}

//----------------------------------------------------------------------------
void vtkScenario::ReplaceLight(vtkIdType index,vtkLight* light)
{
  this->Lights->ReplaceItem(index, light);
}

//----------------------------------------------------------------------------
void vtkScenario::RemoveLight(vtkIdType index)
{
  this->Lights->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkScenario::GetNumberOfLights()
{
  return this->Lights->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkLight * vtkScenario::GetLight(vtkIdType id)
{
  return vtkLight::SafeDownCast(this->Lights->GetItemAsObject(id));
}

//----------------------------------------------------------------------------
void vtkScenario::SetCamera(vtkCamera* camera)
{
  if(this->Camera) this->Camera->Delete();
  this->Camera = camera;
}

//----------------------------------------------------------------------------
vtkCamera * vtkScenario::GetCamera()
{
  return this->Camera;
}

//----------------------------------------------------------------------------
void vtkScenario::SetRenderWindow(vtkRenderWindow *window) {
  this->RenderWindow = window;
}

//----------------------------------------------------------------------------
vtkRenderWindow* vtkScenario::GetRenderWindow() {
  return this->RenderWindow;
}

//----------------------------------------------------------------------------
void vtkScenario::SetRenderWindowInteractor(vtkRenderWindowInteractor *iren) {
  this->RenderWindowInteractor = iren;
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor * vtkScenario::GetRenderWindowInteractor() {
  return this->RenderWindowInteractor;
}

//----------------------------------------------------------------------------
void vtkScenario::Initialize()
{
  if(!this->Initialized)
  {
    if(!this->RenderWindow)
    {
      this->Renderer = vtkRenderer::New();
      this->RenderWindow = vtkRenderWindow::New();
      this->RenderWindow->AddRenderer(this->Renderer);

      this->RenderWindowInteractor = vtkRenderWindowInteractor::New();
      this->RenderWindowInteractor->SetRenderWindow(this->RenderWindow);
    }
    else
    {
      this->Renderer = this->RenderWindow->GetRenderers()->GetFirstRenderer();
      this->RenderWindowInteractor = this->RenderWindow->GetInteractor();
    }

    this->RenderWindow->SetSize(this->WindowSize);
    if(this->Name)
    {
      this->RenderWindow->SetWindowName(this->Name);
    }
    this->Renderer->SetBackground(this->Background);

    //Set scenario camera
    this->Renderer->SetActiveCamera(this->Camera);

    //Add lights to the scenario
    this->Lights->InitTraversal();
    while(vtkLight * l = this->Lights->GetNextItem())
    {
      this->Renderer->AddLight(l);
    }

    for(int i=0; i < this->Objects->GetNumberOfObjects(); i++)
    {
      vtkScenarioObject * o = this->Objects->GetObject(i);
      o->SetRenderWindow(this->RenderWindow);
      o->SetRenderer(this->Renderer);
      o->Update();

      //Add actors to the render window if active
      vtkScenarioElementCollection * elements = o->GetElements();
      elements->InitTraversal();
      while(vtkScenarioElement * e = elements->GetNextElement())
      {
        if(e->GetVisualizationModel())
        {
          this->Renderer->AddActor(e->GetVisualizationModel()->GetActor());
        }
        if(e->GetCollisionModel())
        {
          this->Renderer->AddActor(e->GetCollisionModel()->GetActor());
        }
        if(e->GetDeformationModel())
        {
          this->Renderer->AddActor(e->GetDeformationModel()->GetActor());
        }
      }

      this->RenderWindow->GetInteractor()->Initialize();

      this->Initialized = 1;
    }
  }
}

//----------------------------------------------------------------------------
void vtkScenario::Update()
{
  //cout << "vtkScenario::Update()" << endl;

  if(!this->Initialized) this->Initialize();

  //Process every scenario object and perform an update on it
  this->Objects->InitTraversal();
  while(vtkScenarioObject * o = this->Objects->GetNextObject())
  {
    vtkScenarioElementCollection * elements = o->GetElements();
    elements->InitTraversal();
    while(vtkScenarioElement * e = elements->GetNextElement())
    {
      if(e->GetDeformationModel())
      {
        //Update object deformation
        e->Update();
      }
    }
  }

}

//----------------------------------------------------------------------------
void vtkScenario::Render()
{
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
