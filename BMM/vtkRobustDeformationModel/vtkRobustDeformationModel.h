#ifndef __vtkRobustDeformationModel_h
#define __vtkRobustDeformationModel_h

#include "vtkRobustDeformationModelConfigure.h" // Include configuration header.
#include "vtkRDM.h"
#include "vtkIdList.h"
#include "vtkDoubleArray.h"

//! This class inherits vtkMassSpring class contained in the mss_vtk package
class VTK_vtkRobustDeformationModel_EXPORT vtkRobustDeformationModel : public vtkRDM
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkRobustDeformationModel, vtkObject);
	//! Create new robust deformation model
	static vtkRobustDeformationModel* New();
	//! Print object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set the external forces to apply on each point of the mesh according to ids input parameter
	/*!
	* The number of tuples (size=3) of the the forces should match the number of ids
	* \param ids list of organ mesh point ids that has been contacted
	* \param forces vector containing the force in each cartesian component x, y & z
	*/
	void SetContacts(vtkIdList * ids, vtkDoubleArray * forces);

protected:
	vtkRobustDeformationModel();
	~vtkRobustDeformationModel();

private:
	vtkRobustDeformationModel(const vtkRobustDeformationModel&);  // Not implemented.
	void operator=(const vtkRobustDeformationModel&);  // Not implemented.
};

#endif
