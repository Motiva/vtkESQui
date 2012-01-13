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
#include "vtkScenarioElement.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkTransform.h"
#include "vtkMath.h"
#include "vtkIdList.h"
#include "vtkPointLocator.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"

#include "vtkModelCollection.h"
#include "vtkCollisionModel.h"
#include "vtkVisualizationModel.h"
#include "vtkDeformationModel.h"
#include "vtkCollisionCollection.h"
#include "vtkCollision.h"

vtkCxxRevisionMacro(vtkScenarioElement, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkScenarioElement);

//--------------------------------------------------------------------------
vtkScenarioElement::vtkScenarioElement() {

  this->Id = -1;
  this->Name = NULL;
  this->RenderWindow = NULL;
  this->Renderer = NULL;
  this->Initialized = 0;

  this->Type = Organ;
  this->Status = 1;
  this->Scale[0] = this->Scale[1] = this->Scale[2] = 1.0;
  this->DeltaT = 1.0;

  this->Position[0]=this->Position[1]=this->Position[2]=0.0;
  this->Velocity[0]=this->Velocity[1]=this->Velocity[2]=0.0;
  this->Acceleration[0]=this->Acceleration[1]=this->Acceleration[2]=0.0;
  this->Orientation[0]=this->Orientation[1]=this->Orientation[2]=0.0;
  this->Origin[0]=this->Origin[1]=this->Origin[2]=0.0;
  this->Direction[0]=this->Direction[1]=this->Direction[2]=0.0;

  this->Transform = NULL;
  this->Matrix = NULL;

  this->SynchronizationMap = NULL;
  this->CollisionModel = NULL;
  this->VisualizationModel = NULL;
  this->DeformationModel = NULL;

  this->Models = NULL;
}

//--------------------------------------------------------------------------
vtkScenarioElement::~vtkScenarioElement()
{
  if(this->Models) this->Models->Delete();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetRenderWindow(vtkRenderWindow * r){
  this->RenderWindow = r;
}

//--------------------------------------------------------------------------
vtkRenderWindow * vtkScenarioElement::GetRenderWindow(){
  return this->RenderWindow;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetModels(vtkModelCollection * m){
  this->Models = m;
}

//--------------------------------------------------------------------------
vtkModelCollection * vtkScenarioElement::GetModels(){
  return this->Models;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetVisualizationModel(vtkVisualizationModel * m){
  this->VisualizationModel = m;
}

//--------------------------------------------------------------------------
vtkVisualizationModel * vtkScenarioElement::GetVisualizationModel(){
  return this->VisualizationModel;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetCollisionModel(vtkCollisionModel * m){
  this->CollisionModel = m;
}

//--------------------------------------------------------------------------
vtkCollisionModel * vtkScenarioElement::GetCollisionModel(){
  return this->CollisionModel;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetDeformationModel(vtkDeformationModel * m){
  this->DeformationModel = m;
}

//--------------------------------------------------------------------------
vtkDeformationModel * vtkScenarioElement::GetDeformationModel(){
  return this->DeformationModel;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetTransform(vtkTransform * t){
  this->Transform = t;
}

//--------------------------------------------------------------------------
vtkTransform * vtkScenarioElement::GetTransform(){
  return this->Transform;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::SetMatrix(vtkMatrix4x4 * m){
  this->Matrix = m;
}

//--------------------------------------------------------------------------
vtkMatrix4x4 * vtkScenarioElement::GetMatrix(){
  return this->Matrix;
}

//----------------------------------------------------------------------------
void vtkScenarioElement::Initialize()
{
  if(!this->Initialized)
  {
    this->Transform = vtkTransform::New();
    // Translate model to desired position & orientation
    this->Transform->Translate(this->Position);
    //Rotate model over itself
    this->Transform->RotateX(this->Orientation[0]);
    this->Transform->RotateY(this->Orientation[1]);
    this->Transform->RotateZ(this->Orientation[2]);
    //Set scale model
    this->Transform->Scale(this->Scale);
    // Set transform matrix
    this->Matrix = this->Transform->GetMatrix();

    this->Models = vtkModelCollection::New();
    //Set model object type
    vtkModel::vtkObjectType type;
    if(this->Type == Tool) type = vtkModel::Tool;
    else if(this->Type == Organ) type = vtkModel::Organ;

    if(this->VisualizationModel)
    {
      if(this->VisualizationModel->GetFileName())
      {
        vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
        reader->SetFileName(this->VisualizationModel->GetFileName());
        reader->Update();
        this->VisualizationModel->SetInput(reader->GetOutput());
      }

      this->VisualizationModel->SetId(this->Id);
      this->VisualizationModel->SetObjectId(this->ObjectId);
      this->VisualizationModel->SetObjectType(type);
      this->Models->AddModel(this->VisualizationModel);

      if(this->CollisionModel)
      {
        if(this->CollisionModel->GetFileName())
        {
          vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
          reader->SetFileName(this->CollisionModel->GetFileName());
          reader->Update();
          this->CollisionModel->SetInput(reader->GetOutput());
        }
        this->CollisionModel->SetId(this->Id);
        this->CollisionModel->SetObjectId(this->ObjectId);
        this->CollisionModel->SetObjectType(type);
        //Set optional (synchronization) input
        this->CollisionModel->SetInput(1, this->VisualizationModel->GetOutput());
        this->Models->AddModel(this->CollisionModel);

        if(this->DeformationModel)
        {
          if(this->DeformationModel->GetFileName())
          {
            vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
            reader->SetFileName(this->DeformationModel->GetFileName());
            reader->Update();
            this->DeformationModel->SetInput(reader->GetOutput());
          }
          this->DeformationModel->SetId(this->Id);
          this->DeformationModel->SetObjectId(this->ObjectId);
          this->DeformationModel->SetObjectType(type);
          //Set optional (synchronization) input
          this->Models->AddModel(this->DeformationModel);

          //Synchronize between Deformation and visualization model
          this->VisualizationModel->SetInput(1, this->DeformationModel->GetOutput());

          //Build Synchronization Map
          this->SynchronizationMap = vtkIdList::New();

          vtkPolyData * a = vtkPolyData::SafeDownCast(this->CollisionModel->GetInput());
          vtkPolyData * b = vtkPolyData::SafeDownCast(this->VisualizationModel->GetInput());
          //Create point locator to generate id map
          vtkSmartPointer<vtkPointLocator> locator =
              vtkSmartPointer<vtkPointLocator>::New();
          locator->SetDataSet(b);

          this->SynchronizationMap->SetNumberOfIds(a->GetNumberOfPoints());

          for (int i=0; i<a->GetNumberOfPoints(); i++)
          {
            double * point = a->GetPoint(i);
            vtkIdType id = locator->FindClosestPoint(point);
            this->SynchronizationMap->SetId(i, id);
          }

        }
      }
      //Set Transform matrix to the models
      this->Models->InitTraversal();
      while (vtkModel * model = this->Models->GetNextModel())
      {
        model->SetMatrix(this->Matrix);
      }
    }
    else vtkErrorMacro("A scenario element should have at least a visualization model");

    this->Initialized = 1;
  }
}

//----------------------------------------------------------------------------
void vtkScenarioElement::Update() {

  //cout << "vtkScenarioElement::Update" << endl;

  if(!this->Initialized) this->Initialize();

  //Update sequence
  //Save last position & calculate kinematic vales
  this->Velocity[0] = this->Position[0];
  this->Velocity[1] = this->Position[1];
  this->Velocity[2] = this->Position[2];
  this->Acceleration[0] = this->Velocity[0];
  this->Acceleration[1] = this->Velocity[1];
  this->Acceleration[2] = this->Velocity[2];

  //Get transformed values
  this->Transform->Update();

  this->Transform->GetPosition(this->Position);
  this->Transform->GetOrientation(this->Orientation);

  //Update object velocity
  //Velocity will be calculated from delta(Position)/dt
  vtkMath::Subtract(this->Position, this->Velocity, this->Velocity);
  vtkMath::MultiplyScalar(this->Velocity, 1/this->DeltaT);

  //Update object acceleration
  vtkMath::Subtract(this->Velocity, this->Acceleration, this->Acceleration);
  vtkMath::MultiplyScalar(this->Acceleration, 1/this->DeltaT);

  if(this->CollisionModel)
  {
    this->CollisionModel->SetObjectId(this->ObjectId);
    //Update collision model point positions and compute collisions
    this->CollisionModel->SetVelocity(this->Velocity);
    this->CollisionModel->SetAcceleration(this->Acceleration);
    this->CollisionModel->Modified();
    this->CollisionModel->Update();

    //Update deformation model with detected collisions
    if(this->DeformationModel)
    {
      vtkCollisionCollection * collisions = this->CollisionModel->GetCollisions();
      collisions->InitTraversal();
      //if(collisions->GetNumberOfItems() > 0) collisions->Print(cout);
      while(vtkCollision * c = collisions->GetNextCollision())
      {
        int objectId = 0;
        if(c->GetObjectId(1) == this->GetObjectId())
        {
          objectId = 1;
        }

        // Translate Collision-Deformation point ids
        int id = this->SynchronizationMap->GetId(c->GetPointId(objectId));
        //double * p = this->VisualizationModel->GetOutput()->GetPoint(id);
        //c->SetPointId(1, id);
        //c->SetPoint(1, p);

        //If displacement is zero (object is not moving) collision is ignored
        this->DeformationModel->InsertDisplacement(id, c->GetDisplacement());
      }

      this->DeformationModel->SetObjectId(this->ObjectId);
      this->DeformationModel->Modified();
      this->DeformationModel->Update();
    }
    this->CollisionModel->RemoveAllCollisions();
  }

  this->VisualizationModel->SetObjectId(this->ObjectId);
  this->VisualizationModel->Modified();
  this->VisualizationModel->Update();

  //Get movement parameters
  if(this->CollisionModel)
  {
    this->CollisionModel->GetDirection(this->Direction);
    this->CollisionModel->GetVelocity(this->Velocity);
    this->CollisionModel->GetAcceleration(this->Acceleration);
    this->DeltaT = this->CollisionModel->GetDeltaT();
  }

}

//--------------------------------------------------------------------------
void vtkScenarioElement::Translate(double * vector)
{
  this->Translate(vector[0], vector[1], vector[2]);
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Translate(double x, double y, double z) {
  //cout << this->GetName() << "::Translate: " << x << ", " << y << ", " << z << endl;
  this->Transform->Translate(x, y, z);
  //Modify tool origin (rotation point)
  this->Origin[0] += x;
  this->Origin[1] += y;
  this->Origin[2] -= z;
  this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateWXYZ(double a, double x, double y, double z) {
  this->Transform->RotateWXYZ(a, x, y, z);
  this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateX(double x) {
  //cout << this->GetName() << "::RotateX: " << x << endl;
  this->Transform->RotateX(x);
  this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateY(double y) {
  //cout << this->GetName() << "::RotateY: " << y << endl;
  this->Transform->RotateY(y);
  this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateZ(double z)
{
  //cout << this->GetName() << "::RotateZ: " << z << endl;
  this->Transform->RotateZ(z);
  this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Reset()
{
  //cout << this->GetName() << "::Reset()" << endl;
  this->Transform->Translate(this->Origin);
  this->Transform->RotateZ(-this->Orientation[2]);
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Restore()
{
  //cout << this->GetName() << "::Restore()" << endl;
  this->Transform->RotateZ(this->Orientation[2]);
  vtkMath::MultiplyScalar(this->Origin, -1);
  this->Transform->Translate(this->Origin);
  vtkMath::MultiplyScalar(this->Origin, -1);
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Hide()
{
  this->Models->InitTraversal();
  while (vtkModel * model =  this->Models->GetNextModel())
  {
    model->Hide();
  }

}

//--------------------------------------------------------------------------
void vtkScenarioElement::Enable()
{
  this->Status = 1;
  this->Models->InitTraversal();
  while (vtkModel * model =  this->Models->GetNextModel())
  {
    model->Enable();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Disable()
{
  this->Status = 0;
  this->Models->InitTraversal();
  while (vtkModel * model =  this->Models->GetNextModel())
  {
    model->Disable();
  }
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Show()
{
  this->Models->InitTraversal();
  while (vtkModel * model =  this->Models->GetNextModel())
  {
    model->Show();
  }
}
//--------------------------------------------------------------------------
bool vtkScenarioElement::IsEnabled(){
  return this->Status;
}

//--------------------------------------------------------------------------
bool vtkScenarioElement::IsDisabled()
{
  return !this->Status;
}

//--------------------------------------------------------------------------
bool vtkScenarioElement::IsInitialized()
{
  return this->Initialized;
}

//--------------------------------------------------------------------------
void vtkScenarioElement::PrintSelf(ostream& os,vtkIndent indent) {

  if(this->Name) os << indent << "Name: " << this->Name << "\n";
  else os << indent << "Name: \n";
  os << indent << "Id: " << this->Id << "\n";
  os << indent << "Scale: " << this->Scale[0] << ", " << this->Scale[1] << ", " << this->Scale[2] << endl;
  os << indent << "Origin: " << this->Origin[0] << ", " << this->Origin[1] << ", " << this->Origin[2] << endl;
  os << indent << "Orientation: " << this->Orientation[0] << ", " << this->Orientation[1] << ", " << this->Orientation[2] <<  endl;
  os << indent << "Direction: " << this->Direction[0] << ", " << this->Direction[1] << ", " << this->Direction[2] <<  endl;
  os << indent << "Position: " << this->Position[0] << ", " << this->Position[1] << ", " << this->Position[2] << endl;
  os << indent << "Velocity: " << this->Velocity[0] << ", " << this->Velocity[1] << ", " << this->Velocity[2] <<  endl;
  os << indent << "Acceleration: " << this->Acceleration[0] << ", " << this->Acceleration[1] << ", " << this->Acceleration[2] <<  endl;
  os << indent << "DeltaT: " << this->DeltaT << endl;

  this->Superclass::PrintSelf(os, indent);
}
