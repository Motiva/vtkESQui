#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkActor.h"
#include "vtkProperty.h"

#include "vtkSphereSource.h"
#include "vtkPlaneSource.h"
#include "vtkSphere.h"
#include "vtkGlyph3D.h"
#include "vtkDelaunay3D.h"
#include "vtkPolyDataNormals.h"

#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"

#include "vtkSTLReader.h"

#include "vtkContourFilter.h"
#include "vtkGeometryFilter.h"

#include <iostream>

int vtkTetraMeshSurfaceTest(int ac, char* av[])
{
	vtkUnstructuredGridReader * reader = vtkUnstructuredGridReader::New();
	reader->SetFileName("../data/tetraMesh.vtk");
	reader->Update();
	
	vtkUnstructuredGrid * data = reader->GetOutput();
	
	std::cout << "mesh #points: " << data->GetNumberOfPoints() << "\n";
	std::cout << "mesh #cells: " << data->GetNumberOfCells() << "\n";

	//vtkContourFilter * filter = vtkContourFilter::New();
	//filter->SetInput(data);
	vtkGeometryFilter * filter = vtkGeometryFilter::New();
	filter->SetInput(data);
	filter->Update();

	vtkPolyData * pd = filter->GetOutput();
	std::cout << "surface #points: " << pd->GetNumberOfPoints() << "\n";
	std::cout << "surface #cells: " << pd->GetNumberOfCells() << "\n";

	vtkRenderer * renderer = vtkRenderer::New();
	vtkRenderWindow * renWin = vtkRenderWindow::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(0.05);
	sphere->SetThetaResolution(8); sphere->SetPhiResolution(8);

	/*vtkPolyDataNormals * normals = vtkPolyDataNormals::New();
	normals->SetInput(data);
	normals->FlipNormalsOn();
	normals->Update();
*/

	vtkGlyph3D *spheres = vtkGlyph3D::New();
	spheres->SetInput(pd);
	spheres->SetSource(sphere->GetOutput());
	spheres->SetVectorModeToUseNormal();

	vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(spheres->GetOutputPort());
	//mapper->SetInput(data);
	mapper->GlobalImmediateModeRenderingOn();	
	
	vtkActor * meshActor = vtkActor::New();
	meshActor->SetMapper(mapper);
	meshActor->GetProperty()->SetColor(1,1,0);
	renderer->AddActor(meshActor);

	//vtkGlyph3D * spheres2 = vtkGlyph3D::New();
	//spheres2->SetInput(ogrid);
	//spheres2->SetSource(sphere->GetOutput());

	//vtkPolyDataMapper * mapper2 = vtkPolyDataMapper::New();
	//mapper2->SetInput(pd);
	vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	mapper2->SetInput(data);
	mapper2->ImmediateModeRenderingOn();

	vtkActor * meshActor2 = vtkActor::New();
	meshActor2->SetMapper(mapper2);
	meshActor2->GetProperty()->SetColor(0,1,1);
	renderer->AddActor(meshActor2);
	
	renWin->Render();  
	
  iren->Start();

  reader->Delete();
	sphere->Delete();
	spheres->Delete();
	meshActor->Delete();
	meshActor2->Delete();
	mapper->Delete();
	mapper2->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
  return 0;
}
