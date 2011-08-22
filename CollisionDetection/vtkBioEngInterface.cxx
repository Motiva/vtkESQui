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
#include "vtkIdList.h"
#include "vtkCell.h"
#include "vtkPoints.h"
#include "vtkPointLocator.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"

#include "vtkCollisionDetectionFilter.h"
#include "vtkCollisionModel.h"
#include "vtkModelCollection.h"
#include "vtkCollision.h"
#include "vtkCollisionCollection.h"

vtkCxxRevisionMacro(vtkBioEngInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioEngInterface);

//----------------------------------------------------------------------------
vtkBioEngInterface::vtkBioEngInterface(){

	this->DetectionFilter = NULL;
	this->Matrix0 = NULL;
	this->Matrix1 = NULL;
}

//--------------------------------------------------------------------------
vtkBioEngInterface::~vtkBioEngInterface()
{
	if(this->Matrix0) this->Matrix0->Delete();
	if(this->Matrix1) this->Matrix1->Delete();

	if(this->Collisions) this->Collisions->Delete();

	if(this->DetectionFilter) this->DetectionFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Init()
{
	//Create internal class objects
	this->DetectionFilter = vtkCollisionDetectionFilter::New();
	//Transformation/Rotation matrixes
	this->Matrix0 = vtkMatrix4x4::New();
	this->Matrix1 = vtkMatrix4x4::New();

	//Create collision collection object
	this->Collisions = vtkCollisionCollection::New();

	//Initial setup of the collision detection parameters
	this->DetectionFilter->SetBoxTolerance(0.0);
	this->DetectionFilter->SetCellTolerance(0.0);
	this->DetectionFilter->SetNumberOfCellsPerNode(2);
	//this->DetectionFilter->SetCollisionModeToFirstCollision();
	this->DetectionFilter->SetCollisionModeToHalfContacts();
	//this->DetectionFilter->SetCollisionModeToAllCollisions();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Update()
{
	//Reset from previous executions
	this->Reset();

	for(vtkIdType id = 0; id < this->Models->GetNumberOfItems(); id++)
	{
		vtkCollisionModel * m0 = vtkCollisionModel::SafeDownCast(this->Models->GetModel(id));
		for(vtkIdType jd = id; jd < this->Models->GetNumberOfItems(); jd++)
		{
			//Avoid checking collision between same element
			if(id != jd)
			{
				vtkCollisionModel * m1 = vtkCollisionModel::SafeDownCast(this->Models->GetModel(jd));
				//Avoid collisions between same object elements
				if((m0->GetObjectId() != m1->GetObjectId())
						&& (!m0->IsDisabled() && !m1->IsDisabled()))
				{
					//Each collision model transformed polydata (mx-GetOutput(1) is set as an input of the CDL
					this->DetectionFilter->SetInput(0, m0->GetOutput(1));
					this->DetectionFilter->SetInput(1, m1->GetOutput(1));

					//Transformation matrixes
					this->DetectionFilter->SetMatrix(0, Matrix0);
					this->DetectionFilter->SetMatrix(1, Matrix1);

					this->DetectionFilter->Update();

					vtkIdType numberOfCollisions = this->DetectionFilter->GetNumberOfContacts();

					//Calculate displacement from object velocity
					double d[3];
					m0->GetVelocity(d);
					vtkMath::MultiplyScalar(d, m0->GetDeltaT());
					//Use second collided model (m1) if the first one (m0) is static
					if(vtkMath::Norm(d) == 0)
					{
						m1->GetVelocity(d);
						vtkMath::MultiplyScalar(d, m1->GetDeltaT());
					}

					for(int i =0; i < numberOfCollisions; i++)
					{
						vtkIdList * pointIds = vtkIdList::New();
						vtkIdList * cellIds = vtkIdList::New();
						cellIds->SetNumberOfIds(2);
						pointIds->SetNumberOfIds(2);

						//There has been a collision. A new collision will be created, filling the collision info
						cellIds->SetId(0, this->DetectionFilter->GetContactCells(0)->GetValue(i));
						cellIds->SetId(1, this->DetectionFilter->GetContactCells(1)->GetValue(i));

						//Get both point ids and coordinates from collision meshes
						//Note: mx->GetOutput(1)
						vtkPoints * points0 = m0->GetOutput(1)->GetCell(cellIds->GetId(0))->GetPoints();
						vtkIdList * pointIds0 = m0->GetOutput(1)->GetCell(cellIds->GetId(0))->GetPointIds();
						vtkPoints * points1 = m1->GetOutput(1)->GetCell(cellIds->GetId(1))->GetPoints();
						vtkIdList * pointIds1 = m1->GetOutput(1)->GetCell(cellIds->GetId(1))->GetPointIds();

						//Find closest points in cells
						double dmin = 10e6;
						for(int j=0;j<points0->GetNumberOfPoints(); j++)
						{
							//const double * p0 = points0->GetPoint(j);
							//cout << "p0["<<j<<"]: (" << p0[0] << ", " << p0[1] << ", "  << p0[2] << ")\n";
							for(int k=j;k<points1->GetNumberOfPoints();k++)
							{
								//const double * p1 = points1->GetPoint(k);
								//cout << "p1["<<k<<"]: (" << p1[0] << ", " << p1[1] << ", "  << p1[2] << ")\n";
								double dist = vtkMath::Distance2BetweenPoints(points0->GetPoint(j), points1->GetPoint(k));
								//cout << dist << endl;
								if (dist<dmin)
								{
									pointIds->SetId(0,j);
									pointIds->SetId(1,k);
									dmin=dist;
								}
							}
						}

						//New collision is created
						vtkCollision * collision = vtkCollision::New();

						//Now only tool-organ collisions are being checked
						collision->SetCollisionType(vtkCollision::ToolOrgan);

						//Set scenario object ids
						collision->SetObjectId(0, m0->GetObjectId());
						collision->SetObjectId(1, m1->GetObjectId());

						//Set scenario element ids
						collision->SetElementId(0, m0->GetId());
						collision->SetElementId(1, m1->GetId());

						//Tool cell point
						collision->SetCellId(0, cellIds->GetId(0));
						//Set visualization (hashmap) point id
						collision->SetPointId(0, pointIds0->GetId(pointIds->GetId(0)));
						collision->SetPoint(0, points0->GetPoint(pointIds->GetId(0)));

						//Organ cell point
						collision->SetCellId(1, cellIds->GetId(1));
						//Set visualization (hashmap) point id
						collision->SetPointId(1, pointIds1->GetId(pointIds->GetId(1)));
						collision->SetPoint(1, points1->GetPoint(pointIds->GetId(1)));

						//Set distance between points
						collision->SetDistance(dmin);
						//Set displacement
						collision->SetDisplacement(d);

						// Find collision in the collection whenever the collision has a displacement
						if(vtkMath::Norm(d) > 0 && m1->GetCollisions()->FindCollision(collision) < 0)
						{
							//Not found -> Insert collision in the model
							m0->AddCollision(collision);
							m1->AddCollision(collision);
							//Insert global collision
							this->Collisions->InsertNextCollision(collision);
						}
						else
						{
							//Found -> Delete copy
							collision->Delete();
						}

						pointIds->Delete();
						cellIds->Delete();
					}
				}
			}
		}
	}
}

