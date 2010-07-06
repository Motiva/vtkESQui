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
#ifndef __vtkContactCollection_h
#define __vtkContactCollection_h

#include "vtkCollection.h"
#include "vtkContact.h"
#include "vtkESQuiCommonWin32Header.h"

//! Implementation of a collection of contacts
class VTK_ESQUI_COMMON_EXPORT vtkContactCollection: public vtkCollection
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkContactCollection,vtkCollection);

	//! Create new collection of contacts
	static vtkContactCollection *New();

	//! Print collection info
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Perform a deep copy of the vtkContactCollection Object
	/*!
	* \param collection collection of contacts to be copied.
	*/
	void DeepCopy(vtkContactCollection *collection);

	//! Add a Contact to the list.
	/*!
	* \param contact contact to be added to the collection
	* \sa InsertContact(vtkIdType id, vtkContact *contact)
	*/
	void InsertNextContact(vtkContact *contact);

	//! Replace the Contact on id position.
	/*!
	* \param id contact identifier on the collection
	* \param contact contact to be added to the collection
	* \sa InsertNextContact(vtkContact *contact)
	*/
	void InsertContact(vtkIdType id, vtkContact *contact);

	//! Check if the contact exists within the collection
	/*!
	* \param contact contact to be located in the collection
	*/
	vtkIdType ContainsContact(vtkContact * contact);

	//! Return the Contact on id position.
	/*!
	* \param id identifier of the contact that will be returned
	* \return vtkContact object stored in the collection
	*/
	vtkContact * GetContact(vtkIdType id);

	//! Return iteratively each contact
	/*!
	* \return vtkContact object stored in the collection
	*/
	vtkContact * GetNextContact();

	//! Remove the Contact at the id position
	/*!
	* This method only removes the contact from the collection.
	* The object itself must be deleted.
	*/
	void RemoveContact(vtkIdType id);

	//! Remove All Contacts.
	/*!
	* This method only removes the contact from the collection.
	* The object itself must be deleted.
	*/
	void RemoveContacts();

protected:
	vtkContactCollection() {};
	~vtkContactCollection() {};


private:
	vtkContactCollection(const vtkContactCollection&);  // Not implemented.
	void operator=(const vtkContactCollection&);				// Not implemented.
};


#endif
