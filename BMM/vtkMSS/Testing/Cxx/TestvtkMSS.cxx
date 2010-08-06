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

#include "vtkCommand.h"

#include "vtkMSS.h"


class vtkTimerCallback : public vtkCommand
{
public:
	static vtkTimerCallback *New()
	{
		vtkTimerCallback *cb = new vtkTimerCallback;
		cb->FastTimerId = 0;
		cb->FasterTimerId = 0;
		cb->RenderTimerId = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			int tid = * static_cast<int *>(callData);

			if (tid == this->FastTimerId)
			{
				//Update RDM
				vtkUnstructuredGrid * mesh = vtkUnstructuredGrid::SafeDownCast(this->BMM->GetInput());
				vtkIdList * idList = vtkIdList::New();
				vtkDoubleArray * forces = vtkDoubleArray::New();
				forces->SetNumberOfComponents(3);

				//vtkPoints * points = mesh->GetPoints();
				vtkIdList * cells = vtkIdList::New();
				double force[3] = {0,0,-0.1};
				cout << "#------------------------------------------------#\n";

				for(vtkIdType i = 0; i<this->List->GetNumberOfIds(); i++)
				{
					vtkIdType pointId = this->List->GetId(i);
					mesh->GetPointCells(pointId,cells);
					//double * point = points->GetPoint(pointId);
					//std::cout << "[vtkRobustModelDeformation] Inserted contact\nPoint["<< pointId << "]: " << point[0] << " | "     << point[1] << " | " << point[2] << "\n";
					idList->InsertNextId(pointId);
					forces->InsertNextTuple(force);
				}

				//Set a fictional force
				this->BMM->SetContacts(idList, forces);

			}
			else if (tid == this->FasterTimerId)
			{
				vtkTimerLog * timer = vtkTimerLog::New();
				timer->StartTimer();
				this->BMM->Update();
				timer->StopTimer();

				//std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";

				//std::cout << "[Test] Output grid #points: " << ogrid->GetNumberOfPoints() << "\n";
				//std::cout << "[Test] Output grid #cells: " << ogrid->GetNumberOfCells() << "\n";

			}
			else if (tid == this->RenderTimerId)
			{
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
	}

	void SetFasterTimerId(int tid)
	{
		this->FasterTimerId = tid;
	}

	void SetRenderTimerId(int tid)
	{
		this->RenderTimerId = tid;
	}

	void SetBMM(vtkMSS * bmm)
	{
		this->BMM = bmm;
	}

	void SetContactIds(vtkIdList * list)
	{
		this->List = list;
	}
private:
	int FastTimerId;
	int RenderTimerId;
	int FasterTimerId;

	vtkIdList * List;

	vtkMSS * BMM;
};

int main(int argc, char * argv[])
{
	const char * filename = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/cuboid.vtu";

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

	vtkMSS* mss = vtkMSS::New();
	mss->SetInput(mesh);
	mss->SetDistanceCoefficient(20);
	mss->SetDampingCoefficient(0.5);//Friction
	mss->SetMass(0.1);
	mss->SetDeltaT(0.001);
	mss->SetSteps(10);

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
	actor->GetProperty()->SetOpacity(0.5);

	vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	mapper2->SetInput(mss->GetOutput());
	mapper2->ScalarVisibilityOff();

	vtkActor * actor2 = vtkActor::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	actor2->GetProperty()->SetOpacity(0.8);

	renderer->AddActor(actor);
	renderer->AddActor(actor2);

	iren->Initialize();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetBMM(mss);

	vtkPointLocator * locator = vtkPointLocator::New();
	double bounds[6];
	mesh->GetBounds(bounds);

	//std::cout << "[Test Bounds]: X(" << bounds[0] << "," << bounds[1] << "), Y(" << bounds[2] << "," << bounds[3] << "), Z(" << bounds[4] << "," << bounds[5] << ")\n";
	double point[3] = {(bounds[0]+bounds[1])/2,
			(bounds[2]+bounds[3])/2,
			bounds[5]};

	locator->SetDataSet(mesh);

	vtkIdList * list = vtkIdList::New();
	locator->FindClosestNPoints(3, point, list);
	cb->SetContactIds(list);

	// Create repeating timer for contact set
	tid = iren->CreateRepeatingTimer(1000);
	cb->SetFastTimerId(tid);

	//Create a faster timer for BMM update
	tid = iren->CreateRepeatingTimer(10);
	cb->SetFasterTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 20 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(50);
	cb->SetRenderTimerId(tid);

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

