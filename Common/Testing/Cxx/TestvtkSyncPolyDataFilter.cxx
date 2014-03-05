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
#include <iostream>
#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkSyncPolyDataFilter.h"

using namespace std;

//!This test perform a test of the vtkContactCollection class

int TestvtkSyncPolyDataFilter(int argc, char * argv[])
{
  const char * fni ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball_col.vtp";
  const char * fns ="/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Organs/ball.vtp";

  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(fni);
  reader->Update();

  vtkSmartPointer<vtkXMLPolyDataReader> reader2 = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader2->SetFileName(fns);
  reader2->Update();

  vtkSmartPointer<vtkSyncPolyDataFilter> sync = vtkSmartPointer<vtkSyncPolyDataFilter>::New();
  sync->SetInput(reader->GetOutput());
  sync->SetSource(reader2->GetOutput());
  sync->SmoothingOn();

  // A call to update method is made to force the sync to be at its last state
  sync->Update();

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->SetSize(500,500);
  renWin->AddRenderer(renderer);

  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper->SetInput(sync->GetOutput());
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkActor> actor2 =
      vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkPolyDataMapper> mapper2 =
      vtkSmartPointer<vtkPolyDataMapper>::New();

  mapper2->SetInput(reader->GetOutput());
  actor2->SetMapper(mapper2);
  actor2->GetProperty()->SetColor(1, 0.5, 0.5);
  actor2->GetProperty()->SetRepresentationToWireframe();

  renderer->AddActor(actor);
  renderer->AddActor(actor2);
  renderer->SetBackground(1,1,1);

  renderer->ResetCamera();
  iren->Initialize();

  renWin->Render();

  iren->Start();

  return 0;
}


