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
#include "vtkSimulationManager.h"

vtkCxxRevisionMacro(vtkSimulationManager, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSimulationManager);

//--------------------------------------------------------------------------
vtkSimulationManager::vtkSimulationManager(){
	this->CollisionDetectionLibrary=0;
	this->Scenario = 0;
}

//--------------------------------------------------------------------------
vtkSimulationManager::~vtkSimulationManager(){
	if (CollisionDetectionLibrary!=0)
		this->CollisionDetectionLibrary->Delete();
}

//--------------------------------------------------------------------------
void vtkSimulationManager::Init()
{
	if (!strcmp(this->LibraryName, "vtkbioeng"))
	{
		this->CollisionDetectionLibrary = vtkBioEngInterface::New();
		this->CollisionDetectionLibrary->Init();
	}
	else
	{
		//In case vtkbioeng is not selected new libraries will be added here
	}

	//Scenario & collision detection library must be defined previously
	if(this->Scenario && this->CollisionDetectionLibrary)
	{
		vtkToolCollection * tools = this->Scenario->GetToolCollection();
		vtkOrganCollection * organs = this->Scenario->GetOrganCollection();

		this->CollisionDetectionLibrary->SetTools(tools);
		this->CollisionDetectionLibrary->SetOrgans(organs);
	}
}

//--------------------------------------------------------------------------
void vtkSimulationManager::Update()
{

	//Check if any collision between tools & organs has occurred
	this->CollisionDetectionLibrary->Update();

	//Update organs & tools after collision detection has been performed
	vtkContactCollection * contacts;
	vtkContact * contact;

	contacts = this->CollisionDetectionLibrary->GetContacts();
	contacts->InitTraversal();

	contact = contacts->GetNextContact();
	while(contact)
	{
		//Set organ contact point for deformation purposes
		vtkOrgan * organ = this->Scenario->GetOrgan(contact->GetOrganId());
		vtkTool * tool = this->Scenario->GetTool(contact->GetToolId());

		organ->InsertNextContact(contact);
		tool->InsertNextContact(contact);

		contact = contacts->GetNextContact();
	}

	//Update scenario
	this->Scenario->Update();
}

//--------------------------------------------------------------------------
vtkContactCollection * vtkSimulationManager::GetContacts() {
	return this->CollisionDetectionLibrary->GetContacts();
}

