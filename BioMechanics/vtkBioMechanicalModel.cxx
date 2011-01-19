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

#include "vtkBioMechanicalModel.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkIdList.h"

#include "vtkContactCollection.h"
#include "vtkContact.h"

vtkCxxRevisionMacro(vtkBioMechanicalModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkBioMechanicalModel);

//--------------------------------------------------------------------------
vtkBioMechanicalModel::vtkBioMechanicalModel()
{
	this->Name = NULL;
	this->Contacts = vtkContactCollection::New();

}
//--------------------------------------------------------------------------
vtkBioMechanicalModel::~vtkBioMechanicalModel()
{
	this->Contacts->Delete();
}

//--------------------------------------------------------------------------
/*int vtkBioMechanicalModel::RequestData(vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{
	// get the info objects
	//vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	//vtkInformation *outInfo = outputVector->GetInformationObject(0);
	// get the input and output
	//vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	//vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
	std::cout << "vtkBioMechanicalModel::RequestData" << endl;

	return 1;
}*/

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::Init()
{
 std::cout << "vtkBioMechanicalModel::Init()\n";
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::InsertNextContact(vtkContact* contact)
{
	//Insert collision point coordinates
	this->Contacts->InsertNextContact(contact);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::InsertContacts(vtkContactCollection * collection)
{
	//Extract data from vtkContactCollection object
	collection->InitTraversal();
	vtkContact * c;
	c = collection->GetNextContact();
	while(c)
	{
		this->InsertNextContact(c);
		c = collection->GetNextContact();
	}

	this->Modified();
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::DeleteContacts()
{
	this->Contacts->RemoveContacts();
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkBioMechanicalModel::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent <<  "Name: " << this->Name << endl;
}
