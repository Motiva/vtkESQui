#include "vtkToolLaparoscopy.h"

vtkCxxRevisionMacro(vtkToolLaparoscopy, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkToolLaparoscopy::vtkToolLaparoscopy() {
	this->Depth = 0;
	this->Opening = 0;
	this->YawAngle = 0;
	this->PitchAngle = 0;
	this->RollAngle = 0;
	this->UseHaptic = 0;
	this->ForceFeedback[0] = this->ForceFeedback[1] = this->ForceFeedback[2] = 0;
}

//--------------------------------------------------------------------------
vtkToolLaparoscopy::~vtkToolLaparoscopy() {

}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::Update()
{
	Superclass::Update();
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::ApplyInitialTransform()
{
	Superclass::ApplyInitialTransform();
	this->YawAngle = this->Orientation[0];
	this->PitchAngle = this->Orientation[1];
	this->RollAngle = this->Orientation[2];
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::SetDepth(double depth)
{
	if(depth != this->Depth)
	{
		double step = this->Depth - depth;
		Superclass::Translate(0, 0, step);
		this->GetTransform(0)->GetPosition(this->Position);
		this->Depth = depth;
	}
}

//--------------------------------------------------------------------------
double vtkToolLaparoscopy::GetDepth()
{
	return this->Depth;
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::AddDepth(double step) {
	//Note the step negative sign. VTK Z Axis is inverted
	double depth = this->Depth - step;
	this->SetDepth(depth);
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateX(double angle)
{
	double roll = this->Orientation[2];

	Superclass::Translate(0, 0, 4);
	Superclass::RotateZ(-roll);
	Superclass::RotateX(angle);
	Superclass::RotateZ(roll);
	Superclass::Translate(0, 0, -4);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateY(double angle)
{
	double roll = this->Orientation[2];

	Superclass::Translate(0, 0, 4);
	Superclass::RotateZ(-roll);
	Superclass::RotateY(angle);
	Superclass::RotateZ(roll);
	Superclass::Translate(0, 0, -4);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::RotateZ(double angle)
{
	Superclass::RotateZ(angle);

	//Update Orientation
	this->GetTransform(0)->GetOrientation(this->Orientation);
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::SetOpening(double opening)
{
	this->Opening = opening;
}

//--------------------------------------------------------------------------
double vtkToolLaparoscopy::GetOpening()
{
	return this->Opening;
}

//--------------------------------------------------------------------------
double *  vtkToolLaparoscopy::GetContactForceValue() {
/*	int NumberOfContacts;
	float *ForceFeedback;
	float *TotalForceFeedback;
	ForceFeedback = new float[3];
	TotalForceFeedback = new float[3];
	TotalForceFeedback[0] = TotalForceFeedback[1] = TotalForceFeedback[2] = 0;
	NumberOfContacts = this->_vContacts.size();
	for (int i = 0; i < NumberOfContacts ; i++) {
		//cout << "Dentro del bucle ITERACION " << i << endl;
		this->_vContacts[i]->GetForceFeedback(ForceFeedback);
		//cout << this->_vContacts[i]->GetXForce() << endl;
		TotalForceFeedback[0] = TotalForceFeedback[0] + ForceFeedback[0];
		TotalForceFeedback[1] = TotalForceFeedback[1] + ForceFeedback[1];
		TotalForceFeedback[2] = TotalForceFeedback[2] + ForceFeedback[2];
		cout << "Fx = " << ForceFeedback[0] << " Fy = " << ForceFeedback[1] << " Fz = " << ForceFeedback[2] << endl;
		cout << "Herramienta asignada " << this->_vContacts[i]->GetTool() << endl;
	}
	TotalForceFeedback[0]  = TotalForceFeedback[0] / NumberOfContacts;
	TotalForceFeedback[1]  = TotalForceFeedback[1] / NumberOfContacts;
	TotalForceFeedback[2]  = TotalForceFeedback[2] / NumberOfContacts;
	cout << "Fx = " << TotalForceFeedback[0] << "Fy = " << TotalForceFeedback[1] << "Fz = " << TotalForceFeedback[2] << endl;
*/
	//FIXME: redo this method
	return this->ForceFeedback;
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::SetForceFeedback(float Force[]){
	this->ForceFeedback[0] = Force[0];
	this->ForceFeedback[1] = Force[1];
	this->ForceFeedback[2] = Force[2];
}

//--------------------------------------------------------------------------
void vtkToolLaparoscopy::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Depth: " << this->Depth << "\n";
	os << indent << "Aperture: " << this->Opening << "\n";
	os << indent << "Pitch: " << this->PitchAngle << "\n";
	os << indent << "Yaw: " << this->YawAngle << "\n";
	os << indent << "Roll: " << this->RollAngle << "\n";
	os << indent << "ForceFeedback: " << this->ForceFeedback[0] <<  ", " << this->ForceFeedback[1] <<  ", " << this->ForceFeedback[2] << "\n";

}
