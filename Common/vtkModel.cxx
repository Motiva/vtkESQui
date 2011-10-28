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
#include "vtkModel.h"

#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkActor.h"
#include "vtkMatrix4x4.h"
#include "vtkProperty.h"
#include "vtkPointLocator.h"
#include "vtkPointData.h"
#include "vtkIdList.h"
#include "vtkMath.h"

vtkCxxRevisionMacro(vtkModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkModel);

//--------------------------------------------------------------------------
vtkModel::vtkModel()
{
	this->Id = -1;
	this->ObjectId = -1;
	this->Name = NULL;
	this->FileName = NULL;
	this->Initialized = 0;
	this->Status = Enabled;
	this->Color[0]=this->Color[1]=this->Color[2]=1.0;
	this->Opacity = 1.0;
	this->Visibility = 1;

	this->Matrix = NULL;
	this->Reader = NULL;
	this->SmoothFilter = NULL;
	this->Actor = NULL;
	this->Mapper = NULL;
	this->HashMap = NULL;

	//optional second input
	this->SetNumberOfInputPorts(2);
}

//--------------------------------------------------------------------------
vtkModel::~vtkModel()
{
	if(this->Reader) this->Reader->Delete();
	if(this->HashMap) this->HashMap->Delete();
}

//----------------------------------------------------------------------------
void vtkModel::SetSource(vtkPolyData *source)
{
	this->SetInput(1, source);
}

//----------------------------------------------------------------------------
vtkPolyData *vtkModel::GetSource()
{
	if (this->GetNumberOfInputConnections(1) < 1)
	{
		return NULL;
	}
	return vtkPolyData::SafeDownCast(
			this->GetExecutive()->GetInputData(1, 0));
}


//---------------------------------------------------------------------------
int vtkModel::FillInputPortInformation(int port, vtkInformation *info)
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
int vtkModel::FillOutputPortInformation(int, vtkInformation* info)
{
	info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
	return 1;
}

//----------------------------------------------------------------------------
void vtkModel::SetMatrix(vtkMatrix4x4 * m)
{
	this->Matrix = m;
}

//----------------------------------------------------------------------------
vtkMatrix4x4 * vtkModel::GetMatrix()
{
	return this->Matrix;
}

//----------------------------------------------------------------------------
vtkActor * vtkModel::GetActor()
{
	return this->Actor;
}

//----------------------------------------------------------------------------
vtkPolyDataMapper* vtkModel::GetMapper()
{
	return this->Mapper;
}
//--------------------------------------------------------------------------
vtkIdList * vtkModel::GetHashMap()
{
	return this->HashMap;
}

//--------------------------------------------------------------------------
void vtkModel::Init()
{
	if(!this->Initialized)
	{
		//Retrieve model input from file
		if(this->FileName) {
			this->Reader = vtkXMLPolyDataReader::New();
			this->Reader->SetFileName(this->FileName);
			this->Reader->Update();
			this->SetInputConnection(this->Reader->GetOutputPort());
		}

		this->SmoothFilter = vtkSmoothPolyDataFilter::New();
		this->Actor = vtkActor::New();
		this->Mapper = vtkPolyDataMapper::New();

		//Set smoothed output
		this->SmoothFilter->SetNumberOfIterations(1);

		//Set actor transformation matrix
		if(this->Matrix)
		{
			this->Actor->SetUserMatrix(this->Matrix);
		}

		this->Actor->SetMapper(this->Mapper);

		this->HashMap = vtkIdList::New();

		//Set as initialized
		this->Initialized = 1;
	}
	this->Modified();
}

//----------------------------------------------------------------------------
int vtkModel::RequestData(
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
		vtkDebugMacro("Model source is present\n");

		if(this->HashMap->GetNumberOfIds() == 0)
		{
			this->BuildHashMap(input, source);
		}

		vtkPoints * points = input->GetPoints();
		for(int i=0; i < points->GetNumberOfPoints(); i++){
			double * p = source->GetPoint(this->HashMap->GetId(i));
			points->SetPoint(i, p);
			//double * po = input->GetPoint(i);
			//if(i==20) cout << "ps: " << p[0] << ", " << p[1] << ", " << p[2] <<  " | po: " << po[0] << ", " << po[1] << ", " << po[2] << "\n";
		}
	}

	//Set visualization parameters
	this->Actor->SetVisibility(this->Visibility);
	if(this->Status == Enabled){
		this->Actor->GetProperty()->SetColor(this->Color);
		this->Actor->GetProperty()->SetOpacity(this->Opacity);

		this->Mapper->SetInput(input);
		this->Mapper->Modified();
		this->Mapper->Update();
	}

	output->ShallowCopy(input);

	return 1;
}

//--------------------------------------------------------------------------
void vtkModel::BuildHashMap(vtkPolyData * a, vtkPolyData * b)
{
	vtkDebugMacro("Build model hashmap.")

	//Force data to be updated
	a->Update();
	b->Update();

	//Create point locator to generate id map
	vtkPointLocator * locator = vtkPointLocator::New();
	locator->SetDataSet(b);

	this->HashMap->SetNumberOfIds(a->GetNumberOfPoints());

	for (int i=0; i<a->GetNumberOfPoints(); i++)
	{
		double * point = a->GetPoint(i);
		vtkIdType id = locator->FindClosestPoint(point);
		this->HashMap->SetId(i, id);
	}
}

//--------------------------------------------------------------------------
void vtkModel::Hide()
{
	this->Visibility = 0.0;
}

//--------------------------------------------------------------------------
void vtkModel::Show()
{
	this->Visibility = 1;
}

//--------------------------------------------------------------------------
void vtkModel::Disable()
{
	this->Status = Disabled;
	this->Hide();
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::Enable()
{
	this->Status = Enabled;
	this->Show();
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::PrintSelf(ostream&os, vtkIndent indent)
{
	os << indent << "Id: " << this->Id << "\n";
	os << indent << "ObjectId: " << this->ObjectId << "\n";
	os << indent << "Model Type: " << this->ModelType << "\n";
	os << indent << "Status: " << this->Status << "\n";
	if(this->Name) os << indent << "Name: " << this->Name << "\n";
	if(this->FileName) os << indent << "FileName: " << this->FileName << "\n";
	os << indent << "Color: " << this->Color[0] << ", " << this->Color[1] << ", " << this->Color[2] << endl;
	os << indent << "Opacity: " << this->Opacity << endl;

}
