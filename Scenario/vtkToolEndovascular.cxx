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
#include "vtkToolEndovascular.h"

#include "vtkTransform.h"
#include "vtkTransformCollection.h"

vtkCxxRevisionMacro(vtkToolEndovascular, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkToolEndovascular::vtkToolEndovascular() {
	this->Depth = 0;
	this->YawAngle = 0;
	this->PitchAngle = 0;
	this->RollAngle = 0;
	this->UseHaptic = 0;
	this->ForceFeedback[0] = this->ForceFeedback[1] = this->ForceFeedback[2] = 0;
}

//--------------------------------------------------------------------------
vtkToolEndovascular::~vtkToolEndovascular() {

}

//--------------------------------------------------------------------------
void vtkToolEndovascular::ApplyInitialTransform()
{
	Superclass::ApplyInitialTransform();
	this->YawAngle = this->Orientation[0];
	this->PitchAngle = this->Orientation[1];
	this->RollAngle = this->Orientation[2];
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::SetDepth(double depth)
{
	if(depth != this->Depth)
	{
		double step = this->Depth - depth;
		Superclass::Translate(0, 0, step);
		this->GetTransform(0)->GetPosition(this->Position);
		this->Depth = depth;
	}
}

//--------------------------------------------------------------------------
double vtkToolEndovascular::GetDepth()
{
	return this->Depth;
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::AddDepth(double step) {
	//Note the step negative sign. VTK Z Axis is inverted
	double depth = this->Depth - step;
	this->SetDepth(depth);
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::RotateX(double angle)
{
	double roll = this->Orientation[2];

	Superclass::Translate(0, 0, 4);
	Superclass::RotateZ(-roll);
	Superclass::RotateX(angle);
	Superclass::RotateZ(roll);
	Superclass::Translate(0, 0, -4);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::RotateY(double angle)
{
	double roll = this->Orientation[2];

	Superclass::Translate(0, 0, 4);
	Superclass::RotateZ(-roll);
	Superclass::RotateY(angle);
	Superclass::RotateZ(roll);
	Superclass::Translate(0, 0, -4);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::RotateZ(double angle)
{
	Superclass::RotateZ(angle);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
double *  vtkToolEndovascular::GetContactForceValue() {
/*	int NumberOfContacts;
	float *ForceFeedback;
	float *TotalForceFeedback;
	ForceFeedback = new float[3];
	TotalForceFeedback = new float[3];
	TotalForceFeedback[0] = TotalForceFeedback[1] = TotalForceFeedback[2] = 0;
	NumberOfContacts = this->_vContacts.size();
	for (int i = 0; i < NumberOfContacts ; i++) {
		//cout << "Dentro del bucle ITERACION " << i << endl;
		this->_vContacts[i]->GetForceFeedback(ForceFeedback);
		//cout << this->_vContacts[i]->GetXForce() << endl;
		TotalForceFeedback[0] = TotalForceFeedback[0] + ForceFeedback[0];
		TotalForceFeedback[1] = TotalForceFeedback[1] + ForceFeedback[1];
		TotalForceFeedback[2] = TotalForceFeedback[2] + ForceFeedback[2];
		cout << "Fx = " << ForceFeedback[0] << " Fy = " << ForceFeedback[1] << " Fz = " << ForceFeedback[2] << endl;
		cout << "Herramienta asignada " << this->_vContacts[i]->GetTool() << endl;
	}
	TotalForceFeedback[0]  = TotalForceFeedback[0] / NumberOfContacts;
	TotalForceFeedback[1]  = TotalForceFeedback[1] / NumberOfContacts;
	TotalForceFeedback[2]  = TotalForceFeedback[2] / NumberOfContacts;
	cout << "Fx = " << TotalForceFeedback[0] << "Fy = " << TotalForceFeedback[1] << "Fz = " << TotalForceFeedback[2] << endl;
*/
	//FIXME: redo this method
	return this->ForceFeedback;
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::SetForceFeedback(float Force[]){
	this->ForceFeedback[0] = Force[0];
	this->ForceFeedback[1] = Force[1];
	this->ForceFeedback[2] = Force[2];
}

//--------------------------------------------------------------------------
void vtkToolEndovascular::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Depth: " << this->Depth << "\n";
	os << indent << "Pitch: " << this->PitchAngle << "\n";
	os << indent << "Yaw: " << this->YawAngle << "\n";
	os << indent << "Roll: " << this->RollAngle << "\n";
	os << indent << "ForceFeedback: " << this->ForceFeedback[0] <<  ", " << this->ForceFeedback[1] <<  ", " << this->ForceFeedback[2] << "\n";

}
