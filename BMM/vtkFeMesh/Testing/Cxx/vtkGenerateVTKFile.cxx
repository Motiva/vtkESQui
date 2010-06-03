/*=========================================================================
This file is used to generate VTK file containing a tetrahedral mesh
=========================================================================*/
#include "vtkRectilinearGrid.h"
#include "vtkRectilinearGridToPolyDataFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkRectilinearGridToTetrahedra.h"

#include "vtkPolyData.h"
#include "vtkPlaneSource.h"
#include "vtkTriangleFilter.h"
#include "vtkDelaunay2D.h"
#include "vtkDelaunay3D.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"

int vtkGenerateVTKFile(int ac, char* av[])
{
	//
	// A Plane box mesh (10x10x2) is generated using vtkRectilinearGridToTetrahedra
	//
	int ch;
	std::string fname;	

	std::cerr << "Please select a mesh type cell: \n(2) Tetra (Plane)\n(3) Triangle\n(4) Tetra\n";
	std::cin >> ch;
	std::cout << std::endl;
	
	vtkUnstructuredGrid * mesh = vtkUnstructuredGrid::New();
	
	if(ch == 4)
	{
		std::cerr << "Tetrahedral Mesh have been chosen\n\n";
		fname = "../data/tetragrid.vtk";
		vtkRectilinearGridToTetrahedra * g2t = vtkRectilinearGridToTetrahedra::New();
  	g2t->SetInput(1,1,0.1,0.1,0.1,0.1,0.0001);
  	g2t->SetTetraPerCellTo5();
  	g2t->RememberVoxelIdOn();
  	g2t->Update();
  	
		mesh->DeepCopy(g2t->GetOutput());
	}
	else if(ch == 3)
	{
		std::cerr << "Triangular Mesh have been chosen\n\n";
		fname = "../data/trianglegrid.vtk";
		vtkPlaneSource * plane = vtkPlaneSource::New();
		plane->SetResolution(10,10);

		/*vtkTriangleFilter * filter = vtkTriangleFilter::New();
		filter->SetInputConnection(plane->GetOutputPort());
		filter->Update();*/
		vtkDelaunay2D * delaunay = vtkDelaunay2D::New();
		delaunay->SetInput(plane->GetOutput());
		delaunay->Update();
		plane->Delete();
	
		std::cout << "c: " <<  delaunay->GetOutput()->GetNumberOfCells() << "\n";
	
 		mesh->SetPoints(delaunay->GetOutput()->GetPoints());
 		mesh->SetCells(VTK_TRIANGLE, delaunay->GetOutput()->GetPolys());
		mesh->BuildLinks();
	}
	else if (ch == 2)
	{
		std::cerr << "Tetrahedral Mesh have been chosen\n\n";
		fname = "../data/tetragrid2.vtk";
		vtkRectilinearGridToTetrahedra * g2t = vtkRectilinearGridToTetrahedra::New();
  	g2t->SetInput(1,1,0.1,0.2,0.2,0.1,0.0001);
  	g2t->SetTetraPerCellTo5();
  	g2t->RememberVoxelIdOn();
  	g2t->Update();
  	
		mesh->DeepCopy(g2t->GetOutput());
		mesh->BuildLinks();

	}
	else
	{
		std::cerr << "You should choose a number of the list showed above\n";
		exit(-1);
	}

	vtkPoints * points = mesh->GetPoints();
	vtkFloatArray * scalars = vtkFloatArray::New();
	scalars->SetNumberOfTuples(points->GetNumberOfPoints());	

	for(int i=1; i<points->GetNumberOfPoints();i++)
	{
		scalars->InsertValue(i, 1);
	}
	
	mesh->GetPointData()->SetScalars(scalars);
  mesh->Update();

	//Writing Unstructured grid in a vtk file
  vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
  writer->SetInput(mesh);
  writer->SetFileName(fname.c_str());
  writer->Write();

  std::cout << "---- VTK Mesh File Generated----" << std::endl;
  std::cout << "# Points:" << mesh->GetNumberOfPoints() << std::endl;
  std::cout << "# Cells:" << mesh->GetNumberOfCells() << std::endl;
  std::cout << "--------------------------------" << std::endl;

  return 0;
}
