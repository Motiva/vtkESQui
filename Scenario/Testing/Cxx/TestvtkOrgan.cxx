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

//!This test perform a test of the vtkOrgan class

int TestvtkOrgan(int argc, char * argv[])
{
  const char * fn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";
  const char * cfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp";
  const char * dfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_def_c12.vtp";
  const char * tfn ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Textures/muscle.jpg";

  vtkSmartPointer<vtkVisualizationModel> vis = vtkSmartPointer<vtkVisualizationModel>::New();
  vis->SetName("ellipsoid_16_16_1");
  vis->SetFileName(fn);
  vis->SetTextureFileName(tfn);
  vis->SetOpacity(0.5);
  vis->SetColor(1.0, 1.0, 1.0);

  vtkSmartPointer<vtkCollisionModel> col = vtkSmartPointer<vtkCollisionModel>::New();
  col->SetName("vtkbioeng");
  col->SetFileName(cfn);
  col->SetOpacity(0.5);
  col->SetColor(0.0, 0.0, 1.0);

  //Deformation model. Particle-Spring system
  vtkSmartPointer<vtkParticleSpringSystemInterface> def = vtkSmartPointer<vtkParticleSpringSystemInterface>::New();
  def->SetName("ParticleSpring");
  def->SetFileName(dfn);
  def->SetOpacity(1.0);
  def->SetColor(0.0, 1.0, 0.0);

  vtkSmartPointer<vtkScenarioElement> element = vtkSmartPointer<vtkScenarioElement>::New();
  element->SetId(0);
  element->SetName("ellipsoid");
  element->SetPosition(3.0, 2.5, 0.0);
  element->SetOrientation(25, -15, 30);
  element->SetVisualizationModel(vis);
  element->SetCollisionModel(col);
  element->SetDeformationModel(def);

  element->Update();
  element->RotateZ(35);

  vtkSmartPointer<vtkOrgan> organ = vtkSmartPointer<vtkOrgan>::New();
  organ->AddElement(element);

  organ->Print(cout);

  return 0;
}


