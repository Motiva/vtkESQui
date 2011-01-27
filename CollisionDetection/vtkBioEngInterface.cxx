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

#include "vtkObjectFactory.h"
#include "vtkPolyDataCollection.h"
#include "vtkPoints.h"
#include "vtkGenericCell.h"
#include "vtkTriangle.h"
#include "vtkCell.h"
#include "vtkPointLocator.h"
#include "vtkSelectEnclosedPoints.h"
#include "vtkMatrix4x4.h"

#include "vtkCollisionDetectionFilter.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkBioEngInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioEngInterface);

//----------------------------------------------------------------------------
vtkBioEngInterface::vtkBioEngInterface(){

	this->DetectionFilter = NULL;
	this->Matrix0 = NULL;
	this->Matrix1 = NULL;
	this->Contacts = NULL;

}

//--------------------------------------------------------------------------
vtkBioEngInterface::~vtkBioEngInterface()
{
	this->Reset();

	if(this->Matrix0) this->Matrix0->Delete();
	if(this->Matrix1) this->Matrix1->Delete();

	if(this->Contacts) this->Contacts->Delete();

	if(this->DetectionFilter) this->DetectionFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Init()
{
	//Create internal class objects
	this->DetectionFilter = vtkCollisionDetectionFilter::New();
	this->Matrix0 = vtkMatrix4x4::New();
	this->Matrix1 = vtkMatrix4x4::New();

	//Create contact collection object
	this->Contacts = vtkContactCollection::New();

	//Initial setup of the collision detection parameters
	this->DetectionFilter->SetBoxTolerance(0.0);
	this->DetectionFilter->SetCellTolerance(0.0);
	this->DetectionFilter->SetNumberOfCellsPerNode(2);
	//this->DetectionFilter->SetCollisionModeToFirstContact();
	this->DetectionFilter->SetCollisionModeToHalfContacts();
	//this->DetectionFilter->SetCollisionModeToAllContacts();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Reset()
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

//FIXME: Add Tool-Tool contact

//--------------------------------------------------------------------------
void vtkBioEngInterface::Update()
{
	//Clear from previous executions
	this->Reset();

	for (vtkIdType toolId = 0; toolId < this->Tools->GetNumberOfItems(); toolId++)
	{
		vtkTool * tool =  this->Tools->GetTool(toolId);
		vtkPolyData * toolPolyData = tool->GetOutput();

		for (vtkIdType organId = 0; organId < this->Organs->GetNumberOfItems(); organId++)
		{
			vtkOrgan * organ = this->Organs->GetOrgan(organId);
			vtkPolyData * organPolyData = organ->GetOutput();

			//Each organ polydata is set as an input of the CDL
			this->DetectionFilter->SetInput(0, toolPolyData);

			//Tool bounding box is set as CDL input
			this->DetectionFilter->SetInput(1, organPolyData);

			//Transformation matrixes
			this->DetectionFilter->SetMatrix(0, Matrix0);
			this->DetectionFilter->SetMatrix(1, Matrix1);

			this->DetectionFilter->Update();

			vtkIdType numberOfContacts = this->DetectionFilter->GetNumberOfContacts();

			for(int i =0; i < numberOfContacts; i++)
			{
				vtkIdList * pointIds = vtkIdList::New();
				vtkIdList * cellIds = vtkIdList::New();
				cellIds->SetNumberOfIds(2);
				pointIds->SetNumberOfIds(2);

				//There has been a collision. A new contact will be created, filling the collision info
				cellIds->SetId(0, this->DetectionFilter->GetContactCells(0)->GetValue(i));
				cellIds->SetId(1, this->DetectionFilter->GetContactCells(1)->GetValue(i));

				//Calculate displacement from tool velocity
				double d[3];
				tool->GetVelocity(d);
				vtkMath::MultiplyScalar(d, tool->GetDeltaT());

				//Get both tool & organ point ids and coordinates
				vtkPoints * toolPoints = toolPolyData->GetCell(cellIds->GetId(0))->GetPoints();
				vtkIdList * toolPointIds = toolPolyData->GetCell(cellIds->GetId(0))->GetPointIds();
				vtkPoints * organPoints = organPolyData->GetCell(cellIds->GetId(1))->GetPoints();
				vtkIdList * organPointIds = organPolyData->GetCell(cellIds->GetId(1))->GetPointIds();

				//TODO:Get closest point
				//vtkPolyData * contactData = this->DetectionFilter->GetContactsOutput();
				//double * cp = contactData->GetPoints()->GetPoint(i);
				vtkIdType closestPoint =  0;

				//for each point of the 2nd item a contact will be created
				for(int j=0;j<organPoints->GetNumberOfPoints(); j++)
				{
					//New contact is created
					vtkContact *contact = vtkContact::New();

					//Now only tool-organ contacts are being checked
					contact->SetContactType(vtkContact::ToolOrgan);

					//Set scenario item ids
					contact->SetItemId(0, tool->GetId());
					contact->SetItemId(1, organ->GetId());

					//Tool cell point
					contact->SetCellId(0, cellIds->GetId(0));
					contact->SetPointId(0, toolPointIds->GetId(closestPoint));
					contact->SetPoint(0, toolPoints->GetPoint(closestPoint));

					//Organ cell point
					contact->SetCellId(1, cellIds->GetId(1));
					contact->SetPointId(1, organPointIds->GetId(j));
					contact->SetPoint(1, organPoints->GetPoint(j));

					//Set distance between points
					contact->SetDistance(vtkMath::Distance2BetweenPoints(contact->GetPoint(0), contact->GetPoint(1)));
					contact->SetDisplacement(d);

					// Find contact in the collection
					if(this->Contacts->FindContact(contact) < 0)
					{
						//Not found -> Insert contact
						this->Contacts->InsertNextContact(contact);
					}
					else
					{
						//Found -> Delete copy
						contact->Delete();
					}
				}

				pointIds->Delete();
				cellIds->Delete();

			}
		}
	}
}

//------------------------------------------------------------------------------
vtkPolyData * vtkBioEngInterface::GetContactSurface()
{
	return this->DetectionFilter->GetContactsOutput();
}

