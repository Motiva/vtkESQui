#include "vtkTimerCallback.h"

vtkTimerCallback::vtkTimerCallback()
{
	this->FasterTimerId = 0;
	this->FastTimerId = 0;
	this->RenderTimerId = 0;
}

vtkTimerCallback::~vtkTimerCallback()
{
}

//----------------------------------------------------------------------------
void vtkTimerCallback::Execute(vtkObject *caller, unsigned long eid, void *callData)
{
	if (vtkCommand::TimerEvent == eid)
	{
		int tid = * static_cast<int *>(callData);

		if (tid == this->FasterTimerId)
		{
			//Do whatever
			cout << "FasterTimer\n";
		}
		if (tid == this->FastTimerId)
		{
			//Do whatever
			cout << "FastTimer\n";
		}
		else if (tid == this->RenderTimerId)
		{
			cout << "Render\n";
			if (this->Interactor &&
					this->Interactor->GetRenderWindow() &&
					this->Interactor->GetRenderWindow()->GetRenderers())
			{
				this->Interactor->Render();
			}
		}
	}
}

//--------------------------------------------------------------------------
void vtkTimerCallback::SetFasterTimerId(int tid)
{
	this->FasterTimerId = tid;
}

//--------------------------------------------------------------------------
void vtkTimerCallback::SetFastTimerId(int tid)
{
	this->FastTimerId = tid;
}

//--------------------------------------------------------------------------
void vtkTimerCallback::SetRenderTimerId(int tid)
{
	this->RenderTimerId = tid;
}

//--------------------------------------------------------------------------
void vtkTimerCallback::PrintSelf(ostream&os, vtkIndent indent)
{
}
