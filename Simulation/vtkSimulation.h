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

#include "vtkESQuiSimulationWin32Header.h"
#include "vtkObject.h"

class vtkScenario;
class vtkRenderWindowInteractor;
class vtkSimulationInteractorStyle;
class vtkCollisionDetection;
class vtkCollisionCollection;
class vtkCallbackCommand;

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#endif

//! Implementation of the vtkESQui simulation process.
/*!
 * Simulation process is executed with timer callback function that handles
 * three timed threaded loops at different rates:
 * - Scenario
 * - Collision
 * - Interaction
 */

class VTK_ESQUI_SIMULATION_EXPORT vtkSimulation: public vtkObject
{
public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkSimulation, vtkObject);
	//! Create new vtkSimulation object
	static vtkSimulation *New();

	//! Return class name
	const char *GetClassName() { return "vtkSimulation"; }

	//! Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set simulation render window interactor
	/*!
	 * \sa GetInteractor()
	 */
	void SetInteractor(vtkRenderWindowInteractor * i);

	//! Get simulation render window interactor
	/*!
	 * \sa SetInteractor(vtkRenderWindowInteractor *iren)
	 */
	vtkRenderWindowInteractor * GetInteractor();

	//! Set simulation interactor style
	/*!
	 * \sa GetInteractorStyle()
	 */
	void SetInteractorStyle(vtkSimulationInteractorStyle * style);

	//! Get simulation interactor style
	/*!
	 * \sa SetInteractor(vtkSimulationInteractorStyle *style)
	 */
	vtkSimulationInteractorStyle * GetInteractorStyle();

	//! Set the simulation scenario: organs, tools, etc...
	/*!
	 * Contains the information of the scenario items: organs, tools, contacts, etc...
	 */
	void SetScenario(vtkScenario * s);

	//! Return the scenario of the simulation
	vtkScenario * GetScenario();

	//! Return the collision collection
	vtkCollisionCollection * GetCollisions();

	//! Set haptic device timer refresh rate (s)
	vtkSetMacro(InteractionTimerRate, double);

	//! Set simulation refresh rate (s)
	vtkSetMacro(SimulationTimerRate, double);

	//! Set render refresh rate (s)
	vtkSetMacro(RenderTimerRate, double);

	//! Get haptic device timer refresh rate (s)
	vtkGetMacro(InteractionTimerRate, double);

	//! Get simulation refresh rate (s)
	vtkGetMacro(SimulationTimerRate, double);

	//! Get render refresh rate (s)
	vtkGetMacro(RenderTimerRate, double);

	//! Get haptic device timer identifier
	vtkGetMacro(InteractionTimerId, vtkIdType);

	//! Get simulation timer identifier
	vtkGetMacro(SimulationTimerId, vtkIdType);

	//! Get render timer identifier
	vtkGetMacro(RenderTimerId, vtkIdType);

	//! Set the interaction in the simulation
	vtkSetMacro(Interaction, bool);
	//! Get the interaction in the simulation
	vtkGetMacro(Interaction, bool);

	//! Enable the user interaction
	vtkBooleanMacro(Interaction, bool);

	//! Set the collision detection
	vtkSetMacro(Collision, bool);
	//! Get the collision detection
	vtkGetMacro(Collision, bool);

	//!Enable the verbose
	vtkBooleanMacro(Collision, bool);

	//! Set Gravity Force (m/s2)
	vtkSetVector3Macro(Gravity, double);
	//! Get Gravity Force (m/s2)
	vtkGetVector3Macro(Gravity, double);

	//! Initializes the simulation (Haptic, Manager, etc...)
	void Init();

	//! Starts the simulation
	void Run();

	//!Update the simulation scenario
	void UpdateScenario();

	//!Update user interaction values
	void UpdateInteraction();

#ifndef VTKESQUI_USE_NO_HAPTICS
	//BTX
	//! Set the haptic device for the simulation///
	void SetHapticDevice(vtkHaptic *Haptic) {this->HapticDevice = Haptic;}
	//! Get the haptic device for the simulation
	vtkHaptic *GetHapticDevice() {return this->HapticDevice;}
private:
	//! The haptic device for the simulator
	vtkHaptic *HapticDevice;
	//ETX
#endif

protected:

	vtkSimulation();
	~vtkSimulation();

	//!Simulation callback command. Acts as a multirate timer
	vtkCallbackCommand * Callback;

	vtkIdType InteractionTimerId;
	double InteractionTimerRate;
	vtkIdType SimulationTimerId;
	double SimulationTimerRate;
	vtkIdType RenderTimerId;
	double RenderTimerRate;

	//!Simulation scenario
	vtkScenario * Scenario;

	//!Window interactor to control simulation display
	vtkRenderWindowInteractor *Interactor;

	//!Window interactor style to control simulation objects
	vtkSimulationInteractorStyle *InteractorStyle;

	//!collision detection library
	vtkCollisionDetection* CollisionDetection;

	//! Enable/disable user interaction
	bool Interaction;

	//! Enable/disable collision detection
	bool Collision;

	//! Gravitational force
	double Gravity[3];

private:

	vtkSimulation (const vtkSimulation &); //Not Implemented
	void operator =(const vtkSimulation &); //Not Implemented

};
#endif

