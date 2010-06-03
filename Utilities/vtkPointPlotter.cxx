
#include "vtkPointPlotter.h"

vtkCxxRevisionMacro(vtkPointPlotter, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkPointPlotter);

//----------------------------------------------------------------------------

vtkPointPlotter::vtkPointPlotter()
{
	this->Actor = vtkActor::New();
	this->Mapper = vtkPolyDataMapper::New();
	this->PolyData = vtkPolyData::New();

	this->Glyphs = vtkGlyph3D::New();
	this->Spheres = vtkSphereSource::New();

	this->Points = vtkPoints::New();
	this->Scalars = vtkUnsignedCharArray::New();
}

//--------------------------------------------------------------------------
vtkPointPlotter::~vtkPointPlotter()
{
	this->Actor->Delete();
	this->Mapper->Delete();
	this->PolyData->Delete();

	this->Glyphs->Delete();
	this->Spheres->Delete();

	this->Points->Delete();
	this->Scalars->Delete();
}

//--------------------------------------------------------------------------
void vtkPointPlotter::InsertPoint(double x, double y, double z, unsigned char r, unsigned char g, unsigned char b)
{
	this->Points->InsertNextPoint(x, y, z);
	this->Scalars->InsertNextTuple3(r, g, b);

	this->PolyData->SetPoints(this->Points);
	this->PolyData->Update();

}

//--------------------------------------------------------------------------
void vtkPointPlotter::Init()
{
	this->Points->Reset();
	this->Scalars->SetNumberOfComponents(3);

	this->Spheres->SetRadius(this->Radius);
	this->Spheres->SetThetaResolution(this->Resolution);
	this->Spheres->SetPhiResolution(this->Resolution);

	this->PolyData->GetPointData()->SetScalars(this->Scalars);

	this->Glyphs->SetInput(this->PolyData);
	this->Glyphs->SetSource(this->Spheres->GetOutput());
	this->Glyphs->SetColorModeToColorByScalar();
	this->Glyphs->SetScaleModeToDataScalingOff();

	this->Mapper->SetInput(this->Glyphs->GetOutput());
	this->Mapper->GlobalImmediateModeRenderingOn();
	this->Mapper->ScalarVisibilityOn();
	this->Actor->SetMapper(this->Mapper);

	this->Renderer->AddActor(this->Actor);

}

//--------------------------------------------------------------------------
void vtkPointPlotter::Update()
{
	this->Glyphs->Update();
}

//--------------------------------------------------------------------------
void vtkPointPlotter::Reset()
{
	this->Points->Reset();
}
