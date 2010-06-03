#include "vtkToolCollection.h"

vtkCxxRevisionMacro(vtkToolCollection, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
void vtkToolCollection::InsertTool(vtkIdType id, vtkTool *tool) {
	this->vtkToolCollection::ReplaceItem(id, (vtkObject*) tool);
}

//--------------------------------------------------------------------------
void vtkToolCollection::InsertNextTool(vtkTool *tool) {
	this->vtkCollection::AddItem((vtkObject *) tool);
}

//--------------------------------------------------------------------------
vtkTool * vtkToolCollection::GetTool(vtkIdType id) {
	return static_cast <vtkTool *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkTool * vtkToolCollection::GetNextTool()
{
	return static_cast <vtkTool *>(this->GetNextItemAsObject());
}

//----------------------------------------------------------------------------
void vtkToolCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}
