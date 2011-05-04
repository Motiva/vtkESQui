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

	this->Scale[0] = this->Scale[1] = this->Scale[2] = 1.0;
	this->Origin[0]=this->Origin[1]=this->Origin[2]=0.0;
	this->Position[0]=this->Position[1]=this->Position[2]=0.0;
	this->Orientation[0]=this->Orientation[1]=this->Orientation[2]=0.0;
	this->Color[0]=this->Color[1]=this->Color[2]=1.0;
	this->Opacity = 1.0;
	this->Visibility = 1;

	this->Reader = vtkXMLPolyDataReader::New();
	this->SmoothFilter = vtkSmoothPolyDataFilter::New();
	this->Transform = vtkTransform::New();
	this->Actor = vtkActor::New();
	this->Mapper = vtkPolyDataMapper::New();
	this->HashMap = vtkIdList::New();

	//optional second input
	this->SetNumberOfInputPorts(2);
}

//--------------------------------------------------------------------------
vtkModel::~vtkModel()
{
	if(this->Reader) this->Reader->Delete();
	if(this->Transform) this->Transform->Delete();
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
vtkTransform * vtkModel::GetTransform()
{
	return this->Transform;
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
			this->Reader->SetFileName(this->FileName);
			this->Reader->Update();
			this->SetInputConnection(this->Reader->GetOutputPort());
		}

		// Translate model to desired position & orientation
		this->Transform->Translate(this->Position);
		//Rotate model over itself
		this->Transform->RotateX(this->Orientation[0]);
		this->Transform->RotateY(this->Orientation[1]);
		this->Transform->RotateZ(this->Orientation[2]);
		//Set scale model
		this->Transform->Scale(this->Scale);

		//Set smoothed output
		this->SmoothFilter->SetNumberOfIterations(1);

		//Set actor transformation matrix
		this->Actor->SetUserMatrix(this->Transform->GetMatrix());
		this->Actor->SetMapper(this->Mapper);

		//Set as initialized
		this->Initialized = 1;
	}
}

//----------------------------------------------------------------------------
int vtkModel::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	//cout << "vtkModel::RequestData (" << this->GetName() <<  ")\n";
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	//Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(
			inInfo->Get(vtkDataObject::DATA_OBJECT()));
	//Optional input
	vtkPolyData * source = 0;
	if(sourceInfo)
	{
		source = vtkPolyData::SafeDownCast(
			sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
	}
	vtkPolyData *output = vtkPolyData::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));

	//Get transformed values
	this->Transform->Update();

	this->Transform->GetPosition(this->Position);
	this->Transform->GetOrientation(this->Orientation);

	//If source is defined -> Synchronize mesh
	if(source)
	{
		//cout << "source is present\n";
		/*this->SmoothFilter->SetInput(input);
		this->SmoothFilter->SetSource(source);
		this->SmoothFilter->Update();
		this->Mapper->SetInput(this->SmoothFilter->GetOutput());
		this->Mapper->Update();
		output->ShallowCopy(this->SmoothFilter->GetOutput());*/

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
	this->Actor->GetProperty()->SetColor(this->Color);
	this->Actor->GetProperty()->SetOpacity(this->Opacity);
	this->Actor->SetVisibility(this->Visibility);

	this->Mapper->SetInput(input);
	this->Mapper->Modified();
	this->Mapper->Update();

	output->ShallowCopy(input);

	return 1;
}

//--------------------------------------------------------------------------
void vtkModel::BuildHashMap(vtkPolyData * a, vtkPolyData * b)
{
	//Force data to be updated
	a->Update();
	b->Update();

	//Create point locator to generate id map
	vtkPointLocator * locator = vtkPointLocator::New();
	locator->SetDataSet(b);

	this->HashMap->SetNumberOfIds(a->GetNumberOfPoints());

	//cout << this->GetName() << "::HashMap \n";
	for (int i=0; i<a->GetNumberOfPoints(); i++)
	{
		double * point = a->GetPoint(i);
		vtkIdType id = locator->FindClosestPoint(point);
		this->HashMap->SetId(i, id);
		//double * p = b->GetPoint(id);
		//cout << "Map [" << i << "]: " << "("<< point[0] << "," << point[1] << "," << point[2] << ") ["<<
		//		id << "]: " << "("<< p[0] << "," << p[1] << "," << p[2] << ")" <<  endl;
	}
}

//--------------------------------------------------------------------------
void vtkModel::Translate(double * vector)
{
	this->Translate(vector[0], vector[1], vector[2]);
}

//--------------------------------------------------------------------------
void vtkModel::Translate(double x, double y, double z) {
	//cout << "vtkModel::Translate(" << x << ", " << y << ", "<< z << ")" << endl;
	this->Transform->Translate(x, y, z);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::RotateWXYZ(double a, double x, double y, double z) {
	this->Actor->RotateWXYZ(a, x, y, z);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::RotateX(double x) {
	//cout << this->GetName() << "::Rx\n";
	this->Transform->RotateX(x);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::RotateY(double y) {
	//cout << this->GetName() << "::Ry\n";
	this->Transform->RotateY(y);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::RotateZ(double z)
{
	//cout << this->GetName() << "::Rz\n";
	this->Transform->RotateZ(z);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::Reset()
{
	//cout << this->GetName() << "::Reset\n";
	this->Transform->Translate(this->Origin);
	this->Transform->RotateZ(-this->Orientation[2]);
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::Restore()
{
	//cout << this->GetName() << "::Restore\n";
	this->Transform->RotateZ(this->Orientation[2]);
	vtkMath::MultiplyScalar(this->Origin, -1);
	this->Transform->Translate(this->Origin);
	vtkMath::MultiplyScalar(this->Origin, -1);
	this->Modified();
}
//--------------------------------------------------------------------------
void vtkModel::Hide()
{
	this->Status = Hidden;
	this->Visibility = 0;
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::Disable()
{
	this->Status = Disabled;
	this->Visibility = 0;
	this->Modified();
}

//--------------------------------------------------------------------------
void vtkModel::Show()
{
	this->Status = Visible;
	this->Visibility = 1;
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
	os << indent << "Scale: " << this->Scale[0] << ", " << this->Scale[1] << ", " << this->Scale[2] << endl;
	os << indent << "Origin: " << this->Origin[0] << ", " << this->Origin[1] << ", " << this->Origin[2] << endl;
	os << indent << "Orientation: " << this->Orientation[0] << ", " << this->Orientation[1] << ", " << this->Orientation[2] <<  endl;
	os << indent << "Position: " << this->Position[0] << ", " << this->Position[1] << ", " << this->Position[2] << endl;
	os << indent << "Color: " << this->Color[0] << ", " << this->Color[1] << ", " << this->Color[2] << endl;
	os << indent << "Opacity: " << this->Opacity << endl;

}
