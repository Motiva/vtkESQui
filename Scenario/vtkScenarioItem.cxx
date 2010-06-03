#include "vtkScenarioItem.h"

vtkCxxRevisionMacro(vtkScenarioItem, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkScenarioItem::vtkScenarioItem() {
	this->Id = -1;
	this->Name="";
	this->ScaleFactor= 0.0;

	this->DisplaySimpleMesh = 0;

	this->Position[0]=this->Position[1]=this->Position[2]=0.0;
	this->Orientation[0]=this->Orientation[1]=this->Orientation[2]=0.0;
	this->Origin[0]=this->Origin[1]=this->Origin[2]=0.0;
	this->Direction[0]=this->Direction[1]=this->Direction[2]=0.0;

	this->SimpleMesh = vtkPolyData::New();
}

//--------------------------------------------------------------------------
vtkScenarioItem::~vtkScenarioItem() {
	this->SimpleMesh->Delete();
}

//--------------------------------------------------------------------------
void vtkScenarioItem::Update()
{
	UpdateDirection();
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetId(vtkIdType id) {
	this->Id = id;
}

//--------------------------------------------------------------------------
vtkIdType vtkScenarioItem::GetId() {
	return this->Id;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetType(vtkScenarioItem::vtkScenarioItemType type)
{
	this->Type = type;
}

//--------------------------------------------------------------------------
vtkScenarioItem::vtkScenarioItemType vtkScenarioItem::GetType()
{
	return this->Type;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetName(const char * name) {
	this->Name = name;
}

//--------------------------------------------------------------------------
const char * vtkScenarioItem::GetName() {
	return this->Name;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetScale(double value) {
	this->ScaleFactor = value;
	this->ScaleFactor/=100.0f;
}

//--------------------------------------------------------------------------
double vtkScenarioItem::GetScale() {
	return this->ScaleFactor;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetRenderWindow(vtkRenderWindow *window) {
	this->RenderWindow = window;
	this->Renderer= RenderWindow->GetRenderers()->GetFirstRenderer();
}

//--------------------------------------------------------------------------
vtkRenderWindow* vtkScenarioItem::GetRenderWindow() {
	return this->RenderWindow;
}

//Origin Point. Reference point where the rotation calculus are made
//--------------------------------------------------------------------------
void vtkScenarioItem::SetOrigin(double x, double y, double z)
{
	this->Origin[0] = x;
	this->Origin[1] = y;
	this->Origin[2] = z;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetOrigin(double origin[3])
{
	this->SetOrigin(origin[0], origin[1], origin[2]);
}

//--------------------------------------------------------------------------
double* vtkScenarioItem::GetOrigin()
{
	return this->Origin;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::GetOrigin(double origin[3])
{
	origin[0] = this->Origin[0];
	origin[1] = this->Origin[1];
	origin[2] = this->Origin[2];
}

//Orientation angles. (yaw, pitch, roll)
//--------------------------------------------------------------------------
void vtkScenarioItem::SetOrientation(double x, double y, double z)
{
	this->Orientation[0] = x;
	this->Orientation[1] = y;
	this->Orientation[2] = z;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetOrientation(double orientation[3])
{
	this->SetOrientation(orientation[0], orientation[1], orientation[2]);
}

//--------------------------------------------------------------------------
double* vtkScenarioItem::GetOrientation()
{
	return this->Orientation;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::GetOrientation(double orientation[3])
{
	orientation[0] = this->Orientation[0];
	orientation[1] = this->Orientation[1];
	orientation[2] = this->Orientation[2];
}

//Initial Position
//--------------------------------------------------------------------------
void vtkScenarioItem::SetPosition(double x, double y, double z)
{
	//WXYZ Position
	this->Position[0] = x;
	this->Position[1] = y;
	this->Position[2] = z;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::SetPosition(double position[3])
{
	this->SetOrientation(position[0], position[1], position[2]);
}

//--------------------------------------------------------------------------
double* vtkScenarioItem::GetPosition()
{
	return this->Position;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::GetPosition(double position[3])
{
	position[0] = this->Position[0];
	position[1] = this->Position[1];
	position[2] = this->Position[2];
}

//--------------------------------------------------------------------------
double * vtkScenarioItem::GetDirection()
{
	return this->Direction;
}

//--------------------------------------------------------------------------
void vtkScenarioItem::GetDirection(double direction[3])
{
	direction[0] = this->Direction[0];
	direction[1] = this->Direction[1];
	direction[2] = this->Direction[2];
}

//--------------------------------------------------------------------------
void vtkScenarioItem::UpdateDirection()
{
	//Update object direction
	//Note: VTK Coordinate systems. X-Rotation angle is inverted (check VTK actor coordinate system. The Visualization Toolkit 4th Edition, Pag. 51).
	this->Direction[0] = -sin(vtkMath::RadiansFromDegrees(this->Orientation[1]))*cos(vtkMath::RadiansFromDegrees(this->Orientation[2]));
	this->Direction[1] = sin(vtkMath::RadiansFromDegrees(this->Orientation[0]))*sin(vtkMath::RadiansFromDegrees(this->Orientation[1]));
	this->Direction[2] = -cos(vtkMath::RadiansFromDegrees(this->Orientation[2]))*cos(vtkMath::RadiansFromDegrees(this->Orientation[0]));
}

//--------------------------------------------------------------------------
void vtkScenarioItem::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Name: " << this->GetName() << "\n";
	os << indent << "Id: " << this->GetId() << "\n";
	os << indent << "Position: " << this->Position[0] << ", " << this->Position[1] << ", " << this->Position[2] << endl;
	os << indent << "Origin: " << this->Origin[0] << ", " << this->Origin[1] << ", " << this->Origin[2] << endl;
	os << indent << "Orientation: " << this->Orientation[0] << ", " << this->Orientation[1] << ", " << this->Orientation[2] <<  endl;
	os << indent << "Direction: " << this->Direction[0] << ", " << this->Direction[1] << ", " << this->Direction[2] <<  endl;

}
