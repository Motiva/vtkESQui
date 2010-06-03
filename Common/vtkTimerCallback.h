
#ifndef __vtkTimerCallback_h
#define __vtkTimerCallback_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkCommand.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

class VTK_ESQUI_COMMON_EXPORT vtkTimerCallback : public vtkCommand {
public:

	static vtkTimerCallback *New(){return new vtkTimerCallback;};
	void PrintSelf(ostream& os, vtkIndent indent);
	const char *GetClassName() {return "vtkTimerCallback";};

	// Description:
	// Satisfy the superclass API for callbacks. Recall that the caller is
	// the instance invoking the event; eid is the event id (see
	// vtkCallbackCommand.h); and calldata is information sent when the callback
	// was invoked (e.g., progress value in the vtkCallbackCommand::ProgressEvent).
	virtual void Execute(vtkObject *caller, unsigned long eid, void *callData);

	void SetFasterTimerId(int tid);

	void SetFastTimerId(int tid);

	void SetRenderTimerId(int tid);

protected:
	vtkTimerCallback();
	~vtkTimerCallback();

private:

	vtkTimerCallback (const vtkTimerCallback &);//NotImplemented
	void operator =(const vtkTimerCallback &);//Not Implemented

	vtkRenderWindowInteractor * Interactor;

	int FasterTimerId;
	int FastTimerId;
	int RenderTimerId;
};

#endif
