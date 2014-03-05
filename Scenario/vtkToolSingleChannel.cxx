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
#include "vtkToolSingleChannel.h"

#include "vtkObjectFactory.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"

#include "vtkMath.h"

#ifndef GetAndCopyMacro
#define GetAndCopyMacro(src, dst) \
  dst[0] = dst[1] = dst[2] = 0.0; \
  vtkMath::Add(dst, src, dst);
#endif

vtkCxxRevisionMacro(vtkToolSingleChannel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolSingleChannel);

//--------------------------------------------------------------------------
vtkToolSingleChannel::vtkToolSingleChannel() {
  this->ToolType =  SingleChannel;
  this->Depth = 0;
  this->YawAngle = 0;
  this->PitchAngle = 0;
  this->RollAngle = 0;
  this->ForceFeedback[0] = this->ForceFeedback[1] = this->ForceFeedback[2] = 0;
  this->Attached = false;
  this->DiffRoll = 0;
  this->ToolModel = vtkToolSingleChannel::vtkToolSingleChannelModel(0);
}

//--------------------------------------------------------------------------
vtkToolSingleChannel::~vtkToolSingleChannel() {

}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::SetToolModel(unsigned int Value)
{
  if (Value > vtkToolSingleChannel::Camera)
  {
    vtkErrorMacro("Value out of range of vtkToolSingleChannelModel")
    return;
  }
  this->ToolModel = vtkToolSingleChannel::vtkToolSingleChannelModel(Value);
}

//--------------------------------------------------------------------------
unsigned int vtkToolSingleChannel::GetToolModel()
{
  return this->ToolModel;
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::Initialize()
{
  Superclass::Initialize();

  double * orientation = this->GetElement(0)->GetOrientation();
  this->YawAngle = orientation[0];
  this->PitchAngle = orientation[1];
  this->RollAngle = orientation[2];
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::SetDepth(double depth)
{
  if(depth != this->Depth)
  {
    double step = this->Depth - depth;
    Superclass::Translate(0, 0, step);
    this->Depth = depth;
  }
  this->Update();
}

//--------------------------------------------------------------------------
double vtkToolSingleChannel::GetDepth()
{
  return this->Depth;
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::Pull() {
  //Note the step negative sign. VTK Z Axis is inverted
  double step = 0.1;
  double depth = this->Depth - step;
  this->SetDepth(depth);
}

void vtkToolSingleChannel::Push() {
  //Note the step negative sign. VTK Z Axis is inverted
  double step = -0.1;
  double depth = this->Depth - step;
  this->SetDepth(depth);
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::RotateX(double angle)
{
  this->UpdateTool(this->PitchAngle + angle, this->YawAngle, this->RollAngle);
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::RotateY(double angle)
{
  this->UpdateTool(this->PitchAngle, this->YawAngle + angle, this->RollAngle);
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::RotateZ(double angle)
{
  double Origin[3];
  GetAndCopyMacro(this->GetElement(0)->GetOrigin(), Origin);
  this->Translate(Origin);
  this->Superclass::RotateZ(angle);
  vtkMath::MultiplyScalar(Origin, -1);
  this->Translate(Origin);
  if (!this->Attached) this->DiffRoll += angle;

  // Update orientation values and object
  Superclass::Update();
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::RotateXYZ(double angX, double angY, double angZ)
{
  this->UpdateTool(this->PitchAngle + angX, this->YawAngle + angY, this->RollAngle + angZ);
}

//----------------------------------------------------------------------------
void vtkToolSingleChannel::Yaw(double angle)
{
  if (angle - this->YawAngle != 0)
  {
    this->UpdateTool(this->PitchAngle, angle, this->RollAngle);
  }
}

//----------------------------------------------------------------------------
void vtkToolSingleChannel::Pitch(double angle)
{
  if (angle - this->PitchAngle != 0)
  {
    this->UpdateTool(angle, this->YawAngle, this->RollAngle);
  }
}

//----------------------------------------------------------------------------
void vtkToolSingleChannel::Roll(double angle)
{
  if (angle - this->RollAngle != 0)
  {
    this->UpdateTool(this->PitchAngle, this->YawAngle, angle);
  }
}

//----------------------------------------------------------------------------
void vtkToolSingleChannel::RollPitchYaw(double NewRoll, double NewPitch, double NewYaw)
{
  if (NewRoll - this->RollAngle != 0 || NewPitch - this->PitchAngle != 0 || NewYaw - this->YawAngle != 0)
  {
    this->UpdateTool(NewPitch, NewYaw, NewRoll);
  }
}

void vtkToolSingleChannel::UpdateTool(double NewPitch, double NewYaw, double NewRoll)
{
  vtkScenarioElementCollection *elements;
  vtkScenarioElement *elem;
  double Origin[3];

  elements = this->GetElements();
  elements->InitTraversal();
  while (elem = elements->GetNextElement())
  {
    GetAndCopyMacro(elem->GetOrigin(), Origin);
    elem->Translate(Origin);
    // Reset
    elem->RotateZ(-this->RollAngle - this->DiffRoll);
    elem->RotateX(-this->PitchAngle);
    elem->RotateY(-this->YawAngle);
    // New Angles
    elem->RotateY(NewYaw);
    elem->RotateX(NewPitch);
    elem->RotateZ(NewRoll + this->DiffRoll);
    // Back to position
    vtkMath::MultiplyScalar(Origin, -1);
    elem->Translate(Origin);
    vtkMath::MultiplyScalar(Origin, -1);
  }

  // Update orientation values and object
  Superclass::Update();
  this->RollAngle = NewRoll;
  this->PitchAngle = NewPitch;
  this->YawAngle = NewYaw;
}

//--------------------------------------------------------------------------
double *  vtkToolSingleChannel::GetContactForceValue() {
  return this->ForceFeedback;
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::SetForceFeedback(float Force[]){
  this->ForceFeedback[0] = Force[0];
  this->ForceFeedback[1] = Force[1];
  this->ForceFeedback[2] = Force[2];
}

//--------------------------------------------------------------------------
void vtkToolSingleChannel::PrintSelf(ostream& os,vtkIndent indent) {

  this->Superclass::PrintSelf(os, indent);

  os << indent << "ToolModel: " << this->ToolModel << "\n";
  os << indent << "Depth: " << this->Depth << "\n";
  os << indent << "Pitch: " << this->PitchAngle << "\n";
  os << indent << "Yaw: " << this->YawAngle << "\n";
  os << indent << "Roll: " << this->RollAngle << "\n";
  os << indent << "ForceFeedback: " << this->ForceFeedback[0] <<  ", " << this->ForceFeedback[1] <<  ", " << this->ForceFeedback[2] << "\n";

}
