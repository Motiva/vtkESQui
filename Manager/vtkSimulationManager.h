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
#ifndef _vtkSimulationManager_
#define _vtkSimulationManager_

#include "vtkCollisionDetectionLibrary.h"
#include "vtkObject.h"
#include "vtkScenario.h"
#include "vtkOrganCollection.h"
#include "vtkToolCollection.h"
#include "vtkContactCollection.h"
#include "vtkContact.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkESQuiManagerWin32Header.h"
#include "vtkBioEngInterface.h"

//! Implementation of the simulation manager. Checks for collisions and executes updating methods of the whole scenario
class VTK_ESQUI_MANAGER_EXPORT vtkSimulationManager: public vtkObject
{

public:
	vtkTypeRevisionMacro(vtkSimulationManager, vtkObject);
	static vtkSimulationManager *New();
	const char *GetClassName() {return "vtkCollisionManager";};

	//! Perform a full update of the whole system.
	/*!
	* Checks if any collision has occurred and performs and update of the whole scenario
	*/
	void Update();

	//!Initializes the simulation manager
	void Init();

	//! Set the scenario items: organs, tools, etc...
	/*!
	* \param scenario class vtkScenario contains the information of the scenario items: organs, tools, contacts, etc...
	*/
	void SetScenario(vtkScenario * scenario);

	//!Return the scenario of the simulation
	vtkScenario * GetScenario();

	//! Set the library used for collision detection
	/*!
	* \param library name(string) of the collision detection library.
	*/
	void SetCollisionDetectionLibrary (const char *library);

	//! Return of the collection of contact points between organs and tools
	vtkContactCollection * GetContacts();

	//! Reset the collision manager
	void Reset(){};

protected:
	//BTX
	//Collision detection library name
	const char* libraryName;
	//ETX

	vtkSimulationManager();
	~vtkSimulationManager();

private:
	vtkSimulationManager(const vtkSimulationManager &); //NotImplemented
	void operator =(const vtkSimulationManager &); //Not Implemented

	//!Simulation scenario
	vtkScenario * Scenario;

	//!collision detection library
	vtkCollisionDetectionLibrary* collisionLibrary;

};
#endif 
