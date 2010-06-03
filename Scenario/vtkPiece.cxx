#include "vtkPiece.h"

vtkCxxRevisionMacro(vtkPiece, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkPiece);

//--------------------------------------------------------------------------
vtkPiece::vtkPiece()
{
	this->Name = "";
	this->FileName = NULL;
	this->Type = vtkPiece::Stick;
	this->Id = -1;
	this->Reader = vtkPolyDataReader::New();
	this->PolyData = vtkPolyData::New();
	this->TransformFilter = vtkTransformPolyDataFilter::New();
	this->Transform = vtkTransform::New();
	this->Actor = vtkActor::New();
	this->Mapper = vtkDataSetMapper::New();

	//Bounding Box Display
	this->OBB = vtkOBBTree::New();
	this->SimpleMesh = vtkPolyData::New();
	this->SimpleMeshActor = vtkActor::New();
	this->SimpleMeshMapper = vtkDataSetMapper::New();
}

//--------------------------------------------------------------------------
vtkPiece::~vtkPiece()
{
	this->TransformFilter->Delete();
	this->Transform->Delete();
	this->Actor->Delete();
	this->Mapper->Delete();
	this->PolyData->Delete();
	this->Reader->Delete();

	this->OBB->Delete();
	this->SimpleMesh->Delete();
	this->SimpleMeshActor->Delete();
	this->SimpleMeshMapper->Delete();
}

//--------------------------------------------------------------------------
void vtkPiece::Init()
{
	//Read polydata source file
	if(this->FileName)
	{
		this->Reader->SetFileName(this->FileName);
		this->Reader->SetOutput(this->PolyData);
		this->Reader->Update();
	}

	if(this->Renderer)
	{
		this->TransformFilter->SetInput(this->PolyData);
		this->TransformFilter->SetTransform(this->Transform);
		this->TransformFilter->Update();
		this->Mapper->SetInput(this->TransformFilter->GetOutput());
		this->Actor->SetMapper(this->Mapper);

		//Bounding Box Display
		this->Update();
		this->SimpleMeshMapper->SetInput(this->GetSimpleMesh());
		this->SimpleMeshActor->SetMapper(SimpleMeshMapper);
		this->SimpleMeshActor->GetProperty()->SetColor(0.5, 0.8, 0.5);
		this->SimpleMeshActor->GetProperty()->SetOpacity(0.1);

		this->Renderer->AddActor(this->Actor);
		this->Renderer->AddActor(this->SimpleMeshActor);
	}

}

//--------------------------------------------------------------------------
void vtkPiece::Update()
{
	this->OBB->SetDataSet(this->TransformFilter->GetOutput());
	this->OBB->SetMaxLevel(8);
	this->OBB->SetTolerance(0.0001);
	this->OBB->BuildLocator();

	this->OBB->GenerateRepresentation(4,this->SimpleMesh);
	//this->SimpleMesh->DeepCopy(this->TransformFilter->GetOutput());
}

//--------------------------------------------------------------------------
void vtkPiece::SetFileName(const char * name)
{
	this->FileName = name;
}

//--------------------------------------------------------------------------
const char * vtkPiece::GetFileName()
{
	return this->FileName;
}

//--------------------------------------------------------------------------
void vtkPiece::SetRenderWindow(vtkRenderWindow *window) {
	this->RenderWindow = window;
	this->Renderer= this->RenderWindow->GetRenderers()->GetFirstRenderer();
}

//--------------------------------------------------------------------------
vtkRenderWindow* vtkPiece::GetRenderWindow() {
	return this->RenderWindow;
}

//--------------------------------------------------------------------------
vtkPolyData * vtkPiece::GetSimpleMesh()
{
	return this->SimpleMesh;
}

//--------------------------------------------------------------------------
void vtkPiece::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Name: " << this->Name << endl;
	os << indent << "Id: " << this->Id << endl;
	os << indent << "Type: " << this->Type << endl;
	os << indent << "FileName: " << this->FileName << endl;
	os << indent << "PolyData (#points): " << this->PolyData->GetNumberOfPoints() << endl;

	double * bounds = this->PolyData->GetBounds();
	os << indent << "PolyData Bounds: ";
	for (int i=0;i<6;i++)
	{
		os << bounds[i] << ", ";
	}
	os << indent << "\n";

	bounds = this->Actor->GetBounds();
	os << indent << "Actor Bounds: ";
	for (int i=0;i<6;i++)
	{
		os << bounds[i] << ", ";
	}
	os << indent << "\n";
}
