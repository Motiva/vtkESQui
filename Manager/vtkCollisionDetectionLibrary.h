#ifndef vtkCollisionDetectionLibrary_h
#define vtkCollisionDetectionLibrary_h

#ifdef WIN32
#include <vector>
#else
#include <vector>
#endif

#include "vtkObject.h"
#include "vtkESQuiManagerWin32Header.h"
#include "vtkOrgan.h"
#include "vtkOrganCollection.h"
#include "vtkTool.h"
#include "vtkToolCollection.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

//! Generic interface of the Collision Detection Library

class VTK_ESQUI_MANAGER_EXPORT vtkCollisionDetectionLibrary: public vtkObject
{
public:
	vtkTypeRevisionMacro(vtkCollisionDetectionLibrary, vtkObject);
	vtkCollisionDetectionLibrary();
	~vtkCollisionDetectionLibrary();

	//BTX
	//! Method used to detect the collision between organs and tools from the scene must be implemented in the CollisionDetectionLibrary we want to use
	/*!
	* Abstract method to be defined on each implementation class
	*/
	virtual void Update() = 0;
	//!Initializes the CollisionDetectionLibrary
	/*!
	* Abstract method to be defined on each implementation class
	*/
	virtual void Init() = 0;
	//ETX

	//! Specify the organs to be checked in the collision detection process
	void SetOrgans(vtkOrganCollection * organs){this->Organs = organs;};
	//! Specify the tools to be checked in the collision detection process
	void SetTools(vtkToolCollection * tools){this->Tools = tools;};

	//! Return the contacts obtained after collision detection has been performed
	vtkContactCollection * GetContacts(){ return this->Contacts;};
	//! Get total number of contacts detected
	vtkIdType GetNumberOfContacts(){return this->Contacts->GetNumberOfItems();}

protected:

	//! Collection of Organs
	vtkOrganCollection * Organs;
	//! Collection of Tools
	vtkToolCollection * Tools;
	//! Collection of Contacts
	vtkContactCollection * Contacts;

private:
	vtkCollisionDetectionLibrary(const vtkCollisionDetectionLibrary &); //NotImplemented
	void operator =(const vtkCollisionDetectionLibrary &); //Not Implemented

	//!Clear the CollisionDetectionLibrary
	/*!
	* Pure virtual method. Should be defined in the implementation classes
	*/
	virtual void Clear() = 0;
};


#endif

