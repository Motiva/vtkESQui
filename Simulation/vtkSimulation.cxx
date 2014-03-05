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
#include "vtkSimulation.h"

#include "vtkObjectFactory.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkDefaultInteractorStyle.h"
#include "vtkSingleChannelInteractorStyle.h"
#include "vtkScenario.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"
#include "vtkCollisionModel.h"
#include "vtkCollisionCollection.h"
#include "vtkBioEngInterface.h"
#include "vtkTool.h"

#ifdef VTKESQUI_USE_HAPTICS
#include "vtkHaptic.h"
#endif

void SimulationRenderingCallback ( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
	vtkSimulation * sim = static_cast<vtkSimulation*>(clientData);

	if (vtkCommand::TimerEvent == eventId)
	{
		int tid = * static_cast<int *>(callData);
		if (tid == sim->GetRenderTimerId())
		{
			//cout << "Step\n";
			sim->Interact();
			sim->Step();
			sim->Render();
		}
	}
}

vtkCxxRevisionMacro(vtkSimulation, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulation);

//----------------------------------------------------------------------------
vtkSimulation::vtkSimulation()
{
  #ifdef VTKESQUI_USE_HAPTICS
  this->HapticDevice = NULL;
  #endif
  this->Scenario = NULL;
  this->Interactor = NULL;
  this->InteractorStyle = NULL;
  this->RenderTimerId = 0;
  this->RenderTimerRate = 0;
  this->Interaction = false;
  this->Collision = false;
  this->Initialized = false;
  this->Gravity[0] = this->Gravity[1] = this->Gravity[2] = 0.0;

  this->Callback = vtkCallbackCommand::New();
}

//----------------------------------------------------------------------------
vtkSimulation::~vtkSimulation()
{
  if(this->Callback) this->Callback->Delete();
  if(this->InteractorStyle) this->InteractorStyle->Delete();
}

//----------------------------------------------------------------------------
void vtkSimulation::SetInteractor(vtkRenderWindowInteractor * i)
{
  this->Interactor = i;
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor * vtkSimulation::GetInteractor()
{
  return this->Interactor;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetInteractorStyle(vtkInteractorStyle * style)
{
  this->InteractorStyle = style;
}

//----------------------------------------------------------------------------
vtkInteractorStyle * vtkSimulation::GetInteractorStyle()
{
  return this->InteractorStyle;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetScenario(vtkScenario * s)
{
  this->Scenario = s;
}

//----------------------------------------------------------------------------
vtkScenario * vtkSimulation::GetScenario()
{
  return this->Scenario;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetCollisionModeToSimple()
{
  if(this->Collision) this->CollisionDetection->SetModeToSimple();
}

//----------------------------------------------------------------------------
void vtkSimulation::SetCollisionModeToExtended()
{
  if(this->Collision) this->CollisionDetection->SetModeToExtended();
}

//----------------------------------------------------------------------------
void vtkSimulation::SetCollisionModeToFull()
{
  if(this->Collision) this->CollisionDetection->SetModeToFull();
}

//----------------------------------------------------------------------------
vtkCollisionCollection * vtkSimulation::GetCollisions()
{
  if(this->Collision){
		//this->CollisionDetection->Update();
		return this->CollisionDetection->GetCollisions();
	}
  else return NULL;
}

//----------------------------------------------------------------------------
vtkIntArray * vtkSimulation::GetCollisionPairs()
{
  if(this->Collision) return this->CollisionDetection->GetCollisionPairs();
  else return NULL;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void vtkSimulation::Initialize() {

  if (this->Initialized) return;
  this->Initialized = true;

  //Configure simulation loop
  this->Callback->SetCallback(SimulationRenderingCallback);
  this->Callback->SetClientData(this);

  //Initialize visualization scenario
  this->Scenario->Update();

  //Check if the interactor has been manually set
  if(!this->Interactor)
  {
    this->Interactor = this->Scenario->GetRenderWindow()->GetInteractor();
  }

  this->Interactor->Initialize();

  this->Interactor->AddObserver(vtkCommand::TimerEvent, this->Callback);

  //Check for rendering rate
  if(this->RenderTimerRate == 0)
  {
    vtkErrorMacro("Render time has not been defined");
  }

  this->RenderTimerId = this->Interactor->CreateRepeatingTimer(this->RenderTimerRate);

  //Initialize collision detection process
  if(this->Collision)
  {
    this->CollisionDetection = vtkBioEngInterface::New();

    vtkScenarioObjectCollection * objects = this->Scenario->GetObjects();
    objects->InitTraversal();
    while(vtkScenarioObject * o = objects->GetNextObject())
    {
      vtkScenarioElementCollection * elements = o->GetElements();
      elements->InitTraversal();
      while(vtkScenarioElement * e = elements->GetNextElement())
      {
        e->SetDeltaT(this->RenderTimerRate);
        vtkCollisionModel *m = e->GetCollisionModel();
        if(m)
        {
          m->SetDeltaT(this->RenderTimerRate);
          this->CollisionDetection->AddModel(m);
        }
      }
    }

    this->CollisionDetection->Initialize();
  }

  //FIXME: Redesign interaction
  //Manage user interaction
  if(this->Interaction)
  {
    this->Interactor = this->Scenario->GetRenderWindowInteractor();

    //Initialize simulation interactor style
    if(!this->InteractorStyle) {
      this->InteractorStyle = vtkDefaultInteractorStyle::New();
    }
    this->Interactor->SetInteractorStyle(this->InteractorStyle);
    // Special treatment on each style
    if (this->InteractorStyle->IsA("vtkDefaultInteractorStyle"))
    {
      vtkDefaultInteractorStyle *style = vtkDefaultInteractorStyle::SafeDownCast(this->InteractorStyle);
      style->SetScenario(this->Scenario);
    }
    else if (this->InteractorStyle->IsA("vtkSingleChannelInteractorStyle"))
    {
      vtkSingleChannelInteractorStyle *style = vtkSingleChannelInteractorStyle::SafeDownCast(this->InteractorStyle);
      style->SetScenario(this->Scenario);
      style->Initialize();
    }

    #ifdef VTKESQUI_USE_HAPTICS
      if(this->HapticDevice)
      {
        //Initialize haptic device
        int connected = this->HapticDevice->Init();
        if(connected > 0){
          vtkDebugMacro("Haptic device is connected...");
          //Set tools to be controlled by the device
          vtkScenarioObjectCollection * objects = this->Scenario->GetObjects();
          objects->InitTraversal();
          while(vtkScenarioObject * o = objects->GetNextObject())
          {
            if(o && o->GetType() == vtkScenarioObject::Tool)
            {
              this->HapticDevice->AddTool(vtkTool::SafeDownCast(o));
            }
          }
        }
        else
        {
          vtkErrorMacro("Haptic device is not connected...");
          exit(0);
        }
      }
    #endif
  }
}

//----------------------------------------------------------------------------
void vtkSimulation::Run()
{
  if (!this->Initialized) this->Initialize();
  this->Interactor->Start();
}

//----------------------------------------------------------------------------
void vtkSimulation::Step()
{
  //Check if any collision has occurred
  if(this->Collision){
    this->CollisionDetection->Update();
  }
  this->Scenario->Update();
}

//----------------------------------------------------------------------------
void vtkSimulation::Interact() {

#ifdef VTKESQUI_USE_HAPTICS
  if(this->HapticDevice)
  {
    const char * name = HapticDevice->GetName();
    this->GetHapticDevice()->Update();
  }
#endif
}

//----------------------------------------------------------------------------
void vtkSimulation::Render()
{
  this->Scenario->Render();
}

//----------------------------------------------------------------------------
void vtkSimulation::PrintSelf(ostream& os,vtkIndent indent) {
  //this->Superclass::PrintSelf(os,indent);

  os << indent << "RenderTimerRate: " << this->RenderTimerRate << "\n";
  os << indent << "Gravity: " << this->Gravity[0] << ", " << this->Gravity[1] << ", " << this->Gravity[2] << "\n";
  os << indent << "UseHaptic: " << this->Interaction << "\n";
  os << indent << "Verbose: " << this->Collision << "\n";

}

#ifdef VTKESQUI_USE_HAPTICS
void vtkSimulation::SetHapticDevice(vtkHaptic *Haptic)
{
	this->HapticDevice = Haptic;
}

vtkHaptic *vtkSimulation::GetHapticDevice()
{
	return this->HapticDevice;
}
#endif

