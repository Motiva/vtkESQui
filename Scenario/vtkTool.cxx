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
#include "vtkTool.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkTransform.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyDataCollection.h"
#include "vtkMapperCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkTransformCollection.h"
#include "vtkActorCollection.h"
#include "vtkActor.h"
#include "vtkDoubleArray.h"
#include "vtkProperty.h"
#include "vtkAppendPolyData.h"
#include "vtkMath.h"

#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkTool, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkTool::vtkTool() {
	this->Id = -1;
	this->Type = vtkScenarioItem::Tool;
	this->ToolType = vtkTool::Laparoscopy;
	this->UseHaptic = 0;
	this->NumberOfPieces = 0;
	this->AppendFilter = NULL;

	this->Actors = vtkActorCollection::New();
	this->Transforms = vtkTransformCollection::New();
	this->Pieces = vtkPieceCollection::New();
	this->Contacts =vtkContactCollection::New();
	this->Colors = vtkDoubleArray::New();
	this->Colors->SetNumberOfComponents(3);
	this->Colors->SetNumberOfTuples(10);//Allocate memory for pieces
	for(int i=0;i<10;i++) this->Colors->SetTuple3(i, 1.0, 1.0, 1.0);
}

//--------------------------------------------------------------------------
vtkTool::~vtkTool() {

	this->Actors->Delete();
	this->Transforms->Delete();
	this->Pieces->Delete();
	this->Contacts->Delete();
	this->Colors->Delete();

}

//--------------------------------------------------------------------------
void vtkTool::Init()
{
	vtkPiece * piece;
	vtkTransform * pieceTransform;

	this->Colors->Resize(this->Pieces->GetNumberOfPieces());

	for(vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces() ; id++)
	{
		piece = this->GetPiece(id);
		piece->SetColor(this->Colors->GetTuple3(id));
		double * c = this->Colors->GetTuple3(id);
		pieceTransform = piece->GetTransform();

		pieceTransform->PreMultiply();

		pieceTransform->RotateX(this->Orientation[0]);
		pieceTransform->RotateY(this->Orientation[1]);
		pieceTransform->RotateZ(this->Orientation[2]);

		pieceTransform->Translate(this->Position);

	}

	if(!this->AppendFilter) this->AppendFilter = vtkAppendPolyData::New();
	this->AppendFilter->RemoveAllInputs();

	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		this->GetPiece(id)->Update();
		this->AppendFilter->AddInput(this->GetPiece(id)->GetOutput());
	}

	this->AppendFilter->Update();

	this->SetInput(this->AppendFilter->GetOutput());
}

int vtkTool::RequestData(vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{

	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(
			inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *output = vtkPolyData::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));

	if(this->IsHidden()) this->Hide();
	else if(this->IsVisible()) this->Show();

	//Update position & orientation
	this->Velocity[0] = this->Position[0];
	this->Velocity[1] = this->Position[1];
	this->Velocity[2] = this->Position[2];
	this->Acceleration[0] = this->Velocity[0];
	this->Acceleration[1] = this->Velocity[1];
	this->Acceleration[2] = this->Velocity[2];

	//Get transformed values
	this->GetTransform(0)->GetPosition(this->Position);
	this->GetTransform(0)->GetOrientation(this->Orientation);

	//Update object velocity
	//Velocity will be calculated from delta(Position)/dt
	vtkMath::Subtract(this->Position, this->Velocity, this->Velocity);
	vtkMath::MultiplyScalar(this->Velocity, 1/this->DeltaT);

	//Update object acceleration
	vtkMath::Subtract(this->Velocity, this->Acceleration, this->Acceleration);
	vtkMath::MultiplyScalar(this->Acceleration, 1/this->DeltaT);

	output->ShallowCopy(input);

	return 1;
}

//--------------------------------------------------------------------------
vtkPiece * vtkTool::GetPiece(vtkIdType id)
{
	return this->Pieces->GetPiece(id);
}

//--------------------------------------------------------------------------
void vtkTool::Translate(double * vector)
{
	this->Translate(vector[0], vector[1], vector[2]);
}

//--------------------------------------------------------------------------
void vtkTool::Translate(double x, double y, double z) {
	vtkTransform *transform;

	this->Transforms->InitTraversal();
	for ( vtkIdType id = 0; id < this->Transforms->GetNumberOfItems(); id++) {
		transform = this->Transforms->GetNextItem();
		transform->Translate(x, y, z);
	}
}

//--------------------------------------------------------------------------
void vtkTool::TranslateToOrigin()
{
	//Set back to the origin for translation/rotation
	this->Translate(this->Origin);
	this->RotateZ(-this->Orientation[2]);
}

//--------------------------------------------------------------------------
void vtkTool::TranslateFromOrigin()
{
	//Set back to previous position
	this->RotateZ(this->Orientation[2]);
	vtkMath::MultiplyScalar(this->Origin, -1);
	this->Translate(this->Origin);
	vtkMath::MultiplyScalar(this->Origin, -1);
}

//--------------------------------------------------------------------------
void vtkTool::RotateX(double x) {
	vtkTransform *transform;

	this->Transforms->InitTraversal();
	for ( vtkIdType id = 0; id < this->Transforms->GetNumberOfItems(); id++) {
		transform = this->Transforms->GetNextItem();
		transform->RotateX(x);
	}
}

//--------------------------------------------------------------------------
void vtkTool::RotateY(double y) {
	vtkTransform *transform;

	this->Transforms->InitTraversal();
	for ( vtkIdType id = 0; id < this->Transforms->GetNumberOfItems(); id++) {
		transform = this->Transforms->GetNextItem();
		transform->RotateY(y);
	}
}

//--------------------------------------------------------------------------
void vtkTool::RotateZ(double z)
{
	vtkTransform *transform;

	this->Transforms->InitTraversal();
	for ( vtkIdType id = 0; id < this->Transforms->GetNumberOfItems(); id++) {
		transform = this->Transforms->GetNextItem();
		transform->RotateZ(z);
	}
}

//--------------------------------------------------------------------------
void vtkTool::InsertNextContact(vtkContact* contact){
	this->Contacts->InsertNextContact(contact);
}

//--------------------------------------------------------------------------
void vtkTool::RemoveContacts(){
	this->Contacts->RemoveAllItems();
}

//--------------------------------------------------------------------------
vtkIdType vtkTool::GetNumberOfContacts(){
	return this->Contacts->GetNumberOfItems();
}

//--------------------------------------------------------------------------
vtkActorCollection* vtkTool::GetActors() {
	return this->Actors;
}

//--------------------------------------------------------------------------
vtkActor * vtkTool::GetActor(vtkIdType id) {
	return this->Pieces->GetPiece(id)->GetActor();
}

//--------------------------------------------------------------------------
vtkTransform * vtkTool::GetTransform(vtkIdType id) {
	return this->Pieces->GetPiece(id)->GetTransform();
}

//--------------------------------------------------------------------------
void vtkTool::SetColor(vtkIdType id, double r,double g,double b) {
	this->Colors->SetTuple3(id, r, g, b);
}

//--------------------------------------------------------------------------
vtkDoubleArray* vtkTool::GetColors() {
	return this->Colors;
}

//--------------------------------------------------------------------------
double * vtkTool::GetColor(vtkIdType id) {
	return this->Colors->GetTuple3(id);
}


//--------------------------------------------------------------------------
void vtkTool::Hide()
{
	this->Status = Hidden;
	this->Actors->InitTraversal();
	while (vtkActor * a = this->Actors->GetNextActor())
	{
		a->GetProperty()->SetOpacity(0.0);
	}
}

//--------------------------------------------------------------------------
void vtkTool::Show()
{
	this->Status = Visible;
	this->Actors->InitTraversal();
	while (vtkActor * a = this->Actors->GetNextActor())
	{
		a->GetProperty()->SetOpacity(1.0);
	}
}

//--------------------------------------------------------------------------
void vtkTool::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Type: " << this->ToolType << "\n";
	os << indent << "Number of Pieces: " << this->GetNumberOfPieces() << "\n";
	os << indent << "UseHaptic: " << this->UseHaptic << "\n";

}
