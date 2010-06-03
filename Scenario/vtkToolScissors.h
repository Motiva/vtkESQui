#ifndef _vtkToolScissors_h
#define _vtkToolScissors_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkToolLaparoscopy.h"
#include "vtkStringArray.h"
#include "vtk3DSImporterEsqui.h"
#include "vtkPolyDataReader.h"
#include "vtkTransformPolyDataFilter.h"

//! Implementation class of scissors
/*!
	vtkToolScissors is the class that implements the scissors tool, inside the scene.
	Contains methods for position control of the tool in the scene and collision detection.
	Scissors are composed of 3 pieces: 1 stick and 2 blades (vtkPiece)
	Inherits vtkToolLaparascopy
*/
class VTK_ESQUI_SCENARIO_EXPORT vtkToolScissors : public vtkToolLaparoscopy
{

public:
	vtkTypeRevisionMacro(vtkToolScissors,vtkToolLaparoscopy);
	//! Create new vtkToolScissors object
	static vtkToolScissors *New();
	//! Return the class name
	const char *GetClassName() { return "vtkToolScissors"; }
	//! Print the attributes value
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Initialize the tool from VTK file
	/*!
	 * This function initializes the tool whose mesh is described on a VTK file
	*/
	virtual void Init();

	//!Update the tool
	/*!
	 * This function update the tool values
	 */
	virtual void Update();

	//!Set Blade polydata filename
	/*!
	* \param path path of the file that contains the blade polydata
	*/
	void SetBladeFileName(vtkIdType id, const char * path);

	//!Set stick polydata filename
	/*!
	* \param path path of the file that contains the stick piece polydata
	*/
	void SetStickFileName(const char * path);

	//!Return the Blade polydata file name
	/*!
	 * \param id identifier of the blade piece
	 * \return path of the file that contains the blade piece polydata
	 */
	const char * GetBladeFileName(vtkIdType id);

	//!Return the stick polydata file name
	/*!
	 * \param id identifier of the stick piece
	 * \return path of the file that contains the stick piece polydata
	 */
	const char * GetStickFileName(vtkIdType id);
	
	//!Return whether the Blade is closed or not
	bool IsClosed(){return (this->Opening < 0.0f);}

protected:

	vtkToolScissors();
	~vtkToolScissors();

private:

	vtkToolScissors (const vtkToolScissors &); //Not Implemented
	void operator =(const vtkToolScissors &); //Not Implemented

	//!Return the stick piece
	/*!
	 * Return the vtkPiece object of the stick
	 */
	vtkPiece * GetStick(){return this->GetPiece(0);};

	//!Return the Blade piece
	/*!
	 * Return the vtkPiece object of the Blade at the specified id
	 */
	vtkPiece * GetBlade(vtkIdType id){return this->GetPiece(id);};
};

#endif
