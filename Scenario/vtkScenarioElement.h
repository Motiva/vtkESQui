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
#ifndef __vtkScenarioElement
#define __vtkScenarioElement

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkRenderWindow;
class vtkRenderer;
class vtkPolyData;
class vtkIdList;

class vtkModel;
class vtkModelCollection;
class vtkVisualizationModel;
class vtkCollisionModel;
class vtkDeformationModel;
class vtkCollisionCollection;

//! Class vtkScenarioElement, abstract the use of a surgical Element
/*!
 * vtkScenarioElement abstracts the use of a surgical Element during the simulation exercise.
 * This provide an easy use of surgical Elements collections.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkScenarioElement: public vtkPolyDataAlgorithm {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkScenarioElement,vtkPolyDataAlgorithm);
	//! Create new object
	static vtkScenarioElement * New();
	//!Return the class name
	const char *GetClassName() {return "vtkScenarioElement";}
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Enumeration of piece types
	enum vtkScenarioElementType
	{
		Tool = 0,
		Organ = 1,
		Extra = 2
	};

	//!Enumeration of Element status
	enum vtkScenarioElementStatus
	{
		Visible = 0,
		Hidden = 1,
		Disabled = 2
	};

	//!Abstract initialization function
	/*!
	 * This method initializes the Element physical values, scale, position, etc...
	 */
	virtual void Init();

	//! Assign the identifying key of the Element
	/*!
	 *\sa GetId()
	 */
	vtkSetMacro(Id, vtkIdType);

	//! Returns the identifying key of the Element
	/*!
	 *\sa SetId(vtkIdType Id)
	 */
	vtkGetMacro(Id, vtkIdType);

	//!Set the object id of the model
	/*!
	 *\sa GetObjectId()
	 */
	vtkSetMacro(ObjectId, int);

	//!Get the  object id of the model
	//! Returns the identifying key of the object
	/*!
	 *\sa SetObjectId(vtkIdType Id)
	 */
	vtkGetMacro(ObjectId, int);

	//! Assign the Element type
	/*!
	 *\sa GetType()
	 */
	vtkSetMacro(Type, vtkScenarioElement::vtkScenarioElementType);

	//! Returns the type of the Element
	/*!
	 *\sa SetType(vtkScenarioElementType)
	 */
	vtkGetMacro(Type, vtkScenarioElement::vtkScenarioElementType);

	//! Assign the Element state
	/*!
	 *\sa GetState()
	 */
	vtkSetMacro(Status, vtkScenarioElement::vtkScenarioElementStatus);

	//! Returns the state of the Element
	/*!
	 *\sa SetState(vtkScenarioStateType)
	 */
	vtkGetMacro(Status, vtkScenarioElement::vtkScenarioElementStatus);

	//! Assign Element name
	/*!
	 *\sa GetName()
	 */
	vtkSetStringMacro(Name);

	//!Return Element scale
	/*!
	 *\sa SetName(const char * name)
	 */
	vtkGetStringMacro(Name);

	//! Assign Element scale
	/*!
	 *\sa GetScale()
	 */
	vtkSetVector3Macro(Scale, double);

	//!Return Element scale
	/*!
	 *\sa SetScale(double)
	 */
	vtkGetVector3Macro(Scale, double);

	//! Assign simulation time step
	/*!
	 *\sa GetDeltaT()
	 */
	vtkSetMacro(DeltaT, double);

	//!Return simulation time step
	/*!
	 *\sa SetDeltaT(double)
	 */
	vtkGetMacro(DeltaT, double);

	// **** Graphical Purposes Methods **** //
	//! Set the object origin Point.
	/*!
	 * Reference point where the rotation calculus are made
	 */
	//vtkSetVector3Macro(Origin, double);
	//! Get the object origin Point.
	/*!
	 * Reference point where the rotation calculus are made
	 */
	vtkGetVector3Macro(Origin, double);

	//! Set object orientation angles.
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	//vtkSetVector3Macro(Orientation, double);
	//! Get object orientation angles. (yaw, pitch, roll) (WXYZ)
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	vtkGetVector3Macro(Orientation, double);

	//!Set the Object position (WXYZ)
	//vtkSetVector3Macro(Position, double);
	//!Get the Object position (WXYZ)
	vtkGetVector3Macro(Position, double);

	//!Set the Object velocity (WXYZ)
	//vtkSetVector3Macro(Velocity, double);
	//!Get the Object velocity (WXYZ)
	vtkGetVector3Macro(Velocity, double);

	//! Get the object direction unit vector (WXYZ)
	vtkGetVector3Macro(Direction, double);

	//! Set the render window of the Element
	/*!
	 * Assign the render window for the Element
	 */
	void SetRenderWindow(vtkRenderWindow * r);

	//! Get the render window of the Element
	/*!
	 *Return the render window of the Element
	 */
	vtkRenderWindow * GetRenderWindow(vtkRenderWindow * r);

	//! Set the visualization model of the element
	void SetModels(vtkModelCollection * m);

	//! Get the Visualization Model of the element
	vtkModelCollection * GetModels();

	//! Set the visualization model of the element
	void SetVisualizationModel(vtkVisualizationModel * m);

	//! Get the Visualization Model of the element
	vtkVisualizationModel * GetVisualizationModel();

	//! Set the Collision model of the element
	void SetCollisionModel(vtkCollisionModel * m);

	//! Get the Collision Model of the element
	vtkCollisionModel * GetCollisionModel();

	//! Set the Deformation model of the element
	void SetDeformationModel(vtkDeformationModel * m);

	//! Get the Deformation Model of the element
	vtkDeformationModel * GetDeformationModel();

	//! Implements the translation of the Element (Local coordinate system)
	/*!
		The X & Y parameters contains the relative movement in the horizontal and vertical axes respectively
		\param x x position of the Element
		\param y y position of the Element
		\param z z position of the Element
	 */
	virtual void Translate(double x, double y, double z);

	//! Implements the translation of the Element (Local coordinate system)
	/*!
			\param vector position vector of the translation
	 */
	virtual void Translate(double * vector);

	//! Implements the lateral movements of the model  (Global coordinate system)
	/*!
			The X parameter contains the relative movement in the horizontal axes
			\param a a orientation angle
			\param x x component
			\param y y component
			\param z z component
			Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateWXYZ(double a, double x, double y, double z);

	//! Implements the lateral movements of the Element  (Local coordinate system)
	/*!
		The X parameter contains the relative movement in the horizontal axes
		\param x x orientation angle
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateX(double x);

	//! Implements the lateral movements of the Element  (Local coordinate system)
	/*!
		The Y parameter contains the relative movement in the vertical axes
		\param y y orientation angle
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateY(double y);

	//! Rotate the Element on its own axes  (Local coordinate system)
	/*!
		This function rotate the Element on its own axis the value of an angle given
		by the "Rotation" variable the rotation is produced acting on the actors who compose the Element.
		\param rotation rotation angle (radians)
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateZ(double rotation);

	virtual void Reset();
	virtual void Restore();

	//! Hide scenario Element.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Hide();
	//! Show/Display Element.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Show();

	//! Disable Element.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Disable();

	//! Check if Element is visible
	/*!
	 * When an Element is disabled it will:
	 * - be visible.
	 * - be computed in collision detection.
	 * - be used in simulation.
	 */
	bool IsVisible();

	//! Check if Element is hidden
	/*!
	 * When an Element is disabled it will NOT:
	 * - be visible.
	 * - be computed in collision detection.
	 * It will be used in simulation.
	 */
	bool IsHidden();

	//! Check if Element is disabled.
	/*!
	 * When an Element is disabled it will NOT:
	 * - be visible.
	 * - be computed in collision detection.
	 * - be used in simulation.
	 */
	bool IsDisabled();

	//!Check if element has been initialized
	bool IsInitialized();

protected:

	vtkScenarioElement();
	~vtkScenarioElement();

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//! Element identifier
	vtkIdType Id;
	//!Object Id
	vtkIdType ObjectId;
	//! Element Type (Tool, Organ, etc...)
	vtkScenarioElementType Type;
	//! Element Name
	char * Name;
	//! Element State
	vtkScenarioElementStatus Status;

	//!Visualization model
	vtkVisualizationModel * VisualizationModel;

	//!Collision model
	vtkCollisionModel * CollisionModel;

	//!Deformation model
	vtkDeformationModel * DeformationModel;

	//!Collection of models
	vtkModelCollection * Models;

	//!Collision hash map
	//vtkIdList * CollisionHashMap;

	//!Deformation hash map
	//vtkIdList * DeformationHashMap;

	//! model initialization flag
	bool Initialized;

	//**** Element basic parameters ****//
	//! Origin point for transforms
	double Origin[3];
	//! Object Position
	double Position[3];
	//! Object orientation: Yaw, Pitch, Roll angles
	double Orientation[3];
	//! Unit direction vector
	double Direction[3];
	//! Velocity
	double Velocity[3];
	//! Acceleration
	double Acceleration[3];
	//! Simulation time step
	double DeltaT;

	//! Scale factor (size)
	double Scale[3];

	//**** Common rendering objects ****//
	//!Render Window of the Element
	vtkRenderWindow * RenderWindow;

	//!Renderer of the Element
	vtkRenderer * Renderer;

private:

	//void BuildHashMap(vtkPolyData * a, vtkPolyData * b, vtkIdList * map);

	vtkScenarioElement (const vtkScenarioElement &);
	void operator =(const vtkScenarioElement &);
};

#endif