#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataWriter.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkIdList.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkSphereSource.h"

#include "vtkTimerLog.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"

#include "vtkContact.h"
#include "vtkContactCollection.h"

#include "vtkCommand.h"

#include "vtkEDMInterface.h"


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

			}
			else if (tid == this->FasterTimerId)
			{
				vtkTimerLog * timer = vtkTimerLog::New();
				timer->StartTimer();
				int n = this->DeformationModel->GetNumberOfIterations();
				this->DeformationModel->SetNumberOfIterations(n+1);
				//this->DeformationModel->Modified();
				this->DeformationModel->Update();
				timer->StopTimer();

				std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";

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

	void SetContactIds(vtkIdList * list)
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

	//vtkXMLPolyDataReader * reader = vtkXMLPolyDataReader::New();
	//reader->SetFileName(filename);
	//reader->Update();

	//vtkPolyData * mesh = reader->GetOutput();

	vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(20);
	sphereSource->SetPhiResolution(20);
	sphereSource->SetThetaResolution(20);
	vtkSmartPointer<vtkPolyData> mesh = sphereSource->GetOutput();
	sphereSource->Update();

	std::cout << "[Test] Input grid #points: " << mesh->GetNumberOfPoints() << "\n";
	std::cout << "[Test] Input grid #cells: " << mesh->GetNumberOfCells() << "\n";

	vtkEDMInterface* EDM = vtkEDMInterface::New();
	EDM->SetInput(mesh);
	EDM->Init();

	vtkRenderer * renderer = vtkRenderer::New();

	//Locate contact points
	vtkPointLocator * locator = vtkPointLocator::New();
	double bounds[6];
	mesh->GetBounds(bounds);

	double p[3] = {bounds[0], 0, 0};

	locator->SetDataSet(mesh);

	vtkIdList * list = vtkIdList::New();
	vtkDoubleArray * directions = vtkDoubleArray::New();
	directions->SetNumberOfComponents(3);

	//list->InsertNextId(0);
	locator->FindClosestNPoints(10, p, list);

	//Set Contacts
	vtkContactCollection * contacts = vtkContactCollection::New();

	double dir[3];
	dir[0] = 5;//-0.1;
	dir[1] = 0.1;
	dir[2] = 0;//0.05;

	for(vtkIdType i = 0; i< list->GetNumberOfIds(); i++)
	{
		double * point = mesh->GetPoint(list->GetId(i));
		directions->InsertNextTuple(dir);
		vtkIdType id = list->GetId(i);

		//Insert contact info
		vtkContact * contact = vtkContact::New();
		contact->SetItemId(0, 0);
		contact->SetItemId(1, 0);

		//Organ cell point
		contact->SetPointId(1, id);
		contact->SetPoint(1, point);
		//contact->InsertCellId(0, organCellId);
		contact->SetDisplacement(dir);

		contact->Print(cout);

		contacts->InsertNextContact(contact);
	}

	//Set a fictional force
	EDM->InsertContacts(contacts);

	vtkRenderWindow * renWin = vtkRenderWindow::New();
	renWin->SetSize(500,500);
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
	mapper->SetInput(mesh);
	mapper->ScalarVisibilityOff();

	vtkActor * actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0,1,0);
	actor->GetProperty()->SetOpacity(0.3);

	vtkPolyDataMapper * mapper2 = vtkPolyDataMapper::New();
	mapper2->SetInput(EDM->GetOutput());
	mapper2->ScalarVisibilityOff();

	vtkActor * actor2 = vtkActor::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	actor2->GetProperty()->SetRepresentationToWireframe();

	renderer->AddActor(actor);
	renderer->AddActor(actor2);
	renderer->SetBackground(1,1,1);

	renderer->ResetCamera();
	//renderer->GetActiveCamera()->Azimuth(90);
	iren->Initialize();

	renWin->Render();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetDeformationModel(EDM);

	//Create a faster timer for DeformationModel update
	tid = iren->CreateRepeatingTimer(10);
	cb->SetFasterTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 25 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(25);
	cb->SetRenderTimerId(tid);

	iren->Start();

	EDM->Delete();
	mapper->Delete();
	actor->Delete();
	mapper2->Delete();
	actor2->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}

