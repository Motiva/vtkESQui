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
#include "vtkCollisionCollection.h"

#include "vtkCollision.h"

vtkCxxRevisionMacro(vtkCollisionCollection, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCollisionCollection);

//--------------------------------------------------------------------------
void vtkCollisionCollection::DeepCopy(vtkCollisionCollection *collection) {
  vtkCollision * AuxCopy;
  vtkCollision * Aux;
  this->InitTraversal();
  AuxCopy = collection->GetNextCollision();
  while(AuxCopy) {
    Aux = vtkCollision::New();
    Aux->DeepCopy(AuxCopy);
    this->InsertNextCollision(Aux);
    AuxCopy = collection->GetNextCollision();
  }
}

//--------------------------------------------------------------------------
void vtkCollisionCollection::InsertNextCollision(vtkCollision *collision) {
  this->vtkCollection::AddItem(collision);
}

//-------------------------------------------------------------------------
void vtkCollisionCollection::ReplaceCollision(vtkIdType id, vtkCollision *collision) {
  this->vtkCollection::ReplaceItem(id, collision);
}

//-------------------------------------------------------------------------
vtkIdType vtkCollisionCollection::ContainsCollision(vtkCollision * collision)
{
  return (this->FindCollision(collision) != -1);
}

//-------------------------------------------------------------------------
vtkIdType vtkCollisionCollection::FindCollision(vtkCollision * collision)
{
  vtkCollision * local;

  for(vtkIdType id = 0; id < this->GetNumberOfItems(); id++)
  {
    local = this->GetCollision(id);
    if ((local) &&
        (local->GetObjectId(0) == collision->GetObjectId(0)) &&
        (local->GetObjectId(1) == collision->GetObjectId(1)) &&
        (local->GetElementId(0) == collision->GetElementId(0)) &&
        (local->GetElementId(1) == collision->GetElementId(1)) &&
        (local->GetPointId(1) == collision->GetPointId(1)))
    {
      return id;
    }
  }
  return -1;
}

//--------------------------------------------------------------------------
vtkCollision* vtkCollisionCollection::GetCollision(vtkIdType id) {
  return static_cast <vtkCollision *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkCollision * vtkCollisionCollection::GetNextCollision() {
  return static_cast <vtkCollision*>(this->GetNextItemAsObject());
}

//--------------------------------------------------------------------------
void vtkCollisionCollection::RemoveCollision(vtkIdType id) {
  this->vtkCollection::RemoveItem(id);
}

//--------------------------------------------------------------------------
void vtkCollisionCollection::RemoveCollisions() {
  this->RemoveAllItems();
}

//----------------------------------------------------------------------------
void vtkCollisionCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

