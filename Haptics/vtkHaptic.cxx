#include "vtkHaptic.h"
#include <iostream>

using namespace std;

vtkCxxRevisionMacro(vtkHaptic, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkHaptic::vtkHaptic() {
	this->Name = "";
	this->NumberOfTools = 0;
}

//--------------------------------------------------------------------------
vtkHaptic::~vtkHaptic() {
}

//--------------------------------------------------------------------------
void vtkHaptic::SetName(const char * name)
{
	this->Name = name;
}

//--------------------------------------------------------------------------
const char * vtkHaptic::GetName()
{
	return this->Name;
}

//--------------------------------------------------------------------------
void vtkHaptic::SetNumberOfTools(int numberOfTools)
{
	this->NumberOfTools = numberOfTools;
}

//--------------------------------------------------------------------------
int vtkHaptic::GetNumberOfTools()
{
	return this->NumberOfTools;
}

//--------------------------------------------------------------------------
void vtkHaptic::PrintSelf(ostream& os,vtkIndent indent) {

this->Superclass::PrintSelf(os, indent);

os << indent << "Name: " << this->Name << endl;

}
