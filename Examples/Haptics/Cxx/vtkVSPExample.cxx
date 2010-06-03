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

#include "vtkCylinderSource.h"
#include "vtkSphereSource.h"
#include "vtkProperty.h"
#include "vtkDelaunay3D.h"
#include "vtkTextureMapToCylinder.h"
#include "vtkTransformTextureCoords.h"
#include "vtkJPEGReader.h"

#include "vtkCommand.h"

#include "vtkHaptic.h"
#include "vtkVSP.h"


using namespace std;

namespace{
	//All these objects must be defined globally, in order to access them on timer callback
	//Note: Global variables may have its first letter on uppercase
	vtkVSP * Haptic = NULL;
	vtkRenderWindowInteractor * Interactor = NULL;
	vtkRenderer * Ren1= NULL;

	vtkSphereSource * Balloon = NULL;
	vtkCylinderSource * Cath = NULL;
	vtkActor * CatheterActor = NULL;
	vtkActor * BalloonActor = NULL;
	vtkActor * CylinderActor = NULL;
}


void UpdateHaptic(vtkVSP * Haptic)
{
	//Update  state of the VSP device
	Haptic->UpdateDevice();

	/// Camera Operations ///
	//Get left & right joystick state
	int rightY = Haptic->GetRightJoystickYPosition();
	int rightX = Haptic->GetRightJoystickXPosition();
	int leftY = Haptic->GetLeftJoystickYPosition();
	int leftX = Haptic->GetLeftJoystickXPosition();

	vtkCamera * camera = Ren1->GetActiveCamera();
	vtkTransform * cameraTransfom = vtkTransform::SafeDownCast(camera->GetUserTransform());

	//Translate the camera in accordance with the right joystick state
	cameraTransfom->Translate(rightX/1000.0, rightY/1000.0, 0);

	//Modify camera azimuth/elevation in accordance with the left joystick state
	camera->Azimuth(leftX*0.5);
	camera->Elevation(leftY*0.5);

	//Set the zoom for the camera in function of the VSP buttons state
	if(Haptic->GetZoomState() == 1)
	{
		camera->Dolly(1.01);
	}
	else if (Haptic->GetZoomState() == -1)
	{
		camera->Dolly(0.99);
	}

	/// Pedal Operations ///


	/// Catheter Operations ///
	for (int i = 0; i<3; i++)
	{
		if(Haptic->GetCatheterState(i) > 0)
		{
			//Modify Catheter radius depending on catheter width (0=8F, 1=5F, 2=2.5F)
			float radius = 0.3/(i+1);
			Cath->SetRadius(radius);
			Balloon->SetRadius(radius);

			//Get the position of the catheter inside the VSP device
			float depth = Haptic->GetCatheterDepth(i);

			//Update the position of the balloon & catheter inside of the cylinder
			double factor = 0.02;
			double  position[3];
			CatheterActor->GetPosition(position);
			CatheterActor->SetPosition(0, factor*depth, 0);
			BalloonActor->GetPosition(position);
			BalloonActor->SetPosition(0, factor*depth, 0);

			//Get the catheter roll angle
			float roll = Haptic->GetCatheterRoll(i);
			CatheterActor->RotateY(roll);
			BalloonActor->RotateY(roll);

			///Balloon Inflation///
			if(Haptic->GetInflationPressure() > 0)
			{
				float balloonRadius = radius + 0.5*(Haptic->GetInflationPressure()/20);
				Balloon->SetRadius(balloonRadius);
			}
		}
	}

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

//!This test perform a standard execution of the ESQUI simulation process.

int main(int argc, char * argv[])
{
	/**********  Render Window Definitions  ********/
	//Global Variable
	Ren1 = vtkRenderer::New();
	Ren1->SetBackground(1.0,1.0,1.0);
	
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(Ren1);
	renWin->SetSize(840,480);
	//renWin->Render();

	//Global Variable
	Interactor = vtkRenderWindowInteractor::New();
	Interactor->SetRenderWindow(renWin);

	vtkTransform * cameraTransform = vtkTransform::New();
	Ren1->GetActiveCamera()->SetUserTransform(cameraTransform);
	Ren1->GetActiveCamera()->Dolly(0.02);

	/******* Creation & Initialization of figures ********/

	//A cylinder will emulate a vessel
	//Instance of vtkCylinderSource and set some of its properties.
	vtkCylinderSource * cylinder = vtkCylinderSource::New();
	cylinder->SetHeight(13.0);
	cylinder->SetResolution(20);
	vtkDelaunay3D * del = vtkDelaunay3D::New();
	del->SetInput(cylinder->GetOutput());
	del->SetTolerance(0.01);

	vtkTextureMapToCylinder *  textureMapper = vtkTextureMapToCylinder::New();
	textureMapper->SetInputConnection(del->GetOutputPort());
	textureMapper->PreventSeamOn();

	//Scale the texture coordinate to get some repeat patterns.
	vtkTransformTextureCoords * xForm = vtkTransformTextureCoords::New();
	xForm->SetInput(textureMapper->GetOutput());
	xForm->SetScale(1, 8, 1);

	vtkDataSetMapper *  mapper = vtkDataSetMapper::New();
	mapper->SetInput (xForm->GetOutput());

	//Set the texture of the cylinder
	vtkJPEGReader *  jpegReader = vtkJPEGReader::New();
	jpegReader->SetFileName("Scenario/VesselTexture.jpg");

	vtkTexture * texture = vtkTexture::New();
	texture->SetInputConnection (jpegReader->GetOutputPort());
	texture->InterpolateOn();

	//Global Variable
	CylinderActor = vtkActor::New();
	CylinderActor->SetMapper(mapper);
	CylinderActor->SetTexture(texture);

	vtkProperty * cylinderProp = vtkProperty::New();
	cylinderProp->SetColor(0.2, 0.1, 0);
	cylinderProp->SetAmbient(100);
	cylinderProp->SetSpecular(200);
	cylinderProp->SetOpacity(0.5);
	CylinderActor->SetProperty(cylinderProp);

	//A small cylinder will emulate the catheter
	Cath = vtkCylinderSource::New();
	Cath->SetHeight(10.0);
	Cath->SetRadius(0.3);
	Cath->SetResolution(20);

	//Ball is shifted in depth (note negative sign)
	vtkTransform * transformShift = vtkTransform::New();
	transformShift->Translate(0, -12, 0);

	vtkTransformPolyDataFilter * shiftFilter = vtkTransformPolyDataFilter::New();
	shiftFilter->SetTransform(transformShift);

	shiftFilter->SetInput(Cath->GetOutput());
	shiftFilter->Update();

	vtkDataSetMapper *  cathMapper = vtkDataSetMapper::New();
	cathMapper->SetInput(shiftFilter->GetOutput());

	CatheterActor = vtkActor::New();
	CatheterActor->SetMapper(cathMapper);

	vtkProperty * prop = CatheterActor->GetProperty();
	prop->SetColor(0.5, 0.7, 1);
	prop->SetOpacity(0.8);

	vtkTransform * shift = vtkTransform::New();
	shift->Translate(0,0,0);

	//A sphere will emulate the stent
	Balloon = vtkSphereSource::New();
	Balloon->SetRadius(0.3);
	Balloon->SetCenter(0, -7, 0);
	Balloon->SetPhiResolution(12);
	Balloon->SetThetaResolution(12);

	vtkPolyDataMapper * sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInput(Balloon->GetOutput());

	//Global Variable
	BalloonActor = vtkActor::New();
	BalloonActor->SetMapper(sphereMapper);

	//Add Actors to the renderer
	Ren1->AddActor(CylinderActor);
	Ren1->AddActor(CatheterActor);
	Ren1->AddActor(BalloonActor);

	Interactor->Initialize();

	//Interactor->RemoveAllObservers();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback *cb = vtkTimerCallback::New();
	Interactor->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	/******* Haptic Initialization ********/

	//Create an vtkVSP Haptic Object
	Haptic = vtkVSP::New();

	int connected = Haptic->Init();

	if(connected > 0){
		std::cout << "Device is connected..." << std::endl;

		// Create repeating timer for synchronous haptic update
		tid = Interactor->CreateRepeatingTimer(30);
		cb->SetFastTimerId(tid);
	}
	else
	{
		std::cout << "Device is not connected... \nPlease startup device and restart the application" << std::endl;
		exit(0);
	}

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 20 frames per second.)
	//
	tid = Interactor->CreateRepeatingTimer(20);
	cb->SetRenderTimerId(tid);

	Interactor->Start();

	Haptic->Release();

	Haptic->Delete();
	Ren1->Delete();
	renWin->Delete();
	Interactor->Delete();
	
	return 0;
}


