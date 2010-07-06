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
#include "vtkPiece.h"

vtkCxxRevisionMacro(vtkPiece, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkPiece);

//--------------------------------------------------------------------------
vtkPiece::vtkPiece()
{
	this->Renderer = NULL;
	this->RenderWindow = NULL;
	this->Name = NULL;
	this->FileName = NULL;
	this->PieceType = vtkPiece::Stick;
	this->Id = -1;
	this->Reader = NULL;
	this->PolyData = NULL;
	this->TransformFilter = NULL;
	this->Transform = NULL;
	this->Actor = NULL;
	this->Mapper = NULL;

	//Bounding Box Display
	this->OBB = NULL;
	this->SimpleMesh = NULL;
	this->SimpleMeshActor = NULL;
	this->SimpleMeshMapper = NULL;
}

//--------------------------------------------------------------------------
vtkPiece::~vtkPiece()
{
	if(this->TransformFilter) this->TransformFilter->Delete();
	if(this->Transform) this->Transform->Delete();
	if(this->Actor) this->Actor->Delete();
	if(this->Mapper) this->Mapper->Delete();
	if(this->Reader) this->Reader->Delete();

	if(this->OBB) this->OBB->Delete();

	if(this->SimpleMeshActor) this->SimpleMeshActor->Delete();
	if(this->SimpleMeshMapper) this->SimpleMeshMapper->Delete();
}

//--------------------------------------------------------------------------
void vtkPiece::Init()
{
	//Read polydata source file
	if(this->FileName)
	{
		this->Reader = vtkPolyDataReader::New();
		this->Reader = vtkPolyDataReader::New();
		this->Reader->SetFileName(this->FileName);
		this->Reader->Update();
		this->PolyData = this->Reader->GetOutput();
	}

	if(this->RenderWindow)
	{
		this->Renderer = this->RenderWindow->GetRenderers()->GetFirstRenderer();

		this->Transform = vtkTransform::New();
		this->TransformFilter = vtkTransformPolyDataFilter::New();
		this->TransformFilter->SetInput(this->PolyData);
		this->TransformFilter->SetTransform(this->Transform);
		this->TransformFilter->Update();
		this->Mapper = vtkDataSetMapper::New();
		this->Mapper->SetInput(this->TransformFilter->GetOutput());
		this->Actor = vtkActor::New();
		this->Actor->SetMapper(this->Mapper);

		//Bounding Box Display
		this->OBB = vtkOBBTree::New();
		this->OBB->SetDataSet(this->TransformFilter->GetOutput());
		this->OBB->SetMaxLevel(8);
		this->OBB->SetTolerance(0.0001);
		this->OBB->BuildLocator();
		//this->OBB->GenerateRepresentation(4,this->SimpleMesh);
		this->SimpleMesh = this->TransformFilter->GetOutput();

		this->SimpleMeshMapper = vtkDataSetMapper::New();
		this->SimpleMeshMapper->SetInput(this->SimpleMesh);

		this->SimpleMeshActor = vtkActor::New();
		this->SimpleMeshActor->SetMapper(SimpleMeshMapper);
		this->SimpleMeshActor->GetProperty()->SetColor(0.5, 0.8, 0.5);
		this->SimpleMeshActor->GetProperty()->SetOpacity(0.1);

		this->Renderer->AddActor(this->Actor);
		this->Renderer->AddActor(this->SimpleMeshActor);
	}

}

//--------------------------------------------------------------------------
void vtkPiece::Update()
{
}
//--------------------------------------------------------------------------
vtkPolyData * vtkPiece::GetSimpleMesh()
{
	return this->SimpleMesh;
}

//--------------------------------------------------------------------------
void vtkPiece::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Name: " << this->Name << endl;
	os << indent << "Id: " << this->Id << endl;
	os << indent << "Type: " << this->PieceType << endl;
	os << indent << "FileName: " << this->FileName << endl;
	os << indent << "PolyData (#points): " << this->PolyData->GetNumberOfPoints() << endl;

	double * bounds = this->PolyData->GetBounds();
	os << indent << "PolyData Bounds: ";
	for (int i=0;i<6;i++)
	{
		os << bounds[i] << ", ";
	}
	os << indent << "\n";

	bounds = this->Actor->GetBounds();
	os << indent << "Actor Bounds: ";
	for (int i=0;i<6;i++)
	{
		os << bounds[i] << ", ";
	}
	os << indent << "\n";
}
