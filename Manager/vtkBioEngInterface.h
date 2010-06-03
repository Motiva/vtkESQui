#ifndef __vtkBioEngInterface_h
#define __vtkBioEngInterface_h

#include "vtkESQuiManagerWin32Header.h"

#include "vtkObjectFactory.h"
#include "vtkCollisionDetectionLibrary.h"
#include "vtkCollisionDetectionFilter.h"
#include "vtkPolyDataCollection.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"
#include "vtkPoints.h"
#include "vtkPointLocator.h"

//! This class acts as an interface to the collision detection library

class VTK_ESQUI_MANAGER_EXPORT vtkBioEngInterface: public vtkCollisionDetectionLibrary
{

public:
	vtkTypeRevisionMacro(vtkBioEngInterface, vtkCollisionDetectionLibrary);
	static vtkBioEngInterface *New();
	const char *GetClassName() {return "vtkBioEngInterface";};

	// Inicializa la libreria, reservando el espacio de memoria necesario
	//! Set up the collision detection library with the initial values,
	/*!
	* This method will vary depending on the collision detection library
	*/
	void Init();

	// Detecta colisiones entre un organos y herramientas
	//! Update the collsion detection library
	/*!
	* Executes the collision detection process between an tool and an organ
	*/
	void Update();

protected:

	//! Default constructor
	vtkBioEngInterface();
	//! Default destructor
	~vtkBioEngInterface();

private:

	vtkBioEngInterface(const vtkBioEngInterface &); //NotImplemented
	void operator =(const vtkBioEngInterface &); //Not Implemented

	//! Clear previous executions of the collision detection process
	/*!
	* Should be called every time collision detection is performed
	*/
	void Clear();

	//!Collision detection filter
	vtkCollisionDetectionFilter *DetectionFilter;

	//!Calculation matrix
	vtkMatrix4x4 *Matrix0;
	//!Calculation matrix
	vtkMatrix4x4 *Matrix1;

};

#endif 
