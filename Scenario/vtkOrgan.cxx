#include "vtkOrgan.h"

vtkCxxRevisionMacro(vtkOrgan, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkOrgan);

//----------------------------------------------------------------------------
vtkOrgan::vtkOrgan()
{
	this->Id = -1;
	this->Bmm = NULL;
	this->RenderWindow = NULL;
	this->Input = NULL;

	this->FileName = "";
	this->TextureFileName = "";

	this->Reader = vtkXMLUnstructuredGridReader::New();

	//Graphical Objects
	this->TransformFilter = vtkTransformFilter::New();
	this->Transform = vtkTransform::New();
	this->Actor = vtkActor::New();
	this->Mapper = vtkDataSetMapper::New();

	this->Texture = vtkTexture::New();

	//Empty, undeformed mesh
	this->NumberOfElements = 0;
	this->Deformed = 0;

	//Default gravity in -z - direction
	this->GravityDirection = 2;
	this->GravityOrientation = -1;

	//Incision data
	this->MaxCuttingDistance = 0.0;
	this->CanBeClipped = 0;
	this->Hooked = 0;

	//Initialize organ contact list
	this->Contacts = vtkContactCollection::New();

	//Simple Mesh (collision detection mesh)
	this->SimpleMeshActor = vtkActor::New();
	this->SimpleMeshMapper = vtkDataSetMapper::New();
	//this->OBB = vtkOBBTree::New();

}

//--------------------------------------------------------------------------
vtkOrgan::~vtkOrgan()
{
	this->Reader->Delete();
	this->TransformFilter->Delete();
	this->Transform->Delete();
	this->Actor->Delete();
	this->Mapper->Delete();
	this->Contacts->Delete();
	this->Bmm->Delete();

	this->SimpleMeshActor->Delete();
	this->SimpleMeshMapper->Delete();

	this->Texture->Delete();
}

//--------------------------------------------------------------------------
void vtkOrgan::Init()
{	
	if(this->Bmm)
	{
		if(!this->Input)
		{
			this->Reader->SetFileName(this->FileName);
			this->Reader->Update();
			this->Input = this->Reader->GetOutput();
		}

		if(this->RenderWindow)
		{
			this->TransformFilter->SetInput(this->Input);
			this->TransformFilter->SetTransform(this->Transform);

			this->Transform->Translate(this->Position);
			this->Transform->RotateX(this->Orientation[0]);
			this->Transform->RotateY(this->Orientation[1]);
			this->Transform->RotateZ(this->Orientation[2]);

			this->TransformFilter->Update();

			vtkUnstructuredGrid * txGrid = vtkUnstructuredGrid::SafeDownCast(this->TransformFilter->GetOutputDataObject(0));

			if (txGrid->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE)
			{
				//Set input mesh where deformation will be calculated
				vtkPoints * ps = vtkPoints::New();
				ps->SetDataTypeToDouble();
				ps->DeepCopy(txGrid->GetPoints());
				txGrid->SetPoints(ps);
				txGrid->Update();
			}

			this->Bmm->SetInput(txGrid);
			this->Bmm->Init();

			vtkUnstructuredGrid * output = this->Bmm->GetOutput();
			cout << "Bmm->GetOutput: " << output->GetNumberOfPoints() << endl;

			if(!strcmp(this->TextureFileName, ""))
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

				this->Mapper->SetInputConnection(xform->GetOutputPort());

				vtkJPEGReader * jpegReader = vtkJPEGReader::New();
				jpegReader->SetFileName(this->TextureFileName);
				jpegReader->Update();

				this->Texture->SetInputConnection(jpegReader->GetOutputPort());
				this->Texture->InterpolateOn();

				this->Actor->SetTexture(this->Texture);
			}

			this->Actor->SetMapper(this->Mapper);
			this->Actor->GetProperty()->SetColor(0.8,0.5,0.5);
			this->Renderer->AddActor(this->Actor);

			this->SimpleMeshMapper->SetInput(this->SimpleMesh);
			this->SimpleMeshActor->SetMapper(this->SimpleMeshMapper);
			this->SimpleMeshActor->GetProperty()->SetColor(0.8,0.5,0.5);
			this->SimpleMeshActor->GetProperty()->SetOpacity(0.2);
		}

		this->Update();
	}
	else vtkErrorMacro("BioMechanical Model not defined. You must indicate the bmm...");

}

//--------------------------------------------------------------------------
void vtkOrgan::SetInput(vtkUnstructuredGrid * data)
{
	this->Input = data;
}

//--------------------------------------------------------------------------
void vtkOrgan::Update()
{
	if(this->Contacts->GetNumberOfItems() > 0)
	{
		this->Bmm->InsertContacts(this->Contacts);
	}

	//Force the Biomechanical Model to be recalculated
	this->Bmm->Modified();
	this->Bmm->Update();

	this->UpdateSimpleMesh();

	//Remove Contacts
	this->RemoveContacts();

}

//--------------------------------------------------------------------------
void vtkOrgan::UpdateSimpleMesh()
{
	vtkPoints * points = this->GetOutput()->GetPoints();
	vtkPolyData * pd = vtkPolyData::New();
	pd->SetPoints(points);
	pd->Update();
	vtkDelaunay2D * del = vtkDelaunay2D::New();
	del->SetInput(pd);
	del->Update();
	this->SimpleMesh->DeepCopy(del->GetOutput());
	pd->Delete();
}

//--------------------------------------------------------------------------
vtkUnstructuredGrid * vtkOrgan::GetOutput()
{
	return this->Bmm->GetOutput();
}

//--------------------------------------------------------------------------
void vtkOrgan::SetBioMechanicalModel(vtkBioMechanicalModel * bmm)
{
	if(this->Bmm)
	{
		this->Bmm->Delete();
	}
	this->Bmm = bmm;
}

//--------------------------------------------------------------------------
vtkBioMechanicalModel * vtkOrgan::GetBioMechanicalModel()
{
	return this->Bmm;
}

//--------------------------------------------------------------------------
void vtkOrgan::SetFileName(const char * name)
{
	this->FileName = name;
}

//--------------------------------------------------------------------------
const char * vtkOrgan::GetFileName()
{
	return this->FileName;
}

//--------------------------------------------------------------------------
void vtkOrgan::SetTextureFileName(const char * name)
{
	this->TextureFileName = name;
}

//--------------------------------------------------------------------------
const char * vtkOrgan::GetTextureFileName()
{
	return this->TextureFileName;
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
void vtkOrgan::SetRenderWindow(vtkRenderWindow *Renderer) {
	this->RenderWindow = Renderer;
	this->Renderer= RenderWindow->GetRenderers()->GetFirstRenderer();
}

//--------------------------------------------------------------------------
vtkRenderWindow* vtkOrgan::GetRenderWindow() {
	return this->RenderWindow;
}

//--------------------------------------------------------------------------
void vtkOrgan::SetActor(vtkActor *Actor)
{
	this->Actor = Actor;
}

//--------------------------------------------------------------------------
vtkActor* vtkOrgan::GetActor()
{
	return this->Actor;
}

//--------------------------------------------------------------------------
void vtkOrgan::SetMapper(vtkDataSetMapper *Mapper)
{
	this->Mapper = Mapper;
}

//--------------------------------------------------------------------------
vtkDataSetMapper* vtkOrgan::GetMapper()
{
	return this->Mapper;
}

//--------------------------------------------------------------------------
void vtkOrgan::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);

	os << indent << "FileName: " << this->FileName << "\n";
	os << indent << "TextureFileName: " << this->TextureFileName << "\n";
	os << indent << "BMM: " << this->Bmm->GetClassName() << "\n";


}


