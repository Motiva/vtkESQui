#include "vtkTool.h"

vtkCxxRevisionMacro(vtkTool, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkTool::vtkTool() {
	this->Id = -1;
	this->Type="";
	this->UseHaptic = 0;

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
	Superclass::Update();

	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		this->GetPiece(id)->Update();
	}
	this->UpdateSimpleMesh();
}

//--------------------------------------------------------------------------
void vtkTool::UpdateSimpleMesh()
{
	vtkAppendPolyData * appendFilter = vtkAppendPolyData::New();
	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		appendFilter->AddInput(this->GetPiece(id)->GetSimpleMesh());
	}
	appendFilter->Update();

	this->SimpleMesh->DeepCopy(appendFilter->GetOutput());
	appendFilter->Delete();
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
void vtkTool::SetRenderWindow(vtkRenderWindow *window) {
	this->RenderWindow = window;
	this->Renderer= RenderWindow->GetRenderers()->GetFirstRenderer();
}

//--------------------------------------------------------------------------
vtkRenderWindow* vtkTool::GetRenderWindow() {
	return this->RenderWindow;
}

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

	os << indent << "Type: " << this->Type << "\n";
	os << indent << "Number of Pieces: " << this->GetNumberOfPieces() << "\n";
	os << indent << "UseHaptic: " << this->UseHaptic << "\n";

}
