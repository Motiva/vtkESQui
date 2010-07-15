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
	this->DetectionFilter->SetBoxTolerance(0.0);
	this->DetectionFilter->SetCellTolerance(0.0);
	this->DetectionFilter->SetNumberOfCellsPerNode(2);
	this->DetectionFilter->SetCollisionModeToFirstContact();
	//this->DetectionFilter->SetCollisionModeToHalfContacts();
	//this->DetectionFilter->SetCollisionModeToAllContacts();
	//Debug. Display contact cells.
	this->DetectionFilter->GenerateScalarsOn();
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
	double p0[3], p1[3], p2[3], n[3], c0[3], c1[3];

	//Clear from previous executions
	this->Clear();

	for (vtkIdType organId=0; organId < this->Organs->GetNumberOfItems(); organId++)
	{
		vtkOrgan * organ = this->Organs->GetOrgan(organId);
		vtkPolyData * organBox = organ->GetOutput();

		for (vtkIdType toolId=0; toolId < this->Tools->GetNumberOfItems(); toolId++)
		{
			vtkTool * tool =  this->Tools->GetTool(toolId);
			//Whole tool shall be set as Collision Detection inputs
			vtkPolyData * toolBox = tool->GetOutput();

			//Each organ polydata is set as an input of the CDL
			this->DetectionFilter->SetInput(0, organBox);

			//Tool bounding box is set as CDL input
			this->DetectionFilter->SetInput(1, toolBox);

			//Transformation matrixes
			this->DetectionFilter->SetMatrix(0, Matrix0);
			this->DetectionFilter->SetMatrix(1, Matrix1);

			this->DetectionFilter->Update();

			vtkIdType numberOfContacts = this->DetectionFilter->GetNumberOfContacts();

			for(int i =0; i < numberOfContacts; i++)
			{
				//There has been a collision
				//A new contact will be created, filling the collision info
				vtkIdType organCellId = this->DetectionFilter->GetContactCells(0)->GetValue(i);
				vtkIdType toolCellId = this->DetectionFilter->GetContactCells(1)->GetValue(i);

				//Get cell normal & center
				if(organBox->GetCellType(organCellId) == VTK_TRIANGLE &&
					(toolBox->GetCellType(toolCellId) == VTK_TRIANGLE ||
					toolBox->GetCellType(toolCellId) == VTK_TRIANGLE_STRIP))
				{
					//Organ cell
					//vtkPoints * organPoints = organBox->GetCell(organCellId)->GetPoints();
					vtkPoints * organPoints = this->DetectionFilter->GetOutput(0)->GetCell(organCellId)->GetPoints();
					organPoints->GetPoint(0,p0);
					organPoints->GetPoint(1,p1);
					organPoints->GetPoint(2,p2);

					//cell normal
					//vtkTriangle::ComputeNormal(p0, p1, p2, n);
					//cell center
					vtkTriangle::TriangleCenter(p0, p1, p2 ,c0);

					//Tool cell
					//vtkPoints * toolPoints = toolBox->GetCell(organCellId)->GetPoints();
					vtkPoints * toolPoints = this->DetectionFilter->GetOutput(1)->GetCell(toolCellId)->GetPoints();
					toolPoints->GetPoint(0,p0);
					toolPoints->GetPoint(1,p1);
					toolPoints->GetPoint(2,p2);

					//cell center
					vtkTriangle::TriangleCenter(p0, p1, p2 ,c1);

					//vector between points (Direction)
					vtkMath::Subtract(c0, c1, n);

					// 1 contact per cell point
					for (int j=0;j<2;j++)
					{
						vtkContact *contact = vtkContact::New();

						//Set organ & tool ids
						contact->SetOrganId(organId);
						contact->SetToolId(toolId);
						//Organ cell point
						contact->InsertPointId(0, organBox->GetCell(organCellId)->GetPointId(j));
						contact->InsertPoint(0, organPoints->GetPoint(j));
						contact->InsertCellId(0, organCellId);
						//Tool cell point
						contact->InsertPointId(1, toolBox->GetCell(toolCellId)->GetPointId(j));
						contact->InsertPoint(1, toolPoints->GetPoint(j));
						contact->InsertCellId(1, toolCellId);

						contact->SetDirectionVector(n);

						//
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
	}
}

//------------------------------------------------------------------------------
vtkPolyData * vtkBioEngInterface::GetContactSurface()
{
	return this->DetectionFilter->GetContactsOutput();
}

