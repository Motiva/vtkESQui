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

#include "vtkOrgan.h"
#include "vtkScenarioElement.h"
#include "vtkCollisionModel.h"
#include "vtkVisualizationModel.h"
#include "vtkDeformationModel.h"
#include "vtkPSSInterface.h"
#include "vtkCollision.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPointLocator.h"
#include "vtkIdList.h"
#include "vtkCommand.h"
#include "vtkTimerLog.h"

class vtkOrganTimerCallback : public vtkCommand
{
public:
	static vtkOrganTimerCallback *New()
	{
		vtkOrganTimerCallback *cb = new vtkOrganTimerCallback;
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
				cout << "new collision\n";

				vtkScenarioElement * element = this->Object->GetElement(0);
				vtkDeformationModel * def = element->GetDeformationModel();
				vtkPolyData * mesh = vtkPolyData::SafeDownCast(def->GetOutput());

				//Locate collision points
				vtkPointLocator * locator = vtkPointLocator::New();
				double bounds[6];
				mesh->GetBounds(bounds);

				double p[3] = {bounds[0], 0, 0};

				locator->SetDataSet(mesh);

				vtkIdList * list = vtkIdList::New();
				locator->FindClosestNPoints(5, p, list);

				//Set Collisions
				double dir[3];
				dir[0] = 0.1;//-0.1;
				dir[1] = 0.05;
				dir[2] = 0;//0.05;

				for(vtkIdType i = 0; i< list->GetNumberOfIds(); i++)
				{
					vtkIdType id = list->GetId(i);
					double * point = mesh->GetPoint(id);

					//Insert collision info
					vtkCollision * collision = vtkCollision::New();
					collision->SetCollisionType(vtkCollision::ToolOrgan);
					collision->SetElementId(0, 0);
					collision->SetElementId(1, 0);

					//Organ cell point
					collision->SetPointId(1, id);
					collision->SetPoint(1, point[0], point[1], point[2]);
					//collision->InsertCellId(0, organCellId);
					collision->SetDisplacement(dir);

					collision->Print(cout);

					def->AddCollision(collision);
				}
			}
			else if (tid == this->FasterTimerId)
			{
				vtkTimerLog * timer = vtkTimerLog::New();
				timer->StartTimer();
				this->Object->Modified();
				this->Object->Update();
				timer->StopTimer();

				std::cout << "[Test] Execution Rate: " << 1/(timer->GetElapsedTime()) << "\n";

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

	void SetObject(vtkScenarioObject * o)
	{
		this->Object = o;
	}

	void SetCollisionIds(vtkIdList * list)
	{
		this->List = list;
	}
private:
	int FastTimerId;
	int RenderTimerId;
	int FasterTimerId;

	vtkIdList * List;

	vtkScenarioObject * Object;
};

//!This test perform a test of the vtkOrgan class

int main(int argc, char * argv[])
{
	const char * fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
	const char * cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp";
	const char * dfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c12.vtp";
	const char * tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg";

	double simrate = 0.001;
	double renrate = 0.04;
	double colrate = 5;

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(800,600);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkVisualizationModel> vis = vtkSmartPointer<vtkVisualizationModel>::New();
	vis->SetName("ellipsoid_16_16_1");
	vis->SetFileName(fn);
	vis->SetTextureFileName(tfn);
	vis->SetOpacity(1);
	vis->SetColor(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkCollisionModel> col = vtkSmartPointer<vtkCollisionModel>::New();
	col->SetName("vtkbioeng");
	col->SetFileName(cfn);
	col->SetOpacity(0.5);
	col->SetColor(0.0, 0.0, 1.0);

	//Deformation model. Particle-Spring system
	vtkSmartPointer<vtkPSSInterface> def = vtkSmartPointer<vtkPSSInterface>::New();
	def->SetName("ParticleSpring");
	def->SetFileName(dfn);
	def->SetOpacity(1);
	def->SetColor(0,1,0);
	def->SetVisibility(0);
	def->SetSolverType(vtkMotionEquationSolver::VelocityVerlet);
	def->SetSpringCoefficient(350);
	def->SetDistanceCoefficient(10);
	def->SetDampingCoefficient(5);//Friction
	def->SetMass(.1);
	def->SetDeltaT(simrate);//10ms

	vtkSmartPointer<vtkScenarioElement> element = vtkSmartPointer<vtkScenarioElement>::New();
	element->SetId(0);
	element->SetName("ellipsoid");
	element->SetPosition(3.0, 2.5, 0.0);
	element->SetOrientation(25, 15, 30);
	element->SetVisualizationModel(vis);
	element->SetCollisionModel(col);
	element->SetDeformationModel(def);
	element->Init();

	element->Update();

	vtkSmartPointer<vtkOrgan> organ = vtkSmartPointer<vtkOrgan>::New();
	organ->AddElement(element);
	organ->Init();

	/*vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(col->GetOutput(1));
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetRepresentationToWireframe();
	renderer->AddActor(actor);*/

	renderer->AddActor(vis->GetActor());
	renderer->AddActor(col->GetActor());
	renderer->AddActor(def->GetActor());

	renderer->SetBackground(1,1,1);

	renderer->ResetCamera();
	iren->Initialize();

	vtkOrganTimerCallback * cb = vtkOrganTimerCallback::New();
	iren->AddObserver(vtkCommand::TimerEvent, cb);
	int tid;

	cb->SetObject(organ);

	//Create a faster timer for DeformationModel update (10ms)
	tid = iren->CreateRepeatingTimer(simrate*1000);
	cb->SetFasterTimerId(tid);

	//Create a collision every 5 seconds
	tid = iren->CreateRepeatingTimer(colrate*1000);
	cb->SetFastTimerId(tid);

	// Create a slower repeating timer to trigger Render calls.
	// (This fires at the rate of approximately 25 frames per second.)
	//
	tid = iren->CreateRepeatingTimer(renrate*1000);
	cb->SetRenderTimerId(tid);

	iren->Start();

	return 0;
}


