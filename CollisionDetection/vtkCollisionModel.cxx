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

	this->Collisions = vtkCollisionCollection::New();

	this->TransformFilter = vtkTransformPolyDataFilter::New();

	this->Radius = 1.0;
	this->Sphere = vtkSphereSource::New();
	this->Glyphs = vtkGlyph3D::New();
}

//--------------------------------------------------------------------------
vtkCollisionModel::~vtkCollisionModel()
{
	this->TransformFilter->Delete();
	this->Sphere->Delete();
	this->Glyphs->Delete();
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
	if(this->Collisions) this->Collisions->Delete();
	this->Collisions = c;
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

	//Filter to apply transformations to the mesh
	this->TransformFilter->SetInput(this->GetInput());
	this->TransformFilter->SetTransform(this->Transform);

	// Configure visualization
	this->Glyphs->SetInput(this->GetInput());
	this->Sphere->SetRadius(0.05);
	this->Glyphs->SetSource(this->Sphere->GetOutput());
	this->Glyphs->ScalingOff();

	this->Mapper->SetInput(this->Glyphs->GetOutput());
}

//----------------------------------------------------------------------------
int vtkCollisionModel::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	//cout << "vtkCollisionModel::RequestData" << endl;

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkInformation *outInfo1 = outputVector->GetInformationObject(1);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	//Optional input
	vtkPolyData * source = 0;
	if(sourceInfo)
	{
		source = vtkPolyData::SafeDownCast(
			sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
	}
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *outputTx = vtkPolyData::SafeDownCast(outInfo1->Get(vtkDataObject::DATA_OBJECT()));

	//Update object position
	this->Transform->Update();
	this->TransformFilter->Update();

	//Update position & orientation
	this->Velocity[0] = this->Position[0];
	this->Velocity[1] = this->Position[1];
	this->Velocity[2] = this->Position[2];
	this->Acceleration[0] = this->Velocity[0];
	this->Acceleration[1] = this->Velocity[1];
	this->Acceleration[2] = this->Velocity[2];

	//Get transformed values
	this->Transform->GetPosition(this->Position);
	this->Transform->GetOrientation(this->Orientation);

	//Update object velocity
	//Velocity will be calculated from delta(Position)/dt
	vtkMath::Subtract(this->Position, this->Velocity, this->Velocity);
	vtkMath::MultiplyScalar(this->Velocity, 1/this->DeltaT);

	//Update object acceleration
	vtkMath::Subtract(this->Velocity, this->Acceleration, this->Acceleration);
	vtkMath::MultiplyScalar(this->Acceleration, 1/this->DeltaT);

	//If source is defined -> Synchronize mesh
	if(source)
	{
		if(this->HashMap->GetNumberOfIds() == 0)
		{
			//Build collision mesh hash map
			this->BuildHashMap(input, source);
		}
		for(int i=0; i<this->HashMap->GetNumberOfIds(); i++)
		{
			double * p = source->GetPoint(this->HashMap->GetId(i));
			//double * pv = input->GetPoint(i);
			//cout << "Map[" << i << "]: " << this->CollisionHashMap->GetId(i) <<  endl;
			input->GetPoints()->SetPoint(i, p);
			//double * po = input->GetPoint(i);
			//if(i==20) cout << "ps: " << p[0] << ", " << p[1] << ", " << p[2] <<  " | po: " << po[0] << ", " << po[1] << ", " << po[2] << "\n";
		}
	}

/*	if(source)
	{
		cout << "source is present\n";
		this->SmoothFilter->SetInput(input);
		this->SmoothFilter->SetSource(source);
		this->SmoothFilter->Update();
		this->Glyphs->SetInput(this->SmoothFilter->GetOutput());
		output->ShallowCopy(this->SmoothFilter->GetOutput());
	}
	else
	{
		this->Glyphs->SetInput(input);
		output->ShallowCopy(input);
	}*/

	//Set visualization parameters
	this->Actor->GetProperty()->SetColor(this->Color);
	this->Actor->GetProperty()->SetOpacity(this->Opacity);
	this->Actor->SetVisibility(this->Visibility);

	this->Glyphs->Update();
	this->Mapper->Modified();
	this->Mapper->Update();

	//collision visualization mesh
	output->ShallowCopy(input);
	//Transformed mesh for collision detection purposes
	outputTx->ShallowCopy(this->TransformFilter->GetOutput());

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
