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
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkCleanPolyData.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkExtractSelectedPolyDataIds.h"
#include "vtkSelectEnclosedPoints.h"
#include "vtkCubeSource.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"
#include "vtkActor.h"

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
}

//--------------------------------------------------------------------------
vtkBioEngInterface::~vtkBioEngInterface()
{
  if(this->DetectionFilter) this->DetectionFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Initialize()
{
  //Create internal class objects
  this->DetectionFilter = vtkCollisionDetectionFilter::New();

  //Initial setup of the collision detection parameters
  this->BoxTolerance = 0.0;
  this->CellTolerance = 0.0;
  this->NumberOfCellsPerNode = 2;
  //this->DetectionFilter->SetCollisionModeToFirstContact();
  this->DetectionFilter->SetCollisionModeToAllContacts();
  //this->DetectionFilter->SetCollisionModeToHalfContacts();
  this->DetectionFilter->GenerateScalarsOff();
}

//--------------------------------------------------------------------------
void vtkBioEngInterface::Update()
{
  //Reset from previous executions
  this->Reset();
  this->DetectionFilter->SetBoxTolerance(this->BoxTolerance);
  this->DetectionFilter->SetCellTolerance(this->CellTolerance);
  this->DetectionFilter->SetNumberOfCellsPerNode(this->NumberOfCellsPerNode);
  //Process each active model for every iteration
  for(vtkIdType i = 0; i < this->Models->GetNumberOfItems(); i++)
  {
    vtkCollisionModel * m0 = vtkCollisionModel::SafeDownCast(this->Models->GetModel(i));
    for(vtkIdType j = i; j < this->Models->GetNumberOfItems(); j++)
    {
      //Avoid checking collision between same element
      if(i != j)
      {
        vtkCollisionModel * m1 = vtkCollisionModel::SafeDownCast(this->Models->GetModel(j));
        //Avoid collisions between same object elements
        if((m0->GetObjectId() != m1->GetObjectId())
            && (!m0->IsDisabled() && !m1->IsDisabled()))
        {
          bool check = false;
          if(this->Mode == Simple)
          {

            //Check if m0 is tool and m1 is an organ model and viceversa
            if ((m0->GetObjectType() == vtkModel::Tool &&
                m1->GetObjectType() == vtkModel::Organ) ||
                (m1->GetObjectType() == vtkModel::Tool &&
                    m0->GetObjectType() == vtkModel::Organ))
            {
              check = true;
            }
          }
          else if(this->Mode == Extended)
          {
            //Check:
            // * m0 is tool and m1 is organ and viceversa
            // * m0 is tool and m1 is tool
            if ((m0->GetObjectType() == vtkModel::Tool &&
                m1->GetObjectType() == vtkModel::Organ) ||
                (m1->GetObjectType() == vtkModel::Tool &&
                    m0->GetObjectType() == vtkModel::Organ) ||
                    (m1->GetObjectType() == vtkModel::Tool &&
                        m0->GetObjectType() == vtkModel::Tool))
            {
              check = true;
            }
          }
          else check = true;

          if(check)
          {
            //Each collision model polydata is set as an input of the filter
            this->DetectionFilter->SetInput(0, m0->GetOutput());
            this->DetectionFilter->SetInput(1, m1->GetOutput());

            //Set model transformation matrixes
            this->DetectionFilter->SetMatrix(0, m0->GetActor()->GetMatrix());
            this->DetectionFilter->SetMatrix(1, m1->GetActor()->GetMatrix());

            this->DetectionFilter->Update();

            //Contact intersecting points
            vtkPolyData * contacts = this->DetectionFilter->GetContactsOutput();
            
            //Compute contact distances
            vtkDoubleArray * distances = vtkDoubleArray::New();
            distances->SetNumberOfComponents(1);
            double p0[3];
            double p1[3];

            vtkIdList * pids = vtkIdList::New();
            //Process contact points
            for (int k = 0;k < contacts->GetNumberOfCells();k++){
              contacts->GetCellPoints(k, pids);
              //Cell intersection point
              contacts->GetPoint(pids->GetId(0), p0);
              contacts->GetPoint(pids->GetId(1), p1);
              //Compute line distance between cell contact points
              double d = sqrt(vtkMath::Distance2BetweenPoints(p0, p1));
              distances->InsertNextTuple1(d);
            }
            pids->Delete();

            //For each model extract contact points
            for (int k=0;k<2;k++)
            {
              //Select model
              vtkCollisionModel * model = m0;
              if(k==1) model = m1;
              //
              model->RemoveAllCollisions();
              //Model polydata
              vtkPolyData * pd = this->DetectionFilter->GetOutput(k);
              //Point ids
              vtkIdList * pids = vtkIdList::New();
              vtkIdTypeArray * ps = vtkIdTypeArray::New();
              //Cell points
              vtkIdList * cps = vtkIdList::New();
              //Polydata cell ids
              vtkIdTypeArray * cids = this->DetectionFilter->GetContactCells(k);
              //Cell & point normals
              vtkFloatArray * cns = model->GetCellNormals();
              vtkFloatArray * pns = model->GetCellNormals();
              //TODO: Reduce selection to central points only 
              
              /*              
              if(contacts->GetNumberOfCells() > 0)
              {
                vtkSelectionNode * snode = vtkSelectionNode::New();
                snode->SetFieldType(vtkSelectionNode::CELL);
                snode->SetContentType(vtkSelectionNode::INDICES);
                snode->SetSelectionList(cids);
                   
                vtkSelection * selection = vtkSelection::New();
                selection->AddNode(snode);
              
                vtkExtractSelectedPolyDataIds * selids = vtkExtractSelectedPolyDataIds::New();
                selids->SetInput(0, pd);
                selids->SetInput(1, selection);
                selids->Update();

                vtkPolyData * sel = selids->GetOutput();
                cout << sel->GetNumberOfCells() << ":" << sel->GetNumberOfPoints() << endl;
                double bounds[6];
                sel->GetBounds(bounds);
                cout << bounds[0] <<","<< bounds[1]<<","<<bounds[2] <<","<<bounds[3]<<","<<bounds[4] <<","<< bounds[5] << endl;
                vtkMath::MultiplyScalar(bounds,0.9);
                cout << bounds[0] <<","<< bounds[1]<<","<<bounds[2] <<","<<bounds[3]<<","<<bounds[4] <<","<< bounds[5] << endl;
                double center[3];
                center[0] = bounds[0]+(bounds[1]-bounds[0])/2;
                center[1] = bounds[2]+(bounds[3]-bounds[2])/2;
                center[2] = bounds[4]+(bounds[5]-bounds[4])/2;
                cout << center[0] <<","<< center[1]<<","<<center[2] <<"\n";

                vtkCubeSource * cube = vtkCubeSource::New();
                cube->SetCenter(center);
                cube->SetXLength(bounds[1]-bounds[0]);
                cube->SetYLength(bounds[3]-bounds[2]);
                cube->SetZLength(bounds[5]-bounds[4]);
                cube->Update();

                vtkSelectEnclosedPoints * enclosed = vtkSelectEnclosedPoints::New();
                enclosed->SetInput(pd);
                enclosed->SetSurface(cube->GetOutput());
                enclosed->Update();
                
                vtkDataArray* insideArray = vtkDataArray::SafeDownCast(enclosed->GetOutput()->GetPointData()->GetArray("SelectedPoints"));
              
                cout << insideArray->GetNumberOfTuples() << endl;

              }*/
              

              //sel->Print(cout);
              
              //Loop all over cell ids to get point ids
              for (int l=0;l<cids->GetNumberOfTuples();l++)
              {
                //Cell id
                int cid = cids->GetTuple1(l);
                cout << "\n[" <<cid << "]:";
                //Cell normal
                double * cn = cns->GetTuple3(cid);
                //Cell point ids
                pd->GetCellPoints(cid, pids);
                for (int lk=0;lk<3;lk++)
                {
                  int pid = pids->GetId(lk);
                  cout << "," << pid;
                  //Collision point id not present
                  if (cps->IsId(pid) == -1)
                  {
                    cps->InsertUniqueId(pid);
                    //ps->InsertNextValue(pid);
                    cout << "(i)";
                    //Point normal
                    double * pn = pns->GetTuple3(pid);
                    double d = distances->GetTuple1(l);
                    //Calculate displacement
                    double disp[3];
                    disp[0] = cn[0];
                    disp[1] = cn[1];
                    disp[2] = cn[2];
                    vtkMath::MultiplyScalar(disp, d);

                    //Collision information
                    vtkCollision * c = vtkCollision::New();
                    c->SetModelId(model->GetId());
                    c->SetObjectId(model->GetObjectId());
                    c->SetCellId(cid);
                    c->SetCellNormal(cn); 
                    c->SetPointId(pid);
                    c->SetPoint(pd->GetPoint(pid));
                    c->SetPointNormal(pn); 
                    c->SetDistance(d);
                    c->SetPointDisplacement(disp);

                    //Add collision to the model
                    //c->Print(cout);
                    model->AddCollision(c);
                  }
                }
              }
                                              
            }
            //Add collision pair
            if(m0->GetNumberOfCollisions() > 0 || m1->GetNumberOfCollisions() > 0)
            {
              this->InsertNextCollisionPair(m0->GetObjectId(), m1->GetObjectId());
            }
          }
          //cout << "Collisions: m0["<< m0->GetNumberOfCollisions() << "]:m1[" << m1->GetNumberOfCollisions()<<"]/t["<<this->CollisionPairs->GetNumberOfTuples() << "]\n";
        }
      }
    }
  }
}

