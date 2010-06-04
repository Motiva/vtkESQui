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
// fichero que implementa la clase para representar las pinzas
#include "vtkToolDummy.h"

vtkCxxRevisionMacro(vtkToolDummy, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolDummy);

//----------------------------------------------------------------------------
vtkToolDummy::vtkToolDummy() {

	//Physical pieces Tool Construction
	vtkPiece * piece;

	for(vtkIdType i = 0; i<2; i++)
	{
		piece = vtkPiece::New();
		piece->SetId(0);
		this->Pieces->InsertNextPiece(piece);
	}

	// Tool Piece Types (id)
	// 0 -> Stick
	this->Pieces->GetPiece(0)->SetType(vtkPiece::Stick);
	this->Pieces->GetPiece(0)->SetType(vtkPiece::Ball);
}

//----------------------------------------------------------------------------
vtkToolDummy::~vtkToolDummy()
{
	for (vtkIdType id = 0; id < this->GetNumberOfPieces() ; id++)
	{
		this->GetPiece(id)->Delete();
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Init() {

	vtkPiece * piece;

	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		piece = this->GetPiece(id);
		piece->SetRenderWindow(this->RenderWindow);
		piece->Init();

		this->Actors->AddItem((vtkActor*) piece->GetActor());
		this->Transforms->AddItem((vtkTransform*) piece->GetTransform());
	}

	this->ApplyInitialTransform();

	this->Update();
}

//----------------------------------------------------------------------------
void vtkToolDummy::Update()
{
	this->Superclass::Update();
}

//----------------------------------------------------------------------------
void vtkToolDummy::SetPolyData(vtkIdType id, vtkPolyData* polyData)
{
	this->GetPiece(id)->SetPolyData(polyData);
}

void vtkToolDummy::Yaw(double angle)
{
	double step = angle - this->YawAngle;
	if(step != 0)
	{
		this->RotateY(step);
		this->YawAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Pitch(double angle)
{
	//std::cout << "angle: " << angle << " | Pitch: " << this->PitchAngle << std::endl;
	double step = angle - this->PitchAngle;
	if(step != 0)
	{
		this->RotateX(step);
		this->PitchAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Roll(double angle)
{
	double step = angle - this->RollAngle;
	if(step != 0)
	{
		this->RotateZ(step);
		this->RollAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);
}
