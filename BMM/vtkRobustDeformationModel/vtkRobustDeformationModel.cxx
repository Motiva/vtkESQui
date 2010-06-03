#include "vtkRobustDeformationModel.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkRobustDeformationModel, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkRobustDeformationModel);

//----------------------------------------------------------------------------
vtkRobustDeformationModel::vtkRobustDeformationModel()
{
}

//----------------------------------------------------------------------------
vtkRobustDeformationModel::~vtkRobustDeformationModel()
{
}

//----------------------------------------------------------------------------
void vtkRobustDeformationModel::SetContacts(vtkIdList * ids, vtkDoubleArray * directions)
{
	double point[3];

	vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(this->GetInput());
	vtkPoints * coords = vtkPoints::New();
	input->GetPoints()->GetPoints(ids, coords);

	for(vtkIdType i = 0; i < coords->GetNumberOfPoints(); i++)
	{
		//Get input mesh point position
		coords->GetPoint(i, point);
		vtkIdType id = ids->GetId(i);

		//Get direction vector of the contact
		double * dir = directions->GetTuple(i);
		//Retrieve current Position from ugrid Accelerator
		double* currPos = const_cast<double*>(this->gridAccel->getPoint(id));

		cout << "id [" << id << "]\n";
		cout << "currPos: " << currPos[0] << ", " << currPos[1] << ", " << currPos[2] << ", " << endl;
		cout << "point: " << point[0] << ", " << point[1] << ", " << point[2] << ", " << endl;
		cout << "dir: " << dir[0] << ", " << dir[1] << ", " << dir[2] << ", " << endl;

		//Set fictional force (constant direction)
		currPos[0] = point[0] + dir[0];
		currPos[1] = point[1] + dir[1];
		currPos[2] = point[2] + dir[2];
		cout << "newPos: " << currPos[0] << ", " << currPos[1] << ", " << currPos[2] << ", " << endl;
	}

	this->Modified();


	/*this->SetControlIds(ids);

		coords->GetPoint(id, point);
		double * force = forces->GetTuple(id);
		//TODO: Force displacement calculation
		point[0] += force[0]/10;
		point[1] += force[1]/10;
		point[2] += force[2]/10;

		coords->SetPoint(id, point);
		//std::cout << "[vtkRobustDeformationModel] point + force: " << point[0] << " | "<< point[1] << " | " << point[2] << "\n";

	}
	this->SetControlPoints(coords);*/
}
//----------------------------------------------------------------------------
void vtkRobustDeformationModel::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
