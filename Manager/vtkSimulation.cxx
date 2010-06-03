#include "vtkSimulation.h"


void func ( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData )
{
	vtkSimulation * sim = static_cast<vtkSimulation*>(clientData);

	if (vtkCommand::TimerEvent == eventId)
	{
		int tid = * static_cast<int *>(callData);
		if (tid == sim->GetHapticTimerId())
		{
			//Do whatever
			//cout << "Haptic\n";
			sim->UpdateHaptic();
		}
		if (tid == sim->GetSimulationTimerId())
		{
			//Do whatever
			//cout << "Simulation\n";
			sim->UpdateManager();
		}
		else if (tid == sim->GetRenderTimerId())
		{
			//cout << "Render\n";
			vtkRenderWindowInteractor * Interactor = sim->GetRenderWindowInteractor();
			if (Interactor &&
					Interactor->GetRenderWindow() &&
					Interactor->GetRenderWindow()->GetRenderers())
			{
				Interactor->Render();
			}
		}
	}

}

vtkCxxRevisionMacro(vtkSimulation, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulation);

//----------------------------------------------------------------------------
vtkSimulation::vtkSimulation()
{
	this->SimulationManager = NULL;
	this->Interactor = NULL;
	this->HapticTimerId = 0;
	this->HapticTimerRate = 0;
	this->SimulationTimerId = 0;
	this->SimulationTimerRate = 0;
	this->RenderTimerId = 0;
	this->RenderTimerRate = 0;
	this->UseHaptic = false;
}

//----------------------------------------------------------------------------
vtkSimulation::~vtkSimulation()
{

}

//----------------------------------------------------------------------------
void vtkSimulation::Init() {

	//Init haptic device
#ifndef VTKESQUI_USE_NO_HAPTICS
	if(this->UseHaptic)
	{
		int connected = this->HapticDevice->Init();
		if(connected > 0){
			std::cout << "Haptic device is connected..." << std::endl;
			for(int i = 0; i < this->HapticDevice->GetNumberOfTools(); i++)
			{
				vtkTool * tool =  this->SimulationManager->GetScenario()->GetTool(i);
				tool->UseHapticOn();
				tool->SetHapticDevice(this->HapticDevice);
			}
		}
		else
		{
			std::cout << "Haptic device is not connected... \nPlease startup device and restart the application" << std::endl;
			exit(0);
		}
	}
#endif

	//
	this->Callback = vtkCallbackCommand::New();
	this->Callback->SetCallback(func);
	this->Callback->SetClientData(this);

	//Check if the interactor has been manually set
	if(!this->Interactor)
	{
		this->Interactor = this->SimulationManager->GetScenario()->GetRenderWindow()->GetInteractor();
	}

	this->Interactor->Initialize();

	this->Interactor->AddObserver(vtkCommand::TimerEvent, this->Callback);

	//Check for haptic device resfresh rate
	if(this->HapticTimerRate == 0)
	{
		//Set Default Haptic Rate
		this->HapticTimerRate = 1; //1ms
	}

	this->HapticTimerId = this->Interactor->CreateRepeatingTimer(this->HapticTimerRate);

	//Check for simulation resfresh rate
	if(this->SimulationTimerRate == 0)
	{
		//Set Default Haptic Rate
		this->SimulationTimerRate = 10; //10ms
	}

	this->SimulationTimerId = this->Interactor->CreateRepeatingTimer(this->SimulationTimerRate);

	//Check for rendering rate
	if(this->RenderTimerRate == 0)
	{
		//Set Default Haptic Rate
		this->RenderTimerRate = 30;
	}

	this->RenderTimerId = this->Interactor->CreateRepeatingTimer(this->RenderTimerRate);

}

//----------------------------------------------------------------------------
void vtkSimulation::Run()
{
	this->Interactor->Start();
}

//----------------------------------------------------------------------------
void vtkSimulation::SetSimulationManager(vtkSimulationManager *manager) {
	this->SimulationManager = manager;
}

//----------------------------------------------------------------------------
vtkSimulationManager* vtkSimulation::GetSimulationManager() {
	return this->SimulationManager;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetRenderWindowInteractor(vtkRenderWindowInteractor * iren) {
	this->Interactor = iren;
}

//----------------------------------------------------------------------------
vtkRenderWindowInteractor * vtkSimulation::GetRenderWindowInteractor()
{
	return this->Interactor;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetHapticTimerRate(double rate)
{
	this->HapticTimerRate = rate;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetSimulationTimerRate(double rate)
{
	this->SimulationTimerRate = rate;
}

//----------------------------------------------------------------------------
void vtkSimulation::SetRenderTimerRate(double rate)
{
	this->RenderTimerRate = rate;
}

//----------------------------------------------------------------------------
double vtkSimulation::GetHapticTimerRate()
{
	return this->HapticTimerRate;
}

//----------------------------------------------------------------------------
double vtkSimulation::GetSimulationTimerRate()
{
	return this->SimulationTimerRate;
}

//----------------------------------------------------------------------------
double vtkSimulation::GetRenderTimerRate()
{
	return this->RenderTimerRate;
}

//----------------------------------------------------------------------------
int vtkSimulation::GetHapticTimerId()
{
	return this->HapticTimerId;
}

//----------------------------------------------------------------------------
int vtkSimulation::GetSimulationTimerId()
{
	return this->SimulationTimerId;
}

//----------------------------------------------------------------------------
int vtkSimulation::GetRenderTimerId()
{
	return this->RenderTimerId;
}

//----------------------------------------------------------------------------
void vtkSimulation::UpdateManager(){
	this->SimulationManager->Update();
}

//----------------------------------------------------------------------------
void vtkSimulation::UpdateHaptic() {

#ifndef VTKESQUI_USE_NO_HAPTICS
	if(this->UseHaptic)
	{
		const char * name = HapticDevice->GetName();
		this->GetHapticDevice()->Update();
	}
#endif
}

//----------------------------------------------------------------------------
void vtkSimulation::PrintSelf(ostream& os,vtkIndent indent) {
	//this->Superclass::PrintSelf(os,indent);

	os << indent << "SimulationTimerRate: " << this->SimulationTimerRate << "\n";
	os << indent << "HapticTimerRate: " << this->HapticTimerRate << "\n";
	os << indent << "RenderTimerRate: " << this->RenderTimerRate << "\n";

}
