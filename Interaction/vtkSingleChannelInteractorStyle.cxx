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

#include "vtkSingleChannelInteractorStyle.h"

#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkIdList.h"
#include "vtkMath.h"
#include "vtkStdString.h"

#include "vtkScenario.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"
#include "vtkTool.h"
#include "vtkToolSingleChannel.h"

#ifndef MultipleToolActionMacro
#define MultipleToolActionMacro(action) \
  { \
    vtkToolSingleChannel *tool; \
    for (int i=0; i < this->GetNumberOfTools(); i++) \
    { \
      int tid = this->ToolIds->GetId(i); \
      tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(tid)); \
      tool->action; \
    } \
  }
#endif

#ifndef GetAndCopyMacro  
#define GetAndCopyMacro(src, dst) \
  dst[0] = dst[1] = dst[2] = 0.0; \
  vtkMath::Add(dst,src,dst);
#endif

vtkCxxRevisionMacro(vtkSingleChannelInteractorStyle, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSingleChannelInteractorStyle);

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::SetLensAngle(double angle)
{
  if (angle >= 0.0 && angle <= 90)
  {
    if (this->Initialized)
    {
      this->Camera->Roll(-this->Roll);
      this->Camera->Pitch(this->LensAngle);
      // Set new angle
      this->LensAngle = angle;
      this->Camera->Pitch(-this->LensAngle);
      this->Camera->Roll(this->Roll);
    }
    else
    {
      this->LensAngle = angle;
    }
  }
}

//----------------------------------------------------------------------------
vtkSingleChannelInteractorStyle::vtkSingleChannelInteractorStyle()
{
  this->PreviousPosition[0] = this->PreviousPosition[1] = 0;
  this->LeftButtonPressed = this->RightButtonPressed = false;
  this->Scenario = NULL;
  this->Camera = NULL;
  this->ToolIds = NULL;
  this->ActiveToolId = -1;
  this->Scale = 0.1;
  this->Initialized = false;
  this->CamToolId = -1;
  this->CamDepth = 0.0;
  this->Roll = 0.0;
  this->Pitch = 0.0;
  this->Yaw = 0.0;
  this->CamOrigin = NULL;
  this->LensAngle = 0;
}

//--------------------------------------------------------------------------
vtkSingleChannelInteractorStyle::~vtkSingleChannelInteractorStyle()
{
  if(this->Initialized){
    this->ToolIds->Delete();
    delete[] this->CamOrigin;
  }
}

//--------------------------------------------------------------------------
int vtkSingleChannelInteractorStyle::GetToolId(int id)
{
  return this->ToolIds->GetId(id);
}

//--------------------------------------------------------------------------
int vtkSingleChannelInteractorStyle::GetNumberOfTools()
{
  return this->ToolIds->GetNumberOfIds();
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::Initialize()
{
  if(!this->Initialized)
  {
    this->Initialized = true;

    this->ToolIds = vtkIdList::New();
    this->ToolIds->Initialize();
    this->CamOrigin = new double[3];
    for (int i=0; i<3; i++) this->CamOrigin[i] = 0.0;

    if(this->Scenario)
    {
      this->Scenario->Update();
      this->Camera = this->Scenario->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
      double *origin = this->Camera->GetPosition();
      vtkMath::Add(CamOrigin, origin, CamOrigin); // Set CamOrigin to Camera->GetOrigin()
      
      vtkScenarioObjectCollection * objects = this->Scenario->GetObjects();
      objects->InitTraversal();
      while(vtkScenarioObject * o = objects->GetNextObject())
      {
        if(o->GetType() == vtkScenarioObject::Tool)
        {
          vtkTool *t = vtkTool::SafeDownCast(o);
          if (t->GetToolType() == vtkTool::SingleChannel){
            vtkToolSingleChannel *tool = vtkToolSingleChannel::SafeDownCast(t);
            // Is it the camera collision cover?
            if (tool->GetToolModel() == vtkToolSingleChannel::Camera)
            {
              this->CamToolId = o->GetId();
            }
            // Otherwise it's added to the rest of tools
            else
            {
              this->ToolIds->InsertNextId(o->GetId());
              t->Disable();
            }
          }
        }
      }
      // Camera cover inserted at last position, so it doesn't interfer with other tools
      if (this->CamToolId >= 0) this->ToolIds->InsertNextId(this->CamToolId);

      // Set lens inclination
      this->Camera->Pitch(-this->LensAngle);
    }
  }
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnKeyPress()
{
  if (!this->Initialized) this->Initialize();
  
  //get the pressed key
  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkStdString key = rwi->GetKeySym();
  bool shift = rwi->GetShiftKey() != 0;

  // Exit
  if(key == "q")
  {
    exit(0);
  }
  // control key: tool/camera
  else if(this->Scenario)
  {
    // Tool selection (up to 10 tools: 0..9)
    if (this->GetNumberOfTools() > 0 && key.length() == 1 && key.c_str()[0] >= '0' && key.c_str()[0] <= '9')
    {
      int ToolId = key.c_str()[0] - '0';
      if (ToolId < this->GetNumberOfTools() && ToolId != this->CamToolId)
      {
        if (this->ActiveToolId >= 0){
          int id = this->ToolIds->GetId(this->ActiveToolId);
          vtkToolSingleChannel *t = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(id));
          if (t->GetDepth() > this->CamDepth + 0.5) return;
          else t->Disable();
        }
        
        if (this->ActiveToolId == ToolId) this->ActiveToolId = -1;
        else{
          this->ActiveToolId = ToolId;
          int id = this->ToolIds->GetId(ToolId);
          vtkToolSingleChannel *t = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(id));
          t->Enable();
          t->SetDepth(this->CamDepth);
        }
      }
    }
    // Lens movement
    else if (key == "x")
    {
      this->RotateLens(-1);
      this->RotateZAllTools(-1);
    }
    else if (key == "z")
    {
      this->RotateLens(1);
      this->RotateZAllTools(1);
    }
    // Tool/camera movement
    else
    { 
      // VTK Axis
      //   Y
      //   |
      //   |____X
      //  /
      // Z
      //Laparoscopy tool movement
      
      // Reset lens
      this->Camera->Roll(-this->Roll);
      this->Camera->Pitch(this->LensAngle);
      this->Camera->Roll(this->Roll);
      // Set origin to initial one
      double distance = this->Camera->GetDistance();
      this->Camera->SetPosition(this->CamOrigin);
      // Do alterations
      if(key == "Up")
      {
        this->Camera->Pitch(1);
        this->UpdateAllTools(0,1);
      }
      else if(key == "Down")
      {
        this->Camera->Pitch(-1);
        this->UpdateAllTools(0,-1);
      }
      else if(key == "Left")
      {
        this->Camera->Yaw(1);
        this->UpdateAllTools(1,0);
      }
      else if(key == "Right")
      {
        this->Camera->Yaw(-1);
        this->UpdateAllTools(-1,0);
      }
      else if(key == "Prior")
      {
        if (!shift){
          this->CamDepth += 0.1;
          this->Camera->SetDistance(this->CamDepth + distance);
        }
        MultipleToolActionMacro(Push());
      }
      else if(key == "Next" && this->CamDepth >= 0.1)
      {
        if (!shift){
          this->CamDepth -= 0.1;
          this->Camera->SetDistance(this->CamDepth + distance);
        }
        MultipleToolActionMacro(Pull());
      }
      // Set new origin
      double newpos[3] = {0.0, 0.0, 0.0};
      vtkMath::Add(newpos, this->Camera->GetDirectionOfProjection(), newpos);
      vtkMath::MultiplyScalar(newpos, this->CamDepth);
      vtkMath::Add(this->CamOrigin, newpos, newpos);
      this->Camera->SetPosition(newpos);

      // Restore lens
      this->Camera->Roll(-this->Roll);
      this->Camera->Pitch(-this->LensAngle);
      this->Camera->Roll(this->Roll);
    }
  }
}

void vtkSingleChannelInteractorStyle::OnMouseMove()
{
  if (!this->Initialized) this->Initialize();
  
  vtkRenderWindowInteractor *rwi = this->Interactor;
  int pick[2];
  rwi->GetEventPosition(pick);
  
  if (this->LeftButtonPressed)
  {
    // Get some info
    bool shift = rwi->GetShiftKey() != 0;
    double x = -this->Scale*(pick[0] - this->PreviousPosition[0]);
    double y = this->Scale*(pick[1] - this->PreviousPosition[1]);
    
    // Roll / Move
    if (shift)
    { // Roll lens
      this->RotateLens(x);
      this->RotateZAllTools(x);
    }
    else
    {
      // Reset lens
      this->Camera->Roll(-this->Roll);
      this->Camera->Pitch(this->LensAngle);
      this->Camera->Roll(this->Roll);

      // Move camera
      double distance = this->Camera->GetDistance();
      this->Camera->SetPosition(this->CamOrigin);
      
      this->Camera->Yaw(x);
      this->Camera->Pitch(y);
      
      double newpos[3] = {0.0, 0.0, 0.0};
      vtkMath::Add(newpos, this->Camera->GetDirectionOfProjection(), newpos);
      vtkMath::MultiplyScalar(newpos, this->CamDepth);
      vtkMath::Add(this->CamOrigin, newpos, newpos);
      this->Camera->SetPosition(newpos);

      // Restore lens
      this->Camera->Roll(-this->Roll);
      this->Camera->Pitch(-this->LensAngle);
      this->Camera->Roll(this->Roll);

      // Move tools
      UpdateAllTools(x,y);
    }
  }

  this->PreviousPosition[0] = pick[0];
  this->PreviousPosition[1] = pick[1];
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnLeftButtonDown()
{
  this->LeftButtonPressed = true;
  this->Interactor->GetEventPosition(this->PreviousPosition);
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnLeftButtonUp()
{
  this->LeftButtonPressed = false;
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnMiddleButtonDown()
{
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnMiddleButtonUp()
{
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnRightButtonDown()
{
  this->RightButtonPressed = true;
  this->Interactor->GetEventPosition(this->PreviousPosition);
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnRightButtonUp()
{
  this->RightButtonPressed = false;
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnMouseWheelForward()
{
  if (!this->Initialized) this->Initialize();
  
  bool shift = this->Interactor->GetShiftKey() != 0;
  // Tool moves with camera
  TranslateAllTools(this->Scale, shift);
  // Camera stays steady if moving only tool
  if (!shift)
  {
    // Reset lens
    this->Camera->Roll(-this->Roll);
    this->Camera->Pitch(this->LensAngle);
    this->Camera->Roll(this->Roll);
    // Move camera
    double distance = this->Camera->GetDistance();
    this->CamDepth += this->Scale;
    double newpos[3] = {0.0, 0.0, 0.0};
    vtkMath::Add(newpos, this->Camera->GetDirectionOfProjection(), newpos);
    vtkMath::MultiplyScalar(newpos, this->CamDepth);
    vtkMath::Add(this->CamOrigin, newpos, newpos);
    
    this->Camera->SetPosition(newpos);
    this->Camera->SetDistance(distance);
    // Restore lens
    this->Camera->Roll(-this->Roll);
    this->Camera->Pitch(-this->LensAngle);
    this->Camera->Roll(this->Roll);
  }
}

//--------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::OnMouseWheelBackward()
{
  if (!this->Initialized) this->Initialize();
  
  bool shift = this->Interactor->GetShiftKey() != 0;
  // Tool moves together with camera
  TranslateAllTools(- this->Scale, shift);
  // Camera stays steady if moving only tool
  if (!shift && this->CamDepth >= 0.1)
  {
    // Reset lens
    this->Camera->Roll(-this->Roll);
    this->Camera->Pitch(this->LensAngle);
    this->Camera->Roll(this->Roll);
    // Move camera
    double distance = this->Camera->GetDistance();
    this->CamDepth -= this->Scale;
    double newpos[3] = {0.0, 0.0, 0.0};
    vtkMath::Add(newpos, this->Camera->GetDirectionOfProjection(), newpos);
    vtkMath::MultiplyScalar(newpos, this->CamDepth);
    vtkMath::Add(this->CamOrigin, newpos, newpos);

    this->Camera->SetPosition(newpos);
    this->Camera->SetDistance(distance);
    // Restore lens
    this->Camera->Roll(-this->Roll);
    this->Camera->Pitch(-this->LensAngle);
    this->Camera->Roll(this->Roll);
  }
}

//----------------------------------------------------------------------------
bool vtkSingleChannelInteractorStyle::ChangeTool(int ToolId)
{
  if (this->GetNumberOfTools() > 0 && ToolId >= 0 && ToolId < this->GetNumberOfTools())
  {
    if (this->ActiveToolId >= 0)
    {
      int id = this->ToolIds->GetId(this->ActiveToolId);
      vtkToolSingleChannel *tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(id));
      if (tool->GetDepth() > this->CamDepth + 0.5) return false;
      else tool->Disable();
    }
    
    if (this->ActiveToolId == ToolId) this->ActiveToolId = -1;
    else
    {
      this->ActiveToolId = ToolId;
      int id = this->ToolIds->GetId(ToolId);
      vtkToolSingleChannel *tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(id));
      tool->Enable();
      tool->SetDepth(this->CamDepth);
    }

    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::TranslateAllTools(double step, bool shift)
{
  if (shift || step > 0 || this->CamDepth >= 0.1){
    vtkToolSingleChannel *tool;
    int limit = this->GetNumberOfTools();
    if (shift && this->CamToolId >= 0) limit = limit - 1;
    for (int i=0; i < limit; i++)
    {
      int tid = this->ToolIds->GetId(i);
      tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(tid));
      tool->SetDepth(tool->GetDepth() + (step));
    }
  }
}

//----------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::RotateZAllTools(double angle)
{
  vtkToolSingleChannel *tool;
  
  for (int i=0; i < this->GetNumberOfTools(); i++)
  {
    int tid = this->ToolIds->GetId(i);
    tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(tid));
    tool->RotateZ(angle);
  }
}

//----------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::UpdateAllTools(double YawStep, double PitchStep)
{
  vtkToolSingleChannel *tool;
  
  // Update tools
  for (int i=0; i < this->GetNumberOfTools(); i++)
  {
    // Select tool
    int tid = this->ToolIds->GetId(i);
    tool = vtkToolSingleChannel::SafeDownCast(this->Scenario->GetObject(tid));
    tool->RotateXYZ(PitchStep, YawStep, 0);
  }
  
  // Update Pitch and Yaw values
  this->Yaw += YawStep;
  this->Pitch += PitchStep;
}

//----------------------------------------------------------------------------
void vtkSingleChannelInteractorStyle::RotateLens(double angle)
{
  // Restore camera orientation
  this->Camera->Roll(-this->Roll);
  this->Camera->Pitch(this->LensAngle);
  this->Camera->Roll(this->Roll);
  // Set new angle
  this->Roll = this->Roll + angle;
  // Set new camera orientation
  this->Camera->Roll(-this->Roll);
  this->Camera->Pitch(-this->LensAngle);
  this->Camera->Roll(this->Roll);
}
