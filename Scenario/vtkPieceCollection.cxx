#include "vtkPieceCollection.h"

vtkCxxRevisionMacro(vtkPieceCollection, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
void vtkPieceCollection::InsertPiece(vtkIdType id, vtkPiece *Piece) {
	this->vtkPieceCollection::ReplaceItem(id, (vtkObject*) Piece);
}

//--------------------------------------------------------------------------
void vtkPieceCollection::InsertNextPiece(vtkPiece *Piece) {
	this->vtkCollection::AddItem((vtkObject *) Piece);
}

//--------------------------------------------------------------------------
vtkPiece * vtkPieceCollection::GetPiece(vtkIdType id) {
	return static_cast <vtkPiece *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkPiece * vtkPieceCollection::GetNextPiece()
{
	return static_cast <vtkPiece *>(this->GetNextItemAsObject());
}

//--------------------------------------------------------------------------
vtkIdType vtkPieceCollection::GetNumberOfPieces()
{
	return this->GetNumberOfItems();
}

//----------------------------------------------------------------------------
void vtkPieceCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
