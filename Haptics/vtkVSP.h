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
#ifndef _vtkVSP_h
#define _vtkVSP_h

#include "vtkESQuiHapticsWin32Header.h"
#include "vtkHaptic.h"

#include "XiCathInterface.h"

//!vtkVSP class wraps the access to the IHP haptic device.
/*!
 * This interface enables the interaction with the Xitact VSP haptic device.\n
 * In a cyclic process the haptic is synchronously checked for changes.
 * Device physical attributes are saved and then sent to the simulation
 * process.\n
 * Access method to device initialization has also been implemented.
 */

class VTK_ESQUI_HAPTICS_EXPORT vtkVSP:public vtkHaptic
{

public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkVSP, vtkObject);
	//! Create new vtkVSP object
	static vtkVSP *New();
	const char *GetClassName() { return "vtkVSP"; }
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Initialize the VSP system
	virtual int Init();

	//!Update the device
	virtual void Update();

	//! Liberates the VSP system
	virtual void Release();

	//! Update the state of the VSP system
	void UpdateDevice();

	//! Return the position of the left joystick
	float *GetLeftJoystickState();

	//! Return the position of the left joystick
	void GetLeftJoystickState(float data[2]);

	//! Return the position of the left joystick
	void GetLeftJoystickState(float *data1,float *data2);

	//! Return the position of the right joystick
	float *GetRightJoystickState();

	//! Return the position of the left joystick
	void GetRightJoystickState(float data[2]);

	//! Return the position of the left joystick
	void GetRightJoystickState(float *data1,float *data2);

	//! Return the Y position of the left joystick
	float GetLeftJoystickYPosition();

	//! Return the X position of the left joystick
	float GetLeftJoystickXPosition();

	//! Return the Y position of the right joystick
	float GetRightJoystickYPosition();

	//! Return the X position of the right joystick
	float GetRightJoystickXPosition();

	//! Return the state of the buttons of the control panel
	int GetButtonsState();

	//! Return Zoom button state
	int GetZoomState();

	//! Return Visualization Mode
	int GetVisualizationMode();

	//! Get Sid button state
	int GetSidState();

	//! Get Table button state
	int GetTableState();

	//! Return Left Pedal Button State
	int GetLeftPedalState();

	//! Return Center Pedal Button State
	int GetCenterPedalState();

	//! Return Right Pedal Button State
	int GetRightPedalState();

	//!Return the depth of the catheter specified by the id parameter
	float GetCatheterDepth(int id)
	{
		switch(id)
		{
			case 0:
				return this->GetCatheter0Depth();
			case 1:
				return this->GetCatheter1Depth();
			case 2:
				return this->GetCatheter2Depth();
			default:
				return 0;
		}
	}

	//!Return the state of the catheter specified by the id parameter
	int GetCatheterState(int id)
	{
		switch(id)
		{
			case 0:
				return this->GetCatheter0State();
			case 1:
				return this->GetCatheter1State();
			case 2:
				return this->GetCatheter2State();
			default:
				return 0;
		}
	}

	//!Return the roll of the catheter specified by the id parameter
	float GetCatheterRoll(int id)
	{
		switch(id)
		{
			case 0:
				return this->GetCatheter0Roll();
			case 1:
				return this->GetCatheter1Roll();
			case 2:
				return this->GetCatheter2Roll();
			default:
				return 0;
		}

	}

	//! Return the state flag of the 8F catheter
	int GetCatheter0State();
	//! Return the depth of the 8F catheter
	float GetCatheter0Depth();
	//! Return the roll value of the 8F catheter
	float GetCatheter0Roll();

	//! Return the state flag of the 5F catheter
	int GetCatheter1State();
	//! Return the depth of the 5F catheter
	float GetCatheter1Depth();
	//! Return the roll value of the 5F catheter
	float GetCatheter1Roll();

	//! Return the state flag of the 2.5F catheter
	int GetCatheter2State();
	//! Return the depth of the 2.5F catheter
	float GetCatheter2Depth();
	//! Return the roll value of the 2.5F catheter
	float GetCatheter2Roll();

	//!Return Stent Deployment
	float GetStentDeployment();

	//!Return Contrast Flow
	float GetContrastFlow();

	//!Return Inflation Pressure
	float GetInflationPressure();

private:
	XiControlPanelState DevicePanel;
	XiCathState Catheter0;// *Catheter2, *Catheter1;
	XiCathState Catheter1;
	XiCathState Catheter2;

protected:
	vtkVSP();
	~vtkVSP();

};

#endif
