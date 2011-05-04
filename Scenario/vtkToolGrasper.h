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
#ifndef __vtkToolGrasper_h
#define __vtkToolGrasper_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkToolLaparoscopy.h"

//! Implementation class of grasper
/*!
	vtkToolGrasper is the class that implements the grasper tool, inside the scene.
	Contains methods for position control of the tool in the scene and collision detection.
	Grasper are composed of 3 pieces: 1 stick and 2 graspers (vtkPiece)
	Inherits vtkToolLaparascopy class
*/

class VTK_ESQUI_SCENARIO_EXPORT vtkToolGrasper: public vtkToolLaparoscopy
{
public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkToolGrasper,vtkToolLaparoscopy);
	//!Create new vtkToolGrasper object
	static vtkToolGrasper *New();
	//!Return class name
	const char *GetClassName() { return "vtkToolGrasper"; }
	//!Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//! set the stick element to the object
	void SetStick(vtkScenarioElement * e);

	//! Set the left lever element to the object
	void SetLeftLever(vtkScenarioElement * e);

	//! Set the right lever element to the object
	void SetRightLever(vtkScenarioElement * e);

	//! Set the tool's aperture according with the value given in the "Aperture" parameter
	/*!
		This function set the tool's aperture in function of the value given in the "Aperture" parameter.
		The aperture is established modifying the tool's mesh position.
		The "Aperture" parameter can take any value between "0" $ "1": "0" -> to close the tool "1" -> to open the tool
	 */
	void SetOpening(double opening);
	//!Get pincer opening
	vtkGetMacro(Opening, double);

	//!Initialize the tool from VTK file
	/*!
		This function initializes the tool whose mesh is described on a VTK file
	*/
	virtual void Init();

	//! Open the grasper moving piece actors
	/*!
		Piece actors are transformed to perform tool opening
		Bounding boxes are recalculated.
	*/
	void Open();

	//! Close the grasper moving piece actors
	/*!
		Piece actors are transformed to perform tool closing
		Bounding boxes are recalculated.
	*/
	void Close();

	//!Return whether the grasper is closed or not
	bool IsClosed(){return (this->Opening <= 0.);}

	//! Sets the grasper' depth in its own coordinate system
	void SetDepth(double position);

	//! Rotate the grasper in degrees from current orientation about the X axis using the right hand rule.
	/*!
	 * Grasper specific X-axis rotation.
	 */
	void RotateX(double angle);

	//! Rotate the grasper in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	 * Grasper specific X-axis rotation.
	 */
	void RotateY(double angle);

	//! Rotate the grasper in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	 * Grasper specific X-axis rotation.
	 */
	void RotateZ(double angle);

	//! Sets the grasper yaw angle
	void Yaw(double angle);

	//! Sets the grasper pitch angle
	void Pitch(double angle);

	//! Sets the grasper roll angle
	void Roll(double angle);

protected:	

	vtkToolGrasper();
	~vtkToolGrasper();

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);

private:

	vtkToolGrasper (const vtkToolGrasper &); //Not Implemented
	void operator =(const vtkToolGrasper &); //Not Implemented

	vtkScenarioElement * Stick;
	vtkScenarioElement * LeftLever;
	vtkScenarioElement * RightLever;

	//! Tool pieces opening
	double Opening;
	
	double OpeningAngle;

};
#endif

