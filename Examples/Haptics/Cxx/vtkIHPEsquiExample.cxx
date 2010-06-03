#include <iostream>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"

#include "vtkPolyDataReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkTexture.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkAxesActor.h"

#include "vtkCylinderSource.h"
#include "vtkCubeSource.h"
#include "vtkProperty.h"
#include "vtkCommand.h"

#include "vtkMath.h"
#include "vtkHaptic.h"
#include "vtkIHP.h"

#include "vtkScenario.h"
#include "vtkToolDummy.h"


namespace IHPEsquiExampleNS{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase
	vtkIHP * Haptic = NULL;

	vtkScenario * Scenario = NULL;
	vtkToolDummy * Tool0 = NULL;
	vtkToolDummy * Tool1 = NULL;
	void UpdateHaptic(vtkIHP * Haptic);
}

using namespace std;
using namespace IHPEsquiExampleNS;

void IHPEsquiExampleNS::UpdateHaptic(vtkIHP * Haptic)
{

	float * state;
	float  toolDepth;

	vtkToolDummy * tool;
	float yaw;
	float pitch;
	float depth;
	float roll;
	float opening;

	//Update  state of the IHP device
	Haptic->UpdateDevice();

	for (int i = 0; i<2; i++)
	{
		//Trocar state
		state = Haptic->GetTrocarState(i);

		//Trocar's direction angles
		yaw = Haptic->GetTrocarYaw(i);
		pitch = Haptic->GetTrocarPitch(i);

		//Tool-in-the-trocar parameters
		depth = Haptic->GetToolDepth(i);
		roll = Haptic->GetToolRoll(i);
		opening = Haptic->GetToolOpening(i);

		if(i == 0)
		{
			tool = Tool0;
		}
		else
		{
			tool = Tool1;
		}

		//Haptic coordinate system is different from VTK system
		// |  Haptic  |  VTK  |
		// |      X      |    Y    |
		// |      Y      |    Z    |
		// |      Z      |    X    |
		tool->Pitch(pitch);
		tool->Yaw(yaw);


		//Set tool's roll angle
		tool->Roll(roll);

		//Set tool's depth
		toolDepth = 6*depth;
		tool->SetDepth(toolDepth);

		//Set tool's opening. Modify the color of the tool according to the opening parameter
		vtkProperty * prop = tool->GetActor(1)->GetProperty();
		prop->SetColor(0, opening, 0);
		tool->GetActor(1)->SetProperty(prop);

		//Display tool buttons/pedal state
		if(Haptic->GetToolButtonState(i)){
			std::cout << "Tool("<<i<< ") Main button is pressed...\n";
		}
		if(Haptic->GetLeftPedalState()){
			std::cout << "Tool("<<i<< ") Left pedal is pressed...\n";
		}
		if(Haptic->GetRightPedalState()){
			std::cout << "Tool("<<i<< ") Right pedal is pressed...\n";
		}

		//tool->Print(cout);

		//Set Tool Feedback Forces
		float force[3];
		force[0] = 0;
		force[1] = 0;
		force[2] = 0;

		if(toolDepth > 3.5)
		{
			//A Horizontal plane is simulated to check haptic feedback force.
			//Z-Component Elastic Force is applied
			force[2] = 2*(toolDepth - 3.5);
			std::cout << "Depth: " << toolDepth << " | Feedback Force: "<< force[2] << "N" << std::endl;
		}
		else force[0] = 0;

		Haptic->SetToolTipForce(i, force);
		Haptic->ApplyForce(i);
	}

	Haptic->FlushForces();

}

class vtkTimerCallback : public vtkCommand
{
public:
	static vtkTimerCallback *New()
	{
		vtkTimerCallback *cb = new vtkTimerCallback;
		cb->FastTimerId = 0;
		cb->FastTimerCount = 0;
		cb->RenderTimerId = 0;
		cb->RenderTimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			int tid = * static_cast<int *>(callData);

			if (tid == this->FastTimerId)
			{
				//Update Haptic Trigger
				UpdateHaptic(Haptic);

				++this->FastTimerCount;
			}
			else if (tid == this->RenderTimerId)
			{
				++this->RenderTimerCount;

				vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
				if (iren && iren->GetRenderWindow() && iren->GetRenderWindow()->GetRenderers())
				{
					Scenario->Update();
					iren->Render();
				}
			}
		}
	}

	void SetFastTimerId(int tid)
	{
		this->FastTimerId = tid;
		this->FastTimerCount = 0;
	}

	void SetRenderTimerId(int tid)
	{
		this->RenderTimerId = tid;
		this->RenderTimerCount = 0;
	}

	void Report()
	{
		cout << "vtkTimerCallback::Report" << endl;
		cout << "  FastTimerId: " << this->FastTimerId << endl;
		cout << "  FastTimerCount: " << this->FastTimerCount << endl;
		cout << "  RenderTimerId: " << this->RenderTimerId << endl;
		cout << "  RenderTimerCount: " << this->RenderTimerCount << endl;
	}

private:
	int FastTimerId;
	int FastTimerCount;
	int RenderTimerId;
	int RenderTimerCount;
};

//!This test perform a standard execution of the ESQUI simulation process using the IHP Haptic.

int main(int argc, char * argv[])
{
	/**********  Render Window Definitions  ********/
	vtkRenderer * ren1 = vtkRenderer::New();
	ren1->SetBackground(1.0,1.0,1.0);
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(ren1);
	renWin->SetSize(840,480);

	vtkRenderWindowInteractor * iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkTransform * cameraTransform = vtkTransform::New();
	vtkCamera * camera = ren1->GetActiveCamera();
	camera->SetUserTransform(cameraTransform);
	//Initialize Camera Position
	double * focal = camera->GetFocalPoint();
	std::cout << "Camera Focal Point: " << focal[0] << ", " << focal[1] << ", " << focal[2] << "\n";
	camera->SetFocalPoint(focal[0], focal[1], focal[2] - 12);
	camera->SetPosition(0, 0, 0);
	camera->Yaw(0);
	camera->Elevation(20);
	camera->Pitch(-20);
	camera->Dolly(1);

	/******* Creation & Initialization of Scenario ********/

	Scenario = vtkScenario::New();
	Scenario->SetRenderWindow(renWin);

	//Instance of vtkCylinderSource and set some of its properties.
	vtkCylinderSource * cylinder = vtkCylinderSource::New();
	cylinder->SetHeight(12.0);
	cylinder->SetRadius(.3);
	cylinder->SetResolution(20);
	cylinder->Update();

	vtkCubeSource * cube = vtkCubeSource::New();
	cube->SetXLength(1);
	cube->SetYLength(1.5);
	cube->SetZLength(1);
	cube->Update();

	//A Transform will be performed to the polyData to adjust it to VTK coordinate system
	// Whole Tool is rotated -90 degrees at x axis. Set in a horizontal plane
	vtkTransform * transformRotate = vtkTransform::New();
	transformRotate->RotateX(90);

	//Ball is shifted in depth (note negative sign)
	vtkTransform * transformShift = vtkTransform::New();
	transformShift->Translate(0, 0, -6);

	vtkTransformPolyDataFilter * transformFilter = vtkTransformPolyDataFilter::New();
	transformFilter->SetTransform(transformRotate);

	vtkTransformPolyDataFilter * shiftFilter = vtkTransformPolyDataFilter::New();
	shiftFilter->SetTransform(transformShift);

	transformFilter->SetInput(cylinder->GetOutput());
	transformFilter->Update();

	// Primary (left) Tool

	//A cylinder + cube  will emulate the tool
	Tool0 = vtkToolDummy::New();
	//A new copy of the PolyData is made in tool->SetPolyData()
	Tool0->SetPolyData(0, transformFilter->GetOutput());

	transformFilter->SetInput(cube->GetOutput());
	transformFilter->Update();

	shiftFilter->SetInput(transformFilter->GetOutput());
	shiftFilter->Update();

	//A new copy of the PolyData is made in tool->SetPolyData()
	Tool0->SetPolyData(1, shiftFilter->GetOutput());

	vtkProperty * cylinderProp = vtkProperty::New();
	cylinderProp->SetColor(0, 0, 0);
	cylinderProp->SetAmbient(100);
	cylinderProp->SetSpecular(200);
	cylinderProp->SetOpacity(0.5);
	Tool0->GetActor(0)->SetProperty(cylinderProp);

	Tool0->SetPosition(-3, 0, 0);
	Tool0->SetOrientation(0, 0, 0);

	Scenario->InsertNextTool(Tool0);

	//Secondary (right) tool
	Tool1 = vtkToolDummy::New();
	//A cylinder + cube will emulate the tool
	//Instance of vtkCylinderSource and set some of its properties.
	transformFilter->SetInput(cylinder->GetOutput());
	transformFilter->Update();

	//A new copy of the PolyData is made in tool->SetPolyData()
	Tool1->SetPolyData(0, transformFilter->GetOutput());
	transformFilter->SetInput(cube->GetOutput());
	transformFilter->Update();

	shiftFilter->SetInput(transformFilter->GetOutput());
	shiftFilter->Update();

	//A new copy of the PolyData is made in tool->SetPolyData()
	Tool1->SetPolyData(1, shiftFilter->GetOutput());

	Tool1->GetActor(0)->SetProperty(cylinderProp);

	Tool1->SetPosition(3, 0, 0);
	Tool1->SetOrientation(0, 0, 0);

	Scenario->InsertNextTool(Tool1);

	//Display Axes
	vtkTransform * transform = vtkTransform::New();
	transform->Translate(1.0, 0.0, 0.0);

	vtkAxesActor * axes = vtkAxesActor::New();
	axes->SetTotalLength(2, 2, 2);

	// The axes are positioned with a user transform
	//axes->SetUserTransform(transform);

	//ren1->SetBackground(0.9, 0.8, 0.8);
	//ren1->AddActor(axes);

	iren->Initialize();

	//iren->RemoveAllObservers();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	/******* Haptic Initialization ********/

	//Create an vtkIHP Haptic Object
	Haptic = vtkIHP::New();

	int connected = Haptic->Init();

	if(connected > 0){
		std::cout << "IHP Device is connected..." << std::endl;

		// Create repeating timer for synchronous haptic update
		tid = iren->CreateRepeatingTimer(30);
		cb->SetFastTimerId(tid);
	}
	else
	{
		std::cout << "IHP Device is not connected... \nPlease startup device and restart the application" << std::endl;
		exit(0);
	}

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 20 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(50);
	cb->SetRenderTimerId(tid);

	iren->Start();

	Haptic->Release();

	Haptic->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	
	return 0;
}


