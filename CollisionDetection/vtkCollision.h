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

#include "vtkESQuiCollisionDetectionWin32Header.h"
#include "vtkObject.h"

//class vtkPoints;
class vtkIdList;
class vtkDoubleArray;

//!This class contains the required information to define a collision.
/*!
 * vtkCollision acts as data container storing all the useful information
 * of a scenario collision:\n
 *  - Type
 *  - ObjectId
 *  - ModelId
 *  - Cell Identifier
 *  - Cell Normal
 *  - Point Identifier
 *  - Point Coordinates
 */

class VTK_ESQUI_COLLISIONDETECTION_EXPORT vtkCollision : public vtkObject {
  
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
   * \param objectId object identifier
   * \sa GetObjectId()
   */
  vtkSetMacro(ObjectId, int);

  //! Get the index of the object
  /*!
   * \return Identifying key of the object
   * \sa SetObjectId(int id, vtkIdType objectid)
   */
  vtkGetMacro(ObjectId, int);

  //! Set the id of the model at the i position
  /*!
   * \param elementId model identifier
   * \sa GetModelId()
   */
  vtkSetMacro(ModelId, int);

  //! Get the index of the model
  /*!
   * \return Identifying key of the model
   * \sa SetModelId(int id, vtkIdType modelId)
   */
  vtkGetMacro(ModelId, int);

  //! Set the collided cell of the deformable model
  /*!
   * \param value organ cell id
   * \sa GetCellId()
   */
  vtkSetMacro(CellId, int);

  //! Get the collided cell of the deformable model
  /*!
   * \return cell id of the organ mesh
   * \sa SetCellId(int value)
   */
  vtkGetMacro(CellId, int);

  //! Set the collided cell normal
  /*!
   * \param n normal vector
   * \sa GetCellNormal()
   */
  vtkSetVector3Macro(CellNormal, double);

  //! Get the collided cell of the deformable model
  /*!
   * \return normal vector
   * \sa SetCellNormal(double * n)
   */
  vtkGetVector3Macro(CellNormal, double);

  //! Set the element mesh point identifier where the collision has occurred
  /*!
   * \param pointId mesh point identifier
   * \sa GetPointId()
   */
  vtkSetMacro(PointId, int);

  //! Get the collided point identifier
  /*!
   * \return point id of the organ mesh
   * \sa SetPointId(int id, int pointId)
   */
  vtkGetMacro(PointId, int);
  
  //! Set the organ mesh point position of the collision
  /*!
   * \param x x coordinate of the collision point
   * \param y y coordinate of the collision point
   * \param z z coordinate of the collision point
   * \sa GetPoint(int id)
   */
  vtkSetVector3Macro(Point, double);

  //! Set the collided point position
  /*!
   * \param point[] [x, y, z] coordinates vector of the collision point
   * \sa GetPoint(int id)
   */
  //void SetPoint(double point[3]);
  //! Returns collided point position
  /*!
   * \return pointer to position [x, y, z] coordinates vector of the collision point
   * \sa SetPoint(double point[3])
   * \sa SetPoint(double x, double y, double z)
   */
  vtkGetVector3Macro(Point, double);

  //! Set the collided point normal
  /*!
   * \param n normal vector
   * \sa GetPointNormal()
   */
  vtkSetVector3Macro(PointNormal, double);

  //! Get the collided point normal of the deformable model
  /*!
   * \return normal vector
   * \sa SetPointNormal(double * n)
   */
  vtkGetVector3Macro(PointNormal, double);

  //! Set the displacement vector of the collision
  vtkSetVector3Macro(PointDisplacement, double);

  //! Returns the direction vector of the collision
  vtkGetVector3Macro(PointDisplacement, double);

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

  //! Performs a full detailed copy of the collision
  void DeepCopy(vtkCollision *info);

protected:
  vtkCollision();
  ~vtkCollision();

  //! Collision Type
  vtkCollisionType CollisionType;

  //! Collided objects identifier
  vtkIdType ObjectId;

  //! Collided model identifier
  vtkIdType ModelId;

  //! Clashed point cell identifier
  vtkIdType CellId;

  //! Clashed point cell normal
  double CellNormal[3];

  //! Point identifier of the collided model
  vtkIdType PointId;

  //! Collision point
  double Point[3];

  //! Collision point normal
  double PointNormal[3];

  //! Collision point displacement
  double PointDisplacement[3];

  //! Scalar distance between points
  double Distance;

private:

  vtkCollision (const vtkCollision &);//NotImplemented
  void operator =(const vtkCollision &);//Not Implemented
};
#endif
