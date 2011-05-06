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

#include "vtkSimulationInteractorStyle.h"

#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkIdList.h"

#include "vtkScenario.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioObjectCollection.h"
#include "vtkTool.h"
#include "vtkToolGrasper.h"

#include "vtkOrgan.h"

vtkCxxRevisionMacro(vtkSimulationInteractorStyle, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulationInteractorStyle);

//----------------------------------------------------------------------------

vtkSimulationInteractorStyle::vtkSimulationInteractorStyle()
{
	this->PreviousPosition[0] = this->PreviousPosition[1] = 0;
	this->LeftButtonPressed = this->RightButtonPressed = 0;
	this->Scenario = NULL;
	this->Camera = NULL;
	this->Mode = 0;
	this->ToolIds = vtkIdList::New();
	this->ActiveToolId = 0;
	this->Scale = 0.1;
}

//--------------------------------------------------------------------------
vtkSimulationInteractorStyle::~vtkSimulationInteractorStyle()
{
	this->ToolIds->Delete();
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::SetScenario(vtkScenario * s)
{
	this->Scenario = s;
}

//--------------------------------------------------------------------------
vtkScenario * vtkSimulationInteractorStyle::GetScenario()
{
	return this->Scenario;
}

//--------------------------------------------------------------------------
int vtkSimulationInteractorStyle::GetToolId(int id)
{
	return this->ToolIds->GetId(id);
}

//--------------------------------------------------------------------------
int vtkSimulationInteractorStyle::GetNumberOfTools()
{
	return this->ToolIds->GetNumberOfIds();
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::Init()
{
	if(this->Scenario)
	{
		this->Camera = this->Scenario->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
		vtkScenarioObjectCollection * objects = this->Scenario->GetObjects();
		objects->InitTraversal();
		while(vtkScenarioObject * o = objects->GetNextObject())
		{
			if(o->GetObjectType() == vtkScenarioObject::Tool)
			{
				this->ToolIds->InsertNextId(o->GetId());
			}
		}
		if(this->GetNumberOfTools() > 0) this->ActiveToolId = this->GetToolId(0);
	}
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnKeyPress()
{
	//get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();

	if(this->GetNumberOfTools() > 0)
	{
		//Select control mode: camera/tool
		if(key.compare("c") == 0)
		{
			this->Mode = !this->Mode;
		}

		if(!this->Mode)
		{
			//Camera Mode
			vtkDebugMacro("Camera Mode On");
			if(key.compare("Up") == 0)
			{
				this->Camera->Elevation(1);
			}
			if(key.compare("Down") == 0)
			{
				vtkDebugMacro("RotateX (Down)");
				this->Camera->Elevation(-1);
			}
			if(key.compare("Left") == 0)
			{
				vtkDebugMacro("RotateY (Left)");
				this->Camera->Azimuth(1);
			}
			if(key.compare("Right") == 0)
			{
				vtkDebugMacro("RotateY (Right)");
				this->Camera->Azimuth(-1);
			}
		}
		else
		{
			//Tool control mode
			vtkDebugMacro("Tool Mode On");
			//Tool selection
			if(key.compare("0") == 0)
			{
				vtkDebugMacro("Select Tool (0)");
				this->ActiveToolId = this->GetToolId(0);
			}
			if(key.compare("1") == 0)
			{
				vtkDebugMacro("Select Tool (1)");
				if (this->GetNumberOfTools() > 1)
				{
					this->ActiveToolId = this->GetToolId(1);
				}
			}
			if(key.compare("2") == 0)
			{
				vtkDebugMacro("Select Tool (2)");
				if (this->GetNumberOfTools() > 2)
				{
					this->ActiveToolId = this->GetToolId(2);
				}
			}

			vtkTool * t = vtkTool::SafeDownCast(this->Scenario->GetObject(this->ActiveToolId));

			if(t && t->GetToolType() == vtkTool::Laparoscopy)
			{
				vtkToolLaparoscopy * tool = vtkToolLaparoscopy::SafeDownCast(t);

				//Laparoscopy tool movement
				if(key.compare("Up") == 0)
				{
					vtkDebugMacro("RotateX (Up)");
					tool->RotateX(1);
				}
				if(key.compare("Down") == 0)
				{
					vtkDebugMacro("RotateX (Down)");
					tool->RotateX(-1);
				}
				if(key.compare("Left") == 0)
				{
					vtkDebugMacro("RotateY (Left)");
					tool->RotateY(1);
				}
				if(key.compare("Right") == 0)
				{
					vtkDebugMacro("RotateY (Right)");
					tool->RotateY(-1);
				}
				if(key.compare("x") == 0)
				{
					vtkDebugMacro("Roll Tool Left");
					tool->RotateZ(-10);
				}
				if(key.compare("z") == 0)
				{
					vtkDebugMacro("Roll Tool Right");
					tool->RotateZ(10);
				}
				if(key.compare("Prior") == 0)
				{
					vtkDebugMacro("Push Forward");
					tool->Push();
				}
				if(key.compare("Next") == 0)
				{
					vtkDebugMacro("Pull Back");
					tool->Pull();
				}
				if(key.compare("n") == 0)
				{
					//vtkDebugMacro("Reset");
					//tool->Reset();
					vtkOrgan * o = vtkOrgan::SafeDownCast(this->Scenario->GetObject(1));
					o->RotateX(15);
				}
				if(key.compare("m") == 0)
				{
					//vtkDebugMacro("Restore");
					//tool->Restore();
					vtkOrgan * o = vtkOrgan::SafeDownCast(this->Scenario->GetObject(1));
					o->RotateX(-15);
				}
				//Specific grasper movements
				if(tool->GetToolModel() == vtkToolLaparoscopy::Grasper){
					vtkToolGrasper * grasper = vtkToolGrasper::SafeDownCast(t);
					if(key.compare("a") == 0)
					{
						vtkDebugMacro("Open Tool");
						grasper->Open();
					}
					if(key.compare("s") == 0)
					{
						vtkDebugMacro("Close Tool");
						grasper->Close();
					}
				}
			}
		}
		if(key.compare("q") == 0)
		{
			vtkDebugMacro("Exit Application");
			exit(0);
		}
	}
	// forward events
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

void vtkSimulationInteractorStyle::OnMouseMove()
{
	int pick[2];
	this->GetInteractor()->GetEventPosition(pick);

	if(Mode)
	{
		int x = -this->Scale*(pick[0] - this->PreviousPosition[0]);
		int y = this->Scale*(pick[1] - this->PreviousPosition[1]);

		if(this->GetNumberOfTools())
		{
			vtkTool * t = vtkTool::SafeDownCast(this->Scenario->GetObject(this->ActiveToolId));
			if(t->GetToolType() == vtkTool::Laparoscopy)
			{
				vtkToolLaparoscopy * tool = vtkToolLaparoscopy::SafeDownCast(t);
				if(this->LeftButtonPressed)
				{
					tool->RotateY(x);
					tool->RotateX(y);
				}
				else if (this->RightButtonPressed)
				{
					tool->SetDepth((tool->GetDepth()+0.5*y));
				}
			}
		}
	}

	this->PreviousPosition[0] = pick[0];
	this->PreviousPosition[1] = pick[1];
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnLeftButtonDown()
{
	this->LeftButtonPressed = 1;
	this->GetInteractor()->GetEventPosition(this->PreviousPosition);
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnLeftButtonUp()
{
	this->LeftButtonPressed = 0;
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnMiddleButtonDown()
{
	std::cout << "MiddleButtonDown\n";
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnMiddleButtonUp()
{
	std::cout << "MiddleButtonUp\n";
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnRightButtonDown()
{
	this->RightButtonPressed = 1;
	this->GetInteractor()->GetEventPosition(this->PreviousPosition);
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnRightButtonUp()
{
	this->RightButtonPressed = 0;
}
