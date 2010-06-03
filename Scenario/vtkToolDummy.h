#ifndef __vtkToolPincers_h
#define __vtkToolPincers_h

#include "vtkESQuiScenarioWin32Header.h"

#include "vtkToolLaparoscopy.h"
#include "vtkPoints.h"
#include "vtk3DSImporterEsqui.h"
#include "vtkPolyDataReader.h"
#include "vtkTransformPolyDataFilter.h"

#include "vtkDoubleArray.h"
#include "vtkStringArray.h"

// Fichero que contiene la clase que representa las pinzas

//! Implementation class of pincers
/*!
vtkToolGPincers is the class that implements the pincers tool, inside the scene.
Contains methods for position control of the tool in the scene and collision detection.
Pincers are composed of 3 pieces: 1 stick and 2 graspers (vtkPiece)
*/

class VTK_ESQUI_SCENARIO_EXPORT vtkToolDummy: public vtkToolLaparoscopy
{
public:

	//!Create new vtkToolGPincers object
	static vtkToolDummy *New();

	//!Return class name
	const char *GetClassName() { return "vtkToolGPincers"; }
	vtkTypeRevisionMacro(vtkToolDummy,vtkToolLaparoscopy);

	//!Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Initialize the tool from VTK file
	/*!
	This function initializes the tool whose mesh is described on a VTK file
	*/
	virtual void Init();

	//!Update the tool
	/*!
	* This function update the tool values
	*/
	virtual void Update();

	//! Set Polydata object to the tool.
	//! \param id identifier of the piece
	void SetPolyData(vtkIdType id, vtkPolyData* polyData);

	//! Sets the pincers yaw angle
	void Yaw(double angle);

	//! Sets the pincers pitch angle
	void Pitch(double angle);

	//! Sets the pincers roll angle
	void Roll(double angle);

protected:	

	vtkToolDummy();
	~vtkToolDummy();

private:

	vtkToolDummy (const vtkToolDummy &); //Not Implemented
	void operator =(const vtkToolDummy &); //Not Implemented

};
#endif

