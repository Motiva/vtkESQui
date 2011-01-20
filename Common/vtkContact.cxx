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
#include "vtkContact.h"

#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

vtkCxxRevisionMacro(vtkContact, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkContact);

//--------------------------------------------------------------------------
vtkContact::vtkContact() {
	this->isGrasped=false;
	this->ItemIds = vtkIdList::New();
	this->ItemIds->SetNumberOfIds(2);
	this->PointIds = vtkIdList::New();
	this->PointIds->SetNumberOfIds(2);
	this->CellIds = vtkIdList::New();
	this->CellIds->SetNumberOfIds(2);
	this->Points = vtkPoints::New();
	this->Points->SetDataTypeToDouble();
	this->Points->SetNumberOfPoints(2);
}

//--------------------------------------------------------------------------
vtkContact::~vtkContact()
{
	this->ItemIds->Delete();
	this->PointIds->Delete();
	this->CellIds->Delete();
	this->Points->Delete();
}

//--------------------------------------------------------------------------
void vtkContact::SetItemId(int i, int id)
{
	this->ItemIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkContact::GetItemId(int i){
	return this->ItemIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkContact::SetPointId(int i, int id)
{
	this->PointIds->SetId(i, id);
}

//--------------------------------------------------------------------------
int vtkContact::GetPointId(int i){
	return this->PointIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkContact::SetPoint(int i, double x, double y, double z) {
	this->Points->SetPoint(i, x, y, z);
}

//--------------------------------------------------------------------------
void vtkContact::SetPoint(int i, double point[3]) {
	this->SetPoint(i, point[0],point[1],point[2]);
}

//--------------------------------------------------------------------------
double * vtkContact::GetPoint(int i){
	return this->Points->GetPoint(i);
}

//--------------------------------------------------------------------------
void vtkContact::SetCellId(int i, vtkIdType value){
	this->CellIds->SetId(i, value);
}

//--------------------------------------------------------------------------
int vtkContact::GetCellId(int i){
	return this->CellIds->GetId(i);
}

//--------------------------------------------------------------------------
void vtkContact::DeepCopy(vtkContact *info) {
	this->ItemIds->DeepCopy(info->ItemIds);
	this->PointIds->DeepCopy(info->PointIds);
	this->CellIds->DeepCopy(info->CellIds);
	this->Points->DeepCopy(info->Points);
	this->isGrasped = info->isGrasped;
}

//--------------------------------------------------------------------------
void vtkContact::PrintSelf(ostream&os, vtkIndent indent)
{
	os << indent << "ContactType: " << this->ContactType << endl;
	for(int i = 0; i< 2; i++)
	{
		os << indent << "Item["<< i <<"] Id: " << this->ItemIds->GetId(i) << endl;
		os << indent << "Cell[" << i <<"] Id: " << this->CellIds->GetId(i) << endl;
		os << indent << "Point[" << i <<"] Id: " << this->PointIds->GetId(i)<< endl;
		double * point = this->Points->GetPoint(i);
		os << indent << "Point[" << i <<"] Position: " << point[0] << ", " << point[1] << ", " << point[2] << endl;
	}
	os << indent << "isGrasped: " << this->isGrasped << endl;
	os << indent << "Direction: " << this->Displacement[0] << ", " << this->Displacement[1] << ", " << this->Displacement[2] << endl;
}
