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
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"

#include "vtkScenario.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioObject.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkTool.h"
#include "vtkToolGrasper.h"
#include "vtkToolProbe.h"
#include "vtkOrgan.h"

#include "vtkSimulation.h"
#include "vtkSimulationInteractorStyle.h"
#include "vtkPSSInterface.h"

//!This test perform a standard execution of the ESQUI simulation of a laparoscopic scenario.

int main(int argc, char * argv[])
{

	const char * fn0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/Stick.vtp";
	const char * fn0c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/Stick_col.vtp";
	const char * fn1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/LeftLever.vtp";
	const char * fn1c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/LeftLever_col_lr.vtp";
	const char * fn2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/RightLever.vtp";
	const char * fn2c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/RightLever_col_lr.vtp";
	const char * fn0t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";
	const char * fn3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
	const char * fn3c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp";
	const char * fn3t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg";
	const char * fn4 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp";
	const char * fn4c = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach_col.vtp";
	const char * fn4t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomach.jpg";
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

	/********** Load Tools **********/
	//Generate tool's first element (stick)
	vtkSmartPointer<vtkVisualizationModel> vis_stick = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_stick->SetName("vis_stick");
	vis_stick->SetFileName(fn0);
	vis_stick->SetTextureFileName(fn0t);
	vis_stick->SetOrigin(origin);
	vis_stick->SetVisibility(1.0);
	vis_stick->SetColor(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col_stick = vtkSmartPointer<vtkCollisionModel>::New();
	col_stick->SetName("col_stick");
	col_stick->SetFileName(fn0c);
	col_stick->SetOrigin(origin);
	col_stick->SetVisibility(0.0);
	col_stick->SetColor(0.0, 0.0, 1.0);

	vtkSmartPointer<vtkScenarioElement> stick = vtkSmartPointer<vtkScenarioElement>::New();
	stick->SetName("stick");
	stick->SetVisualizationModel(vis_stick);
	stick->SetCollisionModel(col_stick);

	//Second element (left lever)
	vtkSmartPointer<vtkVisualizationModel> vis_lever_l = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_lever_l->SetName("vis_lever_r");
	vis_lever_l->SetFileName(fn1);
	vis_lever_l->SetTextureFileName(fn0t);
	vis_lever_l->SetOrigin(origin);
	vis_lever_l->SetVisibility(1.0);
	vis_lever_l->SetColor(0.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col_lever_l = vtkSmartPointer<vtkCollisionModel>::New();
	col_lever_l->SetName("col_lever_l");
	col_lever_l->SetFileName(fn1c);
	col_lever_l->SetOrigin(origin);
	col_lever_l->SetVisibility(0.0);
	col_lever_l->SetColor(0.0, 0.0, 1.0);

	vtkSmartPointer<vtkScenarioElement> left_lever = vtkSmartPointer<vtkScenarioElement>::New();
	left_lever->SetName("lever_left");
	left_lever->SetVisualizationModel(vis_lever_l);
	left_lever->SetCollisionModel(col_lever_l);

	//Third element (right lever)
	vtkSmartPointer<vtkVisualizationModel> vis_lever_r = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_lever_r->SetName("vis_lever_r");
	vis_lever_r->SetFileName(fn2);
	vis_lever_r->SetOrigin(origin);
	vis_lever_r->SetTextureFileName(fn0t);
	vis_lever_r->SetVisibility(1.0);
	vis_lever_r->SetColor(0.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col_lever_r = vtkSmartPointer<vtkCollisionModel>::New();
	col_lever_r->SetName("col_lever_r");
	col_lever_r->SetFileName(fn2c);
	col_lever_r->SetOrigin(origin);
	col_lever_r->SetVisibility(0.0);
	col_lever_r->SetColor(0.0, 0.0, 1.0);

	vtkSmartPointer<vtkScenarioElement> right_lever = vtkSmartPointer<vtkScenarioElement>::New();
	right_lever->SetName("lever_right");
	right_lever->SetVisualizationModel(vis_lever_r);
	right_lever->SetCollisionModel(col_lever_r);

	vtkSmartPointer<vtkToolGrasper> leftGrasper = vtkSmartPointer<vtkToolGrasper>::New();
	leftGrasper->SetStick(stick);
	leftGrasper->SetLeftLever(left_lever);
	leftGrasper->SetRightLever(right_lever);

	/**********  Load Organs ********/
	vtkSmartPointer<vtkVisualizationModel> vis_organ = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_organ->SetName("sphere_vis");
	vis_organ->SetFileName(fn3);
	vis_organ->SetTextureFileName(fn3t);
	vis_organ->SetVisibility(1);
	vis_organ->SetColor(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col_organ = vtkSmartPointer<vtkCollisionModel>::New();
	col_organ->SetName("sphere_col");
	col_organ->SetFileName(fn3c);
	col_organ->SetVisibility(1.0);
	col_organ->SetColor(0.0, 0.0, 1.0);

	//Deformation model. Particle-Spring system
	vtkSmartPointer<vtkPSSInterface> def_organ = vtkSmartPointer<vtkPSSInterface>::New();
	def_organ->SetName("sphere_def");
	def_organ->SetFileName(fn3);
	def_organ->SetVisibility(0);
	def_organ->SetColor(0.0, 1.0, 0.0);
	def_organ->SetDeltaT(0.01);
	def_organ->SetGravity(0.0, 0.0, 0.0);

	//Set particle-spring system specific parameters
	def_organ->SetSpringCoefficient(100);
	def_organ->SetDampingCoefficient(5);
	def_organ->SetDistanceCoefficient(20);
	def_organ->SetMass(1);
	def_organ->SetRigidityFactor(2);
	def_organ->SetSolverType(vtkMotionEquationSolver::RungeKutta4);

	vtkSmartPointer<vtkScenarioElement> el_organ = vtkSmartPointer<vtkScenarioElement>::New();
	el_organ->SetName("Sphere");
	el_organ->SetVisualizationModel(vis_organ);
	el_organ->SetCollisionModel(col_organ);
	el_organ->SetDeformationModel(def_organ);

	vtkSmartPointer<vtkOrgan> organ = vtkSmartPointer<vtkOrgan>::New();
	organ->AddElement(el_organ);

	//Stomach
	vtkSmartPointer<vtkVisualizationModel> vis_stomach = vtkSmartPointer<vtkVisualizationModel>::New();
	vis_stomach->SetName("stomach_vis");
	vis_stomach->SetFileName(fn4);
	vis_stomach->SetTextureFileName(fn4t);
	vis_stomach->SetVisibility(1);
	vis_stomach->SetColor(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col_stomach = vtkSmartPointer<vtkCollisionModel>::New();
	col_stomach->SetName("stomach_col");
	col_stomach->SetFileName(fn4c);
	col_stomach->SetVisibility(0);
	col_stomach->SetColor(0.0, 0.0, 1.0);

	//Deformation model. Particle-Spring system
	vtkSmartPointer<vtkPSSInterface> def_stomach = vtkSmartPointer<vtkPSSInterface>::New();
	def_stomach->SetName("stomach_def");
	def_stomach->SetFileName(fn4);
	def_stomach->SetVisibility(0);
	def_stomach->SetColor(0.0, 1.0, 0.0);
	def_stomach->SetDeltaT(0.01);
	def_stomach->SetGravity(0.0, 0.0, 0.0);

	//Set particle-spring system specific parameters
	def_stomach->SetSpringCoefficient(100);
	def_stomach->SetDampingCoefficient(5);
	def_stomach->SetDistanceCoefficient(20);
	def_stomach->SetMass(1);
	def_stomach->SetRigidityFactor(2);
	def_stomach->SetSolverType(vtkMotionEquationSolver::RungeKutta4);

	vtkSmartPointer<vtkScenarioElement> el_stomach = vtkSmartPointer<vtkScenarioElement>::New();
	el_stomach->SetName("Sphere");
	el_stomach->SetVisualizationModel(vis_stomach);
	el_stomach->SetCollisionModel(col_stomach);
	el_stomach->SetDeformationModel(def_stomach);

	vtkSmartPointer<vtkOrgan> stomach = vtkSmartPointer<vtkOrgan>::New();
	stomach->AddElement(el_stomach);

	/**********  Initialize Scenario  ********/
	scenario->AddObject(leftGrasper);
	//scenario->AddObject(organ);
	scenario->AddObject(stomach);
	scenario->Init();

	leftGrasper->Translate(position);
	leftGrasper->RotateX(10);

	//organ->Translate(0, 0, -4);
	stomach->Translate(0, 0, -2);

	//Debug purposes
	/*vtkSmartPointer<vtkActor> ac0 = vtkSmartPointer<vtkActor>::New();
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
	camera->SetPosition(0, 0, 0);
	camera->SetFocalPoint(0, 0, -6);
	camera->Yaw(0);
	camera->Elevation(10);
	camera->Pitch(-15);
	camera->Dolly(1);
	camera->ParallelProjectionOff();
	camera->SetViewAngle(70);

	/**********  Simulation Setup  ********/
	vtkSmartPointer<vtkSimulationInteractorStyle> style = vtkSmartPointer<vtkSimulationInteractorStyle>::New();
	style->SetScenario(scenario);
	style->Init();
	iren->SetInteractorStyle(style);

	vtkSimulation * simulation = vtkSimulation::New();
	simulation->SetScenario(scenario);
	simulation->SetRenderTimerRate(0.02);
	simulation->SetSimulationTimerRate(0.01);
	simulation->SetHapticTimerRate(0.001);
	simulation->Init();

	simulation->Run();

	return 0;
}

