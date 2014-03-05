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
#include "vtkPolyDataMapper.h"
#include "vtkIdList.h"
#include "vtkTransform.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkMath.h"
#include "vtkDoubleArray.h"

#include "vtkSmoothPolyDataFilter.h"

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
  this->Reset = 0;
  this->NumberOfIterations = 0;
  this->ImageSource = NULL;
  this->ImageOrigin[0]= this->ImageOrigin[1] = this->ImageOrigin[2] = 0;
  this->ImageSpacing[0]= this->ImageSpacing[1] = this->ImageSpacing[2] = 1;
  this->ImageExtent[0]= this->ImageExtent[1] = this->ImageExtent[2] = 0;
  this->ImageExtent[3]= this->ImageExtent[4] = this->ImageExtent[5] = 0;

}

//----------------------------------------------------------------------------
vtkEDMInterface::~vtkEDMInterface()
{
  this->DeformableMesh->Delete();
  this->ImageStencilFilter->Delete();
}

//--------------------------------------------------------------------------
void vtkEDMInterface::SetImageSpacing(double spacing)
{
  this->SetImageSpacing(spacing, spacing, spacing);
}

//--------------------------------------------------------------------------
void vtkEDMInterface::Initialize()
{
  this->Superclass::Initialize();

  //Save input data
  vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());
  this->Deformed->ShallowCopy(input);

  if(!this->ImageSource)
  {
    //Get Input data bounds to generate image data
    double * bounds = input->GetBounds();
    this->ImageOrigin[0] = bounds[0];
    this->ImageOrigin[1] = bounds[2];
    this->ImageOrigin[2] = bounds[4];

    //Obtain image dimensions
    int dim[3];
    for (int i = 0; i < 3; i++)
    {
      dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / this->ImageSpacing[i]));
      this->ImageExtent[2*i + 1] = dim[i] -1;
    }

    //Apply spacing, dimension and origin values to image data
    this->ImageData->SetSpacing(this->ImageSpacing);
    this->ImageData->SetDimensions(dim);
    this->ImageData->SetExtent(this->ImageExtent);

    this->ImageData->SetOrigin(this->ImageOrigin);
    this->ImageData->SetScalarTypeToUnsignedChar();
    this->ImageData->AllocateScalars();

    //Create binary image.
    unsigned char inval = 255;
    unsigned char outval = 0;

    //Fill the image with foreground voxels:
    vtkIdType count = this->ImageData->GetNumberOfPoints();
    for (vtkIdType i = 0; i < count; ++i)
    {
      this->ImageData->GetPointData()->GetScalars()->SetTuple1(i, inval);
    }

    //Configure Image stencil filter
    this->ImageStencilFilter->SetInput(input);
    this->ImageStencilFilter->SetOutputOrigin(this->ImageOrigin);
    this->ImageStencilFilter->SetOutputSpacing(this->ImageSpacing);
    this->ImageStencilFilter->SetOutputWholeExtent(this->ImageData->GetExtent());

    //Generate Stencil
    this->Stencil->SetInput(this->ImageData);
    this->Stencil->SetStencil(this->ImageStencilFilter->GetOutput());
    this->Stencil->ReverseStencilOff();
    this->Stencil->SetBackgroundValue(outval);

    //Set pipeline for external force computation
    this->ForceFilter->SetInput(this->Stencil->GetOutput());
    this->MagnitudeFilter->SetInput(this->ForceFilter->GetOutput());
    this->SobelFilter->SetInput(this->MagnitudeFilter->GetOutput());

    //Save image source
    this->ImageSource = this->SobelFilter->GetOutput();
  }

  //Set pipeline for deformable mesh
  this->DeformableMesh->IterateFromZeroOff();
  this->DeformableMesh->SetScaleFactor(this->WarpScaleFactor);
  this->DeformableMesh->SetInput(this->Deformed);
  this->DeformableMesh->SetInput(1, this->ImageSource);
  this->DeformableMesh->SetInputArrayToProcess(
          0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Magnitude" );

}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkEDMInterface::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  //Get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  //Output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if(!this->Initialized) this->Initialize();

  if(this->Status)
  {
    if(this->Reset)
    {
      //Reset deformed mesh to initial state (undeformed)
      this->Deformed->DeepCopy(input);

      //Reset deformable mesh
      vtkSmartPointer<vtkSmoothPolyDataFilter> smooth =
          vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
      smooth->SetInput(this->Deformed);
      smooth->SetNumberOfIterations(50);
      smooth->Update();
      this->DeformableMesh->SetInput(0, smooth->GetOutput());
      this->DeformableMesh->SetInput(1, this->ImageSource);
      this->DeformableMesh->SetNumberOfIterations(0);

      this->Reset = 0;
    }

    int n = this->DeformableMesh->GetNumberOfIterations();
    if(n < this->NumberOfIterations)
    {
      //Increment iteration
      this->DeformableMesh->SetNumberOfIterations(n+1);
    }

    //Update model
    this->DeformableMesh->Update();

    vtkPolyData * out = this->DeformableMesh->GetOutput();

    //Set visualization parameters
    this->Actor->SetVisibility(this->Visibility);
    if(this->IsVisible())
    {
      this->Actor->GetProperty()->SetColor(this->Color);
      this->Actor->GetProperty()->SetOpacity(this->Opacity);
      this->Mapper->SetInput(out);
    }

    output->ShallowCopy(out);
  }
  else output->ShallowCopy(input);

  return 1;
}

//--------------------------------------------------------------------------
void vtkEDMInterface::InsertDisplacement(vtkIdType id, double * vector)
{
  this->InsertDisplacement(id, vector[0], vector[1], vector[2]);
}

//--------------------------------------------------------------------------
void vtkEDMInterface::InsertDisplacement(vtkIdType id, double x, double y, double z)
{
  //Note that forces applied must not be big, so the mesh could recover from it
  //New point position must be stored in memory
  vtkPoints * points = this->Deformed->GetPoints();
  double p[3];

  //Get collided point
  points->GetPoint(id, p);

  //Clamping range
  //double r[2];
  //r[0]=-2*this->ImageSpacing[0];
  //r[1]=2*this->ImageSpacing[0];

  //Add displacement
  double d[3];
  d[0] = x;
  d[1] = y;
  d[2] = z;

  //for(int i=0; i<3; i++)
  //  vtkMath::ClampValue(d+i, r);

  //cout << "xyz:" << x << ", " << y << ", " << z << "\t d:" << d[0] << ", " << d[1] << ", " << d[2] << endl;

  vtkMath::Add(p, d, p);
  points->SetPoint(id, p);

  this->Reset = 1;
}

//--------------------------------------------------------------------------
void vtkEDMInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Reset: " << this->Reset << "\n";
  os << indent << "NumberOfIterations: " << this->NumberOfIterations << "\n";
  os << indent << "WarpFactor: " << this->WarpScaleFactor << "\n";
  os << indent << "Image Spacing: " << this->ImageSpacing[0] << ", " << this->ImageSpacing[1] << ", " << this->ImageSpacing[2] << endl;
  os << indent << "Image Origin: " << this->ImageOrigin[0] << ", " << this->ImageOrigin[1] << ", " << this->ImageOrigin[2] << endl;
  os << indent << "Image Extent: " << this->ImageExtent[0] << ", " << this->ImageExtent[1] << ", " << this->ImageExtent[2] << ", " <<
      this->ImageExtent[3] << ", " << this->ImageExtent[4] << ", " << this->ImageExtent[5] << endl;
  this->Superclass::PrintSelf(os,indent);
}
