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
#include "vtkTriangleFilter.h"
#include "vtkPolyDataNormals.h"

#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"

#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkAppendPolyData.h"

#include <iostream>

int vtkTetraMeshTest(int ac, char* av[])
{
	vtkPolyDataReader * reader = vtkPolyDataReader::New();
	reader->SetFileName("../data/tetraMesh.vtk");
	reader->Update();
	
	//vtkPolyData * data = reader->GetOutput();
	//data->Update();
	vtkPlaneSource * plane = vtkPlaneSource::New();
	plane->SetResolution(10,10);
	vtkTriangleFilter * triangle = vtkTriangleFilter::New();
	triangle->PassVertsOn();
	triangle->PassLinesOn();
	triangle->SetInput(plane->GetOutput());
	triangle->Update();
	plane->Delete();


	//vtkPolyData * data = reader->GetOutput();
	vtkPolyData * data = triangle->GetOutput();
	data->Update();

	vtkPolyDataWriter * writer = vtkPolyDataWriter::New();
	writer->SetFileName("../data/planegrid.vtk");
	writer->SetInput(data);
	writer->Update();

	std::cout << "grid #points: " << data->GetNumberOfPoints() << "\n";
	std::cout << "grid #cells: " << data->GetNumberOfCells() << "\n";
	
	vtkPolyData * pd = vtkPolyData::New();
	pd->DeepCopy(data);
	
	vtkTransform * transform = vtkTransform::New();
 	transform->Scale(0.9,0.9,0.9);
	transform->Translate(0,0,-0.1);  

  vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
  filter->SetInput(pd);
  filter->SetTransform(transform);
  filter->Update();
  vtkPolyData * outputData = filter->GetOutput();

  vtkAppendPolyData * appendFilter = vtkAppendPolyData::New();
  appendFilter->AddInput(data);
  appendFilter->AddInput(outputData);
  appendFilter->Update();
  vtkPolyData * surface = appendFilter->GetOutput();
  
  vtkDelaunay3D * delaunay = vtkDelaunay3D::New();
  delaunay->SetInput(surface);
  delaunay->Update();
  vtkUnstructuredGrid * ogrid = delaunay->GetOutput();
	ogrid->BuildLinks();

	std::cout << "ogrid #points: " << ogrid->GetNumberOfPoints() << "\n";
	std::cout << "ogrid #cells: " << ogrid->GetNumberOfCells() << "\n";

	vtkRenderer * renderer = vtkRenderer::New();
	vtkRenderWindow * renWin = vtkRenderWindow::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(0.003);
	sphere->SetThetaResolution(8); sphere->SetPhiResolution(8);

	/*vtkPolyDataNormals * normals = vtkPolyDataNormals::New();
	normals->SetInput(data);
	normals->FlipNormalsOn();
	normals->Update();
*/

	vtkGlyph3D *spheres = vtkGlyph3D::New();
	spheres->SetInput(data);
	spheres->SetSource(sphere->GetOutput());
	spheres->SetVectorModeToUseNormal();

	vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(spheres->GetOutputPort());
	//mapper->SetInput(data);
	mapper->GlobalImmediateModeRenderingOn();	
	
	vtkActor * meshActor = vtkActor::New();
	meshActor->SetMapper(mapper);
	meshActor->GetProperty()->SetColor(1,0,0);
	renderer->AddActor(meshActor);

	vtkGlyph3D * spheres2 = vtkGlyph3D::New();
	spheres2->SetInput(ogrid);
	spheres2->SetSource(sphere->GetOutput());

	vtkPolyDataMapper * mapper2 = vtkPolyDataMapper::New();
	mapper2->SetInputConnection(spheres2->GetOutputPort());
	//vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	//mapper2->SetInput(ogrid);
	mapper2->ImmediateModeRenderingOn();

	vtkActor * meshActor2 = vtkActor::New();
	meshActor2->SetMapper(mapper2);
	meshActor2->GetProperty()->SetColor(0,0,1);
	//meshActor2->GetProperty()->SetRepresentationToWireframe();
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
