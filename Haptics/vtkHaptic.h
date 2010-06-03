#ifndef __vtkHaptics
#define __vtkHaptics

#include "vtkObject.h"
#include "vtkESQuiHapticsWin32Header.h"



//! Generic Haptic Class

class VTK_ESQUI_HAPTICS_EXPORT vtkHaptic: public vtkObject {

public:
	vtkTypeRevisionMacro(vtkHaptic,vtkObject);
	const char *GetClassName() {return "vtkHaptics";}
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set up the haptic device
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
	virtual int Init() = 0;
	
	//! Executes the control of the haptic device, reimplemented in all subclasses
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
 	virtual void Update() = 0;

 	//! Liberates the haptic connection
	/*!
	 * Abstract method that must be defined in the implementation class
	 */
 	virtual void Release() = 0;

 	//! Set the haptic name
 	/*!
 	 * \param name name of the haptic device
 	 * \sa GetName()
 	 */
 	void SetName(const char * name);

 	//! Return the haptic name
 	/*!
 	 * \return haptic device name
 	 * \sa SetName(const char * name)
 	 */
 	const char * GetName();

 	//! Set the haptic number of tools
 	/*!
 	 * \param numberOfTools number of tools of the haptic device
 	 * \sa GetNumberOfTools
 	 */
 	void SetNumberOfTools(int numberOfTools);

 	//! Return the haptic number of tools
 	/*!
 	 * \return number of tools of the haptic device
 	 * \sa SetNumberOfTools(int numberOfTools)
 	 */
 	int GetNumberOfTools();

protected:	

	vtkHaptic();
	~vtkHaptic();

	//! Haptic device name
	const char * Name;
	//! Number of tools of the haptic device
	int  NumberOfTools;

};

#endif
