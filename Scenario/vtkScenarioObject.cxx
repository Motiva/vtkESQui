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
#include "vtkScenarioObject.h"

#include "vtkObjectFactory.h"
#include "vtkAppendPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include "vtkScenarioElement.h"
#include "vtkScenarioElementCollection.h"
#include "vtkCollisionModel.h"
#include "vtkCollision.h"
#include "vtkCollisionCollection.h"

vtkCxxRevisionMacro(vtkScenarioObject, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenarioObject);

//--------------------------------------------------------------------------
vtkScenarioObject::vtkScenarioObject() {

  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);

  this->Id = -1;
  this->Name = NULL;
  this->RenderWindow = NULL;
  this->Renderer = NULL;
  this->Initialized = 0;

  this->Status = 1;

  this->Elements = vtkScenarioElementCollection::New();
  this->Collisions = vtkCollisionCollection::New();
}

//--------------------------------------------------------------------------
vtkScenarioObject::~vtkScenarioObject()
{
  if(this->Elements) this->Elements->Delete();
  if(this->Collisions) this->Collisions->Delete();
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Init()
{
  if(!this->Initialized)
  {
    for (int id=0; id<this->GetNumberOfElements(); id++)
    {
      //Set common parameters
      vtkScenarioElement * e = this->GetElement(id);
      e->SetObjectId(this->Id);
      e->SetId(id);
      if(this->ObjectType == Tool) e->SetType(vtkScenarioElement::Tool);
      else if(this->ObjectType == Organ) e->SetType(vtkScenarioElement::Organ);

      //Initialize object elements
      if(!e->IsInitialized()) e->Init();
    }
    this->Initialized = 1;
  }
}

//--------------------------------------------------------------------------
int vtkScenarioObject::RequestData(vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{

  //cout << "vtkScenarioObject::RequestData" << endl;
  // get the info objects
  //vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  //vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  //Reset previous collisions
  this->Collisions->RemoveAllItems();

  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =this->Elements->GetNextElement())
  {
    e->Modified();
    e->Update();
  }

  return 1;
}

//--------------------------------------------------------------------------
vtkScenarioElementCollection * vtkScenarioObject::GetElements()
{
  return this->Elements;
}

//--------------------------------------------------------------------------
void vtkScenarioObject::SetElements(vtkScenarioElementCollection * elements)
{
  if(this->Elements) this->Elements->Delete();
  this->Elements = elements;
}

//--------------------------------------------------------------------------
void vtkScenarioObject::AddElement(vtkScenarioElement *e) {
  this->Elements->AddElement(e);
}

//--------------------------------------------------------------------------
void vtkScenarioObject::RemoveElement(vtkIdType id) {
  this->Elements->RemoveItem(id);
}

//--------------------------------------------------------------------------
vtkScenarioElement * vtkScenarioObject::GetElement(vtkIdType id) {
  return this->Elements->GetElement(id);
}

//--------------------------------------------------------------------------
int vtkScenarioObject::GetNumberOfElements()
{
  return this->Elements->GetNumberOfElements();
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Translate(double * vector)
{
  this->Translate(vector[0], vector[1], vector[2]);
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Translate(double x, double y, double z) {
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Translate(x, y, z);
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::RotateWXYZ(double a, double x, double y, double z) {
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->RotateWXYZ(a, x, y, z);
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::RotateX(double x) {
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->RotateX(x);
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::RotateY(double y) {
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->RotateY(y);
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::RotateZ(double z)
{
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->RotateZ(z);
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Reset()
{
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Reset();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Restore()
{
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Restore();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Show()
{
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Show();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Hide()
{
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Hide();
  }

}

//--------------------------------------------------------------------------
void vtkScenarioObject::Enable()
{
  this->Status = 1;
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Enable();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioObject::Disable()
{
  this->Status = 0;
  this->Elements->InitTraversal();
  while (vtkScenarioElement * e =  this->Elements->GetNextElement())
  {
    e->Disable();
  }
}

//Collision management
//--------------------------------------------------------------------------
void vtkScenarioObject::InsertNextCollision(vtkCollision * collision)
{
  this->Collisions->InsertNextCollision(collision);
}

//--------------------------------------------------------------------------
void vtkScenarioObject::SetCollisions( vtkCollisionCollection* collisions )
{
  if(this->Collisions) this->Collisions->Delete();
  this->Collisions->DeepCopy(collisions);
}

//--------------------------------------------------------------------------
vtkCollisionCollection * vtkScenarioObject::GetCollisions()
{
  return this->Collisions;
}

//--------------------------------------------------------------------------
vtkIdType vtkScenarioObject::GetNumberOfCollisions()
{
  return this->Collisions->GetNumberOfItems();
}

//--------------------------------------------------------------------------
void vtkScenarioObject::CleanCollisions()
{
  this->Collisions->RemoveCollisions();
}

//--------------------------------------------------------------------------
void vtkScenarioObject::PrintSelf(ostream& os,vtkIndent indent) {

  os << indent << "Id: " << this->Id << "\n";
  if(this->Name) os << indent << "Name: " << this->Name << "\n";
  else os << indent << "Name: \n";
  os << indent << "Type: " << this->ObjectType << "\n";
  os << indent << "Status: " << this->Status << "\n";
  os << indent << "NumberOfElements: " << this->Elements->GetNumberOfElements() << "\n";
}
