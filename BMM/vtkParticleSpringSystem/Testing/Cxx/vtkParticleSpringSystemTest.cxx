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
#include "vtkCamera.h"

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkTimerLog.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"
#include "vtkPointPlotter.h"

#include "vtkCommand.h"

#include "vtkParticleSpringSystem.h"


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
				this->BMM->Modified();
				this->BMM->Update();
				timer->StopTimer();

				//std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";

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

	void SetBMM(vtkParticleSpringSystem * bmm)
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

	vtkParticleSpringSystem * BMM;
};

int main(int argc, char * argv[])
{
	const char * filename = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/sphere12_12_1.vtp";

	if (argc > 1)
	{
		filename = argv[1];
	}

	vtkXMLPolyDataReader * reader = vtkXMLPolyDataReader::New();
	reader->SetFileName(filename);
	reader->Update();

	vtkPolyData * mesh = reader->GetOutput();

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

	vtkParticleSpringSystem* ParticleSpringSystem = vtkParticleSpringSystem::New();
	ParticleSpringSystem->SetInput(mesh);
	ParticleSpringSystem->SetSolverType(vtkParticleSpringSystem::RungeKutta4);
	ParticleSpringSystem->SetSpringCoefficient(200);
	ParticleSpringSystem->SetDistanceCoefficient(10);
	ParticleSpringSystem->SetDampingCoefficient(1.5);//Friction
	ParticleSpringSystem->SetMass(0.1);
	ParticleSpringSystem->SetDeltaT(0.001);
	ParticleSpringSystem->SetRigidityFactor(2);
	ParticleSpringSystem->Init();

	vtkRenderer * renderer = vtkRenderer::New();

	//Point plotter
	vtkPointPlotter * plotter = vtkPointPlotter::New();
	plotter->SetRadius(0.025);
	plotter->SetResolution(16);
	plotter->SetRenderer(renderer);
	plotter->Init();

	//Locate contact points
	vtkPointLocator * locator = vtkPointLocator::New();
	double bounds[6];
	mesh->GetBounds(bounds);

	double p[3] = {bounds[1], bounds[3], bounds[5]};

	locator->SetDataSet(mesh);

	vtkIdList * list = vtkIdList::New();
	vtkDoubleArray * directions = vtkDoubleArray::New();
	directions->SetNumberOfComponents(3);

	//list->InsertNextId(0);
	locator->FindClosestNPoints(3, p, list);

	//Set Contact
	double dir[3];
	dir[0] = 0;//-0.1;
	dir[1] = -0.2;
	dir[2] = 0;//0.05;

	for(vtkIdType i = 0; i< list->GetNumberOfIds(); i++)
	{
		double * mp = mesh->GetPoint(list->GetId(i));

		plotter->InsertPoint(mp[0], mp[1], mp[2], 0, 128, 64);
		directions->InsertNextTuple(dir);
	}

	plotter->Update();

	//Set a fictional force
	ParticleSpringSystem->SetContacts(list, directions);
	ParticleSpringSystem->Print(cout);

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
	actor->GetProperty()->SetColor(0,1,0);
	//actor->GetProperty()->SetRepresentationToWireframe();
	//actor->GetProperty()->SetOpacity(0.9);

	vtkDataSetMapper * mapper2 = vtkDataSetMapper::New();
	mapper2->SetInput(ParticleSpringSystem->GetOutput());
	mapper2->ScalarVisibilityOff();

	vtkActor * actor2 = vtkActor::New();
	actor2->SetMapper(mapper2);
	actor2->GetProperty()->SetColor(1,0,0);
	actor2->GetProperty()->SetRepresentationToWireframe();


	//renderer->AddActor(actor);
	renderer->AddActor(actor2);
	renderer->SetBackground(1,1,1);

	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(90);
	iren->Initialize();

	renWin->Render();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetBMM(ParticleSpringSystem);

	//Create a faster timer for BMM update
	tid = iren->CreateRepeatingTimer(10);
	cb->SetFasterTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 25 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(40);
	cb->SetRenderTimerId(tid);

	iren->Start();

	ParticleSpringSystem->Delete();
	mapper->Delete();
	actor->Delete();
	mapper2->Delete();
	actor2->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	return 0;
}

