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
#ifndef __vtkContact_h
#define __vtkContact_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkObject.h"

class vtkPoints;
class vtkIdList;

//!This class acts as data container storing all the useful information of an item-item collision.
/*!
 * All the information (ids, points, cells...) is stored in pairs, one for each collided item.
 */

class VTK_ESQUI_COMMON_EXPORT vtkContact : public vtkObject {
	
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkContact,vtkObject);

	//! Create new contact that will be used as a data container
	static vtkContact *New();
	//! Print contact info
	void PrintSelf(ostream& os, vtkIndent indent);
	//! Return class name
	const char *GetClassName() {return "vtkContact";};

	//BTX
	//!Organ type definition
	enum vtkContactType{
		ToolOrgan = 0,
		ToolTool = 1
	};
	//ETX

	//!Set contact type
	vtkSetMacro(ContactType, vtkContact::vtkContactType);
	//!Return contact type
	vtkGetMacro(ContactType, vtkContact::vtkContactType);

	//! Set the id of the item at the i position
	/*!
	 * \param i item index
	 * \param id item identifier
	 * \sa GetItemId()
	 */
	void SetItemId(int i, vtkIdType id);

	//! Get the index of the item
	/*!
	 * \param item index
	 * \return Identifying key of the item
	 * \sa SetItemId(int i, vtkIdType id)
	 */
	vtkIdType GetItemId(int i);
	
	//! Set the item mesh point identifier where the collision has occurred
	/*!
	 * \param i index of the item in the collection
	 * \param id point id of the mesh
	 * \sa GetPointId()
	 */
	void SetPointId(int i, int id);

	//! Get the collided point identifier
	/*!
	 * \param i index of the item in the collection
	 * \return point id of the organ mesh
	 * \sa SetPointId(int i, int id)
	 */
	int GetPointId(int i);
	
	//! Set the organ mesh point position of the collision
	/*!
	 * \param i index of the item in the collection
	 * \param x x coordinate of the collision point
	 * \param y y coordinate of the collision point
	 * \param z z coordinate of the collision point
	 * \sa GetVertexPosition()
	 */
	void SetPoint(int i, double x, double y, double z);

	//! Set the collided point position
	/*!
	 * \param i index of the item in the collection
	 * \param point[] [x, y, z] coordinates vector of the collision point
	 * \sa GetVertexPosition()
	 */
	void SetPoint(int i, double point[3]);
	//! Returns collided point position
	/*!
	 * \param i index of the item in the collection
	 * \return pointer to position [x, y, z] coordinates vector of the collision point
	 * \sa SetVertexPosition(double position[3])
	 * \sa SetVertexPosition(double x, double y, double z)
	 */
	double * GetPoint(int i);

	//! Set the collisioned cell of the deformable model
	/*!
	 * \param i index of the item in the collection
	 * \param value organ cell id
	 * \sa GetCellId()
	 */
	void SetCellId(int i, vtkIdType value);

	//! Get the collisioned cell of the deformable model
	/*!
	 * \param i index of the item in the collection
	 * \return cell id of the organ mesh
	 * \sa SetCellId(int value)
	 */
	int GetCellId(int i);

	//! Set the scalar distance
	vtkSetMacro(Distance, double);

	//! Get the scalar distance
	vtkGetMacro(Distance, double);

	//! Set the displacement vector of the contact
	vtkSetVector3Macro(Displacement, double);

	//! Returns the direction vector of the contact
	vtkGetVector3Macro(Displacement, double);

	//! Performs a full detailed copy of the contact
	void DeepCopy(vtkContact *info);

protected:
	vtkContact();
	~vtkContact();
private:

	//! Contact Type
	vtkContactType ContactType;

	// Identificadores de cada uno de los objetos colisionados
	//! Collided items ids
	vtkIdList * ItemIds;

	// Posición donde se debe mover el vertice para sacarlo de la herramienta
	//! Collision point on both objects
	vtkPoints * Points;

	// Identificadores de los objetos en colisión
	//! Point ids of the collided items.
	vtkIdList * PointIds;

	// Id de la celda del vertice colisionado
	//! Clashed vertex cell Id
	vtkIdList * CellIds;

	// Distancia escalar entre los puntos
	//! Scalar distance
	double Distance;

	// Vector desplazamiento del contacto
	//! Displacement Vector of the contact
	double Displacement[3];

	//! Define the contact as grasped
	bool isGrasped;

	vtkContact (const vtkContact &);//NotImplemented
	void operator =(const vtkContact &);//Not Implemented
};
#endif
