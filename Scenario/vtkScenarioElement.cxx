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

	this->SetNumberOfInputPorts(0);
	this->SetNumberOfOutputPorts(1);

	this->Id = -1;
	this->Name = NULL;
	this->RenderWindow = NULL;
	this->Renderer = NULL;
	this->Initialized = 0;

	this->Type = Organ;
	this->Status = Enabled;
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
void vtkScenarioElement::Init()
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

		if(this->VisualizationModel)
		{
			this->VisualizationModel->SetObjectId(this->ObjectId);
			this->VisualizationModel->SetId(this->Id);
			this->Models->AddModel(this->VisualizationModel);

			if(this->CollisionModel)
			{
				this->CollisionModel->SetObjectId(this->ObjectId);
				this->CollisionModel->SetId(this->Id);
				//Set optional (synchronisation) input
				this->CollisionModel->SetInput(1, this->VisualizationModel->GetOutput());
				this->Models->AddModel(this->CollisionModel);

				if(this->DeformationModel)
				{
					this->DeformationModel->SetObjectId(this->ObjectId);
					this->DeformationModel->SetId(this->Id);
					//Set optional (synchronisation) input
					this->DeformationModel->SetInput(1, this->VisualizationModel->GetOutput());
					this->Models->AddModel(this->DeformationModel);
				}
			}
			//Set Transform matrix to the models
			this->Models->InitTraversal();
			while (vtkModel * model = this->Models->GetNextModel())
			{
				if(!model->IsInitialized()){
					model->SetMatrix(this->Matrix);
					model->Init();
				}
			}
		}
		else vtkErrorMacro("A scenario element should have at least a visualization model");

		this->Initialized = 1;
	}
}

//----------------------------------------------------------------------------
int vtkScenarioElement::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	//cout << "vtkScenarioElement::RequestData (" << this->GetName() << ")\n";
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	this->UpdateProgress(0.10);

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
		//Update collision model point positions and compute collisions
		this->CollisionModel->SetVelocity(this->Velocity);
		this->CollisionModel->SetAcceleration(this->Acceleration);
		this->CollisionModel->Modified();
		this->CollisionModel->Update();

		this->UpdateProgress(0.25);

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
				int id = this->CollisionModel->GetHashMap()->GetId(c->GetPointId(objectId));
				//double * p = this->VisualizationModel->GetOutput()->GetPoint(id);
				//c->SetPointId(1, id);
				//c->SetPoint(1, p);

				//If displacement is zero (object is not moving) collision is ignored
				this->DeformationModel->InsertDisplacement(id, c->GetDisplacement());
			}

			this->DeformationModel->Modified();
			this->DeformationModel->Update();

		}
		this->UpdateProgress(0.50);
		this->CollisionModel->RemoveAllCollisions();
	}

	this->VisualizationModel->Modified();
	this->VisualizationModel->Update();

	this->UpdateProgress(0.75);

	//Get movement parameters
	if(this->CollisionModel)
	{
		this->CollisionModel->GetDirection(this->Direction);
		this->CollisionModel->GetVelocity(this->Velocity);
		this->CollisionModel->GetAcceleration(this->Acceleration);
		this->DeltaT = this->CollisionModel->GetDeltaT();
	}

	this->UpdateProgress(1.0);

	//Set visualization mesh as output
	output->ShallowCopy(this->VisualizationModel->GetOutput());

	return 1;
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
	this->Status = Enabled;
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Enable();
	}
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Disable()
{
	this->Status = Disabled;
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
	return this->Status == Enabled;
}

//--------------------------------------------------------------------------
bool vtkScenarioElement::IsDisabled()
{
	return this->Status == Disabled;
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
