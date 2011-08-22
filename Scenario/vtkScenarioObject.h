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
#ifndef __vtkScenarioObject
#define __vtkScenarioObject

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkRenderWindow;
class vtkRenderer;
class vtkPolyData;
class vtkAppendPolyData;

class vtkCollisionCollection;
class vtkCollision;
class vtkScenarioElement;
class vtkScenarioElementCollection;

//! vtkScenarioObject class implements the use of a surgical scenario object.
/*!
* This class contains all the required attributes to define a scenario
* object, providing an easy use of surgical objects such as: tools,
* organs, etc...\n
* Every scenario object is formed a collection of scenario elements,
* implemented in the vtkScenarioElement class. At least one scenario element
* per scenario object must be defined.\n
* In order to manage scenario objects interaction a collection of collisions
* (see vtkCollisionCollection), has been included in this this class.
*/

class VTK_ESQUI_SCENARIO_EXPORT vtkScenarioObject: public vtkPolyDataAlgorithm {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkScenarioObject,vtkPolyDataAlgorithm);
	//! Create new organ
	static vtkScenarioObject * New();
	//!Return the class name
	const char *GetClassName() {return "vtkScenarioObject";}
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Enumeration of piece types
	enum vtkScenarioObjectType
	{
		Tool = 0,
				Organ = 1,
				Extra = 2
	};

	//!Enumeration of object status
	enum vtkScenarioObjectStatus
	{
		Enabled = 0,
				Disabled = 1
	};

	//!Initialization function
	virtual void Init();

	//! Assign the identifying key of the object
	/*!
	*\sa GetId()
	*/
	vtkSetMacro(Id, vtkIdType);

	//! Returns the identifying key of the object
	/*!
	*\sa SetId(vtkIdType Id)
	*/
	vtkGetMacro(Id, vtkIdType);

	//! Assign the object type
	/*!
	*\sa GetType()
	*/
	vtkSetMacro(ObjectType, vtkScenarioObject::vtkScenarioObjectType);

	//! Returns the type of the object
	/*!
	*\sa SetType(vtkScenarioObjectType)
	*/
	vtkGetMacro(ObjectType, vtkScenarioObject::vtkScenarioObjectType);

	//! Assign the object state
	/*!
	*\sa GetState()
	*/
	vtkSetMacro(Status, vtkScenarioObject::vtkScenarioObjectStatus);

	//! Returns the state of the object
	/*!
	*\sa SetState(vtkScenarioStateType)
	*/
	vtkGetMacro(Status, vtkScenarioObject::vtkScenarioObjectStatus);

	//! Assign object name
	/*!
	*\sa GetName()
	*/
	vtkSetStringMacro(Name);

	//!Return object scale
	/*!
	*\sa SetName(const char * name)
	*/
	vtkGetStringMacro(Name);

	//! Set the render window of the object
	/*!
	* Assign the render window for the object
	*/
	void SetRenderWindow(vtkRenderWindow * r){this->RenderWindow = r;};

	//! Get the render window of the object
	/*!
	* Return the render window of the object
	*/
	vtkRenderWindow * GetRenderWindow(vtkRenderWindow * r){return this->RenderWindow;};

	//! Set the renderer of the object
	/*!
	* Assign the renderer for the object
	*/
	void SetRenderer(vtkRenderer * r){this->Renderer = r;};

	//! Get the renderer of the object
	/*!
	*Return the renderer of the object
	*/
	vtkRenderer * GetRenderer(vtkRenderer * r){return this->Renderer;};

	//! Return the elements of the object
	vtkScenarioElementCollection * GetElements();

	//! Assign the elements to the object
	void SetElements(vtkScenarioElementCollection * elements);

	//! Add an element to the object
	void AddElement(vtkScenarioElement * e);

	//! Remove an element of the collection at the specified id
	void RemoveElement(vtkIdType id);

	//! Returns the element of the object at the specified id
	vtkScenarioElement * GetElement(vtkIdType id);

	//! Return the number elements of the object
	int GetNumberOfElements();

	// **** Geometrical Functions **** //
	//! Implements the translation of the object (Local coordinate system)
	/*!
	* The X & Y parameters contains the relative movement in the horizontal
	* and vertical axes respectively.
	* \param x x position of the object
	* \param y y position of the object
	* \param z z position of the object
	*/
	virtual void Translate(double x, double y, double z);

	//! Implements the translation of the object (Local coordinate system)
	/*!
	* \param vector position vector of the translation
	*/
	virtual void Translate(double * vector);

	//! Implements the lateral movements of the object  (Global coordinate system)
	/*!
	* The X parameter contains the relative movement in the horizontal axes
	* \param a a orientation angle
	* \param x x component
	* \param y y component
	* \param z z component
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void RotateWXYZ(double a, double x, double y, double z);

	//! Implements the lateral movements of the object  (Local coordinate system)
	/*!
	* The X parameter contains the relative movement in the horizontal axes
	* \param x x orientation angle
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void RotateX(double x);

	//! Implements the lateral movements of the object  (Local coordinate system)
	/*!
	* The Y parameter contains the relative movement in the vertical axes
	* \param y y orientation angle
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void RotateY(double y);

	//! Rotate the object on its own axes  (Local coordinate system)
	/*!
	* This function rotate the object on its own axis the value of an angle given
	* by the "Rotation" variable the rotation is produced acting on the actors who compose the object.
	* \param rotation rotation angle (radians)
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void RotateZ(double rotation);

	//! Reset all object elements to their original state
	virtual void Reset();
	//! Restore all element elements to their last position/orientation
	virtual void Restore();

	//! Show/Display object.
	/*!
	* Must be implemented in inherited classes
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void Show();

	//! Hide scenario object.
	/*!
	* Must be implemented in inherited classes
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void Hide();

	//! Enable object.
	/*!
	* Must be implemented in inherited classes
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void Enable();

	//! Disable object.
	/*!
	* Must be implemented in inherited classes
	* Note: this function must be implemented in inheriting classes
	*/
	virtual void Disable();

	//! Check if object is visible
	/*!
	* When an object is enabled it will:
	* - be visible.
	* - be computed in collision detection.
	* - be used in simulation.
	*/
	bool IsEnabled(){ return this->Status == Enabled;};

	//! Check if object is disabled.
	/*!
	* When an object is disabled it will NOT:
	* - be visible.
	* - be computed in collision detection.
	* - be used in simulation.
	*/
	bool IsDisabled(){ return this->Status == Disabled;};

	//! Return if object has been initialized
	bool IsInitialized(){ return this->Initialized;};

	//------- Deformation model interface methods -------//

	//! Add a collision to the list
	/*!
	* \param collision vtkCollision object with the information of the collision( toolId, organId, point, cell... etc)
	* \sa InsertCollisions( vtkCollisionCollection* collisions )
	*/
	void InsertNextCollision( vtkCollision* collision );

	//! Add several collisions to the list
	/*!
	* \param collisions List of vtkCollision objects
	* \sa InsertNextCollision( vtkCollision* collision )
	*/
	void SetCollisions( vtkCollisionCollection* collisions );

	//! Get organ collisions
	vtkCollisionCollection * GetCollisions();

	//! Get number of collisions
	vtkIdType GetNumberOfCollisions();

	//!Remove all collisions
	/*!
	* Remove all collisions from the list. Memory is not freed
	*/
	void CleanCollisions();

protected:

	vtkScenarioObject();
	~vtkScenarioObject();

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);

	//! Object identifier
	vtkIdType Id;
	//! Object Type (Tool, Organ, etc...)
	vtkScenarioObjectType ObjectType;
	//! Object Name
	char * Name;
	//! Object State
	vtkScenarioObjectStatus Status;

	//! Initialization flag
	bool Initialized;

	//! Collection of object elements
	vtkScenarioElementCollection * Elements;

	//!Collection of object collisions
	vtkCollisionCollection * Collisions;

	//!Render Window of the object
	vtkRenderWindow * RenderWindow;

	//!Renderer of the object
	vtkRenderer * Renderer;

	//!Append PolyData filter
	//vtkAppendPolyData * AppendFilter;

private:
	vtkScenarioObject (const vtkScenarioObject &);
	void operator =(const vtkScenarioObject &);
};

#endif
