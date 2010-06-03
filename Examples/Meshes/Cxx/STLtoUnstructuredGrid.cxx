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

const char * ExpandDataFileName(const char * fname)
{
	//const char * path ="C:/Workspace/data/vtkESQuiData";
	const char * path ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes";
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

int main(int ac, char* av[])
{
	
	vtkSTLReader * stl = vtkSTLReader::New();
	stl->SetFileName(ExpandDataFileName("sphere2.stl"));
	stl->Update();

	vtkPolyData * pd = stl->GetOutput();

	vtkPolyDataNormals *normals = vtkPolyDataNormals::New  ();
	normals->SetInput(pd);
	normals->SetFeatureAngle(60.0);
	normals->FlipNormalsOff();
	normals->Update();

	vtkPolyDataWriter * pdWriter = vtkPolyDataWriter::New();
	pdWriter->SetFileName(ExpandDataFileName("sphere2.vtk"));
	pdWriter->SetInput(normals->GetOutput());
	pdWriter->Update();

	vtkDelaunay3D * del = vtkDelaunay3D::New();
	del->SetInput(stl->GetOutput());
	del->Update();

	vtkUnstructuredGrid * grid = del->GetOutput();

	vtkUnstructuredGridWriter * ugWriter = vtkUnstructuredGridWriter::New();
	ugWriter->SetInput(grid);
	ugWriter->SetFileName(ExpandDataFileName("sphere2Tetra.vtk"));
	ugWriter->Update();

	return 0;
}
