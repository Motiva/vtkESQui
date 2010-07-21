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
#include "vtkObject.h"

#include "vtkRenderWindow.h"

class vtkPolyData;

//! Class vtkScenarioItem, abstract the use of a surgical item
/*!
 *vtkScenarioItem abstracts the use of a surgical item during the simulation exercise.
 *This provide an easy use of surgical items collections.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkScenarioItem: public vtkObject {

public:

	//BTX
	//!Enumeration of piece types
	enum vtkScenarioItemType
	{
		Tool = 0,
		Organ = 1,
		Extra = 2
	};
	//ETX

	//!Type revision macro
	vtkTypeRevisionMacro(vtkScenarioItem,vtkObject);
	//!Return the class name
	const char *GetClassName() {return "vtkScenarioItem";}
	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Abstract initialization function
	/*!
	 * This method initializes the item physical values, scale, position, etc...
	 */
	virtual void Init() = 0;

	//!Abstract update function
	/*!
	 * This method updates the item physical values, scale, position, etc...
	 */
	virtual void Update() = 0;

	//! Returns item transformed polydata
	/*!
	 * This method returns the transformed polydata for collision detection purposes
	 */
	virtual vtkPolyData * GetOutput() = 0;

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

	//BTX
	//! Assign the identifying key of the item
	/*!
	 *\sa GetId()
	 */
	vtkSetMacro(Type, vtkScenarioItem::vtkScenarioItemType);

	//! Returns the identifying key of the item
	/*!
	 *\sa SetId(vtkIdType Id)
	 */
	vtkGetMacro(Type, vtkScenarioItem::vtkScenarioItemType);
	//ETX

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
	vtkSetMacro(Scale, double);

	//!Return item scale
	/*!
	 *\sa SetScale(double)
	 */
	vtkGetMacro(Scale, double);

	// **** Graphical Purposes Methods **** //
	//! Set the render window of the item
	/*!
	 * Assign the render window for the item
	 */
	vtkSetObjectMacro(RenderWindow, vtkRenderWindow);

	//! Get the render window of the item
	/*!
	 *Return the render window of the item
	 */
	vtkGetObjectMacro(RenderWindow, vtkRenderWindow);

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

	//! Get the object direction unit vector (WXYZ)
	vtkGetVector3Macro(Direction, double);

protected:

	vtkScenarioItem();
	~vtkScenarioItem();

	//! Item identifier
	vtkIdType Id;
	//! Item Type (Tool, Organ, etc...)
	vtkScenarioItemType Type;
	//! Item Name
	char * Name;

	//**** Graphical Purposes objects ****//
	//! Origin point for transforms
	double Origin[3];
	//! Initial position
	double Position[3];
	//! Object orientation: Yaw, Pitch, Roll angles
	double Orientation[3];
	//! Unit direction vector
	double Direction[3];

	//! Scale factor (size)
	double Scale;

	//!Render Window of the item
	vtkRenderWindow *RenderWindow;

	//!Renderer of the item
	vtkRenderer * Renderer;

	//! Update item direction vector
	void UpdateDirection();
private:
	vtkScenarioItem (const vtkScenarioItem &);
	void operator =(const vtkScenarioItem &);
};

#endif
