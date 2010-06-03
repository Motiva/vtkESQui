#include <iostream>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include "vtkScenario.h"
#include "vtkSimulationManager.h"
#include "vtkToolCollection.h"
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

#include "vtkSRMLImporter.h"


namespace EsquiExampleNS{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase
	const char * ExpandDataFileName(const char * fname);
	//const char * path ="C:/Workspace/data/vtkESQuiData";
	const char * path ="/home/jorge/Workspace/data/vtkESQuiData";
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

//!This test perform a standard execution of the ESQUI simulation process, importing the scenario items from an SRML file

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
	
	/**********  Simulation Setup  ********/
	vtkSimulationManager *SimulationManager = vtkSimulationManager::New();
	SimulationManager->SetCollisionDetectionLibrary("vtkbioeng");
	SimulationManager->SetScenario(Scenario);
	SimulationManager->Init();

	vtkSimulation * Simulation = vtkSimulation::New();
	Simulation->SetSimulationManager(SimulationManager);

	/**********  Simulation Import from SRML File  ********/
	vtkSRMLImporter * Importer = vtkSRMLImporter::New();
	Importer->SetDataPath(path);
	Importer->SetFileName(ExpandDataFileName("laparoscopy.srml"));
	Importer->SetSimulation(Simulation);
	Importer->Read();

	Simulation->Run();

	//TODO: Create Interactor for keyboard control
	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	SimulationManager->Delete();
	Scenario->Delete();
	Importer->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	
	return 0;
}

