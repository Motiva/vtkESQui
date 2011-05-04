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
#include <iostream>

#include "vtkMath.h"
#include "vtkCollision.h"
#include "vtkCollisionCollection.h"


using namespace std;

//!This test perform a test of the vtkCollisionCollection class

int TestvtkCollisionCollection(int argc, char * argv[])
{
	vtkCollision * collision;
	vtkCollisionCollection * collection = vtkCollisionCollection::New();

	for (vtkIdType id = 0; id < 10; id++)
	{
		collision = vtkCollision::New();
		collision->SetElementId(0, 0);
		collision->SetElementId(0, 1);
		collision->SetPoint(0, vtkMath::Random(0,1), vtkMath::Random(0,1), vtkMath::Random(0,1));
		collision->SetPointId(0, id);
		collision->SetCellId(0, 1);
		collision->SetPoint(1, vtkMath::Random(0,1), vtkMath::Random(0,1), vtkMath::Random(0,1));
		collision->SetPointId(1, id);
		collision->SetCellId(0, 1);

		collection->InsertNextCollision(collision);
		std::cout << "Collision (" << id <<  ") has been inserted...\n";
	}

	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	collection->InitTraversal();

	for (vtkIdType id = 0; id < 10; id++)
	{
		std::cout << "#########################\n";
		collision = collection->GetNextCollision();
		collision->Print(std::cout);
		std::cout << "Collision (" << id <<  ") has been removed...\n";
		//collection->RemoveItem(id);
		collision->Delete();
	}
	collection->RemoveAllItems();
	std::cout << "Collection Number of Items: " << collection->GetNumberOfItems() << endl;

	return 0;
}


