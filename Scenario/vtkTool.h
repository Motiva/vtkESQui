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
#ifndef __vtkTool
#define __vtkTool

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkScenarioObject.h"

class vtkContact;
class vtkContactCollection;

//! Implementation class for scenario tool definition.
/*!
 * In vtkESQui a tool is considered an scenario object, so vtkScenarioObject
 * serves as its base class.\n
 * This class serves as base class for specific surgical technique tools:
 * vtkToolLaparoscopy, vtkToolEndovascular, etc...\n
 * As a vtkModel based class, at least a visualization model has to be defined.
 *  If the tool is deformable, a deformation and a collision model must also
 * be defined.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkTool: public vtkScenarioObject {

public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkTool,vtkScenarioObject);

	//! Return the class name
	const char *GetClassName() {return "vtkTool";}

	//! Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Tool type definition
	enum vtkToolType{
		Laparoscopy = 0,
		Endovascular = 1,
		Arthroscopy = 2,
		Camera = 3
	};

	//! Set tool type
	/*!
	 * \sa vtkToolType GetToolType()
	 */
	vtkSetMacro(ToolType, vtkTool::vtkToolType);

	//!Return tool type
	/*!
	 * \sa SetToolType(vtkToolType)
	 */
	vtkGetMacro(ToolType, vtkTool::vtkToolType);

protected:

	vtkTool();
	~vtkTool();

	//! Tool Type
	vtkToolType ToolType;

private:
	vtkTool (const vtkTool &);
	void operator =(const vtkTool &);
};

#endif
