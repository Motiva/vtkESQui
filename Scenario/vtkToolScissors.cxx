#include "vtkToolScissors.h"

vtkCxxRevisionMacro(vtkToolScissors, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkToolScissors);

//--------------------------------------------------------------------------
vtkToolScissors::vtkToolScissors(){
	this->Opening=0;

	//Physical pieces construction
	vtkPiece * piece;

	for (vtkIdType id = 0; id < 3 ; id++)
	{
		piece = vtkPiece::New();
		piece->SetId(id);
		this->Pieces->InsertNextPiece(piece);
	}

	// Tool Piece Types (id)
	// 0 -> Stick
	// 1 -> Blade 1
	// 2 -> Blade 2
	this->Pieces->GetPiece(0)->SetType(vtkPiece::Stick);
	this->Pieces->GetPiece(1)->SetType(vtkPiece::Blade);
	this->Pieces->GetPiece(2)->SetType(vtkPiece::Blade);
}

//--------------------------------------------------------------------------
vtkToolScissors::~vtkToolScissors(){

	for (vtkIdType id = 0; id < 3 ; id++)
	{
		this->GetPiece(id)->Delete();
	}

	this->Pieces->RemoveAllItems();
	this->Pieces->Delete();
}
//--------------------------------------------------------------------------
void vtkToolScissors::Init(){

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

//--------------------------------------------------------------------------
void vtkToolScissors::Update()
{
	this->Superclass::Update();
}

//--------------------------------------------------------------------------
void vtkToolScissors::SetStickFileName(const char * path)
{
	this->GetStick()->SetFileName(path);
}

//--------------------------------------------------------------------------
void vtkToolScissors::SetBladeFileName(vtkIdType id, const char * path)
{
	this->GetBlade(id)->SetFileName(path);
}

//--------------------------------------------------------------------------
const char * vtkToolScissors::GetBladeFileName(vtkIdType id)
{
	return this->GetBlade(id)->GetFileName();
}

//--------------------------------------------------------------------------
const char * vtkToolScissors::GetStickFileName(vtkIdType id)
{
	return this->GetStick()->GetFileName();
}

//--------------------------------------------------------------------------
void vtkToolScissors::PrintSelf(ostream& os,vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
}

