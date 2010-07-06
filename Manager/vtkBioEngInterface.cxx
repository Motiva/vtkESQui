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
#include "vtkBioEngInterface.h"

vtkCxxRevisionMacro(vtkBioEngInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioEngInterface);

//----------------------------------------------------------------------------
vtkBioEngInterface::vtkBioEngInterface(){

	this->DetectionFilter = vtkCollisionDetectionFilter::New();
	this->Matrix0 = vtkMatrix4x4::New();
	this->Matrix1 = vtkMatrix4x4::New();
	//Create contact collection object
	this->Contacts = vtkContactCollection::New();

}

//--------------------------------------------------------------------------
vtkBioEngInterface::~vtkBioEngInterface()
{
	this->Clear();

	this->Matrix0->Delete();
	this->Matrix1->Delete();

	this->Contacts->Delete();

	this->DetectionFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Init()
{
	//Initial setup of the collision detection parameters
	this->DetectionFilter->SetBoxTolerance (0.0);
	this->DetectionFilter->SetCellTolerance (0.0);
	this->DetectionFilter->SetNumberOfCellsPerBucket(2);
	this->DetectionFilter->SetCollisionModeToFirstContact();
	//this->DetectionFilter->SetCollisionModeToHalfContacts();
	//this->DetectionFilter->SetCollisionModeToAllContacts();
	//Debug. Display contact cells.
	this->DetectionFilter->SetGenerateScalars(1);
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Clear()
{
	//Clear contacts from previous executions
	this->Contacts->InitTraversal();
	vtkContact * contact = this->Contacts->GetNextContact();
	while(contact)
	{
		contact->Delete();
		contact = this->Contacts->GetNextContact();
	}
	this->Contacts->RemoveAllItems();

}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Update()
{
	//Get the organs and tools from the collections. Set Inputs of the collision library
	//The organ (converted to vtkPolydata) and a bounding box of the tool. This bounding box (polydata)
	//is returned by a method created in vtkTool

	//Clear from previous executions
	this->Clear();

	for (vtkIdType organId=0; organId < this->Organs->GetNumberOfItems(); organId++)
	{
		vtkOrgan * organ = this->Organs->GetOrgan(organId);
		vtkPolyData * organBox = organ->GetSimpleMesh();

		for (vtkIdType toolId=0; toolId < this->Tools->GetNumberOfItems(); toolId++)
		{
			vtkTool * tool =  this->Tools->GetTool(toolId);
			//Both grasper shall be set as Collision Detection inputs
			vtkPolyData * toolBox = tool->GetSimpleMesh();

			//Each organ bounding box is set as an input of the CDL
			this->DetectionFilter->SetInput(0, organBox);

			//Tool bounding box is set as CDL input
			this->DetectionFilter->SetInput(1, toolBox);

			//Transformation matrixes
			this->DetectionFilter->SetMatrix(0, Matrix0);
			this->DetectionFilter->SetMatrix(1, Matrix1);

			this->DetectionFilter->Update();

			//Detection Filter is set to return the first contact only
			vtkIdType numberOfContacts = this->DetectionFilter->GetNumberOfContacts();

			for(int i =0; i < numberOfContacts; i++)
			{
				//There has been a collision
				//A new contact will be created, filling the info required
				vtkContact *contact = vtkContact::New();

				//Set organ & tool ids
				contact->SetOrganId(organId);
				contact->SetToolId(toolId);

				vtkIdType cellId = this->DetectionFilter->GetContactCells(0)->GetValue(i);
				vtkIdType pointId = organBox->GetCell(cellId)->GetPointId(0);
				double * p0 = organBox->GetCell(cellId)->GetPoints()->GetPoint(0);

				contact->InsertPointId(0, pointId);
				contact->InsertPoint(0, p0);
				contact->InsertCellId(0, cellId);

				cellId = this->DetectionFilter->GetContactCells(1)->GetValue(i);
				pointId = toolBox->GetCell(cellId)->GetPointId(0);
				double * p1 = toolBox->GetCell(cellId)->GetPoints()->GetPoint(0);

				contact->InsertPointId(1, pointId);
				contact->InsertPoint(1, p1);
				contact->InsertCellId(1, cellId);

				//Compute distance vector between tool and organ points
				double vector[3];
				vtkMath::Subtract(p1, p0, vector);
				double n = vtkMath::Norm(vector);
				double dir[3];
				tool->GetDirection(dir);
				vtkMath::MultiplyScalar(dir, n);
				contact->SetDirectionVector(dir);

				if(!this->Contacts->ContainsContact(contact))
				{
					cout << "#######################################\n";
					this->Contacts->InsertNextContact(contact);
					contact->Print(cout);
				}
				else
				{
					contact->Delete();
				}
			}
		}
	}
}



