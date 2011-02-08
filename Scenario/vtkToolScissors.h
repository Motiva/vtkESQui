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
#ifndef _vtkToolScissors_h
#define _vtkToolScissors_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkToolLaparoscopy.h"

//! Implementation class of scissors
/*!
* vtkToolScissors is the class that implements the scissors tool, inside the scene.
* Contains methods for position control of the tool in the scene and collision detection.
* Scissors are composed of 3 pieces: 1 stick and 2 blades (vtkPiece)
* Inherits vtkToolLaparascopy
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkToolScissors : public vtkToolLaparoscopy
{

public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkToolScissors,vtkToolLaparoscopy);
	//! Create new vtkToolScissors object
	static vtkToolScissors *New();
	//! Return the class name
	const char *GetClassName() { return "vtkToolScissors"; }
	//! Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	
	//!Tool tyoe definition
	enum vtkPieceType{
		Stick = 0,
		LeftBlade = 1,
		RightBlade = 2
	};
	

	//!Initialize the tool from VTK file
	/*!
	* This function initializes the tool whose mesh is described on a VTK file
	*/
	virtual void Init();

	//!Update the tool
	/*!
	* This function update the tool values
	*/
	virtual void Update();

	//! Set the tool's aperture according with the value given in the "Aperture" parameter
	/*!
		This function set the tool's aperture in function of the value given in the "Aperture" parameter.
		The aperture is established modifying the tool's mesh position.
		The "Aperture" parameter can take any value between "0" $ "1": "0" -> to close the tool "1" -> to open the tool
	 */
	void SetOpening(double opening);
	//!Get pincer opening
	vtkGetMacro(Opening, double);


	//!Set stick polydata filename
	/*!
	* Specify the path of the file that contains the stick piece polydata
	*/
	vtkSetStringMacro(StickFileName);

	//!Return the stick polydata file name
	/*!
	 * Path of the file that contains the grasper piece polydata
	 */
	vtkGetStringMacro(StickFileName);

	//!Set grasper polydata filename
	/*!
	 * Specify the path of the file that contains the grasper piece polydata
	 */
	vtkSetStringMacro(LeftBladeFileName);

	//!Return the left grasper polydata file name
	/*!
	* Path of the file that contains the grasper piece polydata
	*/
	vtkGetStringMacro(LeftBladeFileName);

	//!Set grasper polydata filename
	/*!
	 * Specify the path of the file that contains the grasper piece polydata
	 */
	vtkSetStringMacro(RightBladeFileName);

	//!Return the left grasper polydata file name
	/*!
	 * Path of the file that contains the grasper piece polydata
	 */
	vtkGetStringMacro(RightBladeFileName);

	//!Return whether the Blade is closed or not
	bool IsClosed(){return (this->Opening < 0.0f);}

protected:

	vtkToolScissors();
	~vtkToolScissors();

private:

	vtkToolScissors (const vtkToolScissors &); //Not Implemented
	void operator =(const vtkToolScissors &); //Not Implemented

	double Opening;
	char * StickFileName;
	char * LeftBladeFileName;
	char * RightBladeFileName;

	//!Return the stick piece
	/*!
	* Return the vtkPiece object of the stick
	*/
	vtkPiece * GetStick(){return this->GetPiece(vtkToolScissors::Stick);};

	//!Return the Blade piece
	/*!
	* Return the vtkPiece object of the Blade at the specified id
	*/
	vtkPiece * GetLeftBlade(){return this->GetPiece(vtkToolScissors::LeftBlade);};

	//!Return the Blade piece
	/*!
	 * Return the vtkPiece object of the Blade at the specified id
	 */
	vtkPiece * GetRightBlade(){return this->GetPiece(vtkToolScissors::RightBlade);};
};

#endif
