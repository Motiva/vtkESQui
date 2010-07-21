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

#include "vtkPointPlotter.h"

#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkGlyph3D.h"
#include "vtkSphereSource.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"


vtkCxxRevisionMacro(vtkPointPlotter, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkPointPlotter);

//----------------------------------------------------------------------------

vtkPointPlotter::vtkPointPlotter()
{
	this->Actor = vtkActor::New();
	this->Mapper = vtkPolyDataMapper::New();
	this->PolyData = vtkPolyData::New();

	this->Glyphs = vtkGlyph3D::New();
	this->Spheres = vtkSphereSource::New();

	this->Points = vtkPoints::New();
	this->Scalars = vtkUnsignedCharArray::New();
}

//--------------------------------------------------------------------------
vtkPointPlotter::~vtkPointPlotter()
{
	this->Actor->Delete();
	this->Mapper->Delete();
	this->PolyData->Delete();

	this->Glyphs->Delete();
	this->Spheres->Delete();

	this->Points->Delete();
	this->Scalars->Delete();
}

//--------------------------------------------------------------------------
void vtkPointPlotter::InsertPoint(double x, double y, double z, unsigned char r, unsigned char g, unsigned char b)
{
	this->Points->InsertNextPoint(x, y, z);
	this->Scalars->InsertNextTuple3(r, g, b);

	this->PolyData->SetPoints(this->Points);
	this->PolyData->Update();

}

//--------------------------------------------------------------------------
void vtkPointPlotter::Init()
{
	this->Points->Reset();
	this->Scalars->SetNumberOfComponents(3);

	this->Spheres->SetRadius(this->Radius);
	this->Spheres->SetThetaResolution(this->Resolution);
	this->Spheres->SetPhiResolution(this->Resolution);

	this->PolyData->GetPointData()->SetScalars(this->Scalars);

	this->Glyphs->SetInput(this->PolyData);
	this->Glyphs->SetSource(this->Spheres->GetOutput());
	this->Glyphs->SetColorModeToColorByScalar();
	this->Glyphs->SetScaleModeToDataScalingOff();

	this->Mapper->SetInput(this->Glyphs->GetOutput());
	this->Mapper->GlobalImmediateModeRenderingOn();
	this->Mapper->ScalarVisibilityOn();
	this->Actor->SetMapper(this->Mapper);

	this->Renderer->AddActor(this->Actor);

}

//--------------------------------------------------------------------------
void vtkPointPlotter::Update()
{
	this->Glyphs->Update();
}

//--------------------------------------------------------------------------
void vtkPointPlotter::Reset()
{
	this->Points->Reset();
}
