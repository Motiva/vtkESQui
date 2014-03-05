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
#include "vtkInteractorStyle.h"

#ifndef VTKESQUI_USE_HAPTICS
/* #undef VTKESQUI_USE_HAPTICS */
#endif

class vtkScenario;
class vtkRenderWindowInteractor;
class vtkDefaultInteractorStyle;
class vtkCollisionDetection;
class vtkCollisionCollection;
class vtkIntArray;
class vtkCallbackCommand;
#ifdef VTKESQUI_USE_HAPTICS
class vtkHaptic;
#endif
 
//! Implementation of the vtkESQui simulation process.

/*!
 * Simulation process is executed with timer callback function that handles
 * three processes at same time:
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
  void SetInteractorStyle(vtkInteractorStyle * style);

  //! Get simulation interactor style
  /*!
   * \sa SetInteractor(vtkSimulationInteractorStyle *style)
   */
  vtkInteractorStyle * GetInteractorStyle();

  //! Set the simulation scenario: organs, tools, etc...
  /*!
   * Contains the information of the scenario items: organs, tools, contacts, etc...
   */
  void SetScenario(vtkScenario * s);

  //! Return the scenario of the simulation
  vtkScenario * GetScenario();

  //! Set the collision mode to simple
  /*!
   * The simulation must be initialized to set the collision mode
   */
  void SetCollisionModeToSimple();

  //! Set the collision mode to extended
  /*!
   * The simulation must be initialized to set the collision mode
   */
  void SetCollisionModeToExtended();

  //! Set the collision mode to full
  /*!
   * The simulation must be initialized to set the collision mode
   */
  void SetCollisionModeToFull();

  //! Get the simulation object collisions
  vtkCollisionCollection * GetCollisions();

  //! Get the simulation object collisions
  vtkIntArray * GetCollisionPairs();

  //! Assign Element name
  /*!
   *\sa GetName()
   */
  vtkSetStringMacro(Name);

  //! Return Element scale
  /*!
   *\sa SetName(const char * name)
   */
  vtkGetStringMacro(Name);

  //! Set render refresh rate (ms)
  vtkSetMacro(RenderTimerRate, double);

  //! Get render refresh rate (ms)
  vtkGetMacro(RenderTimerRate, double);

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
  void Initialize();

  //! Starts the simulation
  void Run();

  //!Render simulation scenario
  void Render();

  //!Update the simulation scenario
  void Step();

  //!Update user interaction values
  void Interact();
  
#ifdef VTKESQUI_USE_HAPTICS
  //! Set the haptic device for the simulation///
  void SetHapticDevice(vtkHaptic *Haptic);
  //! Get the haptic device for the simulation
  vtkHaptic *GetHapticDevice();
private:
  //! The haptic device for the simulator
  vtkHaptic *HapticDevice;
#endif

protected:

  vtkSimulation();
  ~vtkSimulation();

  //!Simulation callback command. Acts as a multirate timer
  vtkCallbackCommand * Callback;

  vtkIdType RenderTimerId;
  double RenderTimerRate;
  
  //! Initialization flag
  bool Initialized;

  //! Simulation name
  char * Name;

  //!Simulation scenario
  vtkScenario * Scenario;

  //!Window interactor to control simulation display
  vtkRenderWindowInteractor *Interactor;

  //!Window interactor style to control simulation objects
  vtkInteractorStyle *InteractorStyle;

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

