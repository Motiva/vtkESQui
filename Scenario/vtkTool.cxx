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

#include "vtkObject.h"
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
#include "vtkAppendPolyData.h"

#include "vtkPiece.h"
#include "vtkPieceCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkTool, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkTool::vtkTool() {
	this->Id = -1;
	this->ToolType = vtkTool::Laparoscopy;
	this->UseHaptic = 0;

	this->AppendFilter = NULL;

	this->Actors = vtkActorCollection::New();
	this->Transforms = vtkTransformCollection::New();

	this->Pieces = vtkPieceCollection::New();
	
	this->Contacts =vtkContactCollection::New();

	this->Position[0]=this->Position[1]=this->Position[2]=0.0;
	this->Orientation[0]=this->Orientation[1]=this->Orientation[2]=0.0;
	this->Origin[0]=this->Origin[1]=this->Origin[2]=0.0;

}

//--------------------------------------------------------------------------
vtkTool::~vtkTool() {

	this->Actors->Delete();
	this->Transforms->Delete();

	this->Pieces->Delete();

	this->Contacts->Delete();

}

//--------------------------------------------------------------------------
void vtkTool::Update()
{
	this->UpdateDirection();

	if(!this->AppendFilter) this->AppendFilter = vtkAppendPolyData::New();
	this->AppendFilter->RemoveAllInputs();

	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		this->GetPiece(id)->Update();
		this->AppendFilter->AddInput(this->GetPiece(id)->GetOutput());
	}

	this->AppendFilter->Update();
}

//--------------------------------------------------------------------------
vtkPolyData * vtkTool::GetOutput()
{
	return this->AppendFilter->GetOutput();
}

//--------------------------------------------------------------------------
void vtkTool::SetFileName(vtkIdType id, const char * filename)
{
	this->GetPiece(id)->SetFileName(filename);
}

//--------------------------------------------------------------------------
const char * vtkTool::GetFileName(vtkIdType id)
{
	return this->GetPiece(id)->GetFileName();
}

//--------------------------------------------------------------------------
vtkPiece * vtkTool::GetPiece(vtkIdType id)
{
	return this->Pieces->GetPiece(id);
}

//--------------------------------------------------------------------------
vtkIdType vtkTool::GetNumberOfPieces() {
	return this->Pieces->GetNumberOfPieces();
};


//--------------------------------------------------------------------------
void vtkTool::ApplyInitialTransform()
{
	vtkPiece * piece;
	vtkTransform * pieceTransform;
	for(vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces() ; id++)
	{
		piece = this->GetPiece(id);
		pieceTransform = piece->GetTransform();

		pieceTransform->PreMultiply();
		
		pieceTransform->RotateX(this->Orientation[0]);
		pieceTransform->RotateY(this->Orientation[1]);
		pieceTransform->RotateZ(this->Orientation[2]);

		pieceTransform->Translate(this->Position);

	}
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
void vtkTool::RotateZ(double angle)
{
	vtkTransform *transform;

	this->Transforms->InitTraversal();
	for ( vtkIdType id = 0; id < this->Transforms->GetNumberOfItems(); id++) {
		transform = this->Transforms->GetNextItem();
		transform->RotateZ(angle);
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
vtkActorCollection* vtkTool::GetActorCollection() {
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
void vtkTool::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Type: " << this->ToolType << "\n";
	os << indent << "Number of Pieces: " << this->GetNumberOfPieces() << "\n";
	os << indent << "UseHaptic: " << this->UseHaptic << "\n";

}
