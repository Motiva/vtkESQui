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
#include "vtkObjectFactory.h"
#include "vtkBoundaryConditionCollection.h"

#include "vtkBoundaryCondition.h"

vtkCxxRevisionMacro(vtkBoundaryConditionCollection, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBoundaryConditionCollection);

//--------------------------------------------------------------------------
void vtkBoundaryConditionCollection::DeepCopy(vtkBoundaryConditionCollection *collection) {
  vtkBoundaryCondition * AuxCopy;
  vtkBoundaryCondition * Aux;
  this->InitTraversal();
  AuxCopy = collection->GetNextBoundaryCondition();
  while(AuxCopy) {
    Aux = vtkBoundaryCondition::New();
    Aux->DeepCopy(AuxCopy);
    this->InsertNextBoundaryCondition(Aux);
    AuxCopy = collection->GetNextBoundaryCondition();
  }
}

//--------------------------------------------------------------------------
void vtkBoundaryConditionCollection::InsertNextBoundaryCondition(vtkBoundaryCondition *condition) {
  this->vtkCollection::AddItem(condition);
}

//-------------------------------------------------------------------------
void vtkBoundaryConditionCollection::ReplaceBoundaryCondition(vtkIdType id, vtkBoundaryCondition *condition) {
  this->vtkCollection::ReplaceItem(id, condition);
}

//-------------------------------------------------------------------------
vtkIdType vtkBoundaryConditionCollection::ContainsBoundaryCondition(vtkBoundaryCondition * condition)
{
  return (this->FindBoundaryCondition(condition) != -1);
}

//-------------------------------------------------------------------------
vtkIdType vtkBoundaryConditionCollection::FindBoundaryCondition(vtkBoundaryCondition * condition)
{
  vtkBoundaryCondition * local;

  for(vtkIdType id = 0; id < this->GetNumberOfItems(); id++)
  {
    local = this->GetBoundaryCondition(id);
    if ((local) &&
      (local->GetId() == condition->GetId()) &&
      (local->GetPointId() == condition->GetPointId()) &&
      (local->GetValue() == condition->GetValue()))
    {
      return id;
    }
  }
  return -1;
}

//--------------------------------------------------------------------------
vtkBoundaryCondition* vtkBoundaryConditionCollection::GetBoundaryCondition(vtkIdType id) {
  return static_cast <vtkBoundaryCondition *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkBoundaryCondition * vtkBoundaryConditionCollection::GetNextBoundaryCondition() {
  return static_cast <vtkBoundaryCondition*>(this->GetNextItemAsObject());
}

//--------------------------------------------------------------------------
void vtkBoundaryConditionCollection::RemoveBoundaryCondition(vtkIdType id) {
  this->vtkCollection::RemoveItem(id);
}

//--------------------------------------------------------------------------
void vtkBoundaryConditionCollection::RemoveBoundaryConditions() {
  this->RemoveAllItems();
}

//----------------------------------------------------------------------------
void vtkBoundaryConditionCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

