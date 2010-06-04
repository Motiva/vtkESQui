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
#ifndef vtkCollisionDetectionLibrary_h
#define vtkCollisionDetectionLibrary_h

#ifdef WIN32
#include <vector>
#else
#include <vector>
#endif

#include "vtkObject.h"
#include "vtkESQuiManagerWin32Header.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

//! Generic interface of the Collision Detection Library

class VTK_ESQUI_MANAGER_EXPORT vtkCollisionDetectionLibrary: public vtkObject
{
public:
	vtkTypeRevisionMacro(vtkCollisionDetectionLibrary, vtkObject);
	vtkCollisionDetectionLibrary();
	~vtkCollisionDetectionLibrary();

	//BTX
	//! Method used to detect the collision between organs and tools from the scene must be implemented in the CollisionDetectionLibrary we want to use
	/*!
	* Abstract method to be defined on each implementation class
	*/
	virtual void Update() = 0;
	//!Initializes the CollisionDetectionLibrary
	/*!
	* Abstract method to be defined on each implementation class
	*/
	virtual void Init() = 0;
	//ETX

	//! Specify the organs to be checked in the collision detection process
	void SetOrgans(vtkOrganCollection * organs){this->Organs = organs;};
	//! Specify the tools to be checked in the collision detection process
	void SetTools(vtkToolCollection * tools){this->Tools = tools;};

	//! Return the contacts obtained after collision detection has been performed
	vtkContactCollection * GetContacts(){ return this->Contacts;};
	//! Get total number of contacts detected
	vtkIdType GetNumberOfContacts(){return this->Contacts->GetNumberOfItems();}

protected:

	//! Collection of Organs
	vtkOrganCollection * Organs;
	//! Collection of Tools
	vtkToolCollection * Tools;
	//! Collection of Contacts
	vtkContactCollection * Contacts;

private:
	vtkCollisionDetectionLibrary(const vtkCollisionDetectionLibrary &); //NotImplemented
	void operator =(const vtkCollisionDetectionLibrary &); //Not Implemented

	//!Clear the CollisionDetectionLibrary
	/*!
	* Pure virtual method. Should be defined in the implementation classes
	*/
	virtual void Clear() = 0;
};


#endif

