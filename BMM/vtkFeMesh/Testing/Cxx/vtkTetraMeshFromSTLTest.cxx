#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataNormals.h"

#include "vtkSTLReader.h"
#include "vtkDelaunay3D.h"

#include <iostream>

int vtkTetraMeshFromSTLTest(int ac, char* av[])
{
	
	vtkSTLReader * stl = vtkSTLReader::New();
  stl->SetFileName("../data/liver.stl");
  stl->Update();
  
	vtkPolyData * pd = stl->GetOutput();

	vtkPolyDataNormals *normals = vtkPolyDataNormals::New  ();
	normals->SetInput(pd);
	normals->SetFeatureAngle(60.0);
	normals->FlipNormalsOff();
	normals->Update();

  vtkPolyDataWriter * pdWriter = vtkPolyDataWriter::New();
  pdWriter->SetFileName("../data/liver.vtk");
  pdWriter->SetInput(normals->GetOutput());
  pdWriter->Update();

  vtkDelaunay3D * del = vtkDelaunay3D::New();
  del->SetInput(stl->GetOutput());
  del->Update();

  vtkUnstructuredGrid * grid = del->GetOutput();
  
  vtkUnstructuredGridWriter * ugWriter = vtkUnstructuredGridWriter::New();
  ugWriter->SetInput(grid);
  ugWriter->SetFileName("../data/liverTetra.vtk");
  ugWriter->Update();

  //-----------------------------------------------------

  return 0;
}
