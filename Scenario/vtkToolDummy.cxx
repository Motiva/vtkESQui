// fichero que implementa la clase para representar las pinzas
#include "vtkToolDummy.h"

vtkCxxRevisionMacro(vtkToolDummy, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolDummy);

//----------------------------------------------------------------------------
vtkToolDummy::vtkToolDummy() {

	//Physical pieces Tool Construction
	vtkPiece * piece;

	for(vtkIdType i = 0; i<2; i++)
	{
		piece = vtkPiece::New();
		piece->SetId(0);
		this->Pieces->InsertNextPiece(piece);
	}

	// Tool Piece Types (id)
	// 0 -> Stick
	this->Pieces->GetPiece(0)->SetType(vtkPiece::Stick);
	this->Pieces->GetPiece(0)->SetType(vtkPiece::Ball);
}

//----------------------------------------------------------------------------
vtkToolDummy::~vtkToolDummy()
{
	for (vtkIdType id = 0; id < this->GetNumberOfPieces() ; id++)
	{
		this->GetPiece(id)->Delete();
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Init() {

	vtkPiece * piece;

	for (vtkIdType id = 0; id < this->Pieces->GetNumberOfPieces(); id++)
	{
		piece = this->GetPiece(id);
		piece->SetRenderWindow(this->RenderWindow);
		piece->Init();

		this->Actors->AddItem((vtkActor*) piece->GetActor());
		this->Transforms->AddItem((vtkTransform*) piece->GetTransform());
	}

	this->ApplyInitialTransform();

	this->Update();
}

//----------------------------------------------------------------------------
void vtkToolDummy::Update()
{
	this->Superclass::Update();
}

//----------------------------------------------------------------------------
void vtkToolDummy::SetPolyData(vtkIdType id, vtkPolyData* polyData)
{
	this->GetPiece(id)->SetPolyData(polyData);
}

void vtkToolDummy::Yaw(double angle)
{
	double step = angle - this->YawAngle;
	if(step != 0)
	{
		this->RotateY(step);
		this->YawAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Pitch(double angle)
{
	//std::cout << "angle: " << angle << " | Pitch: " << this->PitchAngle << std::endl;
	double step = angle - this->PitchAngle;
	if(step != 0)
	{
		this->RotateX(step);
		this->PitchAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::Roll(double angle)
{
	double step = angle - this->RollAngle;
	if(step != 0)
	{
		this->RotateZ(step);
		this->RollAngle = angle;
	}
}

//----------------------------------------------------------------------------
void vtkToolDummy::PrintSelf(ostream& os,vtkIndent indent) {
	this->Superclass::PrintSelf(os,indent);
}
