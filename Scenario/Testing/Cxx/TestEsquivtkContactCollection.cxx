#include <iostream>

#include "vtkMath.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"


using namespace std;

//!This test perform a test of the vtkContactCollection class

int TestEsquivtkContactCollection(int argc, char * argv[])
{
	vtkContact * contact;
	vtkContactCollection * collection = vtkContactCollection::New();

	for (vtkIdType id = 0; id < 10; id++)
	{
		contact = vtkContact::New();
		contact->SetToolId(0);
		contact->SetOrganId(0);
		contact->SetPoint(0, vtkMath::Random(0,1), vtkMath::Random(0,1), vtkMath::Random(0,1));
		contact->SetPointId(0, id);
		contact->SetCellId(0, 1);
		contact->SetPoint(1, vtkMath::Random(0,1), vtkMath::Random(0,1), vtkMath::Random(0,1));
		contact->SetPointId(1, id);
		contact->SetCellId(0, 1);

		collection->InsertNextContact(contact);
		std::cout << "Contact (" << id <<  ") has been inserted...\n";
	}

	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	collection->InitTraversal();

	for (vtkIdType id = 0; id < 10; id++)
	{
		std::cout << "#########################\n";
		contact = collection->GetNextContact();
		contact->Print(std::cout);
		std::cout << "Contact (" << id <<  ") has been removed...\n";
		//collection->RemoveItem(id);
		contact->Delete();
	}
	collection->RemoveAllItems();
	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	return 0;
}


