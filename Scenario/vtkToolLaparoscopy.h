#ifndef __vtkToolLaparoscopy
#define __vtkToolLaparoscopy

#include "vtkESQuiScenarioWin32Header.h"

#include "vtkObject.h"
#include "vtkTransform.h"
#include "vtkTransformCollection.h"
#include "vtkTool.h"

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#include "vtkIHP.h"
#include "vtkLSW.h"
#endif

// Fichero que implementa la clase vtkToolLaparoscopy, para abstraerse del tipo de herramienta
/*
    vtkToolLaparoscopy es la clase que da soporte a la simulacion para abstraerse del tipo de
    herramienta utilizada. De esta forma se pueden usar colecciones de herramientas sin
    problema alguno.
 */

//! Class vtkToolLaparoscopy, abstract the use of a surgical laparoscopic tool
/*!
vtkToolLaparoscopy abstracts the use of a surgical tool during the simulation exercise.
This provide an easy use of surgical tools collections.
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkToolLaparoscopy: public vtkTool {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkToolLaparoscopy,vtkTool);

	//!Return the class name
	const char *GetClassName() {return "vtkToolLaparoscopy";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);


	//!Abstract initialization function
	/*!
	* This method initializes the tool physical values, scale, position, etc...
	*/
	virtual void Init() = 0;

	//!Specific initialization function
	void ApplyInitialTransform();

	//!Abstract update function
	/*!
	* This method updates the tool physical values, scale, position, etc...
	*/
	virtual void Update();

	//!Assign the haptic force as vector
	/*!
	* \param Force force vector of components
	*/
	void SetForceFeedback(float Force[]);

	//!Assign the haptic force components individually
	/*!
	* \param x x component of the force
	* \param y y component of the force
	* \param z z component of the force
	*/
	void SetForceFeedback(float x, float y, float z) {
		this->ForceFeedback[0] = x;
		this->ForceFeedback[1] = y;
		this->ForceFeedback[2] = z;
	}

	//!Return the haptic force in float vector
	/*!
	* \sa SetForceFeedback(float Force[])
	* \sa SetForceFeedback(float x, float y, float z)
	*/
	void GetForceFeedback( float Force[] ) const
	{
		Force[0] = this->ForceFeedback[0];
		Force[1] = this->ForceFeedback[1];
		Force[2] = this->ForceFeedback[2];
	}

	//!Return the x component of the haptic force
	/*!
	* \sa SetForceFeedback(float Force[])
	* \sa SetForceFeedback(float x, float y, float z)
	*/
	float GetXFeedbackForce() {return this->ForceFeedback[0];}
	//!Return the y component of the haptic force
	/*!
	* \sa SetForceFeedback(float Force[])
	* \sa SetForceFeedback(float x, float y, float z)
	*/
	float GetYFeedbackForce() {return this->ForceFeedback[1];}
	//!Return the z component of the haptic force
	/*!
	* \sa SetForceFeedback(float Force[])
	* \sa SetForceFeedback(float x, float y, float z)
	*/
	float GetZFeedbackForce() {return this->ForceFeedback[2];}

	double * GetContactForceValue();

	// **** Geometrical Functions **** //
	//! Sets the tool's depth in its own coordinate system
	virtual void SetDepth(double depth);

	//!Return the tool's depth
	virtual double GetDepth();

	//! Add a distance to the current tool depth
	/*!
	Set the tool depth position. The modification is made by transforming the tool's
	mesh (vtkPolyData).
	\param Position Z axis value (local coordinate system)
	*/
	virtual void AddDepth(double step);

	//! Rotate the tool in degrees from current orientation about the X axis using the right hand rule.
	/*!
	* Standard laparoscopic tool X-axis rotation. Mobile pieces are processed in implementation classes (vtkToolPincers, vtkToolScissors, etc...)
	*/
	virtual void RotateX(double angle);

	//! Rotate the tool in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	* Standard laparoscopic tool Y-axis rotation. Mobile pieces are processed in implementation classes (vtkToolPincers, vtkToolScissors, etc...)
	*/
	virtual void RotateY(double angle);

	//! Rotate the tool in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	* Standard laparoscopic tool Z-axis rotation. Mobile pieces are processed in implementation classes (vtkToolPincers, vtkToolScissors, etc...)
	*/
	virtual void RotateZ(double angle);

	//! Set the tool's aperture according with the value given in the "opening" parameter
	virtual void SetOpening(double opening);

	//! Return the tool's aperture
	virtual double GetOpening();

protected:

	vtkToolLaparoscopy();
	~vtkToolLaparoscopy();

	//! Force Feedabck unit vector
	double ForceFeedback[3];
	//! Tool depth (vtk z-axis direction)
	double Depth;
	//! Tool pieces opening
	double Opening;
	//! Tool Yaw Angle. VTK Y-axis about rotation
	double YawAngle;
	//! Tool Pitch Angle. VTK X-axis about rotation
	double PitchAngle;
	//! Tool Roll Angle. VTK Z-axis about rotation
	double RollAngle;

private:
	vtkToolLaparoscopy (const vtkToolLaparoscopy &);
	void operator =(const vtkToolLaparoscopy &);
};

#endif
