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
#include "vtkToolLaparoscopy.h"

#include "vtkScenarioObject.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"

vtkCxxRevisionMacro(vtkToolLaparoscopy, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkToolLaparoscopy::vtkToolLaparoscopy() {
  this->ToolType =  Laparoscopy;
  this->Depth = 0;
  this->YawAngle = 0;
  this->PitchAngle = 0;
  this->RollAngle = 0;
  this->ForceFeedback[0] = this->ForceFeedback[1] = this->ForceFeedback[2] = 0;
}

//--------------------------------------------------------------------------
vtkToolLaparoscopy::~vtkToolLaparoscopy() {

}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::Init()
{
  Superclass::Init();

  double * orientation = this->GetElement(0)->GetOrientation();
  this->YawAngle = orientation[0];
  this->PitchAngle = orientation[1];
  this->RollAngle = orientation[2];
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::SetDepth(double depth)
{
  if(depth != this->Depth)
  {
    double step = this->Depth - depth;
    Superclass::Translate(0, 0, step);
    this->Depth = depth;
  }
}

//--------------------------------------------------------------------------
double vtkToolLaparoscopy::GetDepth()
{
  return this->Depth;
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::Pull() {
  //Note the step negative sign. VTK Z Axis is inverted
  double step = 0.1;
  double depth = this->Depth - step;
  this->SetDepth(depth);
}

void vtkToolLaparoscopy::Push() {
  //Note the step negative sign. VTK Z Axis is inverted
  double step = -0.1;
  double depth = this->Depth - step;
  this->SetDepth(depth);
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateX(double angle)
{
  this->Reset();
  //Rotate X
  Superclass::RotateX(angle);
  this->Restore();
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateY(double angle)
{
  this->Reset();
  //Rotate Y
  Superclass::RotateY(angle);
  this->Restore();
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateZ(double angle)
{
  Superclass::RotateZ(angle);
}


//--------------------------------------------------------------------------
double *  vtkToolLaparoscopy::GetContactForceValue() {
  return this->ForceFeedback;
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::SetForceFeedback(float Force[]){
  this->ForceFeedback[0] = Force[0];
  this->ForceFeedback[1] = Force[1];
  this->ForceFeedback[2] = Force[2];
}

//----------------------------------------------------------------------------
void vtkToolLaparoscopy::Yaw(double angle)
{
  double step = angle - this->YawAngle;
  if(step != 0)
  {
    this->RotateY(step);
    this->YawAngle = angle;
  }
}

//----------------------------------------------------------------------------
void vtkToolLaparoscopy::Pitch(double angle)
{
  double step = angle - this->PitchAngle;
  if(step != 0)
  {
    this->RotateX(step);
    this->PitchAngle = angle;
  }
}

//----------------------------------------------------------------------------
void vtkToolLaparoscopy::Roll(double angle)
{
  double step = angle - this->RollAngle;
  if(step != 0)
  {
    this->RotateZ(step);
    this->RollAngle = angle;
  }
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::PrintSelf(ostream& os,vtkIndent indent) {

  this->Superclass::PrintSelf(os, indent);

  os << indent << "ToolModel: " << this->ToolModel << "\n";
  os << indent << "Depth: " << this->Depth << "\n";
  os << indent << "Pitch: " << this->PitchAngle << "\n";
  os << indent << "Yaw: " << this->YawAngle << "\n";
  os << indent << "Roll: " << this->RollAngle << "\n";
  os << indent << "ForceFeedback: " << this->ForceFeedback[0] <<  ", " << this->ForceFeedback[1] <<  ", " << this->ForceFeedback[2] << "\n";

}
