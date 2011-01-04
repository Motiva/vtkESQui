/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me 
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM), 
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at 
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License 
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/
#ifndef _vtkScenario_h
#define _vtkScenario_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkObject.h"

#include "vtkCollisionDetectionLibrary.h"

class vtkRenderWindow;
class vtkRenderer;

class vtkActor;
class vtkTextActor;
class vtkActor2DCollection;

class vtkOrgan;
class vtkOrganCollection;
class vtkTool;
class vtkToolCollection;



//!Implementation of the simulation scenario.
/*!
 * The elements in the scenario (organs, tools...) are stored in collections.
 * Any scenario item should be predefined (all of its required parameters set) before being inserted/added to the scenario.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkScenario: public vtkObject
{

public:
	//!Type Revision Macro
	vtkTypeRevisionMacro(vtkScenario, vtkObject);
	//!Create a new Scenario object
	static vtkScenario * New();
	//!Return class name as a string
	const char *GetClassName() {return "vtkScenario";}

	//!Set the collection of organs of the scenario
	/*!
	* \param collection collection of organs to be inserted to the scenario
	* \sa GetOrganCollection()
	*/
	void SetOrgans(vtkOrganCollection * collection);

	//!Get the collection of organs of the scenario
	/*!
	* \sa SetOrganCollection(vtkOrganCollection * collection)
	*/
	vtkOrganCollection * GetOrgans();

	//!Insert organ to the scenario
	/*!
	* The organ will be inserted at the end of the list
	* \param organ vtkOrgan object to be inserted
	* \return position of the added object.
	* \sa InsertOrgan(vtkOrgan * organ)
	*/
	void AddOrgan(vtkOrgan * organ);

	//!Insert an organ in the specified position
	/*!
	* If the given position is not empty, the content is replaced by the new model
	* \param index index of the list where the organ will be added
	* \param organ vtkOrgan object to be inserted
	* \sa InsertNextOrgan(vtkOrgan * organ)
	*/
	void ReplaceOrgan(vtkIdType index, vtkOrgan * organ);


	//!Delete the organ at the specified id from the scenario
	/*!
	* It will only remove the organ from the collection, it does not free memory
	* \param index index of the organ to be removed
	*/
	void RemoveOrgan(vtkIdType index);

	//!Return the organ at the specified position
	/*!
	* \param id index of the organ to be returned
	* \return vtkOrgan object
	*/
	vtkOrgan * GetOrgan(vtkIdType id);

	//!Return number of organs in the scenario
	vtkIdType GetNumberOfOrgans();

	//!Set the collection of tools of the scenario
	/*!
	* \param collection collection of tools
	*/
	void SetTools(vtkToolCollection * collection);

	//!Return the collection of tools of the scenario
	vtkToolCollection * GetTools();

	//!Add tool to the scenario
	/*!
	* \param tool vtkTool object to be inserted
	*/
	void AddTool(vtkTool * tool);

	//!Insert a tool in a given position in the scenario
	/*!
	* If the given position is not empty, the content is replaced by the new model
	* \param index index of the tool to be inserted
	* \param tool vtkTool object to be inserted
	*/
	void ReplaceTool(vtkIdType index, vtkTool * tool);

	//!Delete the tool at the specified id from the scenario
	/*!
	It will only remove the tool from the collection, it does not liberate the memory
	*/
	void RemoveTool(vtkIdType index);

	//!Return the tool stored with the specified id
	/*!
	* \param id index of the tool to be returned
	*/
	vtkTool * GetTool(vtkIdType id);

	//!Return the number of tools contained in the scenario
	vtkIdType GetNumberOfTools();

	//void SetExtras(vtkPropCollection * extras);
	//vtkPropCollection * GetExtras();

	//void InsertNextExtra(vtkProp * extra);

	//!Insert a extra in a given position in the scenario
	/*!
	 * If the given position is not empty, the content is replaced by the new model
	 * \param index index of the extra to be inserted
	 * \param extra vtkActor object to be inserted
	 * \return false if the insert operation was not correctly completed
	 */
	//void InsertExtra(vtkIdType index, vtkProp * extra);

	//!Delete the extra at the specified id from the scenario
	/*!
		It will only remove the extra from the collection, it does not liberate the memory
		\return false if the model could not be deleted
	 */
	//void RemoveExtra(vtkIdType index);

	//!Return the extra stored with the specified id
	/*!
	 * \param id index of the extra to be returned
	 */
	//vtkActor * GetExtra(vtkIdType id);

	//!Return the number of extras contained in the scenario
	//vtkIdType GetNumberOfExtras();

	//!Set/Get legend display
	vtkSetMacro(LegendDisplay, bool);
	vtkGetMacro(LegendDisplay, bool);

	vtkBooleanMacro(LegendDisplay, bool);

	//!Set/Get legend display
	vtkSetMacro(Collision, bool);
	vtkGetMacro(Collision, bool);

	vtkBooleanMacro(Collision, bool);

	//!Assign the render window of the scenario
	/*!
	* \param Renderer render window for displaying purposes
	* \sa GetRenderWindow()
	*/
	void SetRenderWindow(vtkRenderWindow *Renderer);

	//!Return the render window of the scenario
	/*!
	* \sa SetRenderWindow(vtkRenderWindow *Renderer)
	*/
	vtkRenderWindow *GetRenderWindow();

	//!Initialize Scenario
	void Init();

	//!Update the whole Scenario
	/*!
	* Collision Detection is processed. If any contact has occurred the related tools and organs will be updated
	*/
	void Update();

protected:

	vtkScenario();
	~vtkScenario();

private:

	vtkScenario(const vtkScenario&);
	void operator=(const vtkScenario&);

	//!Scenario render window
	vtkRenderWindow * RenderWindow;
	//!Scenario renderer
	vtkRenderer * Renderer;

	//!Collection of the scenario organs
	vtkOrganCollection * Organs;
	//!Collection of the scenario tools
	vtkToolCollection * Tools;
	//!Collection of Extras
	//vtkPropCollection * Extras;

	//!Legends
	vtkActor2DCollection * Legends;
	//!Enable legend display
	bool LegendDisplay;


	//!Enable collision detection
	bool Collision;
	//!collision detection library
	vtkCollisionDetectionLibrary* CollisionDetection;

};

#endif
