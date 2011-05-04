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
#include "vtkEDMInterface.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkImageSobel3D.h"
#include "vtkImageMagnitude.h"
#include "vtkPointData.h"
#include "vtkMetaImageWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkActor.h"

#include "vtkCollision.h"
#include "vtkCollisionCollection.h"
#include "vtkBoundaryCondition.h"
#include "vtkBoundaryConditionCollection.h"

vtkCxxRevisionMacro(vtkEDMInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkEDMInterface);

//----------------------------------------------------------------------------
vtkEDMInterface::vtkEDMInterface()
{
	this->DeformableMesh = vtkDeformableMesh::New();
	this->ImageStencilFilter = vtkPolyDataToImageStencil::New();
	this->Stencil = vtkImageStencil::New();
	this->ForceFilter = vtkImageSobel3D::New();
	this->MagnitudeFilter = vtkImageMagnitude::New();
	this->SobelFilter = vtkImageSobel3D::New();
	this->ImageData = vtkImageData::New();
	this->Deformed = vtkPolyData::New();
	this->NumberOfIterations = 0;

}

//----------------------------------------------------------------------------
vtkEDMInterface::~vtkEDMInterface()
{
	this->DeformableMesh->Delete();
	this->ImageStencilFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkEDMInterface::Init()
{
	this->Deformed->ShallowCopy(this->Reader->GetOutput());

	//Prepare white image data
	double spacing[3];
	spacing[0] = spacing[1] = spacing[2] = 0.5;
	this->ImageData->SetSpacing(spacing);

	this->Deformed->Update();

	double * bounds = this->Deformed->GetBounds();
	int dim[3];
	for (int i = 0; i < 3; i++)
	{
		dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
	}
	this->ImageData->SetDimensions(dim);
	this->ImageData->SetExtent(0, dim[0]-1, 0, dim[1]-1, 0, dim[2]-1);

	double origin[3];
	origin[0] = bounds[0];
	origin[1] = bounds[2];
	origin[2] = bounds[4];
	this->ImageData->SetOrigin(origin);
	this->ImageData->SetScalarTypeToUnsignedChar();
	this->ImageData->AllocateScalars();

	// fill the image with foreground voxels:
	unsigned char inval = 255;
	unsigned char outval = 0;
	vtkIdType count = this->ImageData->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		this->ImageData->GetPointData()->GetScalars()->SetTuple1(i, inval);
	}

	//Define conversion
	this->ImageStencilFilter->SetInput(this->Deformed);
	this->ImageStencilFilter->SetOutputOrigin(origin);
	this->ImageStencilFilter->SetOutputSpacing(spacing);
	this->ImageStencilFilter->SetOutputWholeExtent(this->ImageData->GetExtent());

	//Generate Stencil
	this->Stencil->SetInput(this->ImageData);
	this->Stencil->SetStencil(this->ImageStencilFilter->GetOutput());
	this->Stencil->ReverseStencilOff();
	this->Stencil->SetBackgroundValue(outval);

	//Compute forces
	this->ForceFilter->SetInput(this->Stencil->GetOutput());
	this->MagnitudeFilter->SetInput(this->ForceFilter->GetOutput());
	this->SobelFilter->SetInput(this->MagnitudeFilter->GetOutput());

	//Set deformable mesh inputs
	this->DeformableMesh->IterateFromZeroOff();
	this->DeformableMesh->SetScaleFactor(0.001);
	this->DeformableMesh->SetNumberOfIterations(this->NumberOfIterations);
	this->DeformableMesh->SetInput(0, this->Reader->GetOutput());
	this->DeformableMesh->SetInputConnection(1, this->SobelFilter->GetOutputPort());
}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkEDMInterface::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	// Get the info objects
	//vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// Get the input and output
	//vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));


	/*this->Collisions->InitTraversal();
	double point[3];
	while(vtkCollision * contact = this->Collisions->GetNextCollision())
	{
		if(contact->GetCollisionType() == vtkCollision::ToolOrgan)
		{
			//Apply displacement
			vtkIdType id = contact->GetPointId(1);
			this->Deformed->GetPoint(id, point);
			double * d = contact->GetDisplacement();
			cout << point[0] << ", "<< point[1] << ", " << point[2] << "\n";
			point[0] += d[0];
			point[1] += d[1];
			point[2] += d[2];
			//cout << point[0] << ", "<< point[1] << ", " << point[2] << "\n";
			this->Deformed->GetPoints()->SetPoint(id, point);
		}
	}

	//Reset contacts
	this->Collisions->RemoveCollisions();*/

	this->Deformed->ShallowCopy(this->Reader->GetOutput());

	this->ImageData->Update();

	this->ImageStencilFilter->Update();

	this->Stencil->Update();

	//vtkMetaImageWriter * writer = vtkMetaImageWriter::New();
	//writer->SetFileName("edm.mhd");
	//writer->SetInput(this->Stencil->GetOutput());
	//writer->Write();


	this->DeformableMesh->Update();

	//this->SobelFilter->Update();
	//this->SobelFilter->GetOutput()->Print(cout);
	//writer->SetFileName("sobel.mhd");
	//writer->SetInput(this->SobelFilter->GetOutput());
	//writer->Write();

	this->Actor->SetUserMatrix(this->Transform->GetMatrix());

	output->ShallowCopy(this->DeformableMesh->GetOutput());
	//output->ShallowCopy(this->Deformed);

	return 1;
}

//--------------------------------------------------------------------------
void vtkEDMInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
