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
	this->Status = Visible;
	this->Scale[0] = this->Scale[1] = this->Scale[2] = 1.0;
	this->DeltaT = 1.0;

	this->Position[0]=this->Position[1]=this->Position[2]=0.0;
	this->Velocity[0]=this->Velocity[1]=this->Velocity[2]=0.0;
	this->Acceleration[0]=this->Acceleration[1]=this->Acceleration[2]=0.0;
	this->Orientation[0]=this->Orientation[1]=this->Orientation[2]=0.0;
	this->Origin[0]=this->Origin[1]=this->Origin[2]=0.0;
	this->Direction[0]=this->Direction[1]=this->Direction[2]=0.0;

	this->CollisionModel = NULL;
	this->VisualizationModel = NULL;
	this->DeformationModel = NULL;

	this->Models = vtkModelCollection::New();
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

//----------------------------------------------------------------------------
void vtkScenarioElement::Init()
{
	if(!this->Initialized)
	{
		if(this->VisualizationModel)
		{
			this->VisualizationModel->SetObjectId(this->ObjectId);
			this->VisualizationModel->SetId(this->Id);
			if(!this->VisualizationModel->IsInitialized()) this->VisualizationModel->Init();
			this->Models->AddModel(this->VisualizationModel);

			if(this->CollisionModel)
			{
				this->CollisionModel->SetObjectId(this->ObjectId);
				this->CollisionModel->SetId(this->Id);
				if(!this->CollisionModel->IsInitialized()) this->CollisionModel->Init();
				this->Models->AddModel(this->CollisionModel);

				if(this->DeformationModel)
				{
					this->DeformationModel->SetObjectId(this->ObjectId);
					this->DeformationModel->SetId(this->Id);
					if(!this->DeformationModel->IsInitialized()) this->DeformationModel->Init();
					this->Models->AddModel(this->DeformationModel);

					//Set source mesh for synchronization
					this->VisualizationModel->SetSource(this->DeformationModel->GetOutput());
					//Set source mesh for synchronization
					this->CollisionModel->SetSource(this->VisualizationModel->GetOutput());
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

	//Update element status
	if(this->IsHidden()) this->Hide();
	else if(this->IsVisible()) this->Show();

	this->UpdateProgress(0.10);

	//Update sequence
	if(this->CollisionModel)
	{
		//Update collision model point positions and compute collisions
		this->CollisionModel->Modified();
		this->CollisionModel->Update();

		//cout << "Coll: " << this->CollisionModel->GetCollisions()->GetNumberOfItems() << endl;
		this->UpdateProgress(0.25);

		//Update deformation model with detected collisions
		if(this->DeformationModel)
		{
			vtkCollisionCollection * collisions = this->CollisionModel->GetCollisions();
			collisions->InitTraversal();
			while(vtkCollision * c = collisions->GetNextCollision())
			{
				//Sync collision-deformation point ids
				int id = this->CollisionModel->GetHashMap()->GetId(c->GetPointId(1));
				double * p = this->VisualizationModel->GetOutput()->GetPoint(id);
				//c->Print(cout);
				//cout << "p[" << id << "]: (" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";
				//FIXME: Visualization == Deformation
				c->SetPointId(1, id);
				c->SetPoint(1, p);
				//If displacement is zero (object is not moving) collision is ignored
				if(vtkMath::Norm(c->GetDisplacement())>=0)
				{
					this->DeformationModel->AddCollision(c);
				}
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

	//Get transformed localization values
	this->VisualizationModel->GetPosition(this->Position);
	this->VisualizationModel->GetOrientation(this->Orientation);
	this->VisualizationModel->GetOrigin(this->Origin);

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
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Translate(x, y, z);
	}

	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateWXYZ(double a, double x, double y, double z) {
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->RotateWXYZ(a, x, y, z);
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateX(double x) {
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->RotateX(x);
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateY(double y) {
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->RotateY(y);
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::RotateZ(double z)
{
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->RotateZ(z);
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Reset()
{
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Reset();
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Restore()
{
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Restore();
	}
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkScenarioElement::Hide()
{
	this->Status = Hidden;
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Hide();
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
	this->Status = Visible;
	this->Models->InitTraversal();
	while (vtkModel * model =  this->Models->GetNextModel())
	{
		model->Show();
	}
}
//--------------------------------------------------------------------------
bool vtkScenarioElement::IsVisible(){
	return this->Status == Visible;
}

//--------------------------------------------------------------------------
bool vtkScenarioElement::IsHidden(){
	return this->Status == Hidden;
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

/*void vtkScenarioElement::BuildHashMap(vtkPolyData * a, vtkPolyData * b, vtkIdList * map)
{
	//Force data to be updated
	a->Update();
	b->Update();

	//Create point locator to generate id map
	vtkPointLocator * locator = vtkPointLocator::New();
	locator->SetDataSet(b);

	map->SetNumberOfIds(a->GetNumberOfPoints());

	//cout << this->GetName() << "::HashMap \n";
	for (int i=0; i<a->GetNumberOfPoints(); i++)
	{
		double * point = a->GetPoint(i);
		vtkIdType id = locator->FindClosestPoint(point);
		map->SetId(i, id);
		double * p = b->GetPoint(id);
		cout << "Map [" << i << "]: " << "("<< point[0] << "," << point[1] << "," << point[2] << ") ["<<
				id << "]: " << "("<< p[0] << "," << p[1] << "," << p[2] << ")" <<  endl;
	}
}*/

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
