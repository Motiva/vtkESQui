#include "vtkContact.h"

vtkCxxRevisionMacro(vtkContact, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkContact);

//--------------------------------------------------------------------------
vtkContact::vtkContact() {
	this->ToolId = this->OrganId = -1;
	this->isGrasped=false;
	this->PointIds = vtkIdList::New();
	this->CellIds = vtkIdList::New();
	this->Points = vtkPoints::New();
}

//--------------------------------------------------------------------------
vtkContact::~vtkContact()
{
	this->PointIds->Delete();
	this->CellIds->Delete();
	this->Points->Delete();
}

//--------------------------------------------------------------------------
void vtkContact::DeepCopy(vtkContact *info) {
	this->OrganId = info->OrganId;
	this->ToolId = info->ToolId;
	this->isGrasped = info->isGrasped;
	this->PointIds->DeepCopy(info->PointIds);
	this->CellIds->DeepCopy(info->CellIds);
	this->Points->DeepCopy(info->Points);
}

//--------------------------------------------------------------------------
void vtkContact::PrintSelf(ostream&os, vtkIndent indent)
{
	os << indent << "Organ Id: " << this->OrganId << endl;
	os << indent << "Tool Id: " << this->ToolId << endl;
	for(int i = 0; i< 2; i++)
	{
		os << indent << "Cell[" << i <<"] Id: " << this->CellIds->GetId(i) << endl;
		os << indent << "Point[" << i <<"] Id: " << this->PointIds->GetId(i)<< endl;
		double * point = this->Points->GetPoint(i);
		os << indent << "Point[" << i <<"] Position: " << point[0] << ", " << point[1] << ", " << point[2] << endl;
	}
	os << indent << "isGrasped: " << this->isGrasped << endl;
}
