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

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkDataSetMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkJPEGReader.h"
#include "vtkTransform.h"
#include "vtkTransformTextureCoords.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTexture.h"
#include "vtkTextureMapToSphere.h"

#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkPointPlotter.h"


vtkCxxRevisionMacro(vtkOrgan, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkOrgan);

//----------------------------------------------------------------------------
vtkOrgan::vtkOrgan()
{
	this->Input = NULL;
	this->Bmm = NULL;

	//I/O Objects
	this->FileName = NULL;
	this->TextureFileName = NULL;
	this->Reader = NULL;

	//Graphical Objects
	this->RenderWindow = NULL;
	this->TransformFilter = NULL;
	this->Transform = NULL;
	this->Actor = NULL;
	this->Mapper = NULL;
	this->Texture = NULL;

	//Default gravity in -z - direction
	this->GravityDirection = 2;
	this->GravityOrientation = -1;

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
	if(this->Bmm) this->Bmm->Delete();
	if(this->Texture) this->Texture->Delete();
}

//--------------------------------------------------------------------------
void vtkOrgan::Init()
{	
	if(!this->Input)
	{
		this->Reader = vtkXMLPolyDataReader::New();
		this->Reader->SetFileName(this->FileName);
		this->Reader->Update();
		this->Input = this->Reader->GetOutput();
	}

	if(this->RenderWindow)
	{
		this->Renderer= this->RenderWindow->GetRenderers()->GetFirstRenderer();

		this->Transform = vtkTransform::New();
		this->TransformFilter = vtkTransformPolyDataFilter::New();
		this->TransformFilter->SetInput(this->Input);
		this->TransformFilter->SetTransform(this->Transform);

		this->Transform->Translate(this->Position);
		this->Transform->RotateX(this->Orientation[0]);
		this->Transform->RotateY(this->Orientation[1]);
		this->Transform->RotateZ(this->Orientation[2]);

		this->TransformFilter->Update();

		/*vtkPolyData * txGrid = this->TransformFilter->GetOutput();
			if (txGrid->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE)
			{
				//Set input mesh where deformation will be calculated
				vtkPoints * ps = vtkPoints::New();
				ps->SetDataTypeToDouble();
				ps->DeepCopy(txGrid->GetPoints());
				txGrid->SetPoints(ps);
				txGrid->Update();
			}*/

		vtkPolyData * output;
		if(this->Bmm)
		{
			this->Bmm->SetInput(this->TransformFilter->GetOutput());
			this->Bmm->Init();
			output = this->Bmm->GetOutput();
		}
		else
		{
			vtkErrorMacro("BioMechanical Model not defined. You must indicate the bmm...");
			output = this->TransformFilter->GetOutput();
		}

		if(!this->TextureFileName || !strcmp(this->TextureFileName, ""))
		{
			//No TextureFile has been defined
			this->Mapper->SetInput(output);
		}
		else
		{
			//Texture will be added
			vtkTextureMapToSphere * map = vtkTextureMapToSphere::New();
			map->SetInput(output);
			map->PreventSeamOn();

			vtkTransformTextureCoords *  xform = vtkTransformTextureCoords::New();
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

		this->Actor->SetMapper(this->Mapper);

		this->Renderer->AddActor(this->Actor);
	}

	this->Update();

}

//--------------------------------------------------------------------------
void vtkOrgan::SetInput(vtkPolyData * data)
{
	this->Input = data;
}

//--------------------------------------------------------------------------
void vtkOrgan::Update()
{
	if(this->Bmm)// && this->Contacts->GetNumberOfItems() > 0)
	{
		this->Bmm->InsertContacts(this->Contacts);
		//Force the Biomechanical Model to be recalculated
		this->Bmm->Modified();
		this->Bmm->Update();
	}
	//Remove Contacts
	this->RemoveContacts();

}

//--------------------------------------------------------------------------
vtkPolyData * vtkOrgan::GetOutput()
{
	return this->Bmm->GetOutput();
}

//--------------------------------------------------------------------------
void vtkOrgan::SetDeformationModel(vtkDeformationModel * bmm)
{
	if(this->Bmm)
	{
		this->Bmm->Delete();
	}
	this->Bmm = bmm;
}

//--------------------------------------------------------------------------
vtkDeformationModel * vtkOrgan::GetBioMechanicalModel()
{
	return this->Bmm;
}

//--------------------------------------------------------------------------
vtkPoints * vtkOrgan::GetContactPoints()
{
	return this->Bmm->GetContactPoints();
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
	if(this->Bmm){
		os << indent << "BMM: " << this->Bmm->GetClassName() << "\n";
	}

}


