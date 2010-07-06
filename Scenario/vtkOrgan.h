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

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkDataSetMapper.h"
#include "vtkTextureMapToSphere.h"
#include "vtkTransformTextureCoords.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkDelaunay2D.h"
#include "vtkBioMechanicalModel.h"
#include "vtkMSSInterface.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkScenarioItem.h"
#include "vtkPointPlotter.h"
#include "vtkJPEGReader.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"

#include "vtkTimerLog.h"

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

	//!Set input mesh dataset
	/*!
	* \param data vtkPolyData object containing mesh data (points, cells...)
	* \sa GetOutput()
	*/
	void SetInput(vtkPolyData * data);

	//!Update the object
	void Update();

	//!Regenerate simplified mesh after changes due to transformations/deformations
	void UpdateSimpleMesh();

	//!Return output data as an vtkPolyData object
	/*!
	* \sa SetInput(vtkPolyData *data)
	*/
	vtkPolyData * GetOutput();

	//!Set the Biomechanical Model of the organ
	/*!
	* \param model biomechanical model of the organ
	*/
	void SetBioMechanicalModel(vtkBioMechanicalModel * model);


	//!Get the Biomechanical Model of the organ
	/*!
	* \return biomechanical model of the organ
	* \sa SetBioMechModel(vtkBioMechanicalModel * model)
	*/
	vtkBioMechanicalModel * GetBioMechanicalModel();

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
	void Init();

	//!Set gravity parameters,
	/*!
	* where the direction must be 0, 1, or 2 according to x, y, and z direction, and the orientation must have a value of +1 or -1
	*/

	//FIXME: gravity
	void SetGravityInfo(vtkIdType direction, vtkIdType orientation)
	{
		this->GravityDirection = direction;
		this->GravityOrientation = orientation;
	}

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
	vtkSetObjectMacro(Mapper, vtkDataSetMapper);

	//!Return the data set mapper
	/*!
	* \sa SetMapper(vtkDataSetMapper *)
	*/
	vtkGetObjectMacro(Mapper, vtkDataSetMapper);

	//! Set display simple mesh
	//vtkSetMacro(DisplaySimpleMesh, bool);
	//! Enable/Disable simple mesh display
	//vtkBooleanMacro(DisplaySimpleMesh, bool);

	//!function that returns simplified mesh
	/*!
	 * This method returns a simplified mesh for collision detection purposes
	 */
	vtkGetObjectMacro(SimpleMesh, vtkPolyData);

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

	//!Remove all contacts
	/*!
	* Remove all contacts from the list. Memory is not freed
	*/
	void RemoveContacts() {this->Contacts->RemoveContacts();};

	//!Return a list containing the moved elements on last iteration.
	vtkPoints * GetContactPoints();

	//!Get organ contacts
	vtkContactCollection * GetContacts() {return this->Contacts;};

	//!Get number of contacts
	vtkIdType GetNumberOfContacts() {return this->Contacts->GetNumberOfItems();};

	//!Return a specific cell of the organ mesh
	/*!
	* \param id identifying key of the mesh cell
	*/
	vtkCell * GetCell(vtkIdType id){ return this->Bmm->GetMeshCell(id);};

	//!Cauterize organ at specified element
	//BTX
	void Cauterize( vtkIdType element );
	//ETX

	//BTX
	//!Cut the organ at specified elements
	void Cut(vtkIdList *ids);
	//ETX

	//*****   p r o t e c t e d   m e m b e r s
protected:

	//!Configuration filename
	char * FileName;

	//!Configuration filename
	char * TextureFileName;

	//!Texture
	vtkTexture * Texture;

	//!Input Unstructured Grid
	vtkPolyData * Input;

	//!Unstructured grid reader
	vtkXMLPolyDataReader * Reader;

	//!BioMechamical Model of the organ
	vtkBioMechanicalModel * Bmm;

	//!Collection of organ contact points
	vtkContactCollection * Contacts;

	//!Gravity Direction (0/1/2 for x/y/z)
	vtkIdType GravityDirection;
	//!Gravity Orientation (+/- 1)
	vtkIdType GravityOrientation;

	//!Force estimation
	double ForceFactor;     

	//!Organ is hooked
	vtkIdType Hooked;

	//**** Graphical Purposes objects ****//
	//!Actor of the organ
	vtkActor * Actor;

	//!Dataset mapper of the actor
	vtkDataSetMapper * Mapper;

	//!Transform filter of the organ
	vtkTransformPolyDataFilter *TransformFilter;

	//!Transform function of the organ
	vtkTransform * Transform;

	//!Simplified Mesh for collision detection purposes
	vtkPolyData * SimpleMesh;

	//!Bounding Box Mapper
	vtkDataSetMapper * SimpleMeshMapper;

	//!Bounding Box Actor
	vtkActor * SimpleMeshActor;

	vtkOrgan();
	~vtkOrgan();

private:

	vtkOrgan(const vtkOrgan &);
	void operator=(const vtkOrgan &);

};

#endif

