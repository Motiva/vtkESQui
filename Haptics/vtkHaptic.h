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
#ifndef __vtkHaptics
#define __vtkHaptics

#include "vtkObject.h"
#include "vtkESQuiHapticsWin32Header.h"



//! Generic Haptic Class

class VTK_ESQUI_HAPTICS_EXPORT vtkHaptic: public vtkObject {

public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkHaptic,vtkObject);
	//! Return class name
	const char *GetClassName() {return "vtkHaptics";}
	//! Print object info
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set up the haptic device
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
	virtual int Init() = 0;
	
	//! Executes the control of the haptic device, reimplemented in all subclasses
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
 	virtual void Update() = 0;

 	//! Liberates the haptic connection
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
 	virtual void Release() = 0;

 	//! Set the haptic name
 	/*!
 	 * \param name name of the haptic device
 	 * \sa GetName()
 	 */
 	void SetName(const char * name);

 	//! Return the haptic name
 	/*!
 	 * \return haptic device name
 	 * \sa SetName(const char * name)
 	 */
 	const char * GetName();

 	//! Set the haptic number of tools
 	/*!
 	 * \param numberOfTools number of tools of the haptic device
 	 * \sa GetNumberOfTools
 	 */
 	void SetNumberOfTools(int numberOfTools);

 	//! Return the haptic number of tools
 	/*!
 	 * \return number of tools of the haptic device
 	 * \sa SetNumberOfTools(int numberOfTools)
 	 */
 	int GetNumberOfTools();

protected:	

	vtkHaptic();
	~vtkHaptic();

	//! Haptic device name
	const char * Name;
	//! Number of tools of the haptic device
	int  NumberOfTools;

};

#endif
