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

#include "vtkToolProbe.h"

#include "vtkObjectFactory.h"
#include "vtkTransformCollection.h"
#include "vtkActorCollection.h"

#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkToolProbe, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolProbe);

//----------------------------------------------------------------------------
vtkToolProbe::vtkToolProbe()
{
	this->ToolModel = Probe;
	this->NumberOfPieces = 2;
	this->StickFileName = NULL;
	this->TipFileName = NULL;
}

//----------------------------------------------------------------------------
vtkToolProbe::~vtkToolProbe()
{
	for (vtkIdType id = 0; id < this->GetNumberOfPieces() ; id++)
	{
		this->GetPiece(id)->Delete();
	}
}

//----------------------------------------------------------------------------
void vtkToolProbe::Init() {
	
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
			piece->SetPieceType(vtkPiece::Tip);
			piece->SetFileName(this->TipFileName);
		}
		piece->Init();
		this->Pieces->AddPiece(piece);
		
		this->Actors->AddItem((vtkActor*) piece->GetActor());
		this->Transforms->AddItem((vtkTransform*) piece->GetTransform());
	}
	
	Superclass::Init();
}

//----------------------------------------------------------------------------
void vtkToolProbe::Update()
{
	this->Superclass::Update();

#ifndef VTKESQUI_USE_NO_HAPTICS
	if(UseHaptic)
	{
		vtkIHP * ihp = vtkIHP::SafeDownCast(this->Haptic);
		vtkIdType id = this->GetId();

		//Trocar state
		float * state = ihp->GetTrocarState(id);

		//Trocar's direction angles

		//Haptic coordinate system differs  from VTK system
		// |  Haptic  |  VTK  |
		// |      X      |    Y    |
		// |      Y      |    Z    |
		// |      Z      |    X    |
		this->Yaw(ihp->GetTrocarYaw(id));
		this->Pitch(ihp->GetTrocarPitch(id));
		this->Roll(ihp->GetToolRoll(id));

		//Tool-in-the-trocar parameters

		//Set tool's depth
		this->SetDepth(20*ihp->GetToolDepth(id));

		//Display tool buttons/pedal state
		if(ihp->GetToolButtonState(id)){
			std::cout << "Tool("<<id<< ") Main button is pressed...\n";
		}
		if(ihp->GetLeftPedalState()){
			std::cout << "Tool("<<id<< ") Left pedal is pressed...\n";
		}
		if(ihp->GetRightPedalState()){
			std::cout << "Tool("<<id<< ") Right pedal is pressed...\n";
		}

		//TODO: Obtain Force from contact
		//Set Tool Feedback Forces
		float force [3];
		force[0] = force[1] = force[2] = 0;

		if(this->GetDepth() > 3)
		{
			std::cout << "Tool Depth > 3. Force Applied" << std::endl;
			force[2] = 1;
		}
		else force[2] = 0;

		ihp->SetToolTipForce(id, force);
		ihp->ApplyForce(id);
	}
#endif

}

//----------------------------------------------------------------------------
void vtkToolProbe::Yaw(double angle)
{
	double step = angle - this->YawAngle;
	if(step != 0)
	{
		this->RotateY(step);
		this->YawAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolProbe::Pitch(double angle)
{
	double step = angle - this->PitchAngle;
	if(step != 0)
	{
		this->RotateX(step);
		this->PitchAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolProbe::Roll(double angle)
{
	double step = angle - this->RollAngle;
	if(step != 0)
	{
		this->RotateZ(step);
		this->RollAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolProbe::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);
}
