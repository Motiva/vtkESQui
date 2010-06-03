#ifndef __vtkBioMechanicalModel_h
#define __vtkBioMechanicalModel_h

#ifdef WIN32
#include <vector>
#else
#include <vector>
#endif

#include "vtkESQuiBMMWin32Header.h"
#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridAlgorithm.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkIdList.h"

#include "vtkContactCollection.h"
#include "vtkContact.h"


//! Generic interface to the Biomechanical Model

class VTK_ESQUI_BMM_EXPORT vtkBioMechanicalModel: public vtkUnstructuredGridAlgorithm
{
public:
	vtkTypeRevisionMacro(vtkBioMechanicalModel, vtkUnstructuredGridAlgorithm);

	static vtkBioMechanicalModel *New();
	const char *GetClassName() {return "vtkBioMechanicalModel";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Update function
	virtual int RequestData(vtkInformation* request,
	                          vtkInformationVector** inputVector,
	                          vtkInformationVector* outputVector);

	//!Initialize the Biomechanical Model
	virtual void Init();
	
	//!Cleans algorithm from previous executions
	virtual void Clear();

	//!Set BioMechanical model name
	void SetName(const char * name);

	//!Get BioMechanical model name
	const char * GetName();

	//!Insert a contact into the biomechanical model
	virtual void InsertNextContact(vtkContact * contact) ;

	//!Insert a collection of contacts into the biomechanical model
	virtual void InsertContacts(vtkContactCollection * collection);

	//!Remove contacts of the biomechanical model
	virtual void DeleteContacts();

	//!Return the contact points
	virtual vtkPoints * GetContactPoints();

	//!Return the organ mesh cell specified with the id
	virtual vtkCell * GetMeshCell(vtkIdType id);

	//!Model Name
	const char * Name;

	//!3D Surface Input
	vtkUnstructuredGrid * Mesh;

	//!Contacted points
	vtkPoints * ContactPoints;

	//!Contacted point ids
	vtkIdList * ContactPointIds;

	//!Contacted cell ids
	vtkIdList * ContactCelltIds;

	//!Contact direction of each contact points
	vtkDoubleArray * Directions;

protected:
	vtkBioMechanicalModel();
	~vtkBioMechanicalModel();

private:
	vtkBioMechanicalModel(const vtkBioMechanicalModel &); //NotImplemented
	void operator =(const vtkBioMechanicalModel &); //Not Implemented
};


#endif 

