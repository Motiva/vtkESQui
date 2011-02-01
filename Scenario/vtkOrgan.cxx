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
#include "vtkOrgan.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkJPEGReader.h"
#include "vtkTransform.h"
#include "vtkTransformTextureCoords.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTexture.h"
#include "vtkTextureMapToSphere.h"
#include "vtkProperty.h"
#include "vtkActor2D.h"
#include "vtkSelectVisiblePoints.h"

#include "vtkLabeledDataMapper.h"

#include "vtkBioMechanicalModel.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"


vtkCxxRevisionMacro(vtkOrgan, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkOrgan);

//----------------------------------------------------------------------------
vtkOrgan::vtkOrgan()
{
	this->Input = NULL;
	this->DeformationModel = NULL;

	//I/O Objects
	this->FileName = NULL;
	this->TextureFileName = NULL;
	this->DeformationModelName = NULL;
	this->Reader = NULL;

	//Graphical Objects
	this->RenderWindow = NULL;
	this->TransformFilter = NULL;
	this->Transform = NULL;
	this->Actor = NULL;
	this->Mapper = NULL;
	this->Texture = NULL;

	// Item type
	this->Type = vtkScenarioItem::Organ;

	//Organ Type
	this->OrganType = Static;

	this->SetNumberOfInputPorts(1);
	this->SetNumberOfOutputPorts(1);

	this->Hooked = 0;

	//Initialize organ contact list
	this->Contacts = vtkContactCollection::New();
}

//--------------------------------------------------------------------------
vtkOrgan::~vtkOrgan()
{
	if(this->Reader) this->Reader->Delete();
	if(this->TransformFilter) this->TransformFilter->Delete();
	if(this->Transform) this->Transform->Delete();
	if(this->Actor) this->Actor->Delete();
	if(this->Mapper) this->Mapper->Delete();
	if(this->Contacts) this->Contacts->Delete();
	if(this->DeformationModel) this->DeformationModel->Delete();
	if(this->Texture) this->Texture->Delete();
}

//--------------------------------------------------------------------------
void vtkOrgan::Init()
{	
	//if(!this->Input)
	//{
	this->Reader = vtkXMLPolyDataReader::New();
	this->Reader->SetFileName(this->FileName);
	this->Reader->Update();
	this->SetInputConnection(this->Reader->GetOutputPort());
	//}

	if(this->RenderWindow)
	{
		this->Renderer= this->RenderWindow->GetRenderers()->GetFirstRenderer();

		this->Transform = vtkTransform::New();
		this->TransformFilter = vtkTransformPolyDataFilter::New();
		this->TransformFilter->SetInput(this->GetInput());
		this->TransformFilter->SetTransform(this->Transform);

		this->Transform->Translate(this->Position);
		this->Transform->RotateX(this->Orientation[0]);
		this->Transform->RotateY(this->Orientation[1]);
		this->Transform->RotateZ(this->Orientation[2]);

		this->TransformFilter->Update();

		if(this->DeformationModel)
		{
			this->DeformationModel->SetInputConnection(this->TransformFilter->GetOutputPort());
			this->DeformationModel->Init();
		}
		else
		{
			vtkWarningMacro("BioMechanical Model not defined. You must indicate the bmm...");
		}

		//Display stuff
		if(!this->TextureFileName || !strcmp(this->TextureFileName, ""))
		{
			//No TextureFile has been defined
			this->Mapper->SetInput(this->GetOutput());
		}
		else
		{
			//Texture will be added
			vtkTextureMapToSphere * map = vtkTextureMapToSphere::New();
			map->SetInput(this->GetOutput());
			map->PreventSeamOn();

			vtkTransformTextureCoords * xform = vtkTransformTextureCoords::New();
			xform->SetInputConnection(map->GetOutputPort());
			xform->SetScale(1, 1, 1);

			this->Mapper = vtkDataSetMapper::New();
			this->Mapper->SetInputConnection(xform->GetOutputPort());

			vtkJPEGReader * jpegReader = vtkJPEGReader::New();
			jpegReader->SetFileName(this->TextureFileName);
			jpegReader->Update();

			this->Texture = vtkTexture::New();
			this->Texture->SetInputConnection(jpegReader->GetOutputPort());
			this->Texture->InterpolateOn();

			this->Actor = vtkActor::New();
			this->Actor->SetTexture(this->Texture);
		}

		//Display PointIds
		if(this->GetDebug())
		{
			vtkPolyDataMapper * pointMapper = vtkPolyDataMapper::New();
			pointMapper->SetInput(this->GetOutput());

			vtkActor * pointActor = vtkActor::New();
			pointActor->GetProperty()->SetPointSize(10);
			pointActor->GetProperty()->SetColor(1,1,.4);

			vtkSelectVisiblePoints * visPoints = vtkSelectVisiblePoints::New();
			visPoints->SetInputConnection(this->GetOutputPort());
			visPoints->SelectInvisibleOff();
			visPoints->SetRenderer(this->Renderer);

			vtkLabeledDataMapper * labelMapper = vtkLabeledDataMapper::New();
			labelMapper->SetInputConnection(visPoints->GetOutputPort());

			vtkActor2D * labelActor = vtkActor2D::New();
			labelActor->SetMapper(labelMapper);
			//this->Renderer->AddActor(pointActor);
			this->Renderer->AddActor(labelActor);

			//TODO: Remove "returning AbortExecute of 0"
			this->SetDebug(0);
		}

		this->Actor->SetMapper(this->Mapper);
		this->Renderer->AddActor(this->Actor);
	}

}

//--------------------------------------------------------------------------
int vtkOrgan::RequestData(vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector)
{
	// get the info objects
	//vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	//vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	if(this->DeformationModel)
	{
		this->DeformationModel->InsertContacts(this->Contacts);
		this->DeformationModel->Update();

		output->ShallowCopy(this->DeformationModel->GetOutput());
	}
	else
	{
		output->ShallowCopy(this->TransformFilter->GetOutput());
	}

	//clean previous executions
	this->CleanContacts();

	return 1;
}

//--------------------------------------------------------------------------
void vtkOrgan::SetDeformationModel(vtkBioMechanicalModel * bmm)
{
	if(this->DeformationModel)
	{
		this->DeformationModel->Delete();
	}
	this->DeformationModel = bmm;
}

//--------------------------------------------------------------------------
vtkBioMechanicalModel * vtkOrgan::GetDeformationModel()
{
	return this->DeformationModel;
}

//--------------------------------------------------------------------------
void vtkOrgan::Cauterize(vtkIdType element)
{
	//TODO: Fill in this method
}

//--------------------------------------------------------------------------
void vtkOrgan::Cut(vtkIdList * ids)
{
	//TODO: Fill in this method
}

//--------------------------------------------------------------------------
void vtkOrgan::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os,indent);

	os << indent << "FileName: " << this->FileName << "\n";
	os << indent << "TextureFileName: " << this->TextureFileName << "\n";
	if(this->DeformationModel){
		os << indent << "DeformationModel: " << this->DeformationModel->GetClassName() << "\n";
	}

}


