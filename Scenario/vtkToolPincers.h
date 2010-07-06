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
#ifndef __vtkToolGPincers_h
#define __vtkToolGPincers_h

#include "vtkESQuiScenarioWin32Header.h"

#include "vtkToolLaparoscopy.h"
#include "vtkPoints.h"
#include "vtk3DSImporterEsqui.h"
#include "vtkPolyDataReader.h"
#include "vtkTransformPolyDataFilter.h"

#include "vtkDoubleArray.h"
#include "vtkStringArray.h"

//! Implementation class of pincers
/*!
	vtkToolPincers is the class that implements the pincers tool, inside the scene.
	Contains methods for position control of the tool in the scene and collision detection.
	Pincers are composed of 3 pieces: 1 stick and 2 graspers (vtkPiece)
	Inherits vtkToolLaparascopy class
*/

class VTK_ESQUI_SCENARIO_EXPORT vtkToolPincers: public vtkToolLaparoscopy
{
public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkToolPincers,vtkToolLaparoscopy);

	//!Create new vtkToolPincers object
	static vtkToolPincers *New();

	//!Return class name
	const char *GetClassName() { return "vtkToolGPincers"; }

	//!Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Initialize the tool from VTK file
	/*!
		This function initializes the tool whose mesh is described on a VTK file
	*/
	virtual void Init();

	//!Update the tool
	/*!
	 * This function update the tool values
	 */
	virtual void Update();

	//!Set grasper polydata filename
	/*!
	 * Specify the path of the file that contains the grasper piece polydata
	 */
	void SetGrasperFileName(vtkIdType id, const char * path);

	//!Set stick polydata filename
	/*!
	* Specify the path of the file that contains the stick piece polydata
	*/
	void SetStickFileName(const char * path);

	//!Return the grasper polydata file name
	/*!
	* Path of the file that contains the grasper piece polydata
	*/
	const char * GetGrasperFileName(vtkIdType id);

	//!Return the stick polydata file name
	/*!
	 * Path of the file that contains the grasper piece polydata
	 */
	const char * GetStickFileName(vtkIdType id);

	//! Open the pincers moving piece actors
	/*!
		Piece actors are transformed to perform tool opening
		Bounding boxes are recalculated.
	*/
	void Open();

	//! Close the pincers moving piece actors
	/*!
		Piece actors are transformed to perform tool closing
		Bounding boxes are recalculated.
	*/
	void Close();

	//! Set the tool's aperture according with the value given in the "Aperture" parameter
	/*!
		This function set the tool's aperture in function of the value given in the "Aperture" parameter.
		The aperture is established modifying the tool's mesh position.
		The "Aperture" parameter can take any value between "0" $ "1": "0" -> to close the tool "1" -> to open the tool
	*/
	void SetOpening(double opening);

	//!Return whether the grasper is closed or not
	bool IsClosed(){return (this->Opening <= 0.0f);}

	//! Sets the pincers' depth in its own coordinate system
	void SetDepth(double position);

	//! Rotate the pincers in degrees from current orientation about the X axis using the right hand rule.
	/*!
	 * Pincers specific X-axis rotation.
	 */
	void RotateX(double angle);

	//! Rotate the pincers in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	 * Pincers specific X-axis rotation.
	 */
	void RotateY(double angle);

	//! Rotate the pincers in degrees from current orientation about the Y axis using the right hand rule.
	/*!
	 * Pincers specific X-axis rotation.
	 */
	void RotateZ(double angle);

	//! Sets the pincers yaw angle
	void Yaw(double angle);

	//! Sets the pincers pitch angle
	void Pitch(double angle);

	//! Sets the pincers roll angle
	void Roll(double angle);

protected:	

	vtkToolPincers();
	~vtkToolPincers();

private:

	vtkToolPincers (const vtkToolPincers &); //Not Implemented
	void operator =(const vtkToolPincers &); //Not Implemented

	//!Return the stick piece
	/*!
	 * Return the vtkPiece object of the stick
	 */
	vtkPiece * GetStick(){return this->GetPiece(0);};
	
	//!Return the grasper piece
	/*!
	 * Return the vtkPiece object of the grasper at the specified id
	 */
	vtkPiece * GetGrasper(vtkIdType id){return this->GetPiece(id);};

};
#endif

