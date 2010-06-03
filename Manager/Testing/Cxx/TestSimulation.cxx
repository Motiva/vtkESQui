#include <iostream>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include "vtkSimulation.h"
#include "vtkMath.h"

using namespace std;

#if defined(WIN32)
#include <time.h> 
#endif

//!This test performs a standard execution of the vtkSimulation class

int TestSimulation(int argc, char * argv[])
{
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
	Simulation->SetRenderWindowInteractor(iren);
	Simulation->Init();

	Simulation->Run();

	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	SimulationManager->Delete();
	Scenario->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();

	return 0;
}


