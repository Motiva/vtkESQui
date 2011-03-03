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

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"

#include "vtkScenario.h"
#include "vtkTool.h"
#include "vtkToolLaparoscopy.h"
#include "vtkToolGrasper.h"

//!This test perform a test of the vtkToolLaparoscopy class

int main(int argc, char * argv[])
{

	const char * filename0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/Stick.vtp";
	const char * filename1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/LeftLever.vtp";
	const char * filename2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/RightLever.vtp";

	if (argc > 3)
	{
		filename0 = argv[1];
		filename1 = argv[2];
		filename2 = argv[3];
	}

	/**********  Render Window Definitions  ********/
	vtkRenderer * ren1 = vtkRenderer::New();
	ren1->SetBackground(1.0,1.0,1.0);

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkToolGrasper * grasper = vtkToolGrasper::New();
	//Set tool identifier
	grasper->SetId(0);
	grasper->SetNumberOfPieces(3);
	//Set source data filename
	grasper->SetStickFileName(filename0);
	grasper->SetLeftLeverFileName(filename1);
	grasper->SetRightLeverFileName(filename2);
	//Set geometric parameters
	grasper->SetPosition(3, 0, 0);
	grasper->SetOrientation(0, -10, 0);
	grasper->SetOrigin(0, 0, 4);

	//Set tool scale (size)
	grasper->SetScale(1.0, 1.0, 1.0);

	//Assign render window for display purposes
	grasper->SetRenderWindow(renWin);
	grasper->Init();

	//Adjust Camera
	vtkCamera * camera = ren1->GetActiveCamera();
	camera->SetPosition(0, 0, 2);
	camera->SetFocalPoint(0, 0, -6);
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
