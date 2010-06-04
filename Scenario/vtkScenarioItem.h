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

#include "vtkObject.h"
#include "vtkTransform.h"
#include "vtkESQuiScenarioWin32Header.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyDataCollection.h"
#include "vtkMapperCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkTransformCollection.h"
#include "vtkAssembly.h"
#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkMath.h"
#include "vtkDecimatePro.h"

//! Class vtkScenarioItem, abstract the use of a surgical item
/*!
vtkScenarioItem abstracts the use of a surgical item during the simulation exercise.
This provide an easy use of surgical items collections.
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
	virtual void Update();

	//!Abstract simple mesh update function
	/*!
	* This method updates the simplified mesh item physical values, scale, position, etc...
	*/
	virtual void UpdateSimpleMesh() = 0;

	//! Assign the identifying key of the item
	/*!
	\param id identifying key of the item
	\sa GetId()
	*/
	void SetId(vtkIdType id);

	//! Returns the identifying key of the item
	/*!
	\sa SetId(vtkIdType Id)
	*/
	vtkIdType GetId();

	//BTX
	//! Assign the identifying key of the item
	/*!
	\param id identifying key of the item
	\sa GetId()
	*/
	void SetType(vtkScenarioItem::vtkScenarioItemType type);

	//! Returns the identifying key of the item
	/*!
	\sa SetId(vtkIdType Id)
	*/
	vtkScenarioItem::vtkScenarioItemType GetType();
	//ETX

	//! Assign item name
	/*!
	\param name name of the item
	\sa GetName()
	*/
	void SetName(const char * name);

	//!Return item scale
	/*!
	\sa SetName(const char * name)
	*/
	const char * GetName();

	//! Assign item scale
	/*!
	*\param value scale factor
	*\sa GetScale()
	*/
	void SetScale(double value);

	//!Return item scale
	/*!
	*\sa SetScale(double scale)
	*/
	double GetScale();

	// **** Graphical Purposes Methods **** //
	//! Set the render window of the item
	/*!
	Assign the render window for the item
	\param window Render Window where item will be displayed
	*/
	void SetRenderWindow(vtkRenderWindow *window);

	//! Get the render window of the item
	/*!
	Return the render window of the item
	*/
	vtkRenderWindow *GetRenderWindow();

	//! Set the object origin Point. Reference point where the rotation calculus are made
	void SetOrigin(double x, double y, double z);
	//! Set the object origin Point. Reference point where the rotation calculus are made
	void SetOrigin(double origin[3]);
	//! Get the object origin Point. Reference point where the rotation calculus are made
	double * GetOrigin();
	//! Get the object origin Point. Reference point where the rotation calculus are made
	void GetOrigin(double origin[3]);

	//! Set object orientation angles. (yaw, pitch, roll) (WXYZ)
	void SetOrientation(double yaw, double pitch, double roll);
	//! Set object orientation angles. (yaw, pitch, roll) (WXYZ)
	void SetOrientation(double orientation[3]);
	//! Get object orientation angles. (yaw, pitch, roll) (WXYZ)
	double * GetOrientation();
	//! Get object orientation angles. (yaw, pitch, roll) (WXYZ)
	void GetOrientation(double orientation[3]);

	//!Set the Object position (WXYZ)
	void SetPosition(double x, double y, double z);
	//!Set the Object position (WXYZ)
	void SetPosition(double position[3]);
	//!Get the Object position (WXYZ)
	double * GetPosition();
	//!Get the Object position (WXYZ)
	void GetPosition(double position[3]);

	//! Get the object direction unit vector (WXYZ)
	double * GetDirection();
	//! Get the object direction unit vector (WXYZ)
	void GetDirection(double direction[3]);

	//! Enable simple mesh display
	/*!
	\sa DisplaySimpleMeshOff()
	*/
	void DisplaySimpleMeshOn(){this->DisplaySimpleMesh = 1;};

	//! Enable simple mesh display
	/*!
	\sa DisplaySimpleMeshOff()
	*/
	void DisplaySimpleMeshOff(){this->DisplaySimpleMesh = 0;};

	//!function that returns simplified mesh
	/*!
	* This method returns a simplified mesh for collision detection purposes
	*/
	vtkPolyData * GetSimpleMesh(){return this->SimpleMesh;};

protected:

	vtkScenarioItem();
	~vtkScenarioItem();

	vtkIdType Id;
	vtkScenarioItemType Type;
	const char * Name;

	//**** Graphical Purposes objects ****//
	double Origin[3];
	double Position[3];
	double Orientation[3];
	double Direction[3];

	//! Scale factor (size)
	double ScaleFactor;

	vtkIdType DisplaySimpleMesh;

	//!Render Window of the item
	vtkRenderWindow *RenderWindow;

	//!Renderer of the item
	vtkRenderer * Renderer;

	//!Simplified Mesh for collision detection purposes
	vtkPolyData * SimpleMesh;

	void UpdateDirection();
private:
	vtkScenarioItem (const vtkScenarioItem &);
	void operator =(const vtkScenarioItem &);
};

#endif
