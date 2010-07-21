/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me 
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM), 
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at 
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License 
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/

#ifndef __vtkTimerCallback_h
#define __vtkTimerCallback_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkCommand.h"

class vtkRenderWindowInteractor;

//! Implementation of a standard timer
class VTK_ESQUI_COMMON_EXPORT vtkTimerCallback : public vtkCommand {
public:

	//! Create new timer callback
	static vtkTimerCallback *New(){return new vtkTimerCallback;};
	//! Print timer info
	void PrintSelf(ostream& os, vtkIndent indent);
	//! Return class name
	const char *GetClassName() {return "vtkTimerCallback";};

	//! Execution of the timer
	/*!
	 * Satisfy the superclass API for callbacks.
	 */
	//Description:
	// Satisfy the superclass API for callbacks. Recall that the caller is
	// the instance invoking the event; eid is the event id (see
	// vtkCallbackCommand.h); and calldata is information sent when the callback
	// was invoked (e.g., progress value in the vtkCallbackCommand::ProgressEvent).
	virtual void Execute(vtkObject *caller, unsigned long eid, void *callData);

	//!Set timer identifier
	void SetFasterTimerId(vtkIdType tid);

	//!Set timer identifier
	void SetFastTimerId(vtkIdType tid);

	//!Set render timer identifier
	void SetRenderTimerId(vtkIdType tid);

protected:
	vtkTimerCallback();
	~vtkTimerCallback();

private:

	vtkTimerCallback (const vtkTimerCallback &);//NotImplemented
	void operator =(const vtkTimerCallback &);//Not Implemented

	vtkRenderWindowInteractor * Interactor;

	vtkIdType FasterTimerId;
	vtkIdType FastTimerId;
	vtkIdType RenderTimerId;
};

#endif
