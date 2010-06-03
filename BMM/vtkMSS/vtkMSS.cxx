#include "vtkMSS.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkMSS, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkMSS);

//----------------------------------------------------------------------------
vtkMSS::vtkMSS()
{
}

//----------------------------------------------------------------------------
vtkMSS::~vtkMSS()
{
}

//----------------------------------------------------------------------------
void vtkMSS::SetContacts(vtkIdList * ids, vtkDoubleArray * directions)
{
	vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(this->GetInput());

	this->SetControlIds(ids);
	vtkPoints * coords = vtkPoints::New();
	input->GetPoints()->GetPoints(ids, coords);
	double point[3];
	for(vtkIdType id = 0; id < coords->GetNumberOfPoints(); id++)
	{
		coords->GetPoint(id, point);
		double * direction = directions->GetTuple(id);
		point[0] += direction[0];
		point[1] += direction[1];
		point[2] += direction[2];
		coords->SetPoint(id, point);
		//std::cout << "[vtkMSS] point + force: " << point[0] << " | "<< point[1] << " | " << point[2] << "\n";

	}
	this->SetControlPoints(coords);
}
//----------------------------------------------------------------------------
void vtkMSS::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
