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
#ifndef __vtkScenarioItem
#define __vtkScenarioItem

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkRenderWindow;
class vtkRenderer;
class vtkPolyData;

//! Class vtkScenarioItem, abstract the use of a surgical item
/*!
 * vtkScenarioItem abstracts the use of a surgical item during the simulation exercise.
 * This provide an easy use of surgical items collections.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkScenarioItem: public vtkPolyDataAlgorithm {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkScenarioItem,vtkPolyDataAlgorithm);
	//!Return the class name
	const char *GetClassName() {return "vtkScenarioItem";}
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);


	//!Enumeration of piece types
	enum vtkScenarioItemType
	{
		Tool = 0,
		Organ = 1,
		Extra = 2
	};


	//!Enumeration of piece types
	enum vtkScenarioItemStatus
	{
		Visible = 0,
		Hidden = 1,
		Disabled = 2
	};

	//!Abstract initialization function
	/*!
	 * This method initializes the item physical values, scale, position, etc...
	 */
	virtual void Init() = 0;

	//! Process the algorithm request (Update).
	//virtual int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);
	//! Set the render window of the item
	/*!
	 * Assign the render window for the item
	 */
	void SetRenderWindow(vtkRenderWindow * r){this->RenderWindow = r;};

	//! Get the render window of the item
	/*!
	 *Return the render window of the item
	 */
	vtkRenderWindow * GetRenderWindow(vtkRenderWindow * r){return this->RenderWindow;};

	//! Assign the identifying key of the item
	/*!
	 *\sa GetId()
	 */
	vtkSetMacro(Id, vtkIdType);

	//! Returns the identifying key of the item
	/*!
	 *\sa SetId(vtkIdType Id)
	 */
	vtkGetMacro(Id, vtkIdType);


	//! Assign the item type
	/*!
	 *\sa GetType()
	 */
	vtkSetMacro(Type, vtkScenarioItem::vtkScenarioItemType);

	//! Returns the type of the item
	/*!
	 *\sa SetType(vtkScenarioItemType)
	 */
	vtkGetMacro(Type, vtkScenarioItem::vtkScenarioItemType);

	//! Assign the item state
	/*!
	 *\sa GetState()
	 */
	vtkSetMacro(Status, vtkScenarioItem::vtkScenarioItemStatus);

	//! Returns the state of the item
	/*!
	 *\sa SetState(vtkScenarioStateType)
	 */
	vtkGetMacro(Status, vtkScenarioItem::vtkScenarioItemStatus);

	//! Assign item name
	/*!
	 *\sa GetName()
	 */
	vtkSetStringMacro(Name);

	//!Return item scale
	/*!
	 *\sa SetName(const char * name)
	 */
	vtkGetStringMacro(Name);

	//! Assign item scale
	/*!
	 *\sa GetScale()
	 */
	vtkSetVector3Macro(Scale, double);

	//!Return item scale
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
	vtkSetVector3Macro(Origin, double);
	//! Get the object origin Point.
	/*!
	 * Reference point where the rotation calculus are made
	 */
	vtkGetVector3Macro(Origin, double);

	//! Set object orientation angles.
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	vtkSetVector3Macro(Orientation, double);
	//! Get object orientation angles. (yaw, pitch, roll) (WXYZ)
	/*!
	 * (yaw, pitch, roll) (WXYZ)
	 */
	vtkGetVector3Macro(Orientation, double);

	//!Set the Object position (WXYZ)
	vtkSetVector3Macro(Position, double);
	//!Get the Object position (WXYZ)
	vtkGetVector3Macro(Position, double);

	//!Set the Object velocity (WXYZ)
	vtkSetVector3Macro(Velocity, double);
	//!Get the Object velocity (WXYZ)
	vtkGetVector3Macro(Velocity, double);

	//! Get the object direction unit vector (WXYZ)
	vtkGetVector3Macro(Direction, double);

	// **** Geometrical Functions **** //
	//! Implements the translation of the item (Local coordinate system)
	/*!
		The X & Y parameters contains the relative movement in the horizontal and vertical axes respectively
		\param x x position of the item
		\param y y position of the item
		\param z z position of the item
	 */
	virtual void Translate(double x, double y, double z) = 0;

	//! Implements the translation of the item (Local coordinate system)
	/*!
			\param vector position vector of the translation
	 */
	virtual void Translate(double * vector) = 0;

	//!Set item back to Origin for rotation translation
	//void TranslateToOrigin();

	//!Restore item position from origin
	//void TranslateFromOrigin();

	//! Implements the lateral movements of the item  (Local coordinate system)
	/*!
		The X parameter contains the relative movement in the horizontal axes
		\param x x orientation angle
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateX(double x) = 0;

	//! Implements the lateral movements of the item  (Local coordinate system)
	/*!
		The Y parameter contains the relative movement in the vertical axes
		\param y y orientation angle
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateY(double y) = 0;

	//! Rotate the item on its own axes  (Local coordinate system)
	/*!
		This function rotate the item on its own axis the value of an angle given
		by the "Rotation" variable the rotation is produced acting on the actors who compose the item.
		\param rotation rotation angle (radians)
		Note: this function must be implemented in inheriting classes
	 */
	virtual void RotateZ(double rotation) = 0;

	//! Hide scenario item.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Hide() = 0;
	//! Show/Display item.
	/*!
	 * Must be implemented in inherited classes
	 * Note: this function must be implemented in inheriting classes
	 */
	virtual void Show() = 0;

	//TODO: Define Disable virtual method
	//virtual void Disable() = 0;

	//! Check if item is visible
	bool IsVisible(){ return this->Status == Visible;};

	//! Check if item is hidden
	bool IsHidden(){ return this->Status == Hidden;};

	//! Check if item is disabled
	bool IsDisabled(){ return this->Status == Disabled;};

protected:

	vtkScenarioItem();
	~vtkScenarioItem();

	//! Item identifier
	vtkIdType Id;
	//! Item Type (Tool, Organ, etc...)
	vtkScenarioItemType Type;
	//! Item Name
	char * Name;
	//! Item State
	vtkScenarioItemStatus Status;

	//**** Graphical Purposes objects ****//
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

	//!Render Window of the item
	vtkRenderWindow * RenderWindow;

	//!Renderer of the item
	vtkRenderer * Renderer;

private:
	vtkScenarioItem (const vtkScenarioItem &);
	void operator =(const vtkScenarioItem &);
};

#endif
