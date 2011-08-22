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

#include "vtkToolGrasper.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkAppendPolyData.h"

#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"

vtkCxxRevisionMacro(vtkToolGrasper, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolGrasper);

//----------------------------------------------------------------------------
vtkToolGrasper::vtkToolGrasper()
{
	this->ToolModel = Grasper;
	this->Stick = this->LeftLever = this->RightLever = NULL;
	this->Opening = 0;
	this->OpeningAngle = 30;
}

//----------------------------------------------------------------------------
vtkToolGrasper::~vtkToolGrasper() 
{
}

//----------------------------------------------------------------------------
void vtkToolGrasper::Init()
{
	//Check if elements has been set
	if(this->Stick && this->LeftLever && this->RightLever)
	{
		//Remove any remaining item
		this->Elements->RemoveAllItems();
		//Elements must be inserted in this order
		if(!this->Stick->GetName() || strcmp(this->Stick->GetName(), "") == 0)
			this->Stick->SetName("Stick");
		this->AddElement(this->Stick);
		if(!this->LeftLever->GetName() || strcmp(this->LeftLever->GetName(), "") == 0)
			this->LeftLever->SetName("LeftLever");
		this->AddElement(this->LeftLever);
		if(!this->RightLever->GetName() || strcmp(this->RightLever->GetName(), "") == 0)
			this->RightLever->SetName("RightLever");
		this->AddElement(this->RightLever);
	}
	else
	{
		if(this->GetNumberOfElements() == 3)
		{
			this->Stick = this->GetElement(0);
			this->LeftLever = this->GetElement(1);
			this->RightLever = this->GetElement(2);
		}
		else
		{
			vtkErrorMacro("vtkToolGrasper has not been correctly initialized.");
		}
	}

	//Initialize super class
	Superclass::Init();
}

//----------------------------------------------------------------------------
void vtkToolGrasper::SetStick(vtkScenarioElement * e)
{
	this->Stick = e;
}

//----------------------------------------------------------------------------
void vtkToolGrasper::SetLeftLever(vtkScenarioElement * e)
{
	this->LeftLever = e;
}

//----------------------------------------------------------------------------
void vtkToolGrasper::SetRightLever(vtkScenarioElement * e)
{
	this->RightLever = e;
}

//--------------------------------------------------------------------------
/*int vtkToolGrasper::RequestData(vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{
	// get the info objects
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	this->AppendFilter->Update();

	output->ShallowCopy(this->AppendFilter->GetOutput());

	return 1;
}*/

//----------------------------------------------------------------------------
void vtkToolGrasper::Open(){
	if(this->IsClosed()) {
		this->SetOpening(1);
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::Close(){
	if(!this->IsClosed()) {
		this->SetOpening(0);
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::SetOpening(double opening) {
	//cout << "vtkToolGrasper::SetOpening: " << opening << "\n";
	if(this->Opening != opening)
	{
		double step = opening - this->Opening;

		//Left Lever
		this->LeftLever->RotateX(-this->OpeningAngle*step);
		this->LeftLever->Update();

		//Right Lever
		this->RightLever->RotateX(this->OpeningAngle*step);
		this->RightLever->Update();

		this->Opening = opening;
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::SetDepth(double position)
{
	//cout << "vtkToolGrasper::SetDepth: " << position << "\n";
	double o = this->Opening;
	//Grasper is clamped before being translated
	this->SetOpening(0);
	Superclass::SetDepth(position);
	//Grasper' opening state is restored
	this->SetOpening(o);

}

//----------------------------------------------------------------------------
void vtkToolGrasper::RotateX(double angle)
{
	//cout << "vtkToolGrasper::RotateX: " << angle << "\n";
	double o = this->Opening;
	//Grasper is clamped before being rotated
	this->SetOpening(0);
	Superclass::RotateX(angle);
	//Grasper' opening state is restored
	this->SetOpening(o);

	//Yaw angle value is updated
	//this->YawAngle += angle;
}

//----------------------------------------------------------------------------
void vtkToolGrasper::RotateY(double angle)
{
	//cout << "vtkToolGrasper::RotateY: " << angle << "\n";
	double o = this->Opening;
	//Grasper is clamped before being rotated
	this->SetOpening(0);
	Superclass::RotateY(angle);
	//Grasper' opening state is restored
	this->SetOpening(o);

	// Pitch angle value is updated
	//this->PitchAngle += angle;
}

//----------------------------------------------------------------------------
void vtkToolGrasper::RotateZ(double angle)
{
	//cout << "vtkToolGrasper::RotateY: " << angle << "\n";
	double o = this->Opening;
	//Grasper is clamped before being rotated
	this->SetOpening(0);
	Superclass::RotateZ(angle);
	//Grasper' opening state is restored
	this->SetOpening(o);

	//this->RollAngle += angle;
}

//----------------------------------------------------------------------------
void vtkToolGrasper::Yaw(double angle)
{
	double step = angle - this->YawAngle;
	if(step != 0)
	{
		this->RotateY(step);
		this->YawAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::Pitch(double angle)
{
	double step = angle - this->PitchAngle;
	if(step != 0)
	{
		this->RotateX(step);
		this->PitchAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::Roll(double angle)
{
	double step = angle - this->RollAngle;
	if(step != 0)
	{
		this->RotateZ(step);
		this->RollAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolGrasper::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);
	os << indent << "Opening: " << this->Opening << endl;
	os << indent << "Opening Angle: " << this->OpeningAngle << endl;
}
