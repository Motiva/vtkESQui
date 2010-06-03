#ifndef _vtkSimulationManager_
#define _vtkSimulationManager_

#include "vtkCollisionDetectionLibrary.h"
#include "vtkObject.h"
#include "vtkScenario.h"
#include "vtkOrganCollection.h"
#include "vtkToolCollection.h"
#include "vtkContactCollection.h"
#include "vtkContact.h"
#include "vtkObjectFactory.h"
#include "vtkPoints.h"
#include "vtkESQuiManagerWin32Header.h"
#include "vtkBioEngInterface.h"

//! Implementation of the simulation manager. Checks for collisions and executes updating methods of the whole scenario
class VTK_ESQUI_MANAGER_EXPORT vtkSimulationManager: public vtkObject
{

public:
	vtkTypeRevisionMacro(vtkSimulationManager, vtkObject);
	static vtkSimulationManager *New();
	const char *GetClassName() {return "vtkCollisionManager";};

	//! Perform a full update of the whole system.
	/*!
	* Checks if any collision has occurred and performs and update of the whole scenario
	*/
	void Update();

	//!Initializes the simulation manager
	void Init();

	//! Set the scenario items: organs, tools, etc...
	/*!
	* \param scenario class vtkScenario contains the information of the scenario items: organs, tools, contacts, etc...
	*/
	void SetScenario(vtkScenario * scenario);

	//!Return the scenario of the simulation
	vtkScenario * GetScenario();

	//! Set the library used for collision detection
	/*!
	* \param library name(string) of the collision detection library.
	*/
	void SetCollisionDetectionLibrary (const char *library);

	//! Return of the collection of contact points between organs and tools
	vtkContactCollection * GetContacts();

	//! Reset the collision manager
	void Reset(){};

protected:
	//BTX
	//Collision detection library name
	const char* libraryName;
	//ETX

	vtkSimulationManager();
	~vtkSimulationManager();

private:
	vtkSimulationManager(const vtkSimulationManager &); //NotImplemented
	void operator =(const vtkSimulationManager &); //Not Implemented

	//!Simulation scenario
	vtkScenario * Scenario;

	//!collision detection library
	vtkCollisionDetectionLibrary* collisionLibrary;

};
#endif 
