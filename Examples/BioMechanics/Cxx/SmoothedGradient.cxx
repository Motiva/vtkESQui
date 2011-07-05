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
#include "vtkSphereSource.h"
#include "vtkTriangleFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkBrownianPoints.h"
#include "vtkGradientFilter.h"
#include "vtkSmoothPolyDataFilter.h"

#include <vtkArrowSource.h>
#include <vtkAssignAttribute.h>
#include <vtkGlyph3D.h>
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
#include "vtkThinPlateSplineTransform.h"
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
				//cout << "Smooth\n";
				/*vtkSmartPointer<vtkPolyData> o = vtkSmartPointer<vtkPolyData>::New();
				o->DeepCopy(this->SmoothFilter->GetOutput());
				this->SmoothFilter->SetInput(o);
				this->SmoothFilter->SetSource(vtkPolyData::SafeDownCast(this->SmoothFilter->GetInput()));
				this->SmoothFilter->SetNumberOfIterations(15);*/
				//this->SmoothFilter->Update();
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

	void SetGradientFilter(vtkGradientFilter * filter)
	{
		this->GradientFilter = filter;
	}

	void SetSmoothFilter(vtkSmoothPolyDataFilter * filter)
	{
		this->SmoothFilter = filter;
	}

	void SetWarpFilter(vtkWarpVector * filter)
	{
		this->WarpFilter = filter;
	}


private:
	int FastTimerId;
	int RenderTimerId;
	int FasterTimerId;

	vtkIdList * List;

	vtkGradientFilter * GradientFilter;

	vtkSmoothPolyDataFilter * SmoothFilter;

	vtkWarpVector * WarpFilter;

};

int main(int argc, char * argv[])
{

	vtkSmartPointer<vtkSphereSource> source =
			vtkSmartPointer<vtkSphereSource>::New();
	source->SetRadius(1);
	source->SetPhiResolution(4);
	source->SetThetaResolution(4);
	source->Update();

	vtkPolyData * input = source->GetOutput();

	// Generate random deformations all over the surface
	vtkSmartPointer<vtkBrownianPoints> brownian = vtkSmartPointer<vtkBrownianPoints>::New();
	brownian->SetInput(source->GetOutput());
	brownian->SetMaximumSpeed(0.1);
	brownian->SetMinimumSpeed(-0.1);
	brownian->Update();

	vtkSmartPointer<vtkGradientFilter> gradient =
			vtkSmartPointer<vtkGradientFilter>::New();
	gradient->SetInput(brownian->GetOutput());
	gradient->SetInputScalars(0, "BrownianVectors");
	gradient->Update();

	vtkPolyData * gdata = gradient->GetPolyDataOutput();
	vtkDataArray * arrayg = gdata->GetPointData()->GetArray("Gradients");
	vtkDataArray * arrayb = gdata->GetPointData()->GetArray("BrownianVectors");

	for(int i = 0; i < arrayg->GetNumberOfTuples(); i++)
	{
		double * g = arrayg->GetTuple(i);
		double * b = arrayb->GetTuple(i);

		cout << "g:" << g[0] << ", " << g[1] << ", " << g[2] <<
				" | b:" << b[0] << ", " << b[1] << ", " << b[2] << "\n";
	}

	//Apply Deformation
	vtkSmartPointer<vtkWarpVector> warpVector = vtkSmartPointer<vtkWarpVector>::New();
	warpVector->SetInput(brownian->GetOutput());
	warpVector->SetScaleFactor(1);
	warpVector->Update();

	//vtkPolyData * output = gradient->GetOutput();

	vtkSmartPointer<vtkSmoothPolyDataFilter> smooth =
				vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
	smooth->SetInput(warpVector->GetPolyDataOutput());
	smooth->SetSource(input);
	smooth->SetNumberOfIterations(0);
	smooth->Update();

	vtkSmartPointer<vtkWarpVector> warp = vtkSmartPointer<vtkWarpVector>::New();

	vtkPolyData * deformed = smooth->GetOutput();

	/*vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
	target->DeepCopy(input);

	double * p = input->GetPoint(2);
	target->GetPoints()->SetPoint(2, p[0]+0.2, p[1]+0.2, p[2]);

	vtkSmartPointer<vtkThinPlateSplineTransform> warpto = vtkSmartPointer<vtkThinPlateSplineTransform>::New();
	warpto->SetSourceLandmarks(input->GetPoints());
	warpto->SetTargetLandmarks(target->GetPoints());
	warpto->SetBasisToR();

	vtkPolyData * deformed = target;*/

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
	actor->GetProperty()->SetRepresentationToWireframe();

	vtkSmartPointer<vtkPolyDataMapper> mapperd = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperd->SetInput(deformed);

	vtkSmartPointer<vtkActor> actord = vtkSmartPointer<vtkActor>::New();
	actord->SetMapper(mapperd);
	actord->GetProperty()->SetColor(0.5,0.9,0);
	actord->GetProperty()->SetRepresentationToSurface();

	vtkSmartPointer<vtkAssignAttribute> vectors =
			vtkSmartPointer<vtkAssignAttribute>::New();
	vectors->SetInputConnection(gradient->GetOutputPort());
	vectors->Assign("Gradients", vtkDataSetAttributes::VECTORS,
			vtkAssignAttribute::POINT_DATA);

	vtkSmartPointer<vtkArrowSource> arrow =
			vtkSmartPointer<vtkArrowSource>::New();

	vtkSmartPointer<vtkGlyph3D> glyphs =
			vtkSmartPointer<vtkGlyph3D>::New();
	glyphs->SetInputConnection(0, vectors->GetOutputPort());
	glyphs->SetInputConnection(1, arrow->GetOutputPort());
	glyphs->ScalingOn();
	glyphs->SetScaleModeToScaleByVector();
	glyphs->SetScaleFactor(1);
	glyphs->OrientOn();
	glyphs->ClampingOff();
	glyphs->SetVectorModeToUseVector();
	glyphs->SetIndexModeToOff();

	vtkSmartPointer<vtkPolyDataMapper> glyphMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputConnection(glyphs->GetOutputPort());
	glyphMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> glyphActor =
			vtkSmartPointer<vtkActor>::New();
	glyphActor->SetMapper(glyphMapper);
	glyphActor->GetProperty()->SetColor(0,0,1);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(800,600);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(actor);
	renderer->AddActor(actord);
	renderer->AddActor(glyphActor);

	renderer->SetBackground(0.1,0.1,0.1);

	renderer->ResetCamera();
	iren->Initialize();

	renWin->Render();

	// Sign up to receive TimerEvent:
	//
	vtkTimerCallback * cb = vtkTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetGradientFilter(gradient);
	cb->SetSmoothFilter(smooth);
	cb->SetWarpFilter(warp);

	//Create a faster timer for DeformationModel update
	tid = iren->CreateRepeatingTimer(100);
	cb->SetFastTimerId(tid);

	tid = iren->CreateRepeatingTimer(40);
	cb->SetRenderTimerId(tid);

	iren->Start();

	return 0;
}


