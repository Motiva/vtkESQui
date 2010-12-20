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
#ifndef __vtkOrganCollection_h
#define __vtkOrganCollection_h

#include "vtkCollection.h"
#include "vtkESQuiScenarioWin32Header.h"

class vtkOrgan;

//!  Collection of organs
/*! 
vtkOrganCollection represents and provides methods to manipulate a list of
Organs (i.e., vtkOrgan and subclasses). The list is unsorted and duplicate
entries are not prevented.
\sa vtkCollection, vtkOrgan
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkOrganCollection : public vtkCollection
{
public:
	//!Create new organ object
	static vtkOrganCollection *New();
	//!Type Revision Macro
	vtkTypeRevisionMacro(vtkOrganCollection,vtkCollection);
	//!Print Class Values
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert an organ to the collection
	/*!
	* The organ will be inserted at the end of the list
	* \param organ vtkOrgan object to be inserted
	* \sa InsertOrgan(vtkIdType id, vtkOrgan *organism)
	*/
	void AddOrgan(vtkOrgan *organ);

	//!Insert an organ at the specified index
	/*!
	* \param id index of the list where the organ will be inserted
	* \param organ vtkOrgan object to be inserted
	* \sa InsertNextOrgan(vtkOrgan *organ)
	*/
	void ReplaceOrgan(vtkIdType id, vtkOrgan *organ);

	//!Return an organ in specified position of the list.
	/*!
	* 	Get the organ in the list with the position id. NULL is returned when the collection is exhausted.
	* \param id index of the organ to be returned
	*/
	vtkOrgan *GetOrgan(vtkIdType id);

	//!Return the next organism on the list.
	/*!
	* Get the organ at the pointer position. Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
	*/
	vtkOrgan * GetNextOrgan();


protected:
	vtkOrganCollection() {};
	~vtkOrganCollection() {};

private:
	vtkOrganCollection(const vtkOrganCollection&);  // Not implemented.
	void operator=(const vtkOrganCollection&);					// Not implemented.
};

#endif
