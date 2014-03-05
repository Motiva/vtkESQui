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

#ifndef _vtkSBM_h
#define _vtkSBM_h

#include "vtkESQuiHapticsWin32Header.h"
#include "vtkHaptic.h"
#include "SimballMedicalHID.h"

//! vtkSBM class wraps the access to the Simball haptic device.
/*!
 * This interface enables the interaction with the Simball 4D haptic
 * device.\n
 * In a cyclic process the haptic is synchronously checked for changes.
 * Device physical attributes are saved and then sent to the simulation
 * process.\n
 * Access method to device initialization has also been implemented.
 */
class VTK_ESQUI_HAPTICS_EXPORT vtkSBM:public vtkHaptic
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkSBM, vtkHaptic);
	//! Create new vtkSBM object
	static vtkSBM *New();
	
	const char *GetClassName() { return "vtkSBM"; }
	//!Print object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Initialize the VSP system
	virtual int Init();
	
	//! Update the Scenario tools
	virtual void Update(){}
	
	void UpdateScenario();

	//! Liberates the VSP system
	virtual void Release();

	// SBM specific methods - - - - - - - - - - - - - - - - - - - -
	
	//! Update the state of the SBM system
	void UpdateDevice();

	//! Return the yaw angle (degrees) of the left trocar
	double GetLeftTrocarYaw();
	//! Return the yaw angle (degrees) of the right trocar
	double GetRightTrocarYaw();

	//! Return the  pitch angle (degrees) of the left trocar
	double GetLeftTrocarPitch();
	//! Return the  pitch angle (degrees) of the right trocar
	double GetRightTrocarPitch();

	//! Return the Depth of the left tool
	double GetLeftToolDepth();
	//! Return the Depth of the right tool
	double GetRightToolDepth();

	//! Return the Roll angle (degrees) of the left tool
	double GetLeftToolRoll();
	//! Return the Roll angle (degrees) of the right tool
	double GetRightToolRoll();

	//! Return the Opening of the left tool
	double GetLeftToolOpening();
	//! Return the Opening of the right tool
	double GetRightToolOpening();
	
	//! Return the tool main button 'pressed' state with the specified id
	//! There are 3 buttons per device
	bool GetToolButtonState(int id, int button);

	//! Return Left Pedal State
	bool GetLeftPedalState();
	//! Return Left Pedal State
	bool GetRightPedalState();
	
	void SetLensAngle(double angle);
	
	vtkSetMacro(ToolInsertion, double);
	vtkGetMacro(ToolInsertion, double);

private:

	//! Initialized flag
	bool Initialized;	
	//! Return the yaw angle (degrees) of the  trocar with the specified id
	double GetTrocarYaw(int id);
	//! Return the pitch angle (degrees) of the  trocar with the specified id
	double GetTrocarPitch(int id);
	//! Return the depth of the tool with the specified id
	double GetToolDepth(int id);
	//! Return the Roll angle (degrees) of the tool with the specified id
	double GetToolRoll(int id);
	//! Return the opening of the tool with the specified id
	double GetToolOpening(int id);
	//! Number of devices detected
	int ndevices;
	//! Stored values obtained from each device.
	struct SBMDeviceValues *values;
	//! Identification string of each device
	char **idStr;
	//! Description string of each device
	char **desc;
	double ToolInsertion;

protected:
	vtkSBM();
	~vtkSBM();
};

#endif