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
 * TestEsquivtkTooLaparoscopy.cxx
 *
 *  Created on: 12/01/2010
 *      Author: jballesteros
 */

#include <iostream>

#include "vtkToolDummy.h"
#include "vtkScenarioElement.h"
#include "vtkCollisionModel.h"
#include "vtkVisualizationModel.h"
#include "vtkDeformationModel.h"
#include "vtkParticleSpringSystemInterface.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"

using namespace std;

//!This test perform a test of the vtkToolDummy class

int TestvtkToolLaparoscopy(int argc, char * argv[])
{
  const char * fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick.vtp";
  const char * cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/stick.vtp";
  const char * tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/steel.jpg";

  const char * fnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp";
  const char * cfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Tools/Probe/tip.vtp";
  const char * tfnb ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/metal.jpg";

  //Generate tool's first element (stick)
  vtkSmartPointer<vtkVisualizationModel> vis = vtkSmartPointer<vtkVisualizationModel>::New();
  vis->SetName("stick_vis");
  vis->SetFileName(fn);
  vis->SetTextureFileName(tfn);
  vis->SetOpacity(1.0);
  vis->SetColor(1.0, 1.0, 1.0);

  vtkSmartPointer<vtkCollisionModel> col = vtkSmartPointer<vtkCollisionModel>::New();
  col->SetName("vtkbioeng");
  col->SetFileName(cfn);
  col->SetOpacity(0.5);
  col->SetColor(0.0, 0.0, 1.0);

  vtkSmartPointer<vtkScenarioElement> stick = vtkSmartPointer<vtkScenarioElement>::New();
  stick->SetId(0);
  stick->SetName("stick");
  stick->SetPosition(3.0, 0.0, 0.0);
  stick->SetOrientation(0, 0, -20);
  stick->SetVisualizationModel(vis);
  stick->SetCollisionModel(col);

  stick->Update();

  //Second element (ball)
  vtkSmartPointer<vtkVisualizationModel> visb = vtkSmartPointer<vtkVisualizationModel>::New();
  visb->SetName("ball_vis");
  visb->SetFileName(fnb);
  visb->SetTextureFileName(tfnb);
  visb->SetOpacity(1.0);
  visb->SetColor(1.0, 0.0, 1.0);

  vtkSmartPointer<vtkCollisionModel> colb = vtkSmartPointer<vtkCollisionModel>::New();
  colb->SetName("ball_vtkbioeng");
  colb->SetFileName(cfnb);
  colb->SetOpacity(0.5);
  colb->SetColor(0.0, 0.0, 1.0);

  vtkSmartPointer<vtkScenarioElement> ball = vtkSmartPointer<vtkScenarioElement>::New();
  ball->SetId(1);
  ball->SetName("ball");
  ball->SetPosition(3.0, 0.0, 0.0);
  ball->SetOrientation(0, 0, -20);
  ball->SetVisualizationModel(visb);
  ball->SetCollisionModel(colb);

  ball->Update();

  vtkSmartPointer<vtkToolDummy> tool = vtkSmartPointer<vtkToolDummy>::New();
  tool->AddElement(stick);
  tool->AddElement(ball);

  tool->Update();

  tool->Print(cout);

  return 0;
}


