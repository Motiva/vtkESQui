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
#include "vtkCollision.h"

#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

vtkCxxRevisionMacro(vtkCollision, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCollision);

//--------------------------------------------------------------------------
vtkCollision::vtkCollision() {
  this->CollisionType = ToolTool;
  this->ObjectIds = vtkIdList::New();
  this->ObjectIds->SetNumberOfIds(2);
  this->ElementIds = vtkIdList::New();
  this->ElementIds->SetNumberOfIds(2);
  this->PointIds = vtkIdList::New();
  this->PointIds->SetNumberOfIds(2);
  this->CellIds = vtkIdList::New();
  this->CellIds->SetNumberOfIds(2);
  this->Points = vtkPoints::New();
  this->Points->SetDataTypeToDouble();
  this->Points->SetNumberOfPoints(2);
}

//--------------------------------------------------------------------------
vtkCollision::~vtkCollision()
{
  this->ObjectIds->Delete();
  this->ElementIds->Delete();
  this->PointIds->Delete();
  this->CellIds->Delete();
  this->Points->Delete();
}

//--------------------------------------------------------------------------
void vtkCollision::SetObjectId(int i, int id)
{
  this->ObjectIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkCollision::GetObjectId(int i){
  return this->ObjectIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkCollision::SetElementId(int i, int id)
{
  this->ElementIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkCollision::GetElementId(int i){
  return this->ElementIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkCollision::SetPointId(int i, int id)
{
  this->PointIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkCollision::GetPointId(int i){
  return this->PointIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkCollision::SetSyncPointId(int i, int id)
{
  this->SyncPointIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkCollision::GetSyncPointId(int i){
  return this->SyncPointIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkCollision::SetPoint(int i, double x, double y, double z) {
  this->Points->SetPoint(i, x, y, z);
}

//--------------------------------------------------------------------------
void vtkCollision::SetPoint(int i, double point[3]) {
  this->SetPoint(i, point[0],point[1],point[2]);
}

//--------------------------------------------------------------------------
double * vtkCollision::GetPoint(int i){
  return this->Points->GetPoint(i);
}

//--------------------------------------------------------------------------
void vtkCollision::SetCellId(int i, vtkIdType value){
  this->CellIds->SetId(i, value);
}

//--------------------------------------------------------------------------
int vtkCollision::GetCellId(int i){
  return this->CellIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkCollision::DeepCopy(vtkCollision *info) {
  info->ObjectIds->DeepCopy(this->ObjectIds);
  info->ElementIds->DeepCopy(this->ElementIds);
  info->PointIds->DeepCopy(this->PointIds);
  info->SyncPointIds->DeepCopy(this->SyncPointIds);
  info->CellIds->DeepCopy(this->CellIds);
  info->Points->DeepCopy(this->Points);
  info->Distance = this->Distance;
  info->Displacement[0] = this->Displacement[0];
  info->Displacement[1] = this->Displacement[1];
  info->Displacement[2] = this->Displacement[2];
}

//--------------------------------------------------------------------------
void vtkCollision::PrintSelf(ostream&os, vtkIndent indent)
{
  os << indent << "CollisionType: " << this->CollisionType << endl;
  for(int i = 0; i< 2; i++)
  {
    os << indent << "Object["<< i <<"] Id: " << this->ObjectIds->GetId(i) << endl;
    os << indent << "Element["<< i <<"] Id: " << this->ElementIds->GetId(i) << endl;
    os << indent << "Cell[" << i <<"] Id: " << this->CellIds->GetId(i) << endl;
    os << indent << "Point[" << i <<"] Id: " << this->PointIds->GetId(i)<< endl;
    double * point = this->Points->GetPoint(i);
    os << indent << "Point[" << i <<"] Position: " << point[0] << ", " << point[1] << ", " << point[2] << endl;
  }
  os << indent << "Displacement: " << this->Displacement[0] << ", " << this->Displacement[1] << ", " << this->Displacement[2] << endl;
  os << indent << "Distance: " << this->Distance << endl;
}
