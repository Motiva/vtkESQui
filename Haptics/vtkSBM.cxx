#include "vtkSBM.h"

#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include <math.h>

#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioObject.h"
#include "vtkTool.h"
#include "vtkToolLaparoscopy.h"
#include "vtkToolGrasper.h"
#include "vtkToolSingleChannel.h"

#include <stdio.h>

vtkCxxRevisionMacro(vtkSBM, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSBM);

#ifndef GetAndCopyMacro
#define GetAndCopyMacro(src, dst) \
	dst[0] = dst[1] = dst[2] = 0.0; \
	vtkMath::Add(dst,src,dst);
#endif

//--------------------------------------------------------------------------
void vtkSBM::SetLensAngle(double angle)
{
  if (angle >= 0.0 && angle <= 90)
  {
    if (this->Initialized)
    {
      this->Camera->Roll(-this->LastRoll);
      this->Camera->Pitch(this->LensAngle);
      // Set new angle
      this->LensAngle = angle;
      this->Camera->Pitch(-this->LensAngle);
      this->Camera->Roll(this->LastRoll);
    }
    else
    {
      this->LensAngle = angle;
    }
  }
}

//--------------------------------------------------------------------------
vtkSBM::vtkSBM()
{
	this->ndevices = 0;
	this->values = NULL;
	this->Initialized = false;
	this->ToolInsertion = 0;
}

//--------------------------------------------------------------------------
vtkSBM::~vtkSBM(){ 
	if(this->ndevices > 0)
	{
		delete[] this->values;
		for (int i=0; i < this->ndevices; i++)
		{
			delete[] this->idStr[i];
			delete[] this->desc[i];
		}
		delete[] this->idStr;
		delete[] this->desc;
	}
}

//--------------------------------------------------------------------------
void vtkSBM::PrintSelf(ostream& os,vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//--------------------------------------------------------------------------
int vtkSBM::Init()
{
	if (this->Initialized) return this->ndevices;

	SBMErrorMessage err;
	char errStr[256];

	// Certain checkings in case of single channel mode
	if (this->SingleChannel)
	{
		if (!this->Camera || !this->CameraObject)
		{
			vtkErrorMacro("Single channel mode needs a camera and its object")
			return -1;
		}
		else{
			vtkMath::Add(this->CameraOrigin, this->Camera->GetPosition(), this->CameraOrigin);
			this->Camera->Roll(-this->LastRoll);
			this->Camera->Pitch(-this->LensAngle);
			this->Camera->Roll(this->LastRoll);
		}
	}
	
	// Nr. of devices connected
	err = SBM_Init(this->ndevices);
	if (err != SBMError_NoError){
		SBM_GetErrorString(err, errStr);
		std::cout << errStr << endl;
		return -1;
	}
	if (this->ndevices < 1)
	{
		this->ndevices = 0;
		return 0;
	}
	if (this->NumberOfTools < this->ndevices)
	{
		//this->ndevices = this->NumberOfTools;
	}
	
	// Collect values of each device
	this->values = new struct SBMDeviceValues[this->ndevices];
	
	for (int i=0; i < this->ndevices; i++){
		do {
			// It takes a while before the device starts sending data
			err = SBM_GetValues(i,this->values[i]);
		} while (err == SBMError_DataNotReady);
		if (err != SBMError_NoError){
			SBM_GetErrorString(err, errStr);
			std::cout << errStr << endl;
			return -1;
		}
	}
	
	// Collect devices' IDs and descriptions
	this->idStr = new char*[this->ndevices];
	this->desc  = new char*[this->ndevices];
	for (int i=0; i < this->ndevices; i++)
	{
		this->idStr[i] = new char[16];
		this->desc[i] = new char[128];
	}
	
	for (int i=0; i<this->ndevices; i++){
		SBM_GetDeviceID(i, this->idStr[i]);
		SBM_GetDeviceDescription(i, this->desc[i]);
	}
	
	sprintf(errStr, "%d SBM devices were initialized.", this->ndevices);
	vtkDebugMacro(errStr);
	
	// Return > 0 if a SBM device was found
	this->Initialized = true;
	return this->ndevices;
}

//--------------------------------------------------------------------------
void vtkSBM::UpdateScenario()
{
	//Update device information
	this->UpdateDevice();
	
	//Scenario tools will be updated
	this->Tools->InitTraversal();
	vtkIdType id = 0;
	vtkTool *tool = vtkTool::SafeDownCast(this->Tools->GetNextObject());

	if (this->SingleChannel)
	{
		// Trocar direction angles
		double Roll  = this->GetToolRoll(0);
		double Pitch = this->GetTrocarPitch(0);
		double Yaw   = this->GetTrocarYaw(0);
		double Depth = this->GetToolDepth(0);

		// Moving camera
		// Reset lens inclination
		this->Camera->Roll(-this->LastRoll);
		this->Camera->Pitch(this->LensAngle);
		this->Camera->Roll(this->LastRoll);

		this->Camera->SetPosition(this->CameraOrigin);

		this->Camera->Pitch(-this->LastPitch);
		this->Camera->Yaw(-this->LastYaw);
		// this->Camera->Yaw(Yaw-this->LastYaw);
		this->Camera->Yaw(Yaw);
		this->Camera->Pitch(Pitch);

		double newpos[3] = {0.0, 0.0, 0.0};
		double distance = this->Camera->GetDistance();
		vtkMath::Add(newpos, this->Camera->GetDirectionOfProjection(), newpos);
		vtkMath::MultiplyScalar(newpos, Depth);
		vtkMath::Add(this->CameraOrigin, newpos, newpos);
		this->Camera->SetPosition(newpos);
		this->Camera->SetDistance(distance);

		// Set inclination again
		this->Camera->Roll(-Roll);
		this->Camera->Pitch(-this->LensAngle);
		this->Camera->Roll(Roll);

		// Moving camera volume
		this->CameraObject->RollPitchYaw(Roll, Pitch, Yaw);
		this->CameraObject->SetDepth(Depth);

		// Moving tool if it is one there
		if (tool)
		{
			vtkToolSingleChannel *toolsc = vtkToolSingleChannel::SafeDownCast(tool);
			toolsc->RollPitchYaw(Roll, Pitch, Yaw);
			toolsc->SetDepth(Depth + this->ToolInsertion);
		}

		// Store angle values
		this->LastRoll  = Roll;
		this->LastPitch = Pitch;
		this->LastYaw   = Yaw;
	}
	else
	{
		while(tool && id < this->ndevices)
		{
			if(tool->GetToolType() == vtkTool::Laparoscopy)
			{
				//Trocar's direction angles
				double Roll  = this->GetToolRoll(id);
				double Pitch = this->GetTrocarPitch(id);
				double Yaw   = this->GetTrocarYaw(id);
				double Depth = this->GetToolDepth(id);

				vtkToolLaparoscopy *tl = vtkToolLaparoscopy::SafeDownCast(tool);
				tl->Yaw(Yaw);
				tl->Pitch(Pitch);
				tl->Roll(Roll);
				tl->SetDepth(Depth);
				if(tl->GetToolModel() == vtkToolLaparoscopy::Grasper)
				{
					vtkToolGrasper *grasper = vtkToolGrasper::SafeDownCast(tl);
					//Set tool opening.
					grasper->SetOpening(this->GetToolOpening(id));
				}

				id++;
			}
			tool = vtkTool::SafeDownCast(this->Tools->GetNextObject());
		}
	}
}

//--------------------------------------------------------------------------
void vtkSBM::Release()
{
	if(this->ndevices > 0)
	{
		SBM_Exit();
		delete[] this->values;
		for (int i=0; i < this->ndevices; i++)
		{
			delete[] this->idStr[i];
			delete[] this->desc[i];
		}
		delete[] this->idStr;
		delete[] this->desc;
	}
	this->ndevices = 0;
	this->Initialized = false;
}

//--------------------------------------------------------------------------
void vtkSBM::UpdateDevice()
{
	for (int i=0; i< this->ndevices; i++){
		SBM_GetValues(i,values[i]);
	}
}

//--------------------------------------------------------------------------
double vtkSBM::GetTrocarYaw(int id)
{
	// Simball returns phi from pi/4 to 3pi/4
	return vtkMath::DegreesFromRadians(-(this->values[id].phi - vtkMath::Pi()*0.5));
}

//--------------------------------------------------------------------------
double vtkSBM::GetLeftTrocarYaw()
{
	return this->GetTrocarYaw(0);
}

//--------------------------------------------------------------------------
double vtkSBM::GetRightTrocarYaw()
{
	return this->GetTrocarYaw(1);
}

//--------------------------------------------------------------------------
double vtkSBM::GetTrocarPitch(int id)
{
	// Simball returns theta from pi/4 to 3pi/4
	return vtkMath::DegreesFromRadians(-(vtkMath::Pi()*0.5 - this->values[id].theta));
}

//--------------------------------------------------------------------------
double vtkSBM::GetLeftTrocarPitch()
{
	return this->GetTrocarPitch(0);
}

//--------------------------------------------------------------------------
double vtkSBM::GetRightTrocarPitch()
{
	return this->GetTrocarPitch(1);
}

//--------------------------------------------------------------------------
double vtkSBM::GetToolDepth(int id)
{
	// return depth centimeters.
	// insertion in milimeters. Max 226.569917918
	// insertionA or insertionB, it depends on instrument inserted
	double insertion;
	if (values[id].flags & SBMFlag_AInserted)
		insertion = this->values[id].insertionA;
	else if (values[id].flags & SBMFlag_BInserted)
		insertion = this->values[id].insertionB;
	else
		return 0.0;
	
	//insertion = insertion - 50;
	return insertion * 0.1;
}

//--------------------------------------------------------------------------
double vtkSBM::GetLeftToolDepth()
{
	return this->GetToolDepth(0);
}

//--------------------------------------------------------------------------
double vtkSBM::GetRightToolDepth()
{
	return this->GetToolDepth(1);
}

//--------------------------------------------------------------------------
double vtkSBM::GetToolRoll(int id)
{
	return vtkMath::DegreesFromRadians(this->values[id].gamma);
}

//--------------------------------------------------------------------------
double vtkSBM::GetLeftToolRoll()
{
	return this->GetToolRoll(0);
}

//--------------------------------------------------------------------------
double vtkSBM::GetRightToolRoll()
{
	return this->GetToolRoll(1);
}

//--------------------------------------------------------------------------
double vtkSBM::GetToolOpening(int id)
{
	// Range [1.0 - 0.285760834148,1.0 - 0.935483870968]
	// handleA or handleB, it depends on instrument inserted
	double value;
	if (values[id].flags & SBMFlag_AInserted)
		value = 1.0 - this->values[id].handleA;
	else if (values[id].flags & SBMFlag_BInserted)
		value = 1.0 - this->values[id].handleB;
	else if (this->values[id].instrumentA == SBMInstr_Grasper)
		value = 1.0 - this->values[id].handleA;
	else if (this->values[id].instrumentB == SBMInstr_Grasper)
		value = 1.0 - this->values[id].handleB;
	else
		return 0;
	
	// value between 0.285760834148 and 0.935483870968
	value = value - 0.285760834148;
	if (value < 0.0) return 0.0;
	value = value / 0.64972303682;
	if (value > 1.0) return 1.0;
	return value;
}

//--------------------------------------------------------------------------
double vtkSBM::GetLeftToolOpening()
{
	return this->GetToolOpening(0);
}

//--------------------------------------------------------------------------
double vtkSBM::GetRightToolOpening()
{
	return this->GetToolOpening(1);
}

//--------------------------------------------------------------------------
bool vtkSBM::GetToolButtonState(int id, int button)
{
	if (id < 0 || id > this->ndevices)
	{
		vtkErrorMacro("Invalid device number.");
		return false;
	}
	if (button == 0) return (this->values[id].flags & SBMFlag_Button1) != 0;
	else if (button == 1) return (this->values[id].flags & SBMFlag_Button2) != 0;
	else if (button == 2) return (this->values[id].flags & SBMFlag_Button3) != 0;
	else{
		vtkErrorMacro("Invalid button number.")
		return false;
	}
}

//--------------------------------------------------------------------------
bool vtkSBM::GetLeftPedalState(){
	// Only one pair of pedals implemented by now.
	unsigned int state = this->values[0].flags & SBMFlag_PedalALeft;
	state = state | (this->values[0].flags & SBMFlag_PedalBLeft);
	if (this->ndevices > 1)
	{
		state = state | (this->values[1].flags & SBMFlag_PedalALeft);
		state = state | (this->values[1].flags & SBMFlag_PedalBLeft);
	}
	return state != 0;
}

//--------------------------------------------------------------------------
bool vtkSBM::GetRightPedalState(){
	// Only one pair of pedals implemented by now
	unsigned int state = this->values[0].flags & SBMFlag_PedalARight;
	state = state | (this->values[0].flags & SBMFlag_PedalBRight);
	if (this->ndevices > 1)
	{
		state = state | (this->values[1].flags & SBMFlag_PedalARight);
		state = state | (this->values[1].flags & SBMFlag_PedalBRight);
	}
	return state != 0;
}

