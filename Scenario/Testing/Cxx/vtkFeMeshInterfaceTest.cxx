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
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataWriter.h"
#include "vtkDataSetMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkActor.h"
#include "vtkProperty.h"

#include "vtkSphereSource.h"
#include "vtkPlaneSource.h"
#include "vtkGlyph3D.h"
#include "vtkDelaunay3D.h"
#include "vtkContourFilter.h"
#include "vtkOutlineFilter.h"
#include "vtkCellDataToPointData.h"
#include "vtkEdgePoints.h"

#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkExtractEdges.h"

#include "vtkFeMeshInterface.h"
#include "itkMesh.h"

#include "vtkCommand.h"


#include <iostream>

namespace vtkFeMeshInterfaceNS{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase

	vtkContact * Contact;

	vtkUnstructuredGrid * InputGrid;
	vtkUnstructuredGrid * OutputGrid;
	vtkFeMeshInterface * FeMesh;

	void Update();
}

using namespace std;
using namespace vtkFeMeshInterfaceNS;

void vtkFeMeshInterfaceNS::Update()
{
	FeMesh->InsertNextContact(Contact);
	FeMesh->Update();

	OutputGrid = FeMesh->GetOutput();
	OutputGrid->Update();

	std::cout << "OutputGrid: " << OutputGrid->GetNumberOfPoints() << std::endl;

	vtkUnstructuredGridWriter * writer = vtkUnstructuredGridWriter::New();
	writer->SetFileName("Scenario/femTetra.vtk");
	writer->SetInput(OutputGrid);
	writer->Update();
	writer->Delete();

}

class vtkTimerCallback : public vtkCommand
{
public:
	static vtkTimerCallback *New()
	{
		vtkTimerCallback *cb = new vtkTimerCallback;
		cb->FastTimerId = 0;
		cb->FastTimerCount = 0;
		cb->RenderTimerId = 0;
		cb->RenderTimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			int tid = * static_cast<int *>(callData);

			if (tid == this->FastTimerId)
			{
				//Update Simulation Trigger
				vtkFeMeshInterfaceNS::Update();

				++this->FastTimerCount;
			}
			else if (tid == this->RenderTimerId)
			{
				++this->RenderTimerCount;

				vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
				if (iren && iren->GetRenderWindow() && iren->GetRenderWindow()->GetRenderers())
				{
					iren->Render();
				}
			}
		}
	}

	void SetFastTimerId(int tid)
	{
		this->FastTimerId = tid;
		this->FastTimerCount = 0;
	}

	void SetRenderTimerId(int tid)
	{
		this->RenderTimerId = tid;
		this->RenderTimerCount = 0;
	}

	void Report()
	{
		cout << "vtkTimerCallback::Report" << endl;
		cout << "  FastTimerId: " << this->FastTimerId << endl;
		cout << "  FastTimerCount: " << this->FastTimerCount << endl;
		cout << "  RenderTimerId: " << this->RenderTimerId << endl;
		cout << "  RenderTimerCount: " << this->RenderTimerCount << endl;
	}

private:
	int FastTimerId;
	int FastTimerCount;
	int RenderTimerId;
	int RenderTimerCount;
};

int vtkFeMeshInterfaceTest(int ac, char* av[])
{
	vtkUnstructuredGridReader * reader = vtkUnstructuredGridReader::New();
	if(ac < 1) reader->SetFileName("Scenario/liverTetra.vtk");
	else reader->SetFileName(av[1]);
	reader->Update();
	
	InputGrid = reader->GetOutput();

	vtkExtractEdges * edges = vtkExtractEdges::New();
	edges->SetInput(InputGrid);
	edges->Update();

 	FeMesh = vtkFeMeshInterface::New();
	FeMesh->SetInput(InputGrid);
	FeMesh->Init();
	
	FeMesh->Update();
	OutputGrid = FeMesh->GetOutput();

	Contact = vtkContact::New();

	Contact->SetOrganId(0);
	Contact->SetToolId(0);

	//Get organ bounding box point that has been contacted
	vtkIdType cellId = 15;
	double * point = InputGrid->GetCell(cellId)->GetPoints()->GetPoint(0);
	Contact->SetCellId(cellId);

	//We will find the organ closest point to the bounding box
	vtkPointLocator * locator = vtkPointLocator::New();
	locator->SetDataSet(OutputGrid);
	vtkIdType pointId = locator->FindClosestPoint(point);
	Contact->SetPointId(pointId);
	double * p =OutputGrid->GetPoints()->GetPoint(pointId);
	Contact->SetPoint(p);

	double vector[3];
	vector[0]=0;
	vector[1]=0;
	vector[2]=-1;
	Contact->SetForceMagnitude(1);
	Contact->SetForceVector(vector);

	// Render Window Objects //
	vtkRenderer * renderer = vtkRenderer::New();
	vtkRenderWindow * renWin = vtkRenderWindow::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(0.03);
	sphere->SetThetaResolution(8); sphere->SetPhiResolution(8);
	
	vtkGlyph3D *spheres = vtkGlyph3D::New();
	spheres->SetInput(InputGrid);
	spheres->SetSource(sphere->GetOutput());

	vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(spheres->GetOutputPort());
	mapper->ImmediateModeRenderingOn();

	vtkActor * meshActor = vtkActor::New();
	meshActor->SetMapper(mapper);
	meshActor->GetProperty()->SetColor(1,0,0);
	renderer->AddActor(meshActor);

	vtkSphereSource * sphere2 = vtkSphereSource::New();
	sphere2->SetRadius(0.025);
	sphere2->SetThetaResolution(8); sphere2->SetPhiResolution(8);

	vtkGlyph3D * spheres2 = vtkGlyph3D::New();
	spheres2->SetInput(OutputGrid);
	spheres2->SetSource(sphere2->GetOutput());

	vtkPolyDataMapper * mapper2 = vtkPolyDataMapper::New();
	mapper2->SetInputConnection(spheres2->GetOutputPort());
	mapper2->ImmediateModeRenderingOn();

	vtkActor * meshActor2 = vtkActor::New();
	meshActor2->SetMapper(mapper2);
	meshActor2->GetProperty()->SetColor(0,0,1);
	renderer->AddActor(meshActor2);

	vtkDataSetMapper * mapper3 = vtkDataSetMapper::New();
	mapper3->SetInput(OutputGrid);
	mapper3->ImmediateModeRenderingOn();

	vtkActor * meshActor3 = vtkActor::New();
	meshActor3->SetMapper(mapper3);
	meshActor3->GetProperty()->SetColor(0,0,1);
	//renderer->AddActor(meshActor3);
	
	vtkDataSetMapper * mapper4 = vtkDataSetMapper::New();
	mapper4->SetInputConnection(edges->GetOutputPort());
	mapper4->ScalarVisibilityOff();

	vtkActor * meshActor4 = vtkActor::New();
	meshActor4->SetMapper(mapper4);
	meshActor4->GetProperty()->SetColor(0,1,0);
	//renderer->AddActor(meshActor4);

	/**********  Window Rendering  ********/
	iren->Initialize();
	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	tid = iren->CreateRepeatingTimer(50);
	cb->SetFastTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 20 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(30);
	cb->SetRenderTimerId(tid);

	iren->Start();
	

	reader->Delete();
	edges->Delete();
	sphere->Delete();
	spheres->Delete();
	meshActor->Delete();
	meshActor2->Delete();
	meshActor3->Delete();
	meshActor4->Delete();
	mapper->Delete();
	mapper2->Delete();
	mapper3->Delete();
	mapper4->Delete();
	FeMesh->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
  return 0;
}
