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
#include "vtkLSW.h"
#include "vtkObjectFactory.h"

#include "vtkMath.h"

#include "vtkLSWTool.h"

vtkCxxRevisionMacro(vtkLSW, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkLSW);

//--------------------------------------------------------------------------
vtkLSW::vtkLSW()
{
	this->LSWDevice = NULL;
	this->LSWLeftTool = vtkLSWTool::New();
	this->LSWRightTool = vtkLSWTool::New();
	this->ForceCalculationMethodActive = false;
}

//--------------------------------------------------------------------------
vtkLSW::~vtkLSW()
{
	if(this->LSWLeftTool) this->LSWLeftTool->Delete();
	if(this->LSWRightTool) this->LSWRightTool->Delete();
	ImmSurgicalWorkstationFactory::ReleaseSW(this->LSWDevice);
}

//--------------------------------------------------------------------------
int vtkLSW::Init()
{
	//Acquire Surgical Workstation control
	cout << "Acquiring Immersion Surgical Workstation..." << endl;
	this->LSWDevice = ImmSurgicalWorkstationFactory::AcquireSW();
	if(!this->LSWDevice)
	{
		cout << "Could not acquire the ImmSurgicalWorkstation object!" << endl;
		return VTK_DETECT_FAILURE_LSW;
	}
	else
	{
		cout << "Calibrating Device ..." <<  endl;
		int Result = this->LSWDevice->Calibrate();
		if (Result != 0){
			ImmSurgicalWorkstationFactory::ReleaseSW(LSWDevice);
			return VTK_CALIBRATE_FAILURE_LSW;
		}
		else
		{
			cout << "Device has been properly calibrated" <<  endl;
		}
	}
	//Acquire both, left & right, tools
	if (this->LSWDevice)
	{
		if(this->LSWRightTool->ToolAssign(this->LSWDevice->GetRightTool()) )
		{
			this->LSWRightTool->Initialize();
			cout << this->LSWRightTool->GetDeviceName()  << " initialized"<< endl;
		}
		else
		{
			cout << "There is no right tool!" << endl;
			return VTK_DETECT_FAILURE_LSW;
		}

		if (this->LSWLeftTool->ToolAssign(this->LSWDevice->GetLeftTool()) )
		{
			this->LSWLeftTool->Initialize();
			cout << this->LSWLeftTool->GetDeviceName()  << " initialized"<< endl;
		}
		else
		{
			cout << "There is no left tool!" << endl;
			return VTK_DETECT_FAILURE_LSW;
		}
	}

	if (this->LSWDevice->GetIsSuspended())
		this->LSWDevice->WakeUp();
	return VTK_CORRECT_LSW;
}

//--------------------------------------------------------------------------
void vtkLSW::Update()
{
	for (int n = 0; n < vtk_FORCE_STEP; n++)
	{
		this->UpdateDevice();
	}
}

//--------------------------------------------------------------------------
void vtkLSW::Release()
{
	ImmSurgicalWorkstationFactory::ReleaseSW(this->LSWDevice);
	return;
};

//--------------------------------------------------------------------------
double vtkLSW::GetToolDepth(int id)
{
	if(id == 0)
	{
		return this->GetLeftToolDepth();
	}
	else
	{
		return this->GetRightToolDepth();
	}
}

//--------------------------------------------------------------------------
double vtkLSW::GetLeftToolDepth()
{
	//Haptic Range [40, 216]
	// Return Range [0, 1] -> Correction Factor (K) = 0.005686
	float K= 0.0057;
	return (this->LSWLeftTool->GetToolDepth()-40)*K;
}

//--------------------------------------------------------------------------
double vtkLSW::GetRightToolDepth()
{
	//Range [40, 216]
	// Return Range [0, 1] -> Correction Factor (K) = 0.005686
	float K= 0.0057;
	return (this->LSWRightTool->GetToolDepth()-40)*K;
}

//--------------------------------------------------------------------------
double vtkLSW::GetToolYaw(int id)
{
	if(id == 0)
	{
		return this->GetLeftToolYaw();
	}
	else
	{
		return this->GetRightToolYaw();
	}
}

//--------------------------------------------------------------------------
double vtkLSW::GetLeftToolYaw()
{
	// Haptic API provides Yaw angle value
	// Haptic Range = [InitialAngle +- 50]; Initial Angle=-30.
	// Angle in degrees must be translated to VTK coordinate system
	// Initial angle=-30 -> Range [-30, 90]
	return vtkMath::DegreesFromRadians(acos(this->LSWLeftTool->GetToolYaw()))-90-30;
}

//--------------------------------------------------------------------------
double vtkLSW::GetRightToolYaw()
{
	// Haptic API provides Yaw angle value
	// Haptic Range = [InitialAngle +- 50]; Initial Angle=-30.
	// Angle in degrees must be translated to VTK coordinate system
	// Initial angle=30 -> Range [-90, 30]
	return vtkMath::DegreesFromRadians(acos(this->LSWRightTool->GetToolYaw()))-90+30;
}

//--------------------------------------------------------------------------
double vtkLSW::GetToolPitch(int id)
{
	if(id == 0)
	{
		return this->GetLeftToolPitch();
	}
	else
	{
		return this->GetRightToolPitch();
	}
}

//--------------------------------------------------------------------------
double vtkLSW::GetLeftToolPitch()
{
	// Haptic API provides Pitch angle value
	// Range [-0.87,0.87] (Radians) [-60, 60] Degrees
	// Angle in degrees must be translated to VTK coordinate system
	// Initial angle=-30 -> Range [0, 120] Horizontal plane is taken as reference
	return vtkMath::DegreesFromRadians(-acos(this->LSWLeftTool->GetToolPitch()))+30;
}

//--------------------------------------------------------------------------
double vtkLSW::GetRightToolPitch()
{
	// Haptic API provides Pitch angle value
	// Range [-0.87,0.87] (Radians) [-60, 60] Degrees
	// Angle in degrees must be translated to VTK coordinate system
	// Initial angle=-30 -> Range [0, 120]
	return vtkMath::DegreesFromRadians(-acos(this->LSWRightTool->GetToolPitch()))+30;
}

//--------------------------------------------------------------------------
double vtkLSW::GetToolRoll(int id)
{
	if(id == 0)
	{
		return this->GetLeftToolRoll();
	}
	else
	{
		return this->GetRightToolRoll();
	}
}

//--------------------------------------------------------------------------
double vtkLSW::GetLeftToolRoll()
{
	// Angle is returned in degrees
	return vtkMath::DegreesFromRadians(this->LSWLeftTool->GetToolRoll());
}

//--------------------------------------------------------------------------
double vtkLSW::GetRightToolRoll()
{
	// Angle is returned in degrees
	return vtkMath::DegreesFromRadians(this->LSWRightTool->GetToolRoll());
}

//--------------------------------------------------------------------------
double vtkLSW::GetToolOpening(int id)
{
	if(id == 0)
	{
		return this->GetLeftToolOpening();
	}
	else
	{
		return this->GetRightToolOpening();
	}
}

//--------------------------------------------------------------------------
double vtkLSW::GetLeftToolOpening()
{
	// Aperture is returned in 0..1 scale
	return this->LSWLeftTool->GetToolOpening();
}

//--------------------------------------------------------------------------
double vtkLSW::GetRightToolOpening()
{
	return this->LSWRightTool->GetToolOpening();
}

//--------------------------------------------------------------------------
void vtkLSW::UpdateDevice()
{
	this->LSWDevice->Update();
	this->LSWRightTool->Refresh();
	this->LSWLeftTool->Refresh();
}

//--------------------------------------------------------------------------
void vtkLSW::SetToolForce(int id, float X, float Y, float Z)
{
	if(id == 0)
	{
		return this->SetLeftToolForce(X, Y, Z);
	}
	else
	{
		return this->SetRightToolForce(X, Y, Z);
	}
}

//--------------------------------------------------------------------------
void vtkLSW::SetLeftToolForce(float X, float Y, float Z) {
	ImmCoord3 Force;
	Force.x = X;
	Force.y = Y;
	Force.z = Z;
	this->LSWLeftTool->SetToolForce(Force);
}

//--------------------------------------------------------------------------
void vtkLSW::SetRightToolForce(float X, float Y, float Z) {
	ImmCoord3 Force;
	Force.x = X;
	Force.y = Y;
	Force.z = Z;
	this->LSWRightTool->SetToolForce(Force);
}

//--------------------------------------------------------------------------
void vtkLSW::SetToolData(int id, int Type, double k, int flag)
{
	if(id == 0)
	{
		return this->SetLeftToolData(Type, k, flag);
	}
	else
	{
		return this->SetRightToolData(Type, k, flag);
	}
}

//--------------------------------------------------------------------------
void vtkLSW::SetLeftToolData(int Type, double k, int flag){
	//cout << "SET DATA LEFT" << endl;
	this->LSWLeftTool->SetData(flag, Type, k);
}

//--------------------------------------------------------------------------
void vtkLSW::SetRightToolData(int Type, double k, int flag){
	//cout << "SET DATA RIGTH" << endl; 
	this->LSWRightTool->SetData(flag, Type, k);
}

//--------------------------------------------------------------------------
void vtkLSW::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);
}
