/*
 * TestEsquivtkToolCollection.cxx
 *
 *  Created on: 12/01/2010
 *      Author: jballesteros
 */

#include <iostream>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtkSRMLImporter.h"
#include "vtkSimulation.h"
#include "vtkSimulationManager.h"
#include "vtkScenario.h"

using namespace std;

//!This test perform a test of the vtkToolCollection class

int TestvtkSRMLImporter(int argc, char * argv[])
{
	/**********  Render Window Definitions  ********/
	vtkRenderer * ren1 = vtkRenderer::New();
	ren1->SetBackground(1.0,1.0,1.0);

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkSimulation * Simulation = vtkSimulation::New();
	vtkSimulationManager * Manager = vtkSimulationManager::New();
	vtkScenario * Scenario = vtkScenario::New();
	Scenario->SetRenderWindow(renWin);

	Manager->SetScenario(Scenario);
	Manager->SetCollisionDetectionLibrary("vtkbioeng");
	Manager->Init();

	Simulation->SetSimulationManager(Manager);

	vtkSRMLImporter * Importer = vtkSRMLImporter::New();
	Importer->SetDebug(1);

	Importer->SetFileName("test.srml");
	Importer->SetSimulation(Simulation);
	Importer->Read();

	return 0;
}


