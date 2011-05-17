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
#ifndef __vtkCollision_h
#define __vtkCollision_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkObject.h"

class vtkPoints;
class vtkIdList;


//!This class contains the required information to define a collision.
/*!
 * vtkCollision acts as data container storing all the useful information
 * of a scenario collision:\n
 *  - Type
 *  - Value
 *  - Point Identifier
 *  - Point Position
 *  All the information is stored in pair, one for each scenario object
 *  collided.
 */

class VTK_ESQUI_COMMON_EXPORT vtkCollision : public vtkObject {
	
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkCollision,vtkObject);

	//! Create new collision that will be used as a data container
	static vtkCollision *New();
	//! Print collision info
	void PrintSelf(ostream& os, vtkIndent indent);
	//! Return class name
	const char *GetClassName() {return "vtkCollision";};

	
	//!Organ type definition
	enum vtkCollisionType{
		ToolOrgan = 0,
		ToolTool = 1
	};
	

	//!Set collision type
	vtkSetMacro(CollisionType, vtkCollision::vtkCollisionType);
	//!Return collision type
	vtkGetMacro(CollisionType, vtkCollision::vtkCollisionType);

	//! Set the id of the object at the i position
	/*!
	 * \param id collection index
	 * \param objectId object identifier
	 * \sa GetObjectId()
	 */
	void SetObjectId(int id, vtkIdType objectId);

	//! Get the index of the object
	/*!
	 * \param id collection index
	 * \return Identifying key of the object
	 * \sa SetObjectId(int id, vtkIdType objectid)
	 */
	vtkIdType GetObjectId(int id);

	//! Set the id of the element at the i position
	/*!
	 * \param id collection index
	 * \param elementId element identifier
	 * \sa GetElementId()
	 */
	void SetElementId(int id, vtkIdType elementId);

	//! Get the index of the element
	/*!
	 * \param id collection index
	 * \return Identifying key of the element
	 * \sa SetElementId(int id, vtkIdType elementId)
	 */
	vtkIdType GetElementId(int id);

	//! Set the element mesh point identifier where the collision has occurred
	/*!
	 * \param id index of the element in the collection
	 * \param pointId mesh point identifier
	 * \sa GetPointId()
	 */
	void SetPointId(int id, int pointId);

	//! Get the collided point identifier
	/*!
	 * \param id index of the element in the collection
	 * \return point id of the organ mesh
	 * \sa SetPointId(int id, int pointId)
	 */
	int GetPointId(int id);
	
	//! Set the organ mesh point position of the collision
	/*!
	 * \param id index of the element in the collection
	 * \param x x coordinate of the collision point
	 * \param y y coordinate of the collision point
	 * \param z z coordinate of the collision point
	 * \sa GetPoint(int id)
	 */
	void SetPoint(int id, double x, double y, double z);

	//! Set the collided point position
	/*!
	 * \param id index of the element in the collection
	 * \param point[] [x, y, z] coordinates vector of the collision point
	 * \sa GetPoint(int id)
	 */
	void SetPoint(int id, double point[3]);
	//! Returns collided point position
	/*!
	 * \param id index of the element in the collection
	 * \return pointer to position [x, y, z] coordinates vector of the collision point
	 * \sa SetPoint(int id, double point[3])
	 * \sa SetPoint(int id, double x, double y, double z)
	 */
	double * GetPoint(int id);

	//! Set the collided cell of the deformable model
	/*!
	 * \param id index of the element in the collection
	 * \param value organ cell id
	 * \sa GetCellId()
	 */
	void SetCellId(int id, vtkIdType value);

	//! Get the collided cell of the deformable model
	/*!
	 * \param id index of the element in the collection
	 * \return cell id of the organ mesh
	 * \sa SetCellId(int value)
	 */
	int GetCellId(int id);

	//! Set the scalar distance
	/*!
	 * \sa double GetDistance
	 */
	vtkSetMacro(Distance, double);

	//! Get the scalar distance
	/*!
	 * \sa SetDistance(double)
	 */
	vtkGetMacro(Distance, double);

	//! Set the displacement vector of the collision
	vtkSetVector3Macro(Displacement, double);

	//! Returns the direction vector of the collision
	vtkGetVector3Macro(Displacement, double);

	//! Performs a full detailed copy of the collision
	void DeepCopy(vtkCollision *info);

protected:
	vtkCollision();
	~vtkCollision();

	//! Collision Type
	vtkCollisionType CollisionType;

	//! Collided elements identifiers
	vtkIdList * ElementIds;

	//! Collided objects identifiers
	vtkIdList * ObjectIds;

	//! Collision point on both objects
	vtkPoints * Points;

	//! Point identifiers of the collided elements.
	vtkIdList * PointIds;

	//! Clashed point cell identifier
	vtkIdList * CellIds;

	//! Scalar distance between points
	double Distance;

	//! Displacement vector of the collision
	double Displacement[3];

private:

	vtkCollision (const vtkCollision &);//NotImplemented
	void operator =(const vtkCollision &);//Not Implemented
};
#endif
