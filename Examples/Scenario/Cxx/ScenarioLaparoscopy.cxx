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

#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"

#include "vtkScenario.h"
#include "vtkScenarioElement.h"
#include "vtkScenarioObject.h"
#include "vtkVisualizationModel.h"
#include "vtkCollisionModel.h"
#include "vtkPSSInterface.h"
#include "vtkToolDummy.h"

//!This test perform a test of the vtkToolLaparoscopy class

int main(int argc, char * argv[])
{

	const char * fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick.vtp";
	const char * cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick.vtp";
	const char * tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg";

	const char * fnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip.vtp";
	const char * cfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip.vtp";
	const char * tfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.8,0.8,0.8);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(800,600);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkScenario> scenario = vtkSmartPointer<vtkScenario>::New();

	//Create a probe
	//Generate probe's first element (stick)
	vtkSmartPointer<vtkVisualizationModel> vis = vtkSmartPointer<vtkVisualizationModel>::New();
	vis->SetName("stick_vis");
	vis->SetFileName(fn);
	vis->SetOrigin(0,0,6);
	vis->SetTextureFileName(tfn);
	vis->SetOpacity(1.0);

	vtkSmartPointer<vtkCollisionModel> col = vtkSmartPointer<vtkCollisionModel>::New();
	col->SetName("stick_col");
	col->SetFileName(cfn);
	vis->SetOrigin(0,0,6);
	col->SetOpacity(1.0);
	col->SetColor(0.0, 0.0, 1.0);

	vtkSmartPointer<vtkScenarioElement> stick = vtkSmartPointer<vtkScenarioElement>::New();
	stick->SetId(0);
	stick->SetName("stick");
	stick->SetVisualizationModel(vis);
	stick->SetCollisionModel(col);

	//Second element (tip)
	vtkSmartPointer<vtkVisualizationModel> visb = vtkSmartPointer<vtkVisualizationModel>::New();
	visb->SetName("tip_vis");
	visb->SetFileName(fnb);
	visb->SetTextureFileName(tfnb);
	vis->SetOrigin(0,0,6);
	visb->SetOpacity(1.0);
	visb->SetColor(1.0, 0.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> colb = vtkSmartPointer<vtkCollisionModel>::New();
	colb->SetName("tip_col");
	colb->SetFileName(cfnb);
	vis->SetOrigin(0,0,6);
	colb->SetOpacity(1.0);
	colb->SetColor(0.0, 0.0, 1.0);

	vtkSmartPointer<vtkScenarioElement> tip = vtkSmartPointer<vtkScenarioElement>::New();
	tip->SetId(1);
	tip->SetName("tip");
	tip->SetVisualizationModel(visb);
	tip->SetCollisionModel(colb);

	vtkSmartPointer<vtkToolDummy> probe = vtkSmartPointer<vtkToolDummy>::New();
	probe->AddElement(stick);
	probe->AddElement(tip);

	probe->Translate(2, 0, 0);
	probe->RotateY(5);
	probe->RotateX(-20);

	scenario->SetRenderWindow(renWin);
	scenario->AddObject(probe);
	scenario->Init();

	scenario->Update();

	//Adjust Camera
	vtkCamera * camera = renderer->GetActiveCamera();
	camera->SetPosition(0, 0, 2);
	camera->SetFocalPoint(0, 0, -6);
	camera->SetViewAngle(60);
	camera->Yaw(0);
	camera->Elevation(20);
	camera->Pitch(-15);
	camera->Dolly(0.8);

	iren->Initialize();

	renWin->Render();

	iren->Start();

	return 0;
}
