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
#ifndef __vtkPointPlotter_h
#define __vtkPointPlotter_h
#include "vtkESQuiUtilitiesWin32Header.h"
#include "vtkObject.h"

class vtkRenderer;
class vtkActor;
class vtkSphereSource;
class vtkGlyph3D;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkPoints;
class vtkUnsignedCharArray;


//!This class performs the display of a vtkPoints array
class VTK_ESQUI_UTILITIES_EXPORT vtkPointPlotter : public vtkObject {

public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkPointPlotter,vtkObject);

	//!Create a new point plotter
	static vtkPointPlotter *New();

	//! Return class name
	const char *GetClassName() {return "vtkPointPlotter";};

	//! Initialize plotter
	void Init();

	//! Update the plotter with new points
	void Update();

	//! Restore plotter to initial settings
	void Reset();

	//! Set point radius
	void SetRadius(double radius = 0.025){this->Radius = radius;};

	//! Set point/sphere resolutiom
	void SetResolution(int resolution = 12){this->Resolution = resolution;};

	//! Set Renderer in which points will be displayed
	void SetRenderer(vtkRenderer * renderer){this->Renderer = renderer;};

	//! Return actor
	vtkActor * GetActor(){return this->Actor;};

	//! Return generated polydata
	vtkPolyData * GetPolyData(){return this->PolyData;};

	//! Insert a new point in the polydata
	void InsertPoint(double x, double y, double z, unsigned char r, unsigned char g, unsigned char b);

protected:
	vtkPointPlotter();
	~vtkPointPlotter();
private:

	//! Container of points
	vtkPoints * Points;
	//! Scalars. Color of points
	vtkUnsignedCharArray * Scalars;

	//! Generated polydata for displaying purpoese
	vtkPolyData * PolyData;
	//! Representation Glyphs
	vtkGlyph3D * Glyphs;
	//! Points shape
	vtkSphereSource * Spheres;

	//! Polydata Actor
	vtkActor * Actor;
	//! Polydata mapper
	vtkPolyDataMapper * Mapper;

	//! Renderer
	vtkRenderer * Renderer;

	//! Point/sphere radius
	double Radius;
	//! Sphere resolution
	vtkIdType Resolution;

	vtkPointPlotter (const vtkPointPlotter &);//NotImplemented
	void operator =(const vtkPointPlotter &);//Not Implemented
};
#endif
