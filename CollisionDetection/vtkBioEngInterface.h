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
#ifndef __vtkBioEngInterface_h
#define __vtkBioEngInterface_h

#include "vtkESQuiCollisionDetectionWin32Header.h"
#include "vtkCollisionDetectionLibrary.h"

class vtkCollisionDetectionFilter;
class vtkMatrix4x4;

//! Interface to the collision detection library

class VTK_ESQUI_COLLISIONDETECTION_EXPORT vtkBioEngInterface: public vtkCollisionDetectionLibrary
{


public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkBioEngInterface, vtkCollisionDetectionLibrary);
	//! Create new vtkbioeng interace
	static vtkBioEngInterface *New();
	//!Return class name
	const char *GetClassName() {return "vtkBioEngInterface";};

	//! Set up the collision detection library with the initial values,
	/*!
	* This method will vary depending on the collision detection library
	*/
	void Init();

	//! Update the collision detection library
	/*!
	* Executes the collision detection process between an tool and an organ
	*/
	void Update();

	virtual vtkPolyData * GetContactSurface();

protected:

	//! Default constructor
	vtkBioEngInterface();
	//! Default destructor
	~vtkBioEngInterface();

private:

	vtkBioEngInterface(const vtkBioEngInterface &); //NotImplemented
	void operator =(const vtkBioEngInterface &); //Not Implemented

	//! Clear previous executions of the collision detection process
	/*!
	* Should be called every time collision detection is performed
	*/
	void Reset();

	//!Collision detection filter
	vtkCollisionDetectionFilter *DetectionFilter;

	//!Calculation matrix
	vtkMatrix4x4 *Matrix0;
	//!Calculation matrix
	vtkMatrix4x4 *Matrix1;

};

#endif 
