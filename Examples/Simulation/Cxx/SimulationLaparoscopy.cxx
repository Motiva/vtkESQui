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
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include "vtkScenario.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"
#include "vtkTexture.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkToolGrasper.h"
#include "vtkCubeSource.h"
#include "vtkSimulation.h"
#include "vtkSimulationInteractorStyle.h"
#include "vtkPSSInterface.h"

namespace EsquiExampleNS{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase
	const char * ExpandDataFileName(const char * fname);
	//const char * path ="C:/Workspace/data/vtkESQuiData";
	const char * path ="/home/jballesteros/Workspace/data/vtkESQuiData";
}

using namespace std;
using namespace EsquiExampleNS;

//!This test perform a standard execution of the ESQUI simulation process.

int main(int argc, char * argv[])
{

	const char * filename0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/Stick.vtp";
	const char * filename1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/LeftLever.vtp";
	const char * filename2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/RightLever.vtp";
	const char * filename3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
	const char * filename3t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscletexture.jpg";
	const char * filename4 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp";
	const char * filename4t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomachtexture.jpg";

	/**********  Render Window Definitions  ********/
	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground(1.0,1.0,1.0);
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	/**********  Scenario Definitions  ********/
	vtkScenario * scenario = vtkScenario::New();
	scenario->SetRenderWindow(renWin);

	/**********  Load Deformable Model  ********/
	//Create a Organ
	vtkOrgan * organ = vtkOrgan::New();
	//Set organ identifier
	organ->SetId(0);
	organ->SetName("Sphere");

	//Set source data filename
	organ->SetFileName(filename3);
	organ->SetTextureFileName(filename3t);

	//Set geometric parameters
	organ->SetPosition(0.0, 0.0, -3.0);
	organ->SetOrientation(0.0, 0.0, 0.0);
	organ->SetOrigin(0.0, 0.0, -3.0);

	//Set tool scale (size)
	organ->SetScale(1.0, 1.0, 1.0);

	//Set organ type
	organ->SetOrganType(vtkOrgan::Deformable);

	//Set Deformation Model
	vtkPSSInterface * particleSpring = vtkPSSInterface::New();
	particleSpring->SetDeltaT(0.01);
	particleSpring->SetGravity(0.0, 0.0, 0.0);

	//Set particle-spring system specific parameters
	particleSpring->SetSpringCoefficient(100);
	particleSpring->SetDampingCoefficient(5);
	particleSpring->SetDistanceCoefficient(20);
	particleSpring->SetMass(1);
	particleSpring->SetRigidityFactor(2);
	particleSpring->SetSolverType(vtkParticleSpringSystem::RungeKutta4);

	organ->SetDeformationModel(particleSpring);

	//Add organ to the scenario
	scenario->AddOrgan(organ);

	//Create a cavity organ
	vtkOrgan * cavity = vtkOrgan::New();
	//Set organ identifier
	cavity->SetId(1);
	cavity->SetName("Cavity");

	//Set source data filename
	cavity->SetFileName(filename4);
	cavity->SetTextureFileName(filename4t);

	//Set geometric parameters
	cavity->SetPosition(0.0, 0.0, 0.0);
	cavity->SetOrientation(0.0, 0.0, 0.0);
	cavity->SetOrigin(0.0, 0.0, 0.0);

	//Set tool scale (size)
	cavity->SetScale(1.0, 1.0, 1.0);

	//Set organ type
	cavity->SetOrganType(vtkOrgan::Deformable);

	//Set Deformation Model
	vtkPSSInterface * ps1 = vtkPSSInterface::New();
	ps1->SetDeltaT(0.01);
	ps1->SetGravity(0.0, 0.0, 0.0);

	//Set particle-spring system specific parameters
	ps1->SetSpringCoefficient(100);
	ps1->SetDampingCoefficient(5);
	ps1->SetDistanceCoefficient(20);
	ps1->SetMass(1);
	ps1->SetRigidityFactor(2);
	ps1->SetSolverType(vtkParticleSpringSystem::RungeKutta4);

	cavity->SetDeformationModel(ps1);

	//Add organ to the scenario
	scenario->AddOrgan(cavity);

	/********** Tools **********/
	//Add new tool To the scenario
	//Create a Tool
	vtkToolGrasper * leftGrasper = vtkToolGrasper::New();
	//Set tool identifier
	leftGrasper->SetId(0);
	leftGrasper->SetNumberOfPieces(3);
	//Set source data filename
	leftGrasper->SetStickFileName(filename0);
	leftGrasper->SetLeftLeverFileName(filename1);
	leftGrasper->SetRightLeverFileName(filename2);
	//Set geometric parameters
	leftGrasper->SetPosition(-3, 0, 0);
	leftGrasper->SetOrientation(0, 10, 0);
	leftGrasper->SetOrigin(0, 0, 4);

	//Set tool scale (size)
	leftGrasper->SetScale(1.0, 1.0, 1.0);
	leftGrasper->SetDeltaT(0.01);

	//Add tool to the scenario
	scenario->AddTool(leftGrasper);

	//Create a Tool
	vtkToolGrasper * rightGrasper = vtkToolGrasper::New();
	//Set tool identifier
	rightGrasper->SetId(1);
	rightGrasper->SetNumberOfPieces(3);
	//Set source data filename
	rightGrasper->SetStickFileName(filename0);
	rightGrasper->SetLeftLeverFileName(filename1);
	rightGrasper->SetRightLeverFileName(filename2);
	//Set geometric parameters
	rightGrasper->SetPosition(3, 0, 0);
	rightGrasper->SetOrientation(0, -10, 0);
	rightGrasper->SetOrigin(0, 0, 4);

	//Set tool scale (size)
	rightGrasper->SetScale(1.0, 1.0, 1.0);
	rightGrasper->SetDeltaT(0.01);

	//Add tool to the scenario
	scenario->AddTool(rightGrasper);

	/**********  Load Scene Environment  ********/

	/********** Lights  **********/
	ren1->GetLights()->InitTraversal();
	vtkLight *headLight = vtkLight::New();
	headLight->SetLightTypeToHeadlight();
	headLight->PositionalOn();
	headLight->SetIntensity(0.5);
	headLight->SetConeAngle(20);
	ren1->AddLight(headLight);
		
	vtkLight *ambientLight = vtkLight::New(); 
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
	vtkSimulationInteractorStyle * style = vtkSimulationInteractorStyle::New();
	style->SetScenario(scenario);
	iren->SetInteractorStyle(style);

	vtkSimulation * simulation = vtkSimulation::New();
	simulation->SetScenario(scenario);
	simulation->SetRenderTimerRate(0.02);
	simulation->SetSimulationTimerRate(0.01);
	simulation->SetHapticTimerRate(0.001);
	simulation->Init();

	simulation->Run();

	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	organ->Delete();
	cavity->Delete();
	leftGrasper->Delete();
	rightGrasper->Delete();
	headLight->Delete();
	ambientLight->Delete();
	scenario->Delete();
	simulation->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	
	return 0;
}

