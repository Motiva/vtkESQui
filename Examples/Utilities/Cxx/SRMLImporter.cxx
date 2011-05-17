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
#include "vtkSmartPointer.h"

#include "vtkScenario.h"
#include "vtkSimulationInteractorStyle.h"
#include "vtkSimulation.h"

#include "vtkSRMLImporter.h"

//!This test perform a standard execution of the ESQUI simulation process, importing the scenario items from an SRML file

int main(int argc, char * argv[])
{
	const char * path = "/home/jballesteros/Workspace/data/vtkESQuiData/";
	const char * fn = "/home/jballesteros/Workspace/data/vtkESQuiData/SRML/Laparoscopy/lap_basic.srml";

	if (argc > 1)
	{
		fn = argv[1];
	}

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
	vtkSmartPointer<vtkScenario> Scenario =
			vtkSmartPointer<vtkScenario>::New();
	Scenario->SetRenderWindow(renWin);

	/**********  Interactor Style Definitions  ********/
	vtkSmartPointer<vtkSimulationInteractorStyle> Style =
			vtkSmartPointer<vtkSimulationInteractorStyle>::New();

	/**********  Simulation Definitions  ********/
	vtkSmartPointer<vtkSimulation> Simulation =
			vtkSmartPointer<vtkSimulation>::New();
	Simulation->SetScenario(Scenario);
	Simulation->SetInteractorStyle(Style);

	/**********  Simulation Import from SRML File  ********/
	vtkSmartPointer<vtkSRMLImporter> Importer =
			vtkSmartPointer<vtkSRMLImporter>::New();
	Importer->SetDataPath(path);
	Importer->SetFileName(fn);
	Importer->SetSimulation(Simulation);
	Importer->Read();

	//Initialize simulation with imported values
	Simulation->Init();

	//Run simulation process
	Simulation->Run();

	return 0;
}

