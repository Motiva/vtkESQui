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

#include "vtkToolScissors.h"

#include "vtkObjectFactory.h"
#include "vtkTransformCollection.h"
#include "vtkActorCollection.h"

#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkToolScissors, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolScissors);

//--------------------------------------------------------------------------
vtkToolScissors::vtkToolScissors(){
	this->ToolModel = Scissors;
	this->Opening = 0;
	this->NumberOfPieces = 3;
	this->StickFileName = NULL;
	this->LeftBladeFileName = NULL;
	this->RightBladeFileName = NULL;
}

//--------------------------------------------------------------------------
vtkToolScissors::~vtkToolScissors(){

	for (vtkIdType id = 0; id < 3 ; id++)
	{
		this->GetPiece(id)->Delete();
	}

	this->Pieces->RemoveAllItems();
	this->Pieces->Delete();
}
//--------------------------------------------------------------------------
void vtkToolScissors::Init(){

	//Physical pieces Tool Construction
	vtkPiece * piece;

	for (vtkIdType id = 0; id < this->NumberOfPieces ; id++)
	{
		piece = vtkPiece::New();
		piece->SetId(id);
		piece->SetRenderWindow(this->RenderWindow);
		if(id == 0) {
			piece->SetPieceType(vtkPiece::Stick);
			piece->SetFileName(this->StickFileName);
		}
		else {
			piece->SetPieceType(vtkPiece::Blade);
			if(id==1) piece->SetFileName(this->LeftBladeFileName);
			else piece->SetFileName(this->RightBladeFileName);
		}
		piece->Init();
		this->Pieces->AddPiece(piece);

		this->Actors->AddItem((vtkActor*) piece->GetActor());
		this->Transforms->AddItem((vtkTransform*) piece->GetTransform());
	}

	Superclass::Init();

}

//--------------------------------------------------------------------------
void vtkToolScissors::Update()
{
	this->Superclass::Update();
}

//----------------------------------------------------------------------------
void vtkToolScissors::SetOpening(double opening) {
	double step = opening - this->Opening;
	this->GetLeftBlade()->GetTransform()->RotateX(-20*step);
	this->GetRightBlade()->GetTransform()->RotateX(20*step);
}

//--------------------------------------------------------------------------
void vtkToolScissors::PrintSelf(ostream& os,vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}

