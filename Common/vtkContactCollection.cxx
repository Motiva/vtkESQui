#include "vtkObjectFactory.h"
#include "vtkContactCollection.h"


vtkCxxRevisionMacro(vtkContactCollection, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkContactCollection);

//--------------------------------------------------------------------------
void vtkContactCollection::DeepCopy(vtkContactCollection *collection) {
	vtkContact * AuxCopy;
	vtkContact * Aux;
	this->InitTraversal();
	AuxCopy = collection->GetNextContact();
	while(AuxCopy) {
		Aux = vtkContact::New();
		Aux->DeepCopy(AuxCopy);
		this->InsertNextContact(Aux);
		AuxCopy = collection->GetNextContact();
	}
}

//--------------------------------------------------------------------------
void vtkContactCollection::InsertNextContact(vtkContact *contact) {
	this->vtkCollection::AddItem(contact);
}

//--------------------------------------------------------------------------
void vtkContactCollection::InsertContact(vtkIdType id, vtkContact *contact) {
	this->vtkCollection::ReplaceItem(id, contact);
}

vtkIdType vtkContactCollection::ContainsContact(vtkContact * contact)
{
	vtkContact * local;

	for(vtkIdType id = 0; id < this->GetNumberOfItems(); id++)
	{
		local = this->GetContact(id);
		if ((local) &&
				(local->GetToolId() == contact->GetToolId()) &&
				(local->GetOrganId() == contact->GetOrganId()) &&
				(local->GetPointId(0) == contact->GetPointId(0)) &&
				(local->GetPointId(1) == contact->GetPointId(1)))
		{
			return 1;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
vtkContact* vtkContactCollection::GetContact(vtkIdType id) {
	return static_cast <vtkContact *>(this->GetItemAsObject(id));
}

//--------------------------------------------------------------------------
vtkContact * vtkContactCollection::GetNextContact() {
	return static_cast <vtkContact*>(this->GetNextItemAsObject());
}

//--------------------------------------------------------------------------
void vtkContactCollection::RemoveContact(vtkIdType id) {
	this->vtkCollection::RemoveItem(id);
}

//--------------------------------------------------------------------------
void vtkContactCollection::RemoveContacts() {
	this->RemoveAllItems();
}

//----------------------------------------------------------------------------
void vtkContactCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

