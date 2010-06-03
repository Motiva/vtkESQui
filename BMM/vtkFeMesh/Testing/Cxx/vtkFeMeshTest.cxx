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

#include "vtkFeMesh.h"
#include "itkMesh.h"

#include "vtkCommand.h"


#include <iostream>

namespace vtkFeMeshNS{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase

	vtkIdList * Cells;
	vtkDoubleArray * Forces;

	vtkUnstructuredGrid * InputGrid;
	vtkUnstructuredGrid * OutputGrid;
	vtkFeMesh * FeMesh;

	void Update();
}

using namespace std;
using namespace vtkFeMeshNS;

//!FeMesh Interface Test. A contact is performed in a cell and the BMM response is evaluated.

void vtkFeMeshNS::Update()
{
	FeMesh->SetExternalForces(Cells, Forces);
	FeMesh->Update();

	OutputGrid = FeMesh->GetOutput();
	OutputGrid->Update();
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
				vtkFeMeshNS::Update();

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

int vtkFeMeshTest(int ac, char* av[])
{
	vtkUnstructuredGridReader * reader = vtkUnstructuredGridReader::New();
	if(ac < 1) reader->SetFileName("Scenario/liverTetra.vtk");
	else reader->SetFileName(av[1]);
	reader->Update();
	
	InputGrid = reader->GetOutput();

	vtkExtractEdges * edges = vtkExtractEdges::New();
	edges->SetInput(InputGrid);
	edges->Update();

 	FeMesh = vtkFeMesh::New();
	FeMesh->SetInput(InputGrid);
	
	FeMesh->Update();
	OutputGrid = FeMesh->GetOutput();

	Cells = vtkIdList::New();
	Cells->InsertNextId(15);

	Forces = vtkDoubleArray::New();
	Forces->SetNumberOfComponents(3);
	Forces->InsertNextValue(0);
	Forces->InsertNextValue(0);
	Forces->InsertNextValue(1);

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
	sphere2->SetRadius(0.01);
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

	tid = iren->CreateRepeatingTimer(500);
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
