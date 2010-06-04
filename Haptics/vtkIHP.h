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

#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include "vtkHaptic.h"
#include "XiTrocarInterface.h"
#include <stdio.h>
//! Class vtkVSP, wraps the IHP haptic device
class VTK_ESQUI_HAPTICS_EXPORT vtkIHP:public vtkHaptic
{

public:

	vtkTypeRevisionMacro(vtkIHP, vtkHaptic);
	static vtkIHP *New();
	const char *GetClassName() { return "vtkIHP"; }
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	virtual void Update();

	//! Initialize the VSP system
	virtual int Init();

	//! Liberates the VSP system
	virtual void Release();

	//! Update the state of the VSP system
	void UpdateDevice();

	//!Set the initial pitch angle
	void SetInitialPitchAngle(float angle);
	//!Get the initial pitch angle
	float GetInitialPitchAngle();

	//!Set the initial pitch angle
	//void SetInitialYawAngle(float angle);
	//!Get the initial yaw angle
	//float GetInitialYawAngle();

	//! Return the state of the trocar with indicated id
	float *GetTrocarState(int id);
	//! Return the state of the left trocar
	float *GetLeftTrocarState();
	//! Return the state of the right trocar
	float *GetRightTrocarState();

	//! Return the direction of the  trocar with the specified id
	//! (x, y, z) Unit Vector Direction
	float *GetTrocarDirection(int id);
	//! Return the direction of the left trocar
	float *GetLeftTrocarDirection();
	//! Return the direction of the right trocar
	float *GetRightTrocarDirection();

	//! Return the yaw angle (degrees) of the  trocar with the specified id
	float GetTrocarYaw(int id);
	//! Return the direction of the left trocar
	float GetLeftTrocarYaw();
	//! Return the direction of the right trocar
	float GetRightTrocarYaw();

	//! Return the pitch angle (degrees) of the  trocar with the specified id
	float GetTrocarPitch(int id);
	//! Return the  pitch angle of the left trocar
	float GetLeftTrocarPitch();
	//! Return the  pitch angle of the right trocar
	float GetRightTrocarPitch();

	//! Return the depth of the tool with the specified id
	float GetToolDepth(int id);
	//! Return the Depth of the left tool
	float GetLeftToolDepth();
	//! Return the Depth of the right tool
	float GetRightToolDepth();

	//! Return the roll angle (degrees) of the tool with the specified id
	float GetToolRoll(int id);
	//! Return the Roll of the left tool
	float GetLeftToolRoll();
	//! Return the Roll of the right tool
	float GetRightToolRoll();

	//! Return the opening of the tool with the specified id
	float GetToolOpening(int id);
	//! Return the Opening of the left tool
	float GetLeftToolOpening();
	//! Return the Opening of the right tool
	float GetRightToolOpening();

	//! Return the tool main button state with the specified id
	//! 0 -> button is released
	//! 1 -> button is pressed
	int GetToolButtonState(int id);
	//! Return the left tool main button state
	int GetLeftToolButtonState();
	//! Return the right tool main button state
	int GetRightToolButtonState();

	//! Return the tool left button(pedal) state with the specified id
	//! 0 -> button is released
	//! 1 -> button is pressed
	int GetToolLeftButtonState(int id);
	//! Return the left tool left button(pedal) state
	int GetLeftToolLeftButtonState();
	//! Return the right tool left button(pedal) state
	int GetRightToolLeftButtonState();

	//! Return the tool right button(pedal) state with the specified id
	//! 0 -> button is released
	//! 1 -> button is pressed
	int GetToolRightButtonState(int id);
	//! Return the left tool right button(pedal) state
	int GetLeftToolRightButtonState();
	//! Return the right tool right button(pedal) state
	int GetRightToolRightButtonState();

	//! Return Left Pedal State
	int GetLeftPedalState();
	//! Return Left Pedal State
	int GetRightPedalState();

	//! Apply specified forces to the tools
	void ApplyForce(int id);

	//! Flush forces on the tools
	void FlushForces();

	//! Set the tip(normal) force of the tool with the specified id
	//! (x, y, z) force components in Newtons applied to a virtual instrument tip of unit length
	void SetToolTipForce(int id, float * tipForce);
	//! Set the tip force of the left tool
	void SetLeftToolTipForce(float * tipForce);
	//! Set the tip force of the right tool
	void SetRightToolTipForce(float * tipForce);

	//! Set the tip(normal) force of the tool with the specified id
	void SetToolTipForce(int id, float tipForceX, float tipForceY, float tipForceZ);
	//! Set the tip force of the left tool
	void SetLeftToolTipForce(float tipForceX, float tipForceY, float tipForceZ);
	//! Set the tip force of the right tool
	void SetRightToolTipForce(float tipForceX, float tipForceY, float tipForceZ);

	//! Set the roll force of the tool with the specified id
	//! Clockwise (Nm)
	void SetToolRollForce(int id, float rollForce);
	//! Set the roll force of the left tool
	void SetLeftToolRollForce(float rollForce);
	//! Set the roll force of the right tool
	void SetRightToolRollForce(float rollForce);

	//! Set the opening force of the tool with the specified id
	//! Not supported on every tool
	void SetToolOpeningForce(int id, float openingForce);
	//! Set the opening force of the left tool
	void SetLeftToolOpeningForce(float openingForce);
	//! Set the opening force of the right tool
	void SetRightToolOpeningForce(float openingForce);

	private:

	//Haptic Physical Configuration
	//! Initial Pitch Angle
	float InitialPitch;
	//! Initial Pitch Angle
	//float InitialYaw;

	//! Xitact IHP trocar 0 state
	XiToolState Trocar0;// *Trocar0, *Trocar1;
	//! Xitact IHP trocar 1 state
	XiToolState Trocar1;

	//! Xitact IHP trocar 0 force
	XiToolForce Trocar0Force;
	//! Xitact IHP trocar 1 force
	XiToolForce Trocar1Force;

	//! Get the state of the VSP system
	void  RetrieveDeviceState();

protected:
	vtkIHP();
	~vtkIHP();

};

#endif
