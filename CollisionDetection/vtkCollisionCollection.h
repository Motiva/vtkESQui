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
#ifndef __vtkCollisionCollection_h
#define __vtkCollisionCollection_h

#include "vtkESQuiCollisionDetectionWin32Header.h"
#include "vtkCollection.h"

class vtkCollision;

//! Implementation of a collection of collisions
class VTK_ESQUI_COLLISIONDETECTION_EXPORT vtkCollisionCollection: public vtkCollection
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkCollisionCollection,vtkCollection);

	//! Create new collection of collisions
	static vtkCollisionCollection *New();

	//! Print collection info
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Perform a deep copy of the vtkCollisionCollection Object
	/*!
	* \param collection collection of collisions to be copied.
	*/
	void DeepCopy(vtkCollisionCollection *collection);

	//! Add a Collision to the list.
	/*!
	* \param collision collision to be added to the collection
	* \sa InsertCollision(vtkIdType id, vtkCollision *collision)
	*/
	void InsertNextCollision(vtkCollision *collision);

	//! Replace the Collision on id position.
	/*!
	* \param id collision identifier on the collection
	* \param collision collision to be added to the collection
	* \sa InsertNextCollision(vtkCollision *collision)
	*/
	void ReplaceCollision(vtkIdType id, vtkCollision *collision);

	//! Check if the collision exists within the collection
	/*!
	 * A collision is found if the following params match:
	 * - organId
	 * - toolId
	 * - organPointId
	 * \param collision collision to be located in the collection
	 * \return true if collision is already present
	 */
	vtkIdType ContainsCollision(vtkCollision * collision);

	//! Find collision within the collection
	/*!
	 * A collision is found if the following params match:
	 * - organId
	 * - toolId
	 * - organPointId
	 * \param collision collision to be located in the collection.
	 *  \return position in the collection
	 */
	vtkIdType FindCollision(vtkCollision * collision);

	//! Return the Collision on id position.
	/*!
	* \param id identifier of the collision that will be returned
	* \return vtkCollision object stored in the collection
	*/
	vtkCollision * GetCollision(vtkIdType id);

	//! Return iteratively each collision
	/*!
	* \return vtkCollision object stored in the collection
	*/
	vtkCollision * GetNextCollision();

	//! Remove the Collision at the id position
	/*!
	* This method only removes the collision from the collection.
	* The object itself must be deleted.
	*/
	void RemoveCollision(vtkIdType id);

	//! Remove All Collisions.
	/*!
	* This method only removes the collision from the collection.
	* The object itself must be deleted.
	*/
	void RemoveCollisions();

protected:
	vtkCollisionCollection() {};
	~vtkCollisionCollection() {};

private:
	vtkCollisionCollection(const vtkCollisionCollection&);  // Not implemented.
	void operator=(const vtkCollisionCollection&);				// Not implemented.
};


#endif
