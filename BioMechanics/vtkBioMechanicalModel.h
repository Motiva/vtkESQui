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
#ifndef __vtkBioMechanicalModel_h
#define __vtkBioMechanicalModel_h

#include "vtkESQuiBMMWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkPolyData;

class vtkContact;
class vtkContactCollection;

//! Generic interface to the Biomechanical Model

class VTK_ESQUI_BMM_EXPORT vtkBioMechanicalModel: public vtkPolyDataAlgorithm
{
public:
	vtkTypeRevisionMacro(vtkBioMechanicalModel, vtkPolyDataAlgorithm);

	static vtkBioMechanicalModel *New();
	const char *GetClassName() {return "vtkBioMechanicalModel";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Set BioMechanical model name
	vtkSetStringMacro(Name);

	//!Get BioMechanical model name
	vtkGetStringMacro(Name);

	//! Update function
	//virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//!Initialize the Biomechanical Model
	virtual void Init();

	//!Insert a contact into the biomechanical model
	virtual void InsertNextContact(vtkContact * contact) ;

	//!Insert a collection of contacts into the biomechanical model
	virtual void InsertContacts(vtkContactCollection * collection);

	//!Remove contacts of the biomechanical model
	virtual void DeleteContacts();

protected:
	vtkBioMechanicalModel();
	~vtkBioMechanicalModel();

	//!Model Name
	char * Name;

	//!3D Surface Input
	//vtkPolyData * Mesh;

	//!Contacts
	vtkContactCollection * Contacts;

private:
	vtkBioMechanicalModel(const vtkBioMechanicalModel &); //NotImplemented
	void operator =(const vtkBioMechanicalModel &); //Not Implemented
};


#endif 

