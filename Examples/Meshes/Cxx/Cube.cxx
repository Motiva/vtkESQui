#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include "vtkCleanPolyData.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkDelaunay3D.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

int main(int, char *[])
{
	//Create a cube.
	vtkSmartPointer<vtkCubeSource> cubeSource =
			vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->SetCenter(0.0, 0.0, 0.0);
	cubeSource->SetXLength(1);
	cubeSource->SetYLength(1);
	cubeSource->SetZLength(1);
	cubeSource->Update();

	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInput(cubeSource->GetOutput());
	clean->Update();

	//Write the file.
	vtkSmartPointer<vtkXMLPolyDataWriter> writer =
			vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetInputConnection(clean->GetOutputPort());
	writer->SetDataModeToAscii();
	writer->SetFileName("cube.vtp");
	writer->Write();

	vtkSmartPointer<vtkDelaunay3D> del = vtkSmartPointer<vtkDelaunay3D>::New();
	del->SetInput(clean->GetOutput());
	del->SetOffset(5);
	del->Update();

	vtkSmartPointer<vtkXMLUnstructuredGridWriter> gridWriter = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
	gridWriter->SetInputConnection(del->GetOutputPort());
	gridWriter->SetDataModeToAscii();
	gridWriter->SetFileName("cube.vtu");
	gridWriter->Write();

	vtkSmartPointer<vtkDataSetSurfaceFilter> surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surface->SetInput(del->GetOutput());
	surface->Update();

	//vtkUnstructuredGrid * grid = del->GetOutput();
	vtkPolyData * grid = surface->GetOutput();
	vtkPoints * points = grid->GetPoints();

	vtkSmartPointer<vtkIdList> cells = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList> cellPoints = vtkSmartPointer<vtkIdList>::New();
	cout << grid->GetNumberOfPoints() << endl;
	cout << grid->GetNumberOfCells() << endl;

	writer->SetInput(grid);
	writer->SetFileName("cubeSurface.vtp");
	writer->Update();
	int numPoints =grid->GetNumberOfPoints();
	for(int i=0; i<numPoints ;i++)
	{
		cout << "# Point Id: "<< i<<" #\n";
		grid->GetPointCells(i, cells);
		cout << "cells: "<<cells->GetNumberOfIds() << endl;
		for (vtkIdType cellId = 0; cellId < cells->GetNumberOfIds(); cellId++) {
			grid->GetCellPoints(cellId, cellPoints);
			cout << "cellPoints: " << cellPoints->GetNumberOfIds() << endl;

		}
	}

  return EXIT_SUCCESS;
}
