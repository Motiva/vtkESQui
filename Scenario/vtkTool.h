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
#ifndef __vtkTool
#define __vtkTool

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
#include "vtkActorCollection.h"
#include "vtkAppendPolyData.h"
#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkMath.h"
#include "vtkScenarioItem.h"

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#include "vtkIHP.h"
#include "vtkLSW.h"
#endif

// Fichero que implementa la clase vtkTool, para abstraerse del tipo de herramienta
/*
    vtkTool es la clase que da soporte a la simulacion para abstraerse del tipo de
    herramienta utilizada. De esta forma se pueden usar colecciones de herramientas sin
    problema alguno.
 */

//! Class vtkTool, abstract the use of a surgical tool
/*!
vtkTool abstracts the use of a surgical tool during the simulation exercise.
This provide an easy use of surgical tools collections.
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkTool: public vtkScenarioItem {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkTool,vtkScenarioItem);

	//!Return the class name
	const char *GetClassName() {return "vtkTool";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set FileName for each piece
	void SetFileName(vtkIdType id, const char * filename);

	//! Get FileName for each piece
	const char * GetFileName(vtkIdType id);

	// **** Physical Parameters **** //

	//!Return tool piece with the specified id
	/*!
	* id Identifier of the tool piece
	*/
	vtkPiece * GetPiece(vtkIdType id);

	//! Return the number of pieces
	/*!
	* Total number of pieces of the tool
	*/
	vtkIdType GetNumberOfPieces();

	//!Set tool type
	vtkSetStringMacro(ToolType);
	//!Return tool type
	vtkGetStringMacro(ToolType);

	//!Abstract initialization function
	/*!
	* This method initializes the tool physical values, scale, position, etc...
	*/
	virtual void Init() = 0;

	//!Abstract update function
	/*!
	* This method updates the tool physical values, scale, position, etc...
	*/
	virtual void Update();
	//!function that returns transformed polydata
	/*!
	* This method returns the transformed polydata for collision detection purposes
	*/
	virtual vtkPolyData * GetOutput();

	//! Applies the initial transformation to the tool loaded from vtk files
	/*!
	Applies the initial transformation, placing the tool in its position in the scene.
	*/
	virtual void ApplyInitialTransform();

	// **** Graphical Purposes Methods **** //
	//! Return the actors collection of the tool
	/*!
	Return a pointer to the object who stores all tool's actors
	*/
	vtkActorCollection* GetActorCollection();

	//! Return the actor of the specified piece
	/*!
	* \param id Identifier of the piece
	*/
	vtkActor * GetActor(vtkIdType id);

	//!Get tool piece transform function with the specified id
	/*!
	* \param id Identifier of the piece
	*/
	vtkTransform * GetTransform(vtkIdType id);

	// **** Simulation Manager Methods **** //
	//!Remove all contacts from the collection
	/*!
	* All Contacts are cleared but not erased on memory
	*/
	void RemoveContacts() {this->Contacts->RemoveAllItems();}

	//!Add a contact to the collection
	/*!
	* Add a single contact to the collection of contacts
	* \param contact Contact point
	*/
	void InsertNextContact(vtkContact* contact);

	/// Returns the number of contacts the tool has made
	/*!
	* Total number of tool contact points
	*/
	vtkIdType GetNumberOfContacts(){return this->Contacts->GetNumberOfItems();}

	// **** Haptic Management Methods **** //

	//! Set the use of haptic device
	vtkSetMacro(UseHaptic, bool);
	//! Get the use of haptic device
	vtkGetMacro(UseHaptic, bool);

	//!Enable/Disable haptic device use
	vtkBooleanMacro(UseHaptic, bool);

#ifndef VTKESQUI_USE_NO_HAPTICS
	//BTX
	//! Set the haptic device for the tool
	void SetHapticDevice(vtkHaptic *haptic) {this->Haptic = haptic;}

	//! Get the haptic device for the tool
	vtkHaptic *GetHapticDevice() {return this->Haptic;}

protected:
	//! The haptic device for the tool
	vtkHaptic * Haptic;
	//ETX
#endif


protected:

	vtkTool();
	~vtkTool();

	//! Tool Type
	char * ToolType;

	//! Enable haptic device control
	int UseHaptic;

	//!Collection of tool pieces
	vtkPieceCollection * Pieces;

	//**** Graphical Purposes objects ****//

	//!Collection of tool pieces actors
	vtkActorCollection * Actors;

	//!Collection of tool pieces transforms
	vtkTransformCollection * Transforms;

	//!Collection of contact point information
	vtkContactCollection *Contacts;

	//! Appended Polydata
	vtkAppendPolyData *AppendFilter;

	// **** Geometrical Functions **** //
	//! Implements the translation of the tool (Local coordinate system)
	/*!
	The X & Y parameters contains the relative movement in the horizontal and vertical axes respectively
	\param x x position of the tool
	\param y y position of the tool
	\param z z position of the tool
	*/
	void Translate(double x, double y, double z);

	//! Implements the lateral movements of the tool  (Local coordinate system)
	/*!
	The X parameter contains the relative movement in the horizontal axes
	\param x x orientation angle
	*/
	void RotateX(double x);

	//! Implements the lateral movements of the tool  (Local coordinate system)
	/*!
	The Y parameter contains the relative movement in the vertical axes
	\param y y orientation angle
	*/
	void RotateY(double y);

	//! Rotate the tool on its own axes  (Local coordinate system)
	/*!
	This function rotate the tool on its own axis the value of an angle given
	by the "Rotation" variable the rotation is produced acting on the actors who compose the tool.
	\param rotation rotation angle (radians)
	*/
	void RotateZ(double rotation);

private:
	vtkTool (const vtkTool &);
	void operator =(const vtkTool &);
};

#endif
