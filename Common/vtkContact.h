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
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

//!This class acts as data container storing all the useful information of an organ-tool collision.
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

	//! Performs a full detailed copy of the contact
	void DeepCopy(vtkContact *info);

	//! Set the id of the tool
	/*!
	 * \sa GetToolId()
	 */
	vtkSetMacro(ToolId, vtkIdType);

	//! Get the index of the tool
	/*!
	 * \return Identifying key of the tool
	 * \sa SetToolId(int id)
	 */
	vtkGetMacro(ToolId, vtkIdType);

	//! Set the index of the organ
	/*!
	 * \sa GetOrganId()
	 */
	vtkSetMacro(OrganId, vtkIdType);

	//! Get the index of the deformable organ
	/*!
	 * \return Identifying key of the organ
	 * \sa SetOrganId(int id)
	 */
	vtkGetMacro(OrganId, vtkIdType);
	
	//! Set the organ/tool mesh point where the collision has occured
	/*!
	 * \param position position of the pointId in the PointIds collection
	 * \param id point id of the organ mesh
	 * \sa GetPointId()
	 */
	void InsertPointId(int position, int id) {this->PointIds->InsertId(position, id);};

	//! Get the collisioned point identifier
	/*!
	 * \param position position of the pointId on the PointIds collection
	 * \return point id of the organ mesh
	 * \sa SetPointId(int position, int id)
	 */
	int GetPointId(int position) {return this->PointIds->GetId(position);};
	
	//! Get the collisioned organ point identifier
	int GetOrganPointId() {return this->PointIds->GetId(0);};
	//! Get the collisioned tool point identifier
	int GetToolPointId() {return this->PointIds->GetId(1);};

	//! Set the organ mesh point position of the collision
	/*!
	 * \param position position of the point on the Points collection
	 * \param x x coordinate of the collision point
	 * \param y y coordinate of the collision point
	 * \param z z coordinate of the collision point
	 * \sa GetVertexPosition()
	 */
	void InsertPoint(int position, double x, double y, double z) {
		this->Points->InsertPoint(position, x, y, z);
	};

	//! Set the collisioned point position
	/*!
	 * \param position position of the point on the Points collection
	 * \param point[] [x, y, z] coordinates vector of the collision point
	 * \sa GetVertexPosition()
	 */
	void InsertPoint(int position, double point[3]) {
		this->InsertPoint(position, point[0],point[1],point[2]);
	};
	//! Returns collisioned point position
	/*!
	 * \return pointer to position [x, y, z] coordinates vector of the collision point
	 * \sa SetVertexPosition(double position[3])
	 * \sa SetVertexPosition(double x, double y, double z)
	 */
	double * GetPoint(int position) {return this->Points->GetPoint(position);};

	//! Get the collisioned organ point coordinates
	double * GetOrganPoint() {return this->Points->GetPoint(0);};
	//! Get the collisioned tool point coordinates
	double * GetToolPoint() {return this->Points->GetPoint(1);};

	//! Set the collisioned cell of the deformable model
	/*!
	 * \param position position of the cell on the CellIds collection
	 * \param value organ cell id
	 * \sa GetCellId()
	 */
	void InsertCellId(int position, vtkIdType value) {this->CellIds->InsertId(position, value);}

	//! Get the collisioned cell of the deformable model
	/*!
	 * \param position position of the cell on the CellIds collection
	 * \return cell id of the organ mesh
	 * \sa SetCellId(int value)
	 */
	int GetCellId(int position) {return this->CellIds->GetId(position);};

	//! Get the collisioned organ cell identifier
	int GetOrganCellId() {return this->CellIds->GetId(0);};
	//! Get the collisioned tool cell identifier
	int GetToolCellId() {return this->CellIds->GetId(1);};

	//! Set the direction vector of the contact
	void SetDirectionVector(double x, double y, double z)
	{
		this->Direction[0] = x;
		this->Direction[1] = y;
		this->Direction[2] = z;
	};

	//! Set the direction vector of the contact
	void SetDirectionVector(double dir[3])
	{
		this->SetDirectionVector(dir[0], dir[1], dir[2]);
	};

	//! Returns the direction vector of the contact
	double * GetDirectionVector() {return this->Direction;};

	//! Set force feedback magnitude
	//void SetForceMagnitude(double value) {this->ForceMagnitude = value;};

	//! Get force feedback magnitude
	//double GetForceMagnitude() {return this->ForceMagnitude;};

	//! Get if the organ has been grasped
	//bool IsGrasped() {return this->isGrasped;};

	//! Set the organ as grasped
	//void SetAsGrasped(bool value) { isGrasped = value;};

protected:
	vtkContact();
	~vtkContact();
private:

	// Modelo Deformable colisionado (-1 si no colisiona con ningun modelo)
	//! Deformable model clashed (-1 if there is not any model clashed)
	int OrganId;

	// Herramienta que produjo la colision
	//! Tool which produced the collision
	int ToolId;

	// Posición donde se debe mover el vertice para sacarlo de la herramienta
	//! Position where would be moved the vertex to extract it from the tool
	// Points[0]: Organ Point
	// Points[1]: Tool Point
	vtkPoints * Points;

	// Numero del vertice del modelo deformable colisionado
	//! Number of the vertex of the deformable model clashed
	// PointIds[0]: Organ Point Id
	// PointIds[1]: Tool Point Id
	vtkIdList * PointIds;

	// Id de la celda del vertice colisionado
	//! Clashed vertex cell Id
	// CellIds[0]: Organ Cell Id
	// CellIds[1]: Tool Cell Id
	vtkIdList * CellIds;

	// Vector Director del contacto
	//! Direction Vector of the contact
	double Direction[3];

	//! Define the contact as grasped
	bool isGrasped;

	vtkContact (const vtkContact &);//NotImplemented
	void operator =(const vtkContact &);//Not Implemented
};
#endif
