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
#include "vtkSyncPolyDataFilter.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkIdList.h"
#include "vtkPointLocator.h"
#include "vtkMatrix4x4.h"
#include "vtkProperty.h"

vtkCxxRevisionMacro(vtkSyncPolyDataFilter, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSyncPolyDataFilter);

//--------------------------------------------------------------------------
vtkSyncPolyDataFilter::vtkSyncPolyDataFilter()
{
  this->HashMap = NULL;
  this->Locator = NULL;
  this->Smoothing = 0;
	this->Initialized = 0;

  //optional second input
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(1);
}

//--------------------------------------------------------------------------
vtkSyncPolyDataFilter::~vtkSyncPolyDataFilter()
{
  if(this->Initialized){
    if(this->Locator) this->Locator->Delete();
    if(this->HashMap) this->HashMap->Delete();
  }
}

//----------------------------------------------------------------------------
void vtkSyncPolyDataFilter::SetSource(vtkPolyData *source)
{
  this->SetInput(1, source);
}

//----------------------------------- -----------------------------------------
vtkPolyData *vtkSyncPolyDataFilter::GetSource()
{
  if (this->GetNumberOfInputConnections(1) < 1)
  {
    return NULL;
  }
  return vtkPolyData::SafeDownCast(
      this->GetExecutive()->GetInputData(1, 0));
}


//---------------------------------------------------------------------------
int vtkSyncPolyDataFilter::FillInputPortInformation(int port, vtkInformation *info)
{
  if(!this->Superclass::FillInputPortInformation(port, info))
  {
    return 0;
  }

  if( port == 1 ) // sync mesh
  {
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
  }
  return 1;
}

//----------------------------------------------------------------------------
int vtkSyncPolyDataFilter::FillOutputPortInformation(int, vtkInformation* info)
{
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
  return 1;
}

//--------------------------------------------------------------------------
void vtkSyncPolyDataFilter::Initialize()
{
  if(!this->Initialized)
  {
    vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput(0));
    vtkPolyData * source = vtkPolyData::SafeDownCast(this->GetInput(1));

    this->Locator = vtkPointLocator::New();
    this->Locator->SetDataSet(source);
    this->Locator->BuildLocator();

    this->HashMap = vtkIdList::New();
    this->HashMap->SetNumberOfIds(input->GetNumberOfPoints());

    this->SmoothFilter = vtkSmoothPolyDataFilter::New();
    this->SmoothFilter->SetNumberOfIterations(5);

    for(int i=0; i<input->GetNumberOfPoints(); i++)
    {
      double * p = input->GetPoint(i);
      this->HashMap->SetId(i,this->Locator->FindClosestPoint(p));
    }

    vtkDebugMacro("vtkSyncPolyDataFilter has been initialized.");

    //Set as initialized
    this->Initialized = 1;
  }
  this->Modified();
}

//----------------------------------------------------------------------------
int vtkSyncPolyDataFilter::RequestData(
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

  //If source is defined -> Synchronize mesh
  if(source)
  {
    //Initialize filter
    if(!this->Initialized) this->Initialize();

    vtkDebugMacro("Model source is present\n");

    //Synchronize input and source points
    for(int i=0; i<input->GetNumberOfPoints(); i++)
    {
      double * p = source->GetPoint(this->HashMap->GetId(i));
      input->GetPoints()->SetPoint(i,p);
    }

    if(this->Smoothing)
    {
      this->SmoothFilter->SetInput(input);
      this->SmoothFilter->Update();
      output->ShallowCopy(this->SmoothFilter->GetOutput());
    }
    else
    {
      output->ShallowCopy(input);
    }
  }
  else
  {
    output->ShallowCopy(input);
  }

  return 1;

}

//--------------------------------------------------------------------------
void vtkSyncPolyDataFilter::PrintSelf(ostream&os, vtkIndent indent)
{

}
