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
