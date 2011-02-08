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
#include "vtkToolPincers.h"
#include "vtkCubeSource.h"
#include "vtkSimulation.h"
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

const char * EsquiExampleNS::ExpandDataFileName(const char * fname)
{
	char * fullName;
	fullName = new char[strlen(path) + 1 + strlen(fname)];
	fullName[0] = 0;
	strcat(fullName, path);
	size_t len = strlen(fullName);
	fullName[len] = '/';
	fullName[len+1] = 0;
	strcat(fullName, fname);

	return fullName;
}

int main(int argc, char * argv[])
{
	/**********  Render Window Definitions  ********/
	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground(1.0,1.0,1.0);
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	/**********  Scenario Definitions  ********/
	vtkScenario * Scenario = vtkScenario::New();
	Scenario->SetRenderWindow(renWin);
	vtkPolyDataReader *vtkreader1 = vtkPolyDataReader::New();
	vtkreader1->SetFileName(ExpandDataFileName("Scenario/Misc/Marco.vtk"));
	vtkPolyDataMapper *vtkmapper1 = vtkPolyDataMapper::New();
	vtkmapper1->SetInput(vtkreader1->GetOutput());
	vtkActor *vtkactor1 = vtkActor::New();
	vtkactor1->SetMapper(vtkmapper1);
	ren1->AddActor(vtkactor1);
	
	/**********  Set Frame Texture  ********/
	vtkJPEGReader *textureimage1 = vtkJPEGReader::New();
	textureimage1->SetFileName(ExpandDataFileName("Scenario/Misc/Grey.jpg"));
	vtkTexture *texture1 = vtkTexture::New();
	texture1->SetInput(textureimage1->GetOutput());
	vtkactor1->SetTexture(texture1);
	
	/**********  Load Deformable Model  ********/
	vtkOrgan * organ = vtkOrgan::New();
	organ->SetFileName(ExpandDataFileName("Scenario/Organs/liverTetra.vtk"));
	organ->SetPosition(0, 0,-5);
	organ->SetOrigin(0, 0,-5);

	organ->GetActor()->GetProperty()->SetColor(1,0.2,0.2);

	//Biomechanical model
	vtkPSSInterface * model = vtkPSSInterface::New();
	model->SetDistanceCoefficient(200);
	model->SetDampingCoefficient(100);
	model->SetMass(1);
	model->SetDeltaT(0.01);

	organ->SetDeformationModel(model);
	
	Scenario->AddOrgan(organ);

	/********** Tools **********/
	//Add new tool To the scenario
	vtkToolPincers *pincers = vtkToolPincers::New();
	pincers->SetStickFileName(ExpandDataFileName("Scenario/Tools/Pincers/Stick.vtk"));
	pincers->SetLeftGrasperFileName(ExpandDataFileName("Scenario/Tools/Pincers/LeftGrasper.vtk"));
	pincers->SetRightGrasperFileName(ExpandDataFileName("Scenario/Tools/Pincers/RightGrasper.vtk"));

	pincers->SetScale(1.0, 1.0, 1.0);
	pincers->SetPosition(-3, 0, 0);
	pincers->SetOrientation(0, -10, 0);

	Scenario->AddTool(pincers);

	/**********  Load Scene Environment  ********/

	/********** Lights  **********/
	ren1->GetLights()->InitTraversal();
	vtkLight *light = vtkLight::New();
	light->SetLightTypeToHeadlight();//#La luz se situa encima de la camara
	light->PositionalOn();//# Ademas es posicional(se mueve con ella y actua a modo de linterna)
	light->SetIntensity(0.5);
	light->SetConeAngle(20);
	ren1->AddLight(light);
		
	vtkLight *luzEntorno = vtkLight::New(); 
	luzEntorno->SetIntensity(0.8);
	luzEntorno->SetLightTypeToHeadlight();
	luzEntorno->PositionalOff();
	ren1->AddLight(luzEntorno);//#Se a�ade una luz de entorno para que no se vea completamente negro
	ren1->SetAmbient(0.5,0.5,0.5);
		
	/**********  Camera Definitions  ********/
	vtkCamera * camera = ren1->GetActiveCamera();
	camera->SetPosition(0, 0, 0);
	camera->SetFocalPoint(0, 0, -12);
	camera->Yaw(0);
	camera->Elevation(20);
	camera->Pitch(-20);
	camera->Dolly(1);
	camera->ParallelProjectionOff();
	camera->SetViewAngle(70);

	/**********  Simulation Setup  ********/
	//vtkSimulationManager *SimulationManager = vtkSimulationManager::New();
	//SimulationManager->SetLibraryName("vtkbioeng");
	//SimulationManager->SetScenario(Scenario);
	//SimulationManager->Init();

	vtkSimulation * Simulation = vtkSimulation::New();
	Simulation->SetScenario(Scenario);
	Simulation->Init();

	Simulation->Run();

	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	//SimulationManager->Delete();
	Scenario->Delete();
	vtkreader1->Delete();
	vtkmapper1->Delete();
	vtkactor1->Delete();
	textureimage1->Delete();
	texture1->Delete();
	organ->Delete();
	light->Delete();
	luzEntorno->Delete();
	pincers->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	
	return 0;
}

