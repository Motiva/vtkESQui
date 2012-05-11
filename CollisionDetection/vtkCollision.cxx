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
#include "vtkDoubleArray.h"
#include "vtkIdList.h"

vtkCxxRevisionMacro(vtkCollision, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCollision);

//--------------------------------------------------------------------------
vtkCollision::vtkCollision() {
  this->CollisionType = vtkCollision::ToolOrgan;
  this->ObjectId = -1;
  this->ModelId = -1;
  this->CellId = -1;
  this->CellNormal[0] = this->CellNormal[1] = this->CellNormal[2] = 0;
  this->PointId = -1;
  this->Point[0] = this->Point[1] = this->Point[2] = 0;
  this->PointDisplacement[0] = this->PointDisplacement[1] = this->PointDisplacement[2] = 0;
}

//--------------------------------------------------------------------------
vtkCollision::~vtkCollision()
{
}

//--------------------------------------------------------------------------
void vtkCollision::DeepCopy(vtkCollision *info) {
  info->ObjectId = this->ObjectId;
  info->ModelId = this->ModelId;
  info->CellId = this->CellId;
  for (int i=0;i<3;i++){
    info->CellNormal[i] = this->CellNormal[i];
    info->Point[i] = this->Point[i];
    info->PointDisplacement[i] = this->PointDisplacement[i];
  }
  info->Distance = this->Distance;
}

//--------------------------------------------------------------------------
void vtkCollision::PrintSelf(ostream&os, vtkIndent indent)
{
  os << indent << "CollisionType: " << this->CollisionType << endl;
  os << indent << "ObjectId: " << this->ObjectId << endl;
  os << indent << "ModelId: " << this->ModelId << endl;
  os << indent << "CellId: " << this->CellId << endl;
  double * n = this->CellNormal;
  os << indent << "CellNormal: " << n[0] << ", " << n[1] << ", " << n[2] << endl;
  os << indent << "PointId: " << this->PointId << endl;
  os << indent << "Point: " << this->Point[0] << ", " << this->Point[1] << ", " << this->Point[2] << endl;
  n = this->PointNormal;
  os << indent << "PointNormal: " << n[0] << ", " << n[1] << ", " << n[2] << endl;
  double * d = this->PointDisplacement;
  os << indent << "PointDisplacement: " << d[0] << ", " << d[1] << ", " << d[2] << endl;
  os << indent << "Distance: " << this->Distance << endl;
}
