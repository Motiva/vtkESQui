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

#include "vtkUnstructuredGrid.h"
#include "vtkXMLUnstructuredGridReader.h"

#include "vtkTimerLog.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"

#include "vtkMSS.h"

int main(int argc, char * argv[])
{
	const char * filename = "/home/jorge/Workspace/data/vtkESQuiData/Scenario/Organs/liver.vtu";

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
	vtkPoints * ps = vtkPoints::New();
	ps->SetDataTypeToDouble();
	ps->DeepCopy(mesh->GetPoints());

	if (ps->GetData()->GetDataType() != VTK_DOUBLE) {
		cout << "Error, vtkUnstructuredGrid uses wrong data type, VTK_DOUBLE     is required!" << endl;
		exit(0);
	}

	mesh->SetPoints(ps);
	mesh->Update();

	if (mesh->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE) {
		cout << "Error, mesh vtkUnstructuredGrid uses wrong data type, VTK_DOUBLE     is required!" << endl;
		exit(0);
	}
	vtkMSS* mss = vtkMSS::New();
	mss->SetInput(mesh);
	mss->SetDistanceCoefficient(200);
	mss->SetDampingCoefficient(2);//Friction
	mss->SetMass(0.1);
	mss->SetDeltaT(0.001);
	mss->SetSteps(10);

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

	double direction[3] = {0,-0.1,-0.2};

	locator->SetDataSet(mesh);  

	vtkIdType id = locator->FindClosestPoint(point);

	for(vtkIdType pointId=id; pointId<id+2;pointId++)
	{
		mesh->GetPointCells(pointId,cells);
		double * point = points->GetPoint(pointId);
		idList->InsertNextId(pointId);
		std::cout << "[vtkFeMeshInterface] contact Point: " << point[0] << " | "     << point[1] << " | " << point[2] << "\n";

		forces->InsertNextTuple(direction);
	}

	mss->SetContacts(idList, forces);

	vtkTimerLog * timer = vtkTimerLog::New();
	timer->StartTimer();
	mss->Update();
	timer->StopTimer();

	std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";

	vtkUnstructuredGrid *ogrid = mss->GetOutput();

	std::cout << "[Test] Output grid #points: " << ogrid->GetNumberOfPoints() << "\n";
	std::cout << "[Test] Output grid #cells: " << ogrid->GetNumberOfCells() << "\n";

	/*vtkUnstructuredGridWriter * writer = vtkUnstructuredGridWriter::New();
	writer->SetFileName("../data/in.vtk");
	writer->SetInput(mesh);
	writer->Update();

	writer->SetFileName("../data/out.vtk");
	writer->SetInput(ogrid);
	writer->Update();*/

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
	actor->GetProperty()->SetRepresentationToWireframe();
	actor->GetProperty()->SetColor(1,1,1);

	vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	mapper2->SetInput(ogrid);
	mapper2->ScalarVisibilityOff();

	vtkActor * actor2 = vtkActor::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	renderer->AddActor(actor2);

	iren->Initialize();

	renWin->Render();

	iren->Start();


	mss->Delete();
	mapper->Delete();
	actor->Delete();
	mapper2->Delete();
	actor2->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}
