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
#ifndef _vtkOrgan_h_
#define _vtkOrgan_h_

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkScenarioItem.h"

#include "vtkActor.h"
#include "vtkPolyDataMapper.h"

#include "vtkContactCollection.h"
#include "vtkBioMechanicalModel.h"

class vtkPoints;
class vtkCell;
class vtkIdList;
class vtkXMLPolyDataReader;
class vtkPolyData;
class vtkTransformPolyDataFilter;
class vtkTransform;
class vtkTexture;

//! Implementation class for organ definition
class VTK_ESQUI_SCENARIO_EXPORT vtkOrgan: public vtkScenarioItem
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkOrgan, vtkScenarioItem);
	//! Create new organ
	static vtkOrgan * New();
	//! Return class name
	const char *GetClassName() {return "vtkOrgan";}

	virtual void PrintSelf(ostream &os, vtkIndent indent);

	
	//!Organ type definition
	enum vtkOrganType{
		Static = 0,
		Deformable = 1
	};
	

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//!Set the Biomechanical Model of the organ
	/*!
	* \param model biomechanical model of the organ
	*/
	void SetDeformationModel(vtkBioMechanicalModel * model);


	//!Get the Biomechanical Model of the organ
	/*!
	* \return biomechanical model of the organ
	* \sa SetBioMechModel(vtkBioMechanicalModel * model)
	*/
	vtkBioMechanicalModel * GetDeformationModel();

	//!Set organ type
	vtkSetMacro(OrganType, vtkOrgan::vtkOrganType);
	//!Return organ type
	vtkGetMacro(OrganType, vtkOrgan::vtkOrganType);

	//!Set initial values filename
	/*!
	* \sa GetFileName()
	*/
	vtkSetStringMacro(FileName);

	//!Return initial values filename
	/*!
	* \sa SetFileName(const char* name);
	*/
	vtkGetStringMacro(FileName);

	//!Set texture filename
	/*!
	* \sa GetTextureFileName()
	*/
	vtkSetStringMacro(TextureFileName);

	//!Return texture filename
	/*!
	* \sa SetTextureFileName(const char* name);
	*/
	vtkGetStringMacro(TextureFileName);

	//!Set initial values filename
	/*!
	 * \sa GetFileName()
	 */
	vtkSetStringMacro(DeformationModelName);

	//!Return initial values filename
	/*!
	 * \sa SetFileName(const char* name);
	 */
	vtkGetStringMacro(DeformationModelName);

	//!Set force factor.
	/*!
	* Used for estimating haptic force process
	* \sa GetForceFactor()
	*/
	vtkSetMacro(ForceFactor, double);

	//!GetfForce factor.
	/*!
	* \sa SetForceFactor(double value)
	*/
	vtkGetMacro(ForceFactor, double);

	//!Set organ as hooked
	/*!
	* \sa GetHooked()
	*/
	vtkSetMacro(Hooked, bool);

	//!Return if the organ is hooked
	/*!
	* \sa SetHooked(bool)
	*/
	vtkGetMacro(Hooked, bool);

	//!Set On/Off organ as hooked
	vtkBooleanMacro(Hooked, bool);

	//!Initialize mesh data
	/*!
	* The following parameters should be set before calling this function:
	* Biomechanical Model
	*/
	virtual void Init();

	// **** Geometrical Functions **** //
	//! Implements the translation of the organ (Local coordinate system)
	/*!
		The X & Y parameters contains the relative movement in the horizontal and vertical axes respectively
		\param x x position of the organ
		\param y y position of the organ
		\param z z position of the organ
	 */
	virtual void Translate(double x, double y, double z);

	//! Implements the translation of the organ (Local coordinate system)
	/*!
			\param vector position vector of the translation
	 */
	virtual void Translate(double * vector);

	//! Implements the lateral movements of the organ  (Local coordinate system)
	/*!
		The X parameter contains the relative movement in the horizontal axes
		\param x x orientation angle
	 */
	virtual void RotateX(double x);

	//! Implements the lateral movements of the organ  (Local coordinate system)
	/*!
		The Y parameter contains the relative movement in the vertical axes
		\param y y orientation angle
	 */
	virtual void RotateY(double y);

	//! Rotate the organ on its own axes  (Local coordinate system)
	/*!
		This function rotate the organ on its own axis the value of an angle given
		by the "Rotation" variable the rotation is produced acting on the actors who compose the organ.
		\param rotation rotation angle (radians)
	 */
	virtual void RotateZ(double rotation);
	//------- Rendering purposes ----------//

	//!Set the actor of the organ
	/*!
	* \sa GetActor()
	*/
	vtkSetObjectMacro(Actor, vtkActor);

	//!Return the actor that displays the organ
	/*!
	* \sa SetActor(vtkActor *)
	*/
	vtkGetObjectMacro(Actor, vtkActor);

	//!Set the mapper of the actor
	/*!
	* \sa GetMapper()
	*/
	vtkSetObjectMacro(Mapper, vtkPolyDataMapper);

	//!Return the data set mapper
	/*!
	* \sa SetMapper(vtkDataSetMapper *)
	*/
	vtkGetObjectMacro(Mapper, vtkPolyDataMapper);
	//!Function that returns simplified mesh
	/*!
	 * This method returns a simplified mesh for collision detection purposes
	 */
	//vtkGetObjectMacro(SimpleMesh, vtkPolyData);

	//------- Biomechanical model interface methods -------//

	//!Add a contact to the list
	/*!
	* \param contact vtkContact object with the information of the contact( toolId, organId, point, cell... etc)
	* \sa InsertContacts( vtkContactCollection* contacts )
	*/
	void InsertNextContact( vtkContact* contact )  { this->Contacts->InsertNextContact(contact); };

	//!Add several contacts to the list
	/*!
	* \param contacts List of vtkContact objects
	* \sa InsertNextContact( vtkContact* contact )
	*/
	void InsertContacts( vtkContactCollection* contacts )  { this->Contacts->DeepCopy(contacts); };

	//!Get organ contacts
	vtkContactCollection * GetContacts() {return this->Contacts;};

	//!Get number of contacts
	vtkIdType GetNumberOfContacts() {return this->Contacts->GetNumberOfItems();};

	//!Remove all contacts
	/*!
	 * Remove all contacts from the list. Memory is not freed
	 */
	void CleanContacts() {this->Contacts->RemoveContacts();};

	//! Hide scenario organ.
	virtual void Hide();
	//! Show/Display organ.
	virtual void Show();

	//*****   p r o t e c t e d   m e m b e r s
protected:

	//! Organ Type
	vtkOrganType OrganType;

	//!Configuration filename
	char * FileName;

	//!Configuration filename
	char * TextureFileName;

	//!Deformation Model Name
	char * DeformationModelName;

	//!Texture
	vtkTexture * Texture;

	//!Input Unstructured Grid
	vtkPolyData * Input;

	//!Input Unstructured Grid
	vtkPolyData * Output;

	//!Unstructured grid reader
	vtkXMLPolyDataReader * Reader;

	//!BioMechamical Model of the organ
	vtkBioMechanicalModel * DeformationModel;

	//!Collection of organ contact points
	vtkContactCollection * Contacts;

	//!Force estimation
	double ForceFactor;

	//!Organ is hooked
	bool Hooked;

	//**** Graphical Purposes objects ****//
	//!Actor of the organ
	vtkActor * Actor;

	//!Dataset mapper of the actor
	vtkPolyDataMapper * Mapper;

	//!Transform filter of the organ
	vtkTransformPolyDataFilter *TransformFilter;

	//!Transform function of the organ
	vtkTransform * Transform;

	vtkOrgan();
	~vtkOrgan();

private:

	vtkOrgan(const vtkOrgan &);
	void operator=(const vtkOrgan &);

};

#endif

