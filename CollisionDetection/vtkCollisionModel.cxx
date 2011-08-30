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
#include "vtkCollisionModel.h"

#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkSphereSource.h"
#include "vtkGlyph3D.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkIdList.h"
#include "vtkPointLocator.h"
#include "vtkTransform.h"
#include "vtkMath.h"

#include "vtkCollision.h"
#include "vtkCollisionCollection.h"

vtkCxxRevisionMacro(vtkCollisionModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkCollisionModel);

//--------------------------------------------------------------------------
vtkCollisionModel::vtkCollisionModel() {

	//Collision model has 2 outputs
	this->SetNumberOfOutputPorts(2);

	this->ModelType = vtkModel::Collision;

	this->DeltaT = 1.0;
	this->Velocity[0]=this->Velocity[1]=this->Velocity[2]=0.0;
	this->Acceleration[0]=this->Acceleration[1]=this->Acceleration[2]=0.0;
	this->Direction[0]=this->Direction[1]=this->Direction[2]=0.0;

	this->Collisions = NULL;
	this->Transform = NULL;
	this->TransformFilter = NULL;
	this->Sphere = NULL;
	this->Glyphs = NULL;
	this->Radius = 0.05;
}

//--------------------------------------------------------------------------
vtkCollisionModel::~vtkCollisionModel()
{
	if(this->Sphere) this->Sphere->Delete();
	if(this->Glyphs) this->Glyphs->Delete();
	if(this->Transform) this->Transform->Delete();
	if(this->TransformFilter) this->TransformFilter->Delete();
}

//----------------------------------------------------------------------------
int vtkCollisionModel::FillOutputPortInformation(int port, vtkInformation* info)
{
	if( port==0 ) info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
	else if (port == 1) info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
	return 1;
}

//--------------------------------------------------------------------------
void vtkCollisionModel::SetCollisions(vtkCollisionCollection * c)
{
	this->Collisions->RemoveAllItems();
	this->Collisions->DeepCopy(c);
}

//--------------------------------------------------------------------------
vtkCollisionCollection * vtkCollisionModel::GetCollisions()
{
	return this->Collisions;
}

//--------------------------------------------------------------------------
void vtkCollisionModel::AddCollision(vtkCollision * c)
{
	this->Collisions->AddItem(c);
}

//--------------------------------------------------------------------------
void vtkCollisionModel::RemoveCollision(vtkIdType id)
{
	this->Collisions->RemoveItem(id);
}

//--------------------------------------------------------------------------
void vtkCollisionModel::RemoveAllCollisions()
{
	this->Collisions->RemoveAllItems();
}

//--------------------------------------------------------------------------
void vtkCollisionModel::Init()
{
	this->Superclass::Init();

	this->Collisions = vtkCollisionCollection::New();

	this->Transform = vtkTransform::New();
	this->TransformFilter = vtkTransformPolyDataFilter::New();

	if(this->Matrix)
			this->Transform->SetMatrix(this->Matrix);
	else this->Matrix = this->Transform->GetMatrix();

	//Filter to apply transformations to the mesh
	this->TransformFilter->SetInputConnection(this->GetOutputPort());
	this->TransformFilter->SetTransform(this->Transform);

	// Configure visualization
	this->Sphere = vtkSphereSource::New();
	this->Glyphs = vtkGlyph3D::New();
	this->Glyphs->SetInputConnection(this->GetOutputPort());
	this->Sphere->SetRadius(this->Radius);
	this->Glyphs->SetSource(this->Sphere->GetOutput());
	this->Glyphs->ScalingOff();

	this->Mapper->SetInput(this->Glyphs->GetOutput());
}

//----------------------------------------------------------------------------
int vtkCollisionModel::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkInformation *outInfo1 = outputVector->GetInformationObject(1);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	// Optional input
	vtkPolyData * source = 0;
	if(sourceInfo){
		source = vtkPolyData::SafeDownCast(sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
	}

	// Main output
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
	// Secondary output for collision purposes
	vtkPolyData *outputTx = vtkPolyData::SafeDownCast(outInfo1->Get(vtkDataObject::DATA_OBJECT()));

	//cout << this->GetClassName() << "::RequestData (" << this->GetName() << ")\n";

	if(this->Status == Enabled)
	{
		//collision visualization mesh
		output->ShallowCopy(input);

		//If source is defined -> Synchronize mesh
		if(source)
		{
			if(this->HashMap->GetNumberOfIds() == 0)
			{
				//Build collision mesh hash map
				this->BuildHashMap(input, source);
			}

			//Synchronize both meshes
			vtkPoints * points = output->GetPoints();
			for(int i = 0; i<this->HashMap->GetNumberOfIds(); i++)
			{
				int id = this->HashMap->GetId(i);
				double * p = source->GetPoint(id);
				//double * po = points->GetPoint(i);
				//if(id==122) cout << "ps["<< i << "]: " << p[0] << ", " << p[1] << ", " << p[2] <<  " | po["<< id <<"]: " << po[0] << ", " << po[1] << ", " << po[2] << "\n";
				points->SetPoint(i, p[0], p[1], p[2]);
			}
		}

		//Set visualization parameters
		this->Actor->SetVisibility(this->Visibility);
		if(this->IsVisible())
		{
			this->Glyphs->Modified();
			//this->Glyphs->Update();
			this->Actor->GetProperty()->SetColor(this->Color);
			this->Actor->GetProperty()->SetOpacity(this->Opacity);
			this->Mapper->Modified();
		}


		//Update object position
		this->Transform->SetMatrix(this->Matrix);
		this->Transform->Update();

		//Transformed mesh for collision detection purposes
		outputTx->ShallowCopy(this->TransformFilter->GetOutput());
	}
	else
	{
		//Transformed mesh for collision detection purposes
		outputTx->ShallowCopy(input);
		//collision visualization mesh
		output->ShallowCopy(input);
	}



	return 1;
}

//--------------------------------------------------------------------------
void vtkCollisionModel::PrintSelf(ostream&os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
	os << indent << "Direction: " << this->Direction[0] << ", " << this->Direction[1] << ", " << this->Direction[2] << endl;
	os << indent << "Velocity: " << this->Velocity[0] << ", " << this->Velocity[1] << ", " << this->Velocity[2] << endl;
	os << indent << "Acceleration: " << this->Acceleration[0] << ", " << this->Acceleration[1] << ", " << this->Acceleration[2] <<  endl;
	os << indent << "DeltaT: " << this->DeltaT << endl;
}
