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
#include "vtkTool.h"

void func ( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
	vtkSimulation * sim = static_cast<vtkSimulation*>(clientData);

	if (vtkCommand::TimerEvent == eventId)
	{
		int tid = * static_cast<int *>(callData);
		if (tid == sim->GetHapticTimerId())
		{
			//Do whatever
			//cout << "Haptic\n";
			sim->UpdateHaptic();
		}
		if (tid == sim->GetSimulationTimerId())
		{
			//Do whatever
			//cout << "Simulation\n";
			sim->UpdateScenario();
		}
		else if (tid == sim->GetRenderTimerId())
		{
			//cout << "Render\n";
			vtkRenderWindowInteractor * Interactor = sim->GetInteractor();
			if (Interactor &&
					Interactor->GetRenderWindow() &&
					Interactor->GetRenderWindow()->GetRenderers())
			{
				Interactor->Render();
			}
		}
	}

}

vtkCxxRevisionMacro(vtkSimulation, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulation);

//----------------------------------------------------------------------------
vtkSimulation::vtkSimulation()
{
	this->Scenario = NULL;
	this->Interactor = NULL;
	this->HapticTimerId = 0;
	this->HapticTimerRate = 0;
	this->SimulationTimerId = 0;
	this->SimulationTimerRate = 0;
	this->RenderTimerId = 0;
	this->RenderTimerRate = 0;
	this->UseHaptic = false;
}

//----------------------------------------------------------------------------
vtkSimulation::~vtkSimulation()
{

}

//----------------------------------------------------------------------------
void vtkSimulation::Init() {

	//Init haptic device
#ifndef VTKESQUI_USE_NO_HAPTICS
	if(this->UseHaptic)
	{
		int connected = this->HapticDevice->Init();
		if(connected > 0){
			std::cout << "Haptic device is connected..." << std::endl;
			for(int i = 0; i < this->HapticDevice->GetNumberOfTools(); i++)
			{
				vtkTool * tool =  this->SimulationManager->GetScenario()->GetTool(i);
				tool->UseHapticOn();
				tool->SetHapticDevice(this->HapticDevice);
			}
		}
		else
		{
			std::cout << "Haptic device is not connected... \nPlease startup device and restart the application" << std::endl;
			exit(0);
		}
	}
#endif

	//
	this->Callback = vtkCallbackCommand::New();
	this->Callback->SetCallback(func);
	this->Callback->SetClientData(this);

	//Check if the interactor has been manually set
	if(!this->Interactor)
	{
		this->Interactor = this->Scenario->GetRenderWindow()->GetInteractor();
	}

	this->Interactor->Initialize();

	this->Interactor->AddObserver(vtkCommand::TimerEvent, this->Callback);

	//Check for haptic device resfresh rate
	if(this->HapticTimerRate == 0)
	{
		//Set Default Haptic Rate
		this->HapticTimerRate = 1; //1ms
	}

	this->HapticTimerId = this->Interactor->CreateRepeatingTimer(this->HapticTimerRate);

	//Check for simulation resfresh rate
	if(this->SimulationTimerRate == 0)
	{
		//Set Default Simulation Rate
		this->SimulationTimerRate = 10; //10ms
	}

	this->SimulationTimerId = this->Interactor->CreateRepeatingTimer(this->SimulationTimerRate);

	//Check for rendering rate
	if(this->RenderTimerRate == 0)
	{
		//Set Default Haptic Rate
		this->RenderTimerRate = 30;
	}

	this->RenderTimerId = this->Interactor->CreateRepeatingTimer(this->RenderTimerRate);

	//Init Scenario
	this->Scenario->Init();
}

//----------------------------------------------------------------------------
void vtkSimulation::Run()
{
	this->Interactor->Start();
}

//----------------------------------------------------------------------------
void vtkSimulation::UpdateScenario()
{
	this->Scenario->Update();
}

//----------------------------------------------------------------------------
void vtkSimulation::UpdateHaptic() {

#ifndef VTKESQUI_USE_NO_HAPTICS
	if(this->UseHaptic)
	{
		const char * name = HapticDevice->GetName();
		this->GetHapticDevice()->Update();
	}
#endif
}

//----------------------------------------------------------------------------
void vtkSimulation::PrintSelf(ostream& os,vtkIndent indent) {
	//this->Superclass::PrintSelf(os,indent);

	os << indent << "SimulationTimerRate: " << this->SimulationTimerRate << "\n";
	os << indent << "HapticTimerRate: " << this->HapticTimerRate << "\n";
	os << indent << "RenderTimerRate: " << this->RenderTimerRate << "\n";
	os << indent << "Gravity: " << this->Gravity << "\n";
	os << indent << "UseHaptic: " << this->UseHaptic << "\n";
	os << indent << "Verbose: " << this->Verbose << "\n";

}
