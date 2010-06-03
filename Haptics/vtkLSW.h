#ifndef _vtkEsquiHaptics_h
#define _vtkEsquiHaptics_h
#include "vtkESQuiHapticsWin32Header.h"

#include "vtkHaptic.h"
#include "vtkLSWTool.h"
#include "vtkMath.h"
#include <stdio.h>

static const unsigned VTK_CORRECT_LSW = 1;
static const unsigned VTK_DETECT_FAILURE_LSW = 0;
static const unsigned VTK_CALIBRATE_FAILURE_LSW = -1;

//! Class vtkLSW, wraps the LSW haptic device
class VTK_ESQUI_HAPTICS_EXPORT vtkLSW:public vtkHaptic
{

public:

	vtkTypeRevisionMacro(vtkLSW, vtkHaptic);

	static vtkLSW *New();
	const char *GetClassName() { return "vtkLSW"; }
	//vtkTypeRevisionMacro(vtkLSW,vtkTool);
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set up the LSW
	virtual int Init();

	//!Update the device
	virtual void Update();

	//! Close the communication and turn off the LSW
	virtual void Release();

	//!Update the device
	void UpdateDevice();

	//! Get the tool depth
	double GetToolDepth(int id);
	//! Get the tool yaw
	double GetToolYaw(int id);
	//! Get the tool pitch
	double GetToolPitch(int id);
	//! Get the tool aperture
	double GetToolOpening(int id);
	//! Get the tool twist
	double GetToolRoll(int id);

	//! Get the left tool depth
	double GetLeftToolDepth();
	//! Get the left tool yaw
	double GetLeftToolYaw();
	//! Get the left tool pitch
	double GetLeftToolPitch();
	//! Get the left tool aperture
	double GetLeftToolOpening();
	//! Get the left tool twist
	double GetLeftToolRoll();

	//! Get the right tool depth
	double GetRightToolDepth();
	//! Get the right tool yaw
	double GetRightToolYaw();
	//! Get the right tool pitch
	double GetRightToolPitch();
	//! Get the right tool aperture
	double GetRightToolOpening();
	//! Get the right tool twist
	double GetRightToolRoll();

	//! Set the specified tool contact force
	void SetToolForce(int id, float X, float Y, float Z);
	//! Set the right tool contact force
	void SetRightToolForce(float X, float Y, float Z);
	//! Set the left tool contact force
	void SetLeftToolForce(float X, float Y, float Z);

	//!Set data into the specified tool
	void SetToolData(int id, int Type, double k, int flag);
	//!Set data into the left tool
	void SetLeftToolData(int Type, double k, int flag);
	//!Set data into the left tool
	void SetRightToolData(int Type, double k, int flag);
private:

	bool StopFlag;
	bool ForceCalculationMethodActive;
	// Objeto vtkLSWTool para abstraer el control de la herramienta derecha de la LSW
	//! vtkLSWTool object to abstract the LSW's right tool
	vtkLSWTool* LSWRightTool;
	// Objeto vtkLSWTool para abstraer el control de la herramienta izquierda de la LSW
	//! vtkLSWTool object to abstract the LSW's left tool
	vtkLSWTool* LSWLeftTool;
	// Objeto de la ImmSurgicalWorkstation API de la LSW, hace de interfaz con la LSW
	//! ImmSurgicalWorkstation object,
	ImmSurgicalWorkstation* LSWDevice;

protected:

	vtkLSW();
	~vtkLSW();
};

#endif
