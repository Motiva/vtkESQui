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
#include "vtkDeformationModel.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkPolyData.h"
#include "vtkActor.h"
#include "vtkProperty.h"

#include "vtkBoundaryConditionCollection.h"
#include "vtkBoundaryCondition.h"

vtkCxxRevisionMacro(vtkDeformationModel, "$Revision: 0.1 $");
//vtkStandardNewMacro(vtkDeformationModel);

//--------------------------------------------------------------------------
vtkDeformationModel::vtkDeformationModel() {

	this->SetModelType(vtkModel::Deformation);
	this->Name = NULL;
	this->BoundaryConditions = NULL;
}

//--------------------------------------------------------------------------
vtkDeformationModel::~vtkDeformationModel() {

	if(this->BoundaryConditions) this->BoundaryConditions->Delete();
}

//--------------------------------------------------------------------------
void vtkDeformationModel::Init()
{
	this->Superclass::Init();

	this->BoundaryConditions = vtkBoundaryConditionCollection::New();

	//Display as a wire frame
	this->Actor->GetProperty()->SetRepresentationToWireframe();
}

//--------------------------------------------------------------------------
void vtkDeformationModel::InsertNextBoundaryCondition(vtkBoundaryCondition* condition)
{
	//Insert collision point coordinates
	this->BoundaryConditions->InsertNextBoundaryCondition(condition);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkDeformationModel::InsertBoundaryConditions(vtkBoundaryConditionCollection * collection)
{
	//Extract data from vtkBoundaryConditionCollection object
	collection->InitTraversal();
	vtkBoundaryCondition * c;
	c = collection->GetNextBoundaryCondition();
	while(c)
	{
		this->InsertNextBoundaryCondition(c);
		c = collection->GetNextBoundaryCondition();
	}

	this->Modified();
}

//--------------------------------------------------------------------------
void vtkDeformationModel::RemoveBoundaryConditions()
{
	this->BoundaryConditions->RemoveBoundaryConditions();
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkDeformationModel::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);
}
