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
	vtkTypeRevisionMacro(vtkContact,vtkObject);

	static vtkContact *New();
	void PrintSelf(ostream& os, vtkIndent indent);
	const char *GetClassName() {return "vtkContact";};

	//! Performs a full detailed copy of the contact
	void DeepCopy(vtkContact *info);

	//! Set the id of the tool
	/*!
	 * \param id Identifying key of the tool that has contacted the organ
	 * \sa GetToolId()
	 */
	void SetToolId(int id) {ToolId = id;};

	//! Get the index of the tool
	/*!
	 * \return Identifying key of the tool
	 * \sa SetToolId(int id)
	 */
	int GetToolId() {return this->ToolId;};

	//! Set the index of the organ
	/*!
	 * \param id Identifying key of the organ that has been contacted
	 * \sa GetOrganId()
	 */
	void SetOrganId(int id) {OrganId = id;};

	//! Get the index of the deformable organ
	/*!
	 * \return Identifying key of the organ
	 * \sa SetOrganId(int id)
	 */
	int GetOrganId() {return this->OrganId;};
	
	//! Set the organ/tool mesh point where the collision has occured
	/*!
	 * \param position position of the pointId in the PointIds collection
	 * \param value point id of the organ mesh
	 * \sa GetPointId()
	 */
	void SetPointId(int position, int id) {this->PointIds->InsertId(position, id);};

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
	void SetPoint(int position, double x, double y, double z) {
		this->Points->InsertPoint(position, x, y, z);
	};

	//! Set the collisioned point position
	/*!
	 * \param position position of the point on the Points collection
	 * \param point[] [x, y, z] coordinates vector of the collision point
	 * \sa GetVertexPosition()
	 */
	void SetPoint(int position, double point[3]) {
		this->SetPoint(position, point[0],point[1],point[2]);
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
	void SetCellId(int position, vtkIdType value) {this->CellIds->InsertId(position, value);}

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

	//! Set the unit direction vector of the contact
	void SetDirectionVector(double x, double y, double z) {
		this->Direction[0] =x;
		this->Direction[1] =y;
		this->Direction[2] =z;
	};

	//! Set the unit direction vector of the contact
	void SetDirectionVector(double vector[3]) {
		this->SetDirectionVector(vector[0], vector[1], vector[2]);
	};

	//! Returns the force unit vector of the contact
	double * GetDirectionVector() {return this->Direction;};

	//! Set force feedback magnitude
	//void SetForceMagnitude(double value) {this->ForceMagnitude = value;};

	//! Get force feedback magnitude
	//double GetForceMagnitude() {return this->ForceMagnitude;};

	//! Get if the organ has been grasped
	bool IsGrasped() {return this->isGrasped;};

	//! Set the organ as grasped
	void SetAsGrasped(bool value) { isGrasped = value;};

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
	//! clashed vertex cell Id
	// CellIds[0]: Organ Cell Id
	// CellIds[1]: Tool Cell Id
	vtkIdList * CellIds;

	// Vector Director del contacto
	//! Direction Vector of the contact
	double Direction[3];


	//!Define the contact as grasped
	bool isGrasped;

	vtkContact (const vtkContact &);//NotImplemented
	void operator =(const vtkContact &);//Not Implemented
};
#endif
