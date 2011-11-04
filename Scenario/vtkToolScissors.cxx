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

#include "vtkScenarioElement.h"

vtkCxxRevisionMacro(vtkToolScissors, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolScissors);

//----------------------------------------------------------------------------
vtkToolScissors::vtkToolScissors()
{
  this->ToolModel = Scissors;
  this->Opening = 0;
}

//----------------------------------------------------------------------------
vtkToolScissors::~vtkToolScissors()
{
}

//----------------------------------------------------------------------------
void vtkToolScissors::Init()
{
  Superclass::Init();
}

//----------------------------------------------------------------------------
void vtkToolScissors::Open(){
  if(this->IsClosed()) {
    this->SetOpening(1);
  }
}

//----------------------------------------------------------------------------
void vtkToolScissors::Close(){
  if(!this->IsClosed()) {
    this->SetOpening(0);
  }
}

//----------------------------------------------------------------------------
void vtkToolScissors::SetOpening(double opening) {
  double step = opening - this->Opening;
  int angle = 30;
  //Left Lever
  this->GetElement(1)->RotateX(-angle*step);
  //Right Lever
  this->GetElement(2)->RotateX(angle*step);
  this->Opening = opening;
}

//----------------------------------------------------------------------------
void vtkToolScissors::SetDepth(double position)
{
  //Scissors is clampled before being translated
  double opening = this->Opening;
  this->SetOpening(0);
  Superclass::SetDepth(position);
  //Scissors' opening state is restored
  this->SetOpening(opening);
}

//----------------------------------------------------------------------------
void vtkToolScissors::RotateX(double angle)
{
  //Scissors is clampled before being rotated
  double opening = this->Opening;
  this->SetOpening(0);
  Superclass::RotateX(angle);
  //Scissors' opening state is restored
  this->SetOpening(opening);

}

//----------------------------------------------------------------------------
void vtkToolScissors::RotateY(double angle)
{
  //Scissors is clampled before being rotated
  double opening = this->Opening;
  this->SetOpening(0);
  Superclass::RotateY(angle);
  //Scissors' opening state is restored
  this->SetOpening(opening);
}

//----------------------------------------------------------------------------
void vtkToolScissors::RotateZ(double angle)
{
  //Scissors is clampled before being rotated
  double opening = this->Opening;
  this->SetOpening(0);
  Superclass::RotateZ(angle);
  //Scissors' opening state is restored
  this->SetOpening(opening);
}

//----------------------------------------------------------------------------
void vtkToolScissors::Yaw(double angle)
{
  double step = angle - this->YawAngle;
  if(step != 0)
  {
    this->RotateY(step);
    this->YawAngle = angle;
  }
}

//----------------------------------------------------------------------------
void vtkToolScissors::Pitch(double angle)
{
  double step = angle - this->PitchAngle;
  if(step != 0)
  {
    this->RotateX(step);
    this->PitchAngle = angle;
  }
}

//----------------------------------------------------------------------------
void vtkToolScissors::Roll(double angle)
{
  double step = angle - this->RollAngle;
  if(step != 0)
  {
    this->RotateZ(step);
    this->RollAngle = angle;
  }
}

//----------------------------------------------------------------------------
void vtkToolScissors::PrintSelf(ostream& os,vtkIndent indent) {
  this->Superclass::PrintSelf(os,indent);
}
