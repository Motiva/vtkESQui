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
#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"

#include "vtkScenario.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioObject.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkTool.h"
#include "vtkToolGrasper.h"
#include "vtkToolProbe.h"
#include "vtkOrgan.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkSimulation.h"
#include "vtkSimulationInteractorStyle.h"
#include "vtkPSSInterface.h"

class vtkTimerCallback2 : public vtkCommand
{
public:
	static vtkTimerCallback2 *New()
	{
		vtkTimerCallback2 *cb = new vtkTimerCallback2;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId,
			void * vtkNotUsed(callData))
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;
		}
		//std::cout << this->TimerCount << std::endl;
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		Scenario->Update();
		iren->GetRenderWindow()->Render();
	}

private:
	int TimerCount;
public:
	vtkScenario* Scenario;
};

//!This test perform a standard execution of the ESQUI simulation of a laparoscopic scenario.

int main(int argc, char * argv[])
{

	const char * fn0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick.vtp";
	const char * fn0c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick_col.vtp";
	const char * fn1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l.vtp";
	const char * fn1c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l_col.vtp";
	const char * fn2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r.vtp";
	const char * fn2c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r_col.vtp";
	const char * fn0t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";
//	const char * fn3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
//	const char * fn3t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg";
//	const char * fn4 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp";
//	const char * fn4t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomach.jpg";
//	const char * fn5 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick.vtp";
//	const char * fn6 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip.vtp";

	double origin[3];
	double position[3];

	origin[0]=0;
	origin[1]=0;
	origin[2]=6;
	position[0]=-2;
	position[1]=0;
	position[2]=-2;

	/**********  Render Window Definitions  ********/
	vtkSmartPointer<vtkRenderer> ren1 =
			vtkSmartPointer<vtkRenderer>::New();
	ren1->SetBackground(1.0,1.0,1.0);
	
	vtkSmartPointer<vtkRenderWindow> renWin =
			vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	/**********  Scenario Definitions  ********/
	vtkSmartPointer<vtkScenario> scenario = vtkSmartPointer<vtkScenario>::New();
	scenario->SetRenderWindow(renWin);

	/**********  Load Organs ********/

	/********** Load Tools **********/
	//Generate tool's first element (stick)
	vtkSmartPointer<vtkVisualizationModel> vis_stick = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_stick->SetName("vis_stick");
	vis_stick->SetFileName(fn0);
	vis_stick->SetTextureFileName(fn0t);
	vis_stick->SetOrigin(origin);
	vis_stick->SetVisibility(1);
	vis_stick->SetOpacity(1);
	vis_stick->SetColor(1.0, 1.0, 1.0);
	vis_stick->Init();

	vtkSmartPointer<vtkCollisionModel> col_stick = vtkSmartPointer<vtkCollisionModel>::New();
	col_stick->SetName("col_stick");
	col_stick->SetFileName(fn0c);
	col_stick->SetOrigin(origin);
	col_stick->SetVisibility(1);
	col_stick->SetOpacity(0.0);
	col_stick->SetColor(0.0, 0.0, 1.0);
	col_stick->Init();

	vtkSmartPointer<vtkScenarioElement> stick = vtkSmartPointer<vtkScenarioElement>::New();
	stick->SetId(0);
	stick->SetName("stick");
	stick->SetVisualizationModel(vis_stick);
	stick->SetCollisionModel(col_stick);
	stick->Init();

	//Second element (left lever)
	vtkSmartPointer<vtkVisualizationModel> vis_lever_l = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_lever_l->SetName("vis_lever_l");
	vis_lever_l->SetFileName(fn1);
	vis_lever_l->SetTextureFileName(fn0t);
	vis_lever_l->SetOrigin(origin);
	vis_lever_l->SetVisibility(1);
	vis_lever_l->SetOpacity(1.0);
	vis_lever_l->SetColor(1.0, 1.0, 0.0);
	vis_lever_l->Init();

	vtkSmartPointer<vtkCollisionModel> col_lever_l = vtkSmartPointer<vtkCollisionModel>::New();
	col_lever_l->SetName("col_lever_l");
	col_lever_l->SetFileName(fn1c);
	col_lever_l->SetOrigin(origin);
	col_lever_l->SetVisibility(1);
	col_lever_l->SetOpacity(0.0);
	col_lever_l->SetColor(0.0, 0.0, 1.0);
	col_lever_l->Init();

	vtkSmartPointer<vtkScenarioElement> left = vtkSmartPointer<vtkScenarioElement>::New();
	left->SetId(1);
	left->SetName("lever_left");
	left->SetVisualizationModel(vis_lever_l);
	left->SetCollisionModel(col_lever_l);
	left->Init();

	//Third element (right lever)
	vtkSmartPointer<vtkVisualizationModel> vis_lever_r = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_lever_r->SetName("vis_lever_r");
	vis_lever_r->SetFileName(fn2);
	vis_lever_r->SetOrigin(origin);
	vis_lever_r->SetTextureFileName(fn0t);
	vis_lever_r->SetVisibility(1);
	vis_lever_r->SetOpacity(1.0);
	vis_lever_r->SetColor(1.0, 1.0, 0.0);
	vis_lever_r->Init();

	vtkSmartPointer<vtkCollisionModel> col_lever_r = vtkSmartPointer<vtkCollisionModel>::New();
	col_lever_r->SetName("col_lever_r");
	col_lever_r->SetFileName(fn2c);
	col_lever_r->SetOrigin(origin);
	col_lever_r->SetVisibility(1);
	col_lever_r->SetOpacity(0.0);
	col_lever_r->SetColor(0.0, 0.0, 1.0);
	col_lever_r->Init();

	vtkSmartPointer<vtkScenarioElement> right = vtkSmartPointer<vtkScenarioElement>::New();
	right->SetId(2);
	right->SetName("lever_right");
	right->SetVisualizationModel(vis_lever_r);
	right->SetCollisionModel(col_lever_r);
	right->Init();

	vtkSmartPointer<vtkToolGrasper> leftGrasper = vtkSmartPointer<vtkToolGrasper>::New();
	leftGrasper->SetStick(stick);
	leftGrasper->SetLeftLever(left);
	leftGrasper->SetRightLever(right);
	leftGrasper->Init();

	/**********  Init Scene Environment  ********/
	scenario->AddObject(leftGrasper);
	scenario->Init();

	leftGrasper->Translate(position);
	leftGrasper->RotateX(10);

	/********** Lights  **********/
	ren1->GetLights()->InitTraversal();
	vtkSmartPointer<vtkLight> headLight = vtkSmartPointer<vtkLight>::New();
	headLight->SetLightTypeToHeadlight();
	headLight->PositionalOn();
	headLight->SetIntensity(0.5);
	headLight->SetConeAngle(20);
	ren1->AddLight(headLight);
		
	vtkSmartPointer<vtkLight> ambientLight = vtkSmartPointer<vtkLight>::New();
	ambientLight->SetIntensity(0.8);
	ambientLight->SetLightTypeToHeadlight();
	ambientLight->PositionalOff();
	ren1->AddLight(ambientLight);
	ren1->SetAmbient(0.5,0.5,0.5);
		
	/**********  Camera Definitions  ********/
	vtkCamera * camera = ren1->GetActiveCamera();
	camera->SetPosition(0, 0, 2);
	camera->SetFocalPoint(0, 0, -6);
	camera->Yaw(0);
	camera->Elevation(20);
	camera->Pitch(-15);
	camera->Dolly(1);
	camera->ParallelProjectionOff();
	camera->SetViewAngle(70);

	/**********  Simulation Setup  ********/
	vtkSmartPointer<vtkSimulationInteractorStyle> style = vtkSmartPointer<vtkSimulationInteractorStyle>::New();
	//style->DebugOn();
	style->SetScenario(scenario);
	style->Init();
	iren->SetInteractorStyle(style);

/*	vtkSmartPointer<vtkActor> ac0 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkPolyDataMapper> mc0 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mc0->SetInput(col_stick->GetOutput(1));
	ac0->SetMapper(mc0);
	ac0->GetProperty()->SetColor(0,1,0);
	ren1->AddActor(ac0);

	vtkSmartPointer<vtkActor> ac1 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkPolyDataMapper> mc1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mc0->SetInput(col_lever_l->GetOutput(1));
	ac1->SetMapper(mc1);
	ac1->GetProperty()->SetColor(0,1,0);
	ren1->AddActor(ac1);

	vtkSmartPointer<vtkActor> ac2 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkPolyDataMapper> mc2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mc2->SetInput(col_lever_r->GetOutput(1));
	ac2->SetMapper(mc2);
	ac2->GetProperty()->SetColor(0,1,0);
	ren1->AddActor(ac2);*/

	iren->Initialize();

	// Sign up to receive TimerEvent
	vtkSmartPointer<vtkTimerCallback2> cb =
			vtkSmartPointer<vtkTimerCallback2>::New();
	cb->Scenario = scenario;
	iren->AddObserver(vtkCommand::TimerEvent, cb);

	int timerId = iren->CreateRepeatingTimer(10);
	std::cout << "timerId: " << timerId << std::endl;


	iren->Start();

	return 0;
}

