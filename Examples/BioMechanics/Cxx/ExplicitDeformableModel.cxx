#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkIdList.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkSphereSource.h"

#include "vtkTimerLog.h"
#include "vtkPointLocator.h"

#include "vtkCommand.h"

#include "vtkEDMInterface.h"

class vtkEDMTimerCallback : public vtkCommand
{
public:
	static vtkEDMTimerCallback *New()
	{
		vtkEDMTimerCallback *cb = new vtkEDMTimerCallback;
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
				vtkPolyData * mesh = vtkPolyData::SafeDownCast(this->DeformationModel->GetInput());

				//Locate collision points
					vtkPointLocator * locator = vtkPointLocator::New();
					double bounds[6];
					mesh->GetBounds(bounds);

					double p[3] = {bounds[0], 0, 0};

					vtkIdList * list = vtkIdList::New();
					locator->SetDataSet(mesh);
					locator->FindClosestNPoints(5, p, list);

					//Set Collisions
					double force[3];
					force[0] = 0.1;
					force[1] = 0.05;
					force[2] = 0;

					for(vtkIdType i = 0; i<list->GetNumberOfIds(); i++)
					{
						int id = list->GetId(i);
						this->DeformationModel->AddDisplacement(id, force);
					}

					cout << "Force applied...\n";
			}
			else if (tid == this->FasterTimerId)
			{
				vtkTimerLog * timer = vtkTimerLog::New();
				timer->StartTimer();
				this->DeformationModel->Modified();
				this->DeformationModel->Update();
				timer->StopTimer();

				std::cout << "[Test] Rate: " << 1/(timer->GetElapsedTime()) << "\n";
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

	void SetDeformationModel(vtkEDMInterface * DeformationModel)
	{
		this->DeformationModel = DeformationModel;
	}

	void SetCollisionIds(vtkIdList * list)
	{
		this->List = list;
	}
private:
	int FastTimerId;
	int RenderTimerId;
	int FasterTimerId;

	vtkIdList * List;

	vtkEDMInterface * DeformationModel;
};

int main(int argc, char * argv[])
{
	const char * filename = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/sphere12_12_1.vtp";

	if (argc > 1)
	{
		filename = argv[1];
	}

	double r = 1;
	vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(r);
	sphereSource->SetPhiResolution(16);
	sphereSource->SetThetaResolution(16);
	vtkSmartPointer<vtkPolyData> mesh = sphereSource->GetOutput();
	sphereSource->Update();

	double s = r/30;
	double warp = s/100000;

	std::cout << "[Test] Input grid #points: " << mesh->GetNumberOfPoints() << "\n";
	std::cout << "[Test] Input grid #cells: " << mesh->GetNumberOfCells() << "\n";
	std::cout << "[Test] Image spacing: " << s << "\n";
	std::cout << "[Test] Image warp scale: " << warp << "\n";

	vtkSmartPointer<vtkEDMInterface> EDM = vtkSmartPointer<vtkEDMInterface>::New();
	EDM->SetNumberOfIterations(500);
	EDM->SetWarpScaleFactor(warp);
	EDM->SetImageSpacing(s);
	EDM->SetInput(mesh);
	EDM->Init();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(mesh);
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetRepresentationToSurface();
	actor->GetProperty()->SetColor(0.5,1,0);
	actor->GetProperty()->SetOpacity(.3);

	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInput(EDM->GetOutput());
	mapper2->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	actor2->GetProperty()->SetRepresentationToWireframe();

	renderer->AddActor(actor);
	renderer->AddActor(actor2);
	//renderer->AddActor(EDM->GetActor());
	renderer->SetBackground(1,1,1);

	renderer->ResetCamera();
	iren->Initialize();

	renWin->Render();

	// Sign up to receive TimerEvent:
	//
	vtkSmartPointer<vtkEDMTimerCallback> cb = vtkSmartPointer<vtkEDMTimerCallback>::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetDeformationModel(EDM);

	//Create a faster timer for DeformationModel update
	tid = iren->CreateRepeatingTimer(5);
	cb->SetFasterTimerId(tid);

	//Create a collision every 5 seconds
	tid = iren->CreateRepeatingTimer(5000);
	cb->SetFastTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 25 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(40);
	cb->SetRenderTimerId(tid);

	iren->Start();

	return 0;
}

