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
#ifndef __vtkBoundaryConditionCollection_h
#define __vtkBoundaryConditionCollection_h

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkCollection.h"

class vtkBoundaryCondition;

//! Implementation of a collection of conditions
class VTK_ESQUI_BIOMECHANICS_EXPORT vtkBoundaryConditionCollection: public vtkCollection
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkBoundaryConditionCollection,vtkCollection);

	//! Create new collection of conditions
	static vtkBoundaryConditionCollection *New();

	//! Print collection info
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Perform a deep copy of the vtkBoundaryConditionCollection Object
	/*!
	* \param collection collection of conditions to be copied.
	*/
	void DeepCopy(vtkBoundaryConditionCollection *collection);

	//! Add a BoundaryCondition to the list.
	/*!
	* \param condition condition to be added to the collection
	* \sa InsertBoundaryCondition(vtkIdType id, vtkBoundaryCondition *condition)
	*/
	void InsertNextBoundaryCondition(vtkBoundaryCondition *condition);

	//! Replace the BoundaryCondition on id position.
	/*!
	* \param id condition identifier on the collection
	* \param condition condition to be added to the collection
	* \sa InsertNextBoundaryCondition(vtkBoundaryCondition *condition)
	*/
	void ReplaceBoundaryCondition(vtkIdType id, vtkBoundaryCondition *condition);

	//! Check if the condition exists within the collection
	/*!
	 * \param condition condition to be located in the collection
	 * \return true if condition is already present
	 */
	vtkIdType ContainsBoundaryCondition(vtkBoundaryCondition * condition);

	//! Find condition within the collection
	/*!
	 * \param condition condition to be located in the collection.
	 *  \return position in the collection
	 */
	vtkIdType FindBoundaryCondition(vtkBoundaryCondition * condition);

	//! Return the Boundary Condition on id position.
	/*!
	* \param id identifier of the condition that will be returned
	* \return vtkBoundaryCondition object stored in the collection
	*/
	vtkBoundaryCondition * GetBoundaryCondition(vtkIdType id);

	//! Return iteratively each condition
	/*!
	* \return vtkBoundaryCondition object stored in the collection
	*/
	vtkBoundaryCondition * GetNextBoundaryCondition();

	//! Remove the Boundary Condition at the id position
	/*!
	* This method only removes the condition from the collection.
	* The object itself must be deleted.
	*/
	void RemoveBoundaryCondition(vtkIdType id);

	//! Remove All Boundary Conditions.
	/*!
	* This method only removes the condition from the collection.
	* The object itself must be deleted.
	*/
	void RemoveBoundaryConditions();

protected:
	vtkBoundaryConditionCollection() {};
	~vtkBoundaryConditionCollection() {};


private:
	vtkBoundaryConditionCollection(const vtkBoundaryConditionCollection&);  // Not implemented.
	void operator=(const vtkBoundaryConditionCollection&);				// Not implemented.
};


#endif
