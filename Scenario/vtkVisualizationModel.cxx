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
#include "vtkVisualizationModel.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkTransform.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkTexture.h"
#include "vtkTextureMapToSphere.h"
#include "vtkTransformTextureCoords.h"
#include "vtkJPEGReader.h"

#include "vtkSyncPolyDataFilter.h"

vtkCxxRevisionMacro(vtkVisualizationModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkVisualizationModel);

//--------------------------------------------------------------------------
vtkVisualizationModel::vtkVisualizationModel() {

  this->SetType(vtkModel::Visualization);
  this->TextureFileName = NULL;
  this->Texture = NULL;
  this->TextureReader = NULL;
  this->TextureMap = NULL;
  this->TextureCoords = NULL;

}

//--------------------------------------------------------------------------
vtkVisualizationModel::~vtkVisualizationModel() {

  if(this->Initialized){
    this->Texture->Delete();
    this->TextureReader->Delete();
    this->TextureMap->Delete();
    this->TextureCoords->Delete();
  }
}

//--------------------------------------------------------------------------
void vtkVisualizationModel::Initialize()
{
  //cout << this->GetClassName() << "::Initialize()\n";
  this->Superclass::Initialize();

  //Texture will be added
  if(this->TextureFileName){
    this->Texture = vtkTexture::New();
    this->TextureReader = vtkJPEGReader::New();
    this->TextureMap = vtkTextureMapToSphere::New();
    this->TextureCoords = vtkTransformTextureCoords::New();
  }
}

//----------------------------------------------------------------------------
int vtkVisualizationModel::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  //Get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  //Optional input
  vtkPolyData * source = 0;

  if(sourceInfo){
    source = vtkPolyData::SafeDownCast(sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  }
  //Output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  //Initialize model
  if(!this->Initialized) this->Initialize();

  //If source is defined -> Synchronize mesh
  if(source)
  {
    vtkDebugMacro("Model source is present\n");

    this->SyncFilter->SetInput(input);
    this->SyncFilter->SetSource(source);
    this->SyncFilter->Update();

    output->ShallowCopy(this->SyncFilter->GetOutput());
  }
  else
  {
    output->ShallowCopy(input);
  }

  //Set visualization parameters
  this->Actor->SetVisibility(this->Visibility);

  if(this->Status){

  	if(this->TextureFileName)
  	{
  		this->Texture->SetInputConnection(this->TextureReader->GetOutputPort());

  		this->TextureMap->SetInput(this->GetInput());
  		this->TextureMap->PreventSeamOn();

  		this->TextureReader->SetFileName(this->TextureFileName);
  		//this->TextureCoords->SetInputConnection(this->TextureMap->GetOutputPort());
  		//this->TextureCoords->SetScale(1, 1, 1);

  		//this->Texture->InterpolateOn();

  		this->Mapper->SetInputConnection(this->TextureMap->GetOutputPort());
  		this->Actor->SetTexture(this->Texture);
  	}
  	else
  	{
  		this->Mapper->SetInput(input);
  	}

  	this->Actor->GetProperty()->SetColor(this->Color);
  	this->Actor->GetProperty()->SetOpacity(this->Opacity);

  	this->Actor->SetMapper(this->Mapper);

  	this->Mapper->Modified();
  }

  return 1;
}

//--------------------------------------------------------------------------
void vtkVisualizationModel::PrintSelf(ostream& os,vtkIndent indent) {

  this->Superclass::PrintSelf(os, indent);
  if(this->TextureFileName) os << indent << "TextureFileName: " << this->TextureFileName << "\n";

}
