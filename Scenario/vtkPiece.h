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
//vtkPiece
//----------------------
#ifndef __vtkPiece_h
#define __vtkPiece_h

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkOBBTree.h"

#include "vtkESQuiScenarioWin32Header.h"

//!This class defines an standard vtkTool piece
//TODO: Migrate this class to vtkScenarioItem. vtkTool must not inherit vtkScenarioItem

class VTK_ESQUI_SCENARIO_EXPORT vtkPiece:public vtkObject
{

public:

	//BTX
	//!Enumeration of piece types
	enum vtkPieceType{
		Blade = 0,
		Grasper = 1,
		Stick = 2,
		Ball = 3
	};
	//ETX

	//! Type revision macro
	vtkTypeRevisionMacro(vtkPiece, vtkObject);
	//! Crete new tool piece
	static vtkPiece *New();
	//! Return clas name
	const char *GetClassName() {return "vtkPiece";};

	//!Initialize Piece object
	/*!
	 * This method will apply the initial conditions set on the piece
	 * At least set the filename path before calling this method
	 */
	void Init();

	//!Update Piece object
	/*!
	 * This method will update the piece
	 * At least set the filename path before calling this method
	 */
	void Update();

	//BTX
	//!Set piece type
	/*!
	 * \sa GetType()
	 */
	vtkSetMacro(PieceType, vtkPiece::vtkPieceType);

	//!Get piece type
	/*!
	 * \sa SetType(vtkPiece::vtkPieceType)
	 */
	vtkGetMacro(PieceType, vtkPiece::vtkPieceType);
	//ETX

	//!Set piece name
	/*!
	 * \sa GetName()
	 */
	vtkSetStringMacro(Name);

	//!Get piece name
	/*!
	 * \sa SetName(const char *)
	 */
	vtkGetStringMacro(Name);

	//!Set filename path
	/*!
	 * Overwrites previously assigned polydata.
	 * \sa GetFileName()
	 */
	vtkSetStringMacro(FileName);

	//!Get filename path
	/*!
	 * SetFileName(const char *)
	 */
	vtkGetStringMacro(FileName);

	//!Set piece identifier
	/*!
	 * \sa GetId()
	 */
	vtkSetMacro(Id, vtkIdType);

	//!Get piece identifier
	/*!
	 * \sa SetId(vtkIdType)
	 */
	vtkGetMacro(Id, vtkIdType);

	//!Set piece polydata.
	/*!
	 * Piece Polydata contains all the info required for displaying the piece
	 */
	void SetPolyData(vtkPolyData * polyData){this->PolyData->DeepCopy(polyData);};

	//!Get piece polydata
	/*!
	 * Piece Polydata contains all the info required for displaying
	 */
	vtkPolyData * GetPolyData(){return this->PolyData;};

	// **** Graphical Purposes Methods **** //

	//! Set the render window of the tool
	/*!
	 * Assign the render window for the tool
	 */
	vtkSetObjectMacro(RenderWindow, vtkRenderWindow);

	//! Get the render window of the tool
	/*!
	 * Return the render window of the tool
	 */
	vtkGetObjectMacro(RenderWindow, vtkRenderWindow);

	//!Get piece polydata output
	/*!
	 * Get transformed polydata for collision detection purposes
	 */
	vtkPolyData * GetOutput();

	//!Get transform function
	/*!
	 * Transform function for displaying purposes
	 */
	vtkGetObjectMacro(Transform, vtkTransform);

	//!Get dataSet mapper
	/*!
	 * Dataset mapper of the piece actor
	 */
	vtkGetObjectMacro(Mapper, vtkDataSetMapper);

	//!Get piece actor
	/*!
	 * The piece actor will be used for displaying purposes
	 */
	vtkGetObjectMacro(Actor, vtkActor);

	//!Print class values
	void PrintSelf(ostream& os,vtkIndent indent);

protected:

	vtkPiece();
	~vtkPiece();

private:

	vtkPiece (const vtkPiece &);
	vtkPiece operator =(const vtkPiece &);

	//!Piece identifier
	vtkIdType Id;

	//!Piece type
	vtkPieceType PieceType;

	//Piece name
	char * Name;

	//!File path to polydata VTK file
	char * FileName;

	//!Polydata reader
	vtkPolyDataReader * Reader;

	//!Polydata of the piece
	vtkPolyData * PolyData;

	//!Render Window of the item
	vtkRenderWindow *RenderWindow;

	//!Renderer of the item
	vtkRenderer * Renderer;

	//!Transform filter of the piece
	vtkTransformPolyDataFilter *TransformFilter;

	//!Transform function of the filter
	vtkTransform * Transform;

	//!DataSetMapper of the piece data
	vtkDataSetMapper * Mapper;

	//!Tool piece actor
	vtkActor * Actor;

};
#endif
