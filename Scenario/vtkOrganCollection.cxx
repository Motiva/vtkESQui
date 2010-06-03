#include "vtkOrganCollection.h"

vtkCxxRevisionMacro(vtkOrganCollection, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkOrganCollection);

//----------------------------------------------------------------------------
void vtkOrganCollection::InsertOrgan(vtkIdType id, vtkOrgan *organ) {
	this->vtkCollection::ReplaceItem(id, (vtkObject*) organ);
}

//--------------------------------------------------------------------------
void vtkOrganCollection::InsertNextOrgan(vtkOrgan *organ) {
	this->vtkCollection::AddItem((vtkObject *) organ);
}

//--------------------------------------------------------------------------
vtkOrgan * vtkOrganCollection::GetOrgan(vtkIdType id) {
	return static_cast <vtkOrgan *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkOrgan * vtkOrganCollection::GetNextOrgan() {
	return static_cast <vtkOrgan *>(this->GetNextItemAsObject());
}

//----------------------------------------------------------------------------
void vtkOrganCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
