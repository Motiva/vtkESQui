/*
 * TestEsquivtkToolCollection.cxx
 *
 *  Created on: 12/01/2010
 *      Author: jballesteros
 */

#include <iostream>

#include "vtkToolDummy.h"
#include "vtkToolCollection.h"

using namespace std;

//!This test perform a test of the vtkToolCollection class

int TestEsquivtkToolCollection(int argc, char * argv[])
{
	vtkToolDummy * tool;
	vtkToolCollection * collection = vtkToolCollection::New();

	for (vtkIdType id = 0; id < 10; id++)
	{
		tool = vtkToolDummy::New();
		tool->SetPosition(0,0,0);
		tool->SetId(id);

		collection->InsertNextTool(tool);
		//tool->Print(std::cout);
		std::cout << "Tool (" << id <<  ") has been inserted...\n";
	}

	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	collection->InitTraversal();

	for (vtkIdType id = 0; id < 10; id++)
	{
		std::cout << "#########################\n";
		vtkTool * tool = collection->GetNextTool();
		tool->Print(std::cout);
		std::cout << "Tool (" << id <<  ") has been removed...\n";
		//collection->RemoveItem(id);
		tool->Delete();
	}
	collection->RemoveAllItems();
	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	return 0;
}


