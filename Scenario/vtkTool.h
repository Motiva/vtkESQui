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

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#include "vtkIHP.h"
#include "vtkLSW.h"
#endif

class vtkContact;
class vtkContactCollection;

//! Class vtkTool, abstract the use of a surgical tool
/*!
vtkTool abstracts the use of a surgical tool during the simulation exercise.
This provide an easy use of surgical tools collections.
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkTool: public vtkScenarioObject {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkTool,vtkScenarioObject);

	//!Return the class name
	const char *GetClassName() {return "vtkTool";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);
	
	//!Tool type definition
	enum vtkToolType{
		Laparoscopy = 0,
		Endovascular = 1,
		Arthroscopy = 2,
		Camera = 3
	};
	
	//!Set tool type
	vtkSetMacro(ToolType, vtkTool::vtkToolType);
	//!Return tool type
	vtkGetMacro(ToolType, vtkTool::vtkToolType);

	// **** Haptic Management Methods **** //

	//! Set the use of haptic device
	vtkSetMacro(UseHaptic, bool);
	//! Get the use of haptic device
	vtkGetMacro(UseHaptic, bool);

	//!Enable/Disable haptic device use
	vtkBooleanMacro(UseHaptic, bool);

#ifndef VTKESQUI_USE_NO_HAPTICS
	
	//BTX
	//! Set the haptic device for the tool
	void SetHapticDevice(vtkHaptic *haptic) {this->Haptic = haptic;}

	//! Get the haptic device for the tool
	vtkHaptic *GetHapticDevice() {return this->Haptic;}

protected:
	//! The haptic device for the tool
	vtkHaptic * Haptic;
	//ETX
#endif


protected:

	vtkTool();
	~vtkTool();

	//! Tool Type
	vtkToolType ToolType;

	//! Enable haptic device control
	bool UseHaptic;

private:
	vtkTool (const vtkTool &);
	void operator =(const vtkTool &);
};

#endif
