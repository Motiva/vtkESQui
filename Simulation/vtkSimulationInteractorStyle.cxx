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

#include "vtkSimulationInteractorStyle.h"

#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"


#include "vtkTool.h"
#include "vtkToolPincers.h"

vtkCxxRevisionMacro(vtkSimulationInteractorStyle, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulationInteractorStyle);

//----------------------------------------------------------------------------

vtkSimulationInteractorStyle::vtkSimulationInteractorStyle()
{
	this->Scenario = NULL;
	this->Mode = 0;
	this->ActiveToolId = 0;
}

//--------------------------------------------------------------------------
vtkSimulationInteractorStyle::~vtkSimulationInteractorStyle()
{
}

//--------------------------------------------------------------------------
void vtkSimulationInteractorStyle::OnKeyPress()
{
	//get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();

	vtkTool * t = this->Scenario->GetTool(this->ActiveToolId);

	//TODO: Make this method generic for any tool
	vtkToolPincers * tool = vtkToolPincers::SafeDownCast(t);

	//handle a "normal" key
	if(key.compare("c") == 0)
	{
		this->Mode = !this->Mode;
		cout << "Camera selection/deselection: " << this->Mode << endl;
	}
	if(this->Mode)
	{
		//Tool Movement
		if(key.compare("Up") == 0)
		{
			tool->RotateX(1);
		}
		if(key.compare("Down") == 0)
		{
			tool->RotateX(-1);
		}
		if(key.compare("Left") == 0)
		{
			tool->RotateY(1);
		}
		if(key.compare("Right") == 0)
		{
			tool->RotateY(-1);
		}
		if(key.compare("Prior") == 0)
		{
			tool->Push();
		}
		if(key.compare("Next") == 0)
		{
			tool->Pull();
		}
		if(key.compare("i") == 0)
		{
			this->ActiveToolId = 0;
		}
		if(key.compare("d") == 0)
		{
			this->ActiveToolId = 1;
		}
		if(key.compare("a") == 0)
		{
			cout << "Open Tool" << endl;
			tool->Open();
		}
		if(key.compare("s") == 0)
		{
			cout << "Close Tool" << endl;
			tool->Close();
		}
		if(key.compare("x") == 0)
		{
			cout << "Rotate Tool Left" << endl;
			tool->RotateZ(-10);
		}
		if(key.compare("z") == 0)
		{
			cout << "Rotate Tool Right" << endl;
			tool->RotateZ(10);
		}
	}
	if(key.compare("q") == 0)
	{
		cout << "Exit" << endl;
		exit(0);
	}

	// forward events
	vtkInteractorStyleTrackballCamera::OnKeyPress();
}

