#ifndef _vtkVSP_h
#define _vtkVSP_h
#include "vtkESQuiHapticsWin32Header.h"

#include "vtkObjectFactory.h"
#include "vtkHaptic.h"
#include "XiCathInterface.h"
#include <stdio.h>
//! Class vtkVSP, wraps the VSP haptic device
class VTK_ESQUI_HAPTICS_EXPORT vtkVSP:public vtkHaptic
{

public:

	vtkTypeRevisionMacro(vtkVSP, vtkObject);
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
	float GetLeftJoystickYPosition() { return this->DevicePanel.joyLeft.up;}

	//! Return the X position of the left joystick
	float GetLeftJoystickXPosition() { return this->DevicePanel.joyLeft.right;}

	//! Return the Y position of the right joystick
	float GetRightJoystickYPosition() { return this->DevicePanel.joyRight.up;}

	//! Return the X position of the right joystick
	float GetRightJoystickXPosition() { return this->DevicePanel.joyRight.right;}

	//! Return the state of the buttons of the control panel
	int GetButtonsState() {return this->DevicePanel.buttons;}

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
	float GetStentDeployment(){return this->DevicePanel.stentDeployment;};

	//!Return Contrast Flow
	float GetContrastFlow(){return this->DevicePanel.contrastFlow;};

	//!Return Inflation Pressure
	float GetInflationPressure(){return this->DevicePanel.inflationPressure;};

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
