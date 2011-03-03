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
#ifndef __vtkToolLaparoscopy
#define __vtkToolLaparoscopy

#include "vtkESQuiScenarioWin32Header.h"
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
 * vtkToolLaparoscopy abstracts the use of a surgical tool during the simulation exercise.
 * This provide an easy use of surgical tools collections.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkToolLaparoscopy: public vtkTool {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkToolLaparoscopy,vtkTool);

	//!Return the class name
	const char *GetClassName() {return "vtkToolLaparoscopy";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Tool type definition
	enum vtkToolLaparoscopyModel{
		Probe = 0,
		Grasper = 1,
		Scissors = 2,
		Hook = 3
	};

	//!Set tool model
	vtkSetMacro(ToolModel, vtkToolLaparoscopy::vtkToolLaparoscopyModel);
	//!Return tool model
	vtkGetMacro(ToolModel, vtkToolLaparoscopy::vtkToolLaparoscopyModel);

	//!Abstract initialization function
	/*!
	* This method initializes the tool physical values, scale, position, etc...
	*/
	virtual void Init();

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

	//! Return tool contact force
	double * GetContactForceValue();

	//! Pull back tool
	/*!
	 *Set the tool depth position back.
	 *\param step Z axis value (local coordinate system)
	*/
	virtual void Pull();

	//! Pull back tool
	/*!
	 *Set the tool depth position forward.
	 *\param step Z axis value (local coordinate system)
	 */
	virtual void Push();

	// **** Geometrical Functions **** //
	//! Sets the tool's depth in its own coordinate system
	virtual void SetDepth(double depth);

	//!Return the tool's depth
	virtual double GetDepth();

	//! Rotate the tool in degrees from current orientation about the X axis using the right hand rule.
	/*!
	* Standard laparoscopic tool X-axis rotation. Mobile pieces are processed in implementation classes (vtkToolGrasper, vtkToolScissors, etc...)
	*/
	virtual void RotateX(double angle);

	//! Rotate the tool in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	* Standard laparoscopic tool Y-axis rotation. Mobile pieces are processed in implementation classes (vtkToolGrasper, vtkToolScissors, etc...)
	*/
	virtual void RotateY(double angle);

	//! Rotate the tool in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	* Standard laparoscopic tool Z-axis rotation. Mobile pieces are processed in implementation classes (vtkToolGrasper, vtkToolScissors, etc...)
	*/
	virtual void RotateZ(double angle);

protected:

	vtkToolLaparoscopy();
	~vtkToolLaparoscopy();

	//! Tool Type
	vtkToolLaparoscopyModel ToolModel;

	//! Force Feedabck unit vector
	double ForceFeedback[3];
	//! Tool depth (vtk z-axis direction)
	double Depth;
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
