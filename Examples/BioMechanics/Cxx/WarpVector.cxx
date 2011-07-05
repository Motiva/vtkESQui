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
/*
 * TestEsquivtkOrganCollection.cxx
 *
 *  Created on: 12/01/2010
 *      Author: jballesteros
 */

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkCubeSource.h"
#include "vtkSphereSource.h"
#include "vtkTriangleFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkBrownianPoints.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPointData.h"
#include "vtkWarpVector.h"
#include "vtkInterpolatedVelocityField.h"
#include "vtkDoubleArray.h"
#include "vtkCommand.h"

using namespace std;

//!This test perform a test of the vtkOrgan class

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
				vtkSmartPointer<vtkPolyData> o = vtkSmartPointer<vtkPolyData>::New();
				o->DeepCopy(this->WarpVector->GetOutput());
				this->WarpVector->SetInput(o);
				this->WarpVector->SetScaleFactor(.01);
				this->WarpVector->Update();
			}
			else if (tid == this->FasterTimerId)
			{

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

	void SetWarpVector(vtkWarpVector * warpvector)
	{
		this->WarpVector = warpvector;
	}

private:
	int FastTimerId;
	int RenderTimerId;
	int FasterTimerId;

	vtkIdList * List;

	vtkWarpVector * WarpVector;
};

int main(int argc, char * argv[])
{

	vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();

	source->Update();

	// Generate random deformations all over the surface
	vtkSmartPointer<vtkBrownianPoints> brownian = vtkSmartPointer<vtkBrownianPoints>::New();
	brownian->SetInput(source->GetOutput());
	brownian->SetMaximumSpeed(0.1);
	brownian->SetMinimumSpeed(-0.1);
	brownian->Update();
	vtkPolyData * input = vtkPolyData::SafeDownCast(brownian->GetOutput());

	//input->Print(cout);

	vtkSmartPointer<vtkWarpVector> warpVector = vtkSmartPointer<vtkWarpVector>::New();
	warpVector->SetInput(input);
	warpVector->SetScaleFactor(1);
	warpVector->Update();

	vtkPolyData * output = warpVector->GetPolyDataOutput();

	/*output->Print(cout);
	for(int i=0; i < output->GetNumberOfPoints(); i++)
	{
		double *p = output->GetPoint(i);
		cout << "p: " << p[0] << ", " << p[1] << ", "  << p[2] << "\n";

	}*/

	/*------ Rendering stuff------*/
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(input);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetOpacity(0.9);

	vtkSmartPointer<vtkPolyDataMapper> mapperw = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperw->SetInput(output);

	vtkSmartPointer<vtkActor> actorw = vtkSmartPointer<vtkActor>::New();
	actorw->SetMapper(mapperw);
	actorw->GetProperty()->SetColor(1.0, 0.0, 0.0);
	actorw->GetProperty()->SetRepresentationToWireframe();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(800,600);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(actor);
	renderer->AddActor(actorw);

	renderer->SetBackground(0.1,0.1,0.1);

	renderer->ResetCamera();
	iren->Initialize();

	renWin->Render();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback * cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetWarpVector(warpVector);

	//Create a faster timer for DeformationModel update
	tid = iren->CreateRepeatingTimer(120);
	cb->SetFastTimerId(tid);

	tid = iren->CreateRepeatingTimer(40);
	cb->SetRenderTimerId(tid);

	iren->Start();

	return 0;
}


