#include "vtk3DSImporterEsqui.h"
#include "vtkObjectFactory.h"
#include "vtkStripper.h"
#include "vtkPolyDataNormals.h"
#include "vtkRenderWindow.h"
#include "vtkPolyDataMapper.h"
#include "vtkActorCollection.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"

vtkCxxRevisionMacro(vtk3DSImporterEsqui, "$Revision: 0.1 $");
vtkStandardNewMacro(vtk3DSImporterEsqui);

//--------------------------------------------------------------------------
vtk3DSImporterEsqui::vtk3DSImporterEsqui()
{
  this->Actors= vtkActorCollection::New();
}

//--------------------------------------------------------------------------
void vtk3DSImporterEsqui::ImportActors (vtkRenderer *renderer)
{

  vtk3DSMesh *mesh;
  vtkStripper *polyStripper;
  vtkPolyDataNormals *polyNormals;
  vtkPolyData *polyData;
  vtkPolyData *polyDataAux;

  
  for (mesh = this->MeshList; mesh != (vtk3DSMesh *) NULL; 
       mesh = (vtk3DSMesh *) mesh->next)
    {
    if (mesh->faces == 0)
      {
      vtkWarningMacro (<< "part " << mesh->name << " has zero faces... skipping\n");
      continue;
      }

    polyData = this->GeneratePolyData (mesh);
	
	vtkTransform *Transform = vtkTransform::New();
		Transform->Scale(this->Scale,this->Scale,this->Scale);
	this->Filter= vtkTransformPolyDataFilter::New();
	this->Filter->SetInput(polyData);
	this->Filter->SetTransform(Transform);
	this->Filter->Update();
	

	polyDataAux = this->Filter->GetOutput();

	mesh->aStripper = polyStripper = vtkStripper::New ();

    if (this->ComputeNormals)
      {
      mesh->aNormals = polyNormals = vtkPolyDataNormals::New ();
      polyNormals->SetInput (polyDataAux);
      polyStripper->SetInput (polyNormals->GetOutput ());
      }
    else
      {
      polyStripper->SetInput (polyDataAux);
      }
	Transform->Delete();
	this->Filter->Delete();
	vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
		Mapper->SetInput(polyStripper->GetOutput());
	vtkActor *Actor = vtkActor::New();
		Actor->SetMapper(Mapper);
	Actors->AddItem(Actor);
    vtkDebugMacro (<< "Importing PolyDatas: " << mesh->name);
  }
}

//--------------------------------------------------------------------------
void vtk3DSImporterEsqui::ReadData()
{
  this->ImportActors (this->Renderer);
}

//--------------------------------------------------------------------------
vtkActorCollection *vtk3DSImporterEsqui::GetActorCollection() 
{
	this->Read();
	return this->Actors;
}

//--------------------------------------------------------------------------
void vtk3DSImporterEsqui::SetScale(float fRadius) {
	this->Scale = fRadius;
}

//--------------------------------------------------------------------------
float vtk3DSImporterEsqui::GetScale() {
	return this->Scale;
}
