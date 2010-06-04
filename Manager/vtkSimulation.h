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
#ifndef __vtkSimulation_h
#define __vtkSimulation_h

#include "vtkESQuiManagerWin32Header.h"

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkCallbackCommand.h"

#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"

#include "vtkSimulationManager.h"
#include "vtkScenario.h"
#include "vtkTool.h"

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#endif


class VTK_ESQUI_MANAGER_EXPORT vtkSimulation: public vtkObject
{
public:

	// Instancia un nuevo objeto
	//!Create new vtkSimulation object
	vtkTypeRevisionMacro(vtkSimulation, vtkObject);
	static vtkSimulation *New();

	//Devuelve el nombre de la clase
	//!Return class name
	const char *GetClassName() { return "vtkSimulation"; }

	// Imprime el valor de los atributos
	//!Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Initializes the simulation (Haptic, Manager, etc...)
	void Init();

	//!Starts the simulation
	void Run();

	//! Set the collision detection manager
	/*!
	* \param manager collision detection manager
	* \sa GetCollisionManager
	*/
	void SetSimulationManager(vtkSimulationManager *manager);

	//! Get the collision detection manager
	/*!
	* \sa SetCollisionManager(vtkCollisionManager *manager)
	*/
	vtkSimulationManager* GetSimulationManager();

	//!Set simulation render window interactor
	/*!
	* \sa GetRenderWindowInteractor()
	*/
	void SetRenderWindowInteractor(vtkRenderWindowInteractor *iren);

	//!Get simulation render window interactor
	/*!
	* \sa SetRenderWindowInteractor(vtkRenderWindowInteractor *iren)
	*/
	vtkRenderWindowInteractor * GetRenderWindowInteractor();

	//Set/Get timer refresh rates
	//!Set haptic device timer refresh rate (ms)
	void SetHapticTimerRate(double rate);

	//!Set simulation refresh rate (ms)
	void SetSimulationTimerRate(double rate);

	//!Set render refresh rate (ms)
	void SetRenderTimerRate(double rate);

	//!Get haptic device timer refresh rate (ms)
	double GetHapticTimerRate();

	//!Get simulation refresh rate (ms)
	double GetSimulationTimerRate();

	//!Get render refresh rate (ms)
	double GetRenderTimerRate();

	//Get timer identifiers for event handling
	//!Get haptic device timer identifier
	int GetHapticTimerId();

	//!Get simulation timer identifier
	int GetSimulationTimerId();

	//!Get render timer identifier
	int GetRenderTimerId();

	//! Set/Get the use of haptic in the simulation
	void SetUseHaptic(bool value){this->UseHaptic = value;};
	bool GetUseHaptic(){return this->UseHaptic;};

	//!Enable the use of haptic
	void UseHapticOn(){this->SetUseHaptic(true);};
	//!Disable the use of haptic
	void UseHapticOff(){this->SetUseHaptic(false);};

	//!Update the simulation Manager. The whole Scenario is updated
	void UpdateManager();

	//!Update the haptic state and obtains the value of the force feedback for the haptic device
	void UpdateHaptic();

#ifndef VTKESQUI_USE_NO_HAPTICS
	//BTX
	//! Set the haptic device for the simulation///
	void SetHapticDevice(vtkHaptic *Haptic) {this->HapticDevice = Haptic;}
	//! Get the haptic device for the simulation
	vtkHaptic *GetHapticDevice() {return this->HapticDevice;}
protected:
	//! The haptic device for the simulator
	vtkHaptic *HapticDevice;
	//ETX
#endif

protected:

	vtkSimulation();
	~vtkSimulation();

	//!Collision detection manager
	vtkSimulationManager *SimulationManager;

	//!Window interactor to control simulation display
	vtkRenderWindowInteractor *Interactor;

	//! Enable/disable haptic device usage
	bool UseHaptic;

private:

	vtkSimulation (const vtkSimulation &); //Not Implemented
	void operator =(const vtkSimulation &); //Not Implemented

	//!Simulation callback command. Acts as a multirate timer
	vtkCallbackCommand * Callback;

	int HapticTimerId;
	double HapticTimerRate;
	int SimulationTimerId;
	double SimulationTimerRate;
	int RenderTimerId;
	double RenderTimerRate;

};
#endif

