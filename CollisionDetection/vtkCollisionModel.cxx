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
#include "vtkCollisionModel.h"

#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkSyncPolyDataFilter.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkSphereSource.h"
#include "vtkGlyph3D.h"
#include "vtkPoints.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkLookupTable.h"
#include "vtkIdList.h"
#include "vtkTransform.h"
#include "vtkMath.h"

#include "vtkCollision.h"
#include "vtkCollisionCollection.h"

vtkCxxRevisionMacro(vtkCollisionModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCollisionModel);

//--------------------------------------------------------------------------
vtkCollisionModel::vtkCollisionModel() {

  this->Type = vtkModel::Collision;

  this->DeltaT = 1.0;
  this->Velocity[0]=this->Velocity[1]=this->Velocity[2]=0.0;
  this->Acceleration[0]=this->Acceleration[1]=this->Acceleration[2]=0.0;
  this->Direction[0]=this->Direction[1]=this->Direction[2]=0.0;

  this->Collisions = NULL;
  this->Normals = NULL;
  this->Sphere = NULL;
  this->Glyphs = NULL;
  this->Scalars = NULL;

  this->DisplayCollisions = 0;
  this->Radius = 0.05;
}

//--------------------------------------------------------------------------
vtkCollisionModel::~vtkCollisionModel()
{
  if(this->Initialized){
    this->Collisions->Delete();
    this->Normals->Delete();
    this->Sphere->Delete();
    this->Glyphs->Delete();
    this->Scalars->Delete();
    this->LUT->Delete();
  }

}

//--------------------------------------------------------------------------
void vtkCollisionModel::Initialize()
{
  this->Superclass::Initialize();

  this->Collisions = vtkCollisionCollection::New();

  this->Normals = vtkPolyDataNormals::New();
  this->Normals->ComputeCellNormalsOn();
  this->Normals->ComputePointNormalsOn();

  // Configure visualization
  this->Sphere = vtkSphereSource::New();
  this->Sphere->SetRadius(this->Radius);
  this->Glyphs = vtkGlyph3D::New();
  this->Glyphs->ScalingOff();
  this->Scalars = vtkDoubleArray::New();
  this->Scalars->SetNumberOfComponents(1);
  this->Scalars->SetName("Colors");
  this->LUT = vtkLookupTable::New();
  this->LUT->SetNumberOfColors(64);
  this->LUT->Build();
  for(int i=0;i<8;i++)
  {
    double r = (8.0-i)/8.0;
    this->LUT->SetTableValue(8*i,r,1.0,1.0);
  }
}

//--------------------------------------------------------------------------
void vtkCollisionModel::SetCollisions(vtkCollisionCollection * c)
{
  this->Collisions->RemoveAllItems();
  c->InitTraversal();
  while(vtkCollision * col = c->GetNextCollision()){
    this->AddCollision(col);
  }
}

//--------------------------------------------------------------------------
vtkCollisionCollection * vtkCollisionModel::GetCollisions()
{
  return this->Collisions;
}

//--------------------------------------------------------------------------
void vtkCollisionModel::AddCollision(vtkCollision * c)
{
  this->Collisions->AddItem(c);
}

//--------------------------------------------------------------------------
void vtkCollisionModel::RemoveCollision(vtkIdType id)
{
  //Remove collision
  this->Collisions->RemoveItem(id);
}

//--------------------------------------------------------------------------
void vtkCollisionModel::RemoveAllCollisions()
{
  //Remove all collisions
  this->Collisions->RemoveAllItems();
}

//--------------------------------------------------------------------------
int vtkCollisionModel::GetNumberOfCollisions()
{
  return this->GetCollisions()->GetNumberOfItems();
}

//--------------------------------------------------------------------------
vtkFloatArray * vtkCollisionModel::GetCellNormals()
{
  return vtkFloatArray::SafeDownCast(this->GetOutput()->GetCellData()->GetNormals());
}

//--------------------------------------------------------------------------
vtkFloatArray * vtkCollisionModel::GetPointNormals()
{
  return vtkFloatArray::SafeDownCast(this->GetOutput()->GetPointData()->GetNormals());
}

//----------------------------------------------------------------------------
int vtkCollisionModel::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  // Optional input
  vtkPolyData * source = 0;
  if(sourceInfo){
    source = vtkPolyData::SafeDownCast(sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  }

  // Main output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if(!this->Initialized) {
    this->Initialize();
  }

  //Set visualization parameters
  this->Actor->SetVisibility(this->Visibility);

  if(this->Status)
  {
    //If source is defined -> Synchronize mesh
    if(source)
    {
      vtkDebugMacro("Model source is present\n");

      this->SyncFilter->SetInput(input);
      this->SyncFilter->SetSource(source);
      this->Normals->SetInputConnection(this->SyncFilter->GetOutputPort());
      
      //Update object position
      this->Normals->Update();
      output->ShallowCopy(this->Normals->GetOutput());

    }
    else
    {
      this->Normals->SetInput(input);
      //Update object position
      this->Normals->Update();
      output->ShallowCopy(this->Normals->GetOutput());
    }

    //Set glyphs
    this->Glyphs->SetInput(output);
    this->Glyphs->SetSource(this->Sphere->GetOutput());

    //Display point collisions
    if(this->DisplayCollisions)
    {
      this->Scalars->SetNumberOfTuples(input->GetNumberOfPoints());
      for(int i=0; i<this->Scalars->GetNumberOfTuples(); i++)
      {
        this->Scalars->SetTuple1(i,0);
      }
      this->Collisions->InitTraversal();
      while(vtkCollision *c = this->Collisions->GetNextCollision())
      {
        this->Scalars->SetValue(c->GetPointId(), c->GetDistance());
      }
      //Add scalars
      output->GetPointData()->SetScalars(this->Scalars);
      this->Mapper->ScalarVisibilityOn();
      this->Mapper->SetScalarRange(this->Scalars->GetValueRange());
      this->Mapper->SetLookupTable(this->LUT);
    }

    this->Mapper->SetInput(this->Glyphs->GetOutput());
    
    this->Actor->SetUserMatrix(this->Matrix);
    this->Actor->GetProperty()->SetColor(this->Color);
    this->Actor->GetProperty()->SetOpacity(this->Opacity);

  }

  return 1;
}

//--------------------------------------------------------------------------
void vtkCollisionModel::PrintSelf(ostream&os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Direction: " << this->Direction[0] << ", " << this->Direction[1] << ", " << this->Direction[2] << endl;
  os << indent << "Velocity: " << this->Velocity[0] << ", " << this->Velocity[1] << ", " << this->Velocity[2] << endl;
  os << indent << "Acceleration: " << this->Acceleration[0] << ", " << this->Acceleration[1] << ", " << this->Acceleration[2] <<  endl;
  os << indent << "DeltaT: " << this->DeltaT << endl;
}
