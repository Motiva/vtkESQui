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
#include "vtkUnstructuredGridWriter.h"
#include "vtkXMLUnstructuredGridReader.h"

#include "vtkTimerLog.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"

#include "vtkRobustDeformationModel.h"

int main(int argc, char * argv[])
{
	const char * filename = "/home/jorge/Workspace/data/vtkESQuiData/Scenario/Organs/sphere.vtu";

	if (argc > 1)
	{
		filename = argv[1];
	}

	vtkXMLUnstructuredGridReader * reader = vtkXMLUnstructuredGridReader::New();
	reader->SetFileName(filename);
	reader->Update();

	vtkUnstructuredGrid * mesh = reader->GetOutput();

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

	if (mesh->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE) {
		cout << "Error, mesh vtkUnstructuredGrid uses wrong data type, VTK_DOUBLE     is required!" << endl;
		exit(0);
		}

	vtkRobustDeformationModel* rdm = vtkRobustDeformationModel::New();
	rdm->SetInput(mesh);
	rdm->SetDamping(2);//Friction
	rdm->SetDistanceForceCoefficient(500);
	rdm->SetSurfaceForceCoefficient(0);
	rdm->SetVolumeForceCoefficient(100);
	rdm->Init();

	vtkIdList * idList = vtkIdList::New();
	vtkDoubleArray * forces = vtkDoubleArray::New();
	forces->SetNumberOfComponents(3);

	vtkPoints * points = mesh->GetPoints();
	vtkIdList * cells = vtkIdList::New();

	vtkPointLocator * locator = vtkPointLocator::New();
	double bounds[6];
	mesh->GetBounds(bounds);

	std::cout << "[Test Bounds]: X(" << bounds[0] << "," << bounds[1] << "), Y(" << bounds[2] << "," << bounds[3] << "), Z(" << bounds[4] << "," << bounds[5] << ")\n";
	double point[3] = {(bounds[0]+(bounds[1]-bounds[0])/2),
	                       (bounds[2]+(bounds[3]-bounds[2])/2),
	                        bounds[5]};
	                       //(bounds[4]+(bounds[5]-bounds[4])/2)};

	double force[3] = {0,0,0.1};

	locator->SetDataSet(mesh);

	vtkIdType id = locator->FindClosestPoint(point);

	for(vtkIdType pointId=id; pointId<id+1; pointId++)
	{
		mesh->GetPointCells(pointId,cells);
		double * point = points->GetPoint(pointId);
		std::cout << "[vtkRobustModelDeformation] Inserted contact\nPoint["<< pointId << "]: " << point[0] << " | "     << point[1] << " | " << point[2] << "\n";
		idList->InsertNextId(pointId);
		forces->InsertNextTuple(force);
	}

	//Set a fictional force
	rdm->SetContacts(idList, forces);

	vtkTimerLog * timer = vtkTimerLog::New();
	timer->StartTimer();
	rdm->Update();
	timer->StopTimer();

	vtkUnstructuredGrid *ogrid = rdm->GetOutput();
	std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";


	ogrid->Update();
	std::cout << "[Test] Output grid #points: " << ogrid->GetNumberOfPoints() << "\n";
	std::cout << "[Test] Output grid #cells: " << ogrid->GetNumberOfCells() << "\n";

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
	actor->GetProperty()->SetColor(1,1,0);
	actor->GetProperty()->SetRepresentationToWireframe();
	actor->GetProperty()->SetOpacity(0.8);

	vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	mapper2->SetInput(ogrid);
	mapper2->ScalarVisibilityOff();

	vtkActor * actor2 = vtkActor::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	actor2->GetProperty()->SetOpacity(1);

	renderer->AddActor(actor2);

	iren->Initialize();

	renWin->Render();

	iren->Start();


	rdm->Delete();
	mapper->Delete();
	actor->Delete();
	mapper2->Delete();
	actor2->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}
