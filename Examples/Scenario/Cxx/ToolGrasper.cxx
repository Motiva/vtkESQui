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
/*
 * TestEsquivtkTooLaparoscopy.cxx
 *
 *  Created on: 12/01/2010
 *      Author: jballesteros
 */

#include <iostream>

#include "vtkToolGrasper.h"
#include "vtkScenarioElement.h"
#include "vtkCollisionModel.h"
#include "vtkVisualizationModel.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"

using namespace std;

//!This test perform a test of the vtkToolDummy class

int main(int argc, char * argv[])
{
	const char * fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick.vtp";
	const char * cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/stick_col.vtp";
	const char * tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg";

	const char * fnl ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l.vtp";
	const char * cfnl ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_l_col.vtp";
	const char * tfnl ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";

	const char * fnr ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r.vtp";
	const char * cfnr ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/lever_r_col.vtp";
	const char * tfnr ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";

	double origin[3];
	double position[3];

	origin[0]=0;
	origin[1]=0;
	origin[2]=6;
	position[0]=2.0;
	position[1]=0;
	position[2]=0;

	//Generate tool's first element (stick)
	vtkSmartPointer<vtkVisualizationModel> vis = vtkSmartPointer<vtkVisualizationModel>::New();
	vis->SetName("stick_vis");
	vis->SetFileName(fn);
	vis->SetTextureFileName(tfn);
	vis->SetOrigin(origin);
	vis->SetOpacity(1.0);
	vis->SetColor(1.0, 1.0, 1.0);
	vis->Init();

	vtkSmartPointer<vtkCollisionModel> col = vtkSmartPointer<vtkCollisionModel>::New();
	col->SetName("vtkbioeng");
	col->SetFileName(cfn);
	col->SetOrigin(origin);
	col->SetOpacity(0.5);
	col->SetColor(0.0, 0.0, 1.0);
	col->Init();

	vtkSmartPointer<vtkScenarioElement> stick = vtkSmartPointer<vtkScenarioElement>::New();
	stick->SetId(0);
	stick->SetName("stick");
	stick->SetVisualizationModel(vis);
	stick->SetCollisionModel(col);
	stick->Init();

	//Second element (left lever)
	vtkSmartPointer<vtkVisualizationModel> visl = vtkSmartPointer<vtkVisualizationModel>::New();
	visl->SetName("left_vis");
	visl->SetFileName(fnl);
	visl->SetOrigin(origin);
	visl->SetTextureFileName(tfnl);
	visl->SetOpacity(1.0);
	visl->SetColor(0.0, 1.0, 1.0);
	visl->Init();

	vtkSmartPointer<vtkCollisionModel> coll = vtkSmartPointer<vtkCollisionModel>::New();
	coll->SetName("left_vtkbioeng");
	coll->SetFileName(cfnl);
	coll->SetOrigin(origin);
	coll->SetOpacity(0.5);
	coll->SetColor(0.0, 0.0, 1.0);
	coll->Init();

	vtkSmartPointer<vtkScenarioElement> left = vtkSmartPointer<vtkScenarioElement>::New();
	left->SetId(1);
	left->SetName("left");
	left->SetVisualizationModel(visl);
	left->SetCollisionModel(coll);
	left->Init();

	//Third element (right lever)
	vtkSmartPointer<vtkVisualizationModel> visr = vtkSmartPointer<vtkVisualizationModel>::New();
	visr->SetName("right_vis");
	visr->SetFileName(fnr);
	visr->SetOrigin(origin);
	visr->SetTextureFileName(tfnr);
	visr->SetOpacity(1.0);
	visr->SetColor(1.0, 0.0, 1.0);
	visr->Init();

	vtkSmartPointer<vtkCollisionModel> colr = vtkSmartPointer<vtkCollisionModel>::New();
	colr->SetName("right_vtkbioeng");
	colr->SetFileName(cfnr);
	colr->SetOrigin(origin);
	colr->SetOpacity(0.5);
	colr->SetColor(0.0, 0.0, 1.0);
	colr->Init();

	vtkSmartPointer<vtkScenarioElement> right = vtkSmartPointer<vtkScenarioElement>::New();
	right->SetId(2);
	right->SetName("right");
	right->SetVisualizationModel(visr);
	right->SetCollisionModel(colr);
	right->Init();

	vtkSmartPointer<vtkToolGrasper> tool = vtkSmartPointer<vtkToolGrasper>::New();
	tool->SetStick(stick);
	tool->SetLeftLever(left);
	tool->SetRightLever(right);
	tool->Init();

	tool->Translate(position);
	tool->RotateX(-10);
	//tool->Update();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(800,600);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	//Visualization of collision meshes is disabled
	renderer->AddActor(vis->GetActor());
	renderer->AddActor(col->GetActor());
	renderer->AddActor(visl->GetActor());
	renderer->AddActor(coll->GetActor());
	renderer->AddActor(visr->GetActor());
	renderer->AddActor(colr->GetActor());
	renderer->SetBackground(1,1,1);

	//Adjust Camera
	vtkCamera * camera = renderer->GetActiveCamera();
	camera->SetPosition(0, 0, 6);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewAngle(60);
	camera->Yaw(0);
	camera->Elevation(20);
	camera->Pitch(-15);
	camera->Dolly(1);

	iren->Initialize();

	renWin->Render();

	iren->Start();

	return 0;
}


