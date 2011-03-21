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
#include "vtkPolyDataReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"
#include "vtkTexture.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPoints.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkExtractVOI.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartPointer.h"

#include "vtkScenario.h"
#include "vtkToolCollection.h"
#include "vtkTool.h"
#include "vtkToolGrasper.h"
#include "vtkToolProbe.h"
#include "vtkOrganCollection.h"
#include "vtkOrgan.h"
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

//!This test perform a standard execution of the ESQUI simulation of a laparoscopic scenario.

int main(int argc, char * argv[])
{

	const char * fn0 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/Stick.vtp";
	const char * fn1 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/LeftLever.vtp";
	const char * fn2 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Grasper/RightLever.vtp";
	const char * fn3 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
	const char * fn3t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg";
	const char * fn4 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/stomach.vtp";
	const char * fn4t = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/stomach.jpg";
	const char * fn5 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Stick.vtp";
	const char * fn6 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/Tip.vtp";
	const char * fn10 = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/DICOM/AbdVessels/AbdVesselsClip.vtk";

	/**********  Render Window Definitions  ********/
	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground(0.0, 0.0, 0.0);
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	/**********  Scenario Definitions  ********/
	vtkScenario * scenario = vtkScenario::New();
	scenario->SetRenderWindow(renWin);

	/**********  Load Deformable Models (Organs)  ********/
	//Create a Organ
	vtkOrgan * organ = vtkOrgan::New();
	//Set organ identifier
	organ->SetId(0);
	organ->SetName("Sphere");

	//Set source data fn
	organ->SetFileName(fn3);
	organ->SetTextureFileName(fn3t);

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
	//scenario->AddOrgan(organ);

	/********** Load Tools **********/
	//Add new tool To the scenario
	//Create a Tool
	vtkToolGrasper * leftGrasper = vtkToolGrasper::New();
	//Set tool identifier
	leftGrasper->SetId(0);
	leftGrasper->SetNumberOfPieces(3);
	//Set source data fn
	leftGrasper->SetStickFileName(fn0);
	leftGrasper->SetLeftLeverFileName(fn1);
	leftGrasper->SetRightLeverFileName(fn2);
	//Set geometric parameters
	leftGrasper->SetPosition(-3, 0, 0);
	leftGrasper->SetOrientation(0, 10, 0);
	leftGrasper->SetOrigin(0, 0, 4);

	//Set tool scale (size)
	leftGrasper->SetScale(1.0, 1.0, 1.0);
	leftGrasper->SetDeltaT(0.01);

	//Add tool to the scenario
	//scenario->AddTool(leftGrasper);

	//Create a Tool
	vtkToolProbe * rightProbe = vtkToolProbe::New();
	//Set tool identifier
	rightProbe->SetId(1);
	rightProbe->SetNumberOfPieces(2);
	//Set source data fn
	rightProbe->SetStickFileName(fn5);
	rightProbe->SetTipFileName(fn6);
	//Set geometric parameters
	rightProbe->SetPosition(3, 0, 0);
	rightProbe->SetOrientation(0, -10, 0);
	rightProbe->SetOrigin(0, 0, 4);

	//Set tool scale (size)
	rightProbe->SetScale(1.0, 1.0, 1.0);
	rightProbe->SetDeltaT(0.01);

	//Add tool to the scenario
	scenario->AddTool(rightProbe);

	/********** Load Volume **********/
	vtkStructuredPointsReader * reader = vtkStructuredPointsReader::New();
	reader->SetFileName(fn10);
	reader->Update();

	vtkImageData * image = reader->GetOutput();
	image->Print(cout);

	vtkExtractVOI * extract = vtkExtractVOI::New();
	extract->SetInput(image);
	extract->SetVOI(image->GetExtent());
	extract->SetSampleRate(2,2,2);
	extract->Update();
	vtkImageData * i = extract->GetOutput();
	i->Print(cout);

	vtkVolume * volume = vtkVolume::New();
	vtkSmartVolumeMapper * volmapper = vtkSmartVolumeMapper::New();
	volmapper->SetInput(i);

	//Set color transfer function
	//Create Default Color transfer function
	vtkSmartPointer<vtkColorTransferFunction> colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	vtkSmartPointer<vtkPiecewiseFunction> opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkPiecewiseFunction> gradientFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();

	//Grey Scale
	/*
	colorFunction->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );

	// The opacity transfer function is used to control the opacity
	// of different tissue types.
	int opacityLevel = 2048;
	int opacityWindow = 4096;
	opacityFunction->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0,
			opacityLevel + 0.5*opacityWindow, 0.2 );
	//mapper->SetBlendModeToMaximumIntensity();
	 */
	double * r = i->GetScalarRange();

	int minT = r[0];
	int lowT = 90;
	int highT = 1150;
	int maxT = r[1];

	colorFunction->AddRGBPoint(minT, 0, 0, 0);//, 0.5, 0.0 );
	colorFunction->AddRGBPoint(lowT, 0.73, 0.25, 0.30);//, 0.5, .61 );
	colorFunction->AddRGBPoint(highT, 0.9, 0.8, 0.5);//, .5, 0.0 );
	colorFunction->AddRGBPoint(maxT, 1.0, 1.0, 1.0);//, .5, 0.0 );

	opacityFunction->AddPoint(minT, 0.0);
	opacityFunction->AddPoint(lowT, 0.0);
	opacityFunction->AddPoint(highT, 0.1);
	opacityFunction->AddPoint(1150, 0.1);
	opacityFunction->AddPoint(maxT, 0.9);

	gradientFunction->AddPoint(0,   0.0);
	gradientFunction->AddPoint(90,  0.5);
	gradientFunction->AddPoint(100, 1.0);

	volmapper->SetBlendModeToComposite();

	//Create the volume property
	vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
	property->SetIndependentComponents(1);
	property->SetColor(colorFunction);
	property->SetScalarOpacity(opacityFunction);
	property->SetGradientOpacity(gradientFunction);
	property->SetInterpolationTypeToLinear();
	property->ShadeOn();
	property->SetAmbient(0.4);
	property->SetDiffuse(0.6);
	property->SetSpecular(0.2);
	property->SetSpecularPower(10);
	property->SetScalarOpacityUnitDistance(0.8919);

	volume->SetProperty(property);
	volume->SetMapper(volmapper);
	volume->RotateX(-90);

	ren1->AddVolume(volume);

	/**********  Load Scene Environment  ********/

	/********** Lights  **********/
	ren1->GetLights()->InitTraversal();
	vtkLight *headLight = vtkLight::New();
	headLight->SetLightTypeToHeadlight();
	headLight->PositionalOn();
	headLight->SetIntensity(0.5);
	headLight->SetConeAngle(30);
	ren1->AddLight(headLight);

	vtkLight *ambientLight = vtkLight::New(); 
	ambientLight->SetIntensity(0.8);
	ambientLight->SetLightTypeToSceneLight();
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
	organ->Delete();;
	leftGrasper->Delete();
	rightProbe->Delete();
	headLight->Delete();
	ambientLight->Delete();
	scenario->Delete();
	simulation->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	
	return 0;
}

