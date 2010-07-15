#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataWriter.h"
#include "vtkDataSetMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkIdList.h"
#include "vtkActor.h"
#include "vtkProperty.h"

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkTimerLog.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"

#include "vtkDataSetSurfaceFilter.h"

int main(int argc, char * argv[])
{
	const char * filename = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/sphere12_12_1.vtp";

	if (argc > 1)
	{
		filename = argv[1];
	}

	vtkXMLPolyDataReader * reader = vtkXMLPolyDataReader::New();
	reader->SetFileName(filename);
	reader->Update();

	vtkPolyData * mesh = reader->GetOutput();

	std::cout << "[Test] Input grid #points: " << mesh->GetNumberOfPoints() << "\n";
	std::cout << "[Test] Input grid #cells: " << mesh->GetNumberOfCells() << "\n";

	//Float to Double Conversion
	if(mesh->GetPoints()->GetDataType() != VTK_DOUBLE)
	{
		vtkPoints * ps = vtkPoints::New();
		ps->SetDataTypeToDouble();
		ps->DeepCopy(mesh->GetPoints());
		mesh->SetPoints(ps);
		cout << "DataType has been modified\n";
	}

	mesh->Update();

	vtkIdList * cells = vtkIdList::New();
	for (int i=0; i<mesh->GetNumberOfPoints(); i++)
	{
		mesh->GetPointCells(i, cells);
		std::cout << i<< " cells[" << cells->GetNumberOfIds() << "]"<< std::endl;
	}

	vtkDataSetSurfaceFilter * surfaceFilter = vtkDataSetSurfaceFilter::New();
	surfaceFilter->SetInput(mesh);
	surfaceFilter->Update();
	vtkPolyData * surface = surfaceFilter->GetOutput();

	//vtkIdList * cells = vtkIdList::New();
	for (int i=0; i<surface->GetNumberOfPoints(); i++)
	{
		surface->GetPointCells(i, cells);
		std::cout << i << " s cells[" << cells->GetNumberOfIds() << "]"<< std::endl;
	}


	vtkRenderer * renderer = vtkRenderer::New();

	vtkRenderWindow * renWin = vtkRenderWindow::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkDataSetMapper * mapper = vtkDataSetMapper::New();
	mapper->SetInput(mesh);
	mapper->ScalarVisibilityOff();

	vtkActor * actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0,1,0);

	renderer->AddActor(actor);
	renderer->SetBackground(1,1,1);

	iren->Initialize();

	renWin->Render();

	iren->Start();

	mapper->Delete();
	actor->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}

