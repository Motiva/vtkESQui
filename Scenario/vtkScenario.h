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

class vtkRenderWindow;
class vtkRenderer;

class vtkScenarioObject;
class vtkScenarioObjectCollection;

//!Implementation of a surgical simulation scenario.
/*!
 * In vtkESQui framework a surgical scenario is implemented with this
 * class.\n
 * All the elements in the scenario (objects, tools...) are stored in a
 * collection, Objects. Any scenario object should be previously defined
 * (all of its required parameters set) before being inserted/added to the
 * scenario.\n
 * Apart from scenario objects, a vtkRenderWindow object must be assigned
 * before object initialization for displaying purposes.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkScenario: public vtkObject
{

public:
  //!Type Revision Macro
  vtkTypeRevisionMacro(vtkScenario, vtkObject);
  //!Create a new Scenario object
  static vtkScenario * New();
  //! Print scenario info
  void PrintSelf(ostream& os, vtkIndent indent);
  //!Return class name as a string
  const char *GetClassName() {return "vtkScenario";}

  //! Assign model name
  /*!
   *\sa GetName()
   */
  vtkSetStringMacro(Name);

  //!Return model name
  /*!
   *\sa SetName(const char * name)
   */
  vtkGetStringMacro(Name);

  //!Set the collection of objects of the scenario
  /*!
   * \param collection collection of objects to be inserted to the scenario
   * \sa GetObjects()
   */
  void SetObjects(vtkScenarioObjectCollection * collection);

  //!Get the collection of objects of the scenario
  /*!
   * \sa SetObjects(vtkScenarioObjectCollection * collection)
   */
  vtkScenarioObjectCollection * GetObjects();

  //!Insert object to the scenario
  /*!
   * The object will be inserted at the end of the list
   * \param object vtkScenarioObject object to be inserted
   * \return position of the added object.
   * \sa InsertObject(vtkScenarioObject * object)
   */
  void AddObject(vtkScenarioObject * object);

  //!Insert an object in the specified position
  /*!
   * If the given position is not empty, the content is replaced by the new model
   * \param index index of the list where the object will be added
   * \param object vtkScenarioObject object to be inserted
   * \sa InsertNextObject(vtkScenarioObject * object)
   */
  void ReplaceObject(vtkIdType index, vtkScenarioObject * object);

  //!Delete the object at the specified id from the scenario
  /*!
   * It will only remove the object from the collection, it does not free memory
   * \param index index of the object to be removed
   */
  void RemoveObject(vtkIdType index);

  //!Return the object at the specified position
  /*!
   * \param id index of the object to be returned
   * \return vtkScenarioObject object
   */
  vtkScenarioObject * GetObject(vtkIdType id);

  //!Return number of objects in the scenario
  vtkIdType GetNumberOfObjects();

  //!Set/Get legend display
  vtkSetMacro(Collision, bool);
  vtkGetMacro(Collision, bool);

  vtkBooleanMacro(Collision, bool);

  //!Assign the render window of the scenario
  /*!
   * \param window render window for displaying purposes
   * \sa GetRenderWindow()
   */
  void SetRenderWindow(vtkRenderWindow *window);

  //!Return the render window of the scenario
  /*!
   * \sa SetRenderWindow(vtkRenderWindow *Renderer)
   */
  vtkRenderWindow *GetRenderWindow();

  //! Check if scenario has been initialized
  bool IsInitialized(){return this->Initialized;};

  //!Initialize Scenario
  void Init();

  //!Update the whole Scenario
  /*!
   * Collision Detection is processed. If any contact has occurred the related tools and objects will be updated
   */
  void Update();

protected:

  vtkScenario();
  ~vtkScenario();


  //! Name of the scenario
  char* Name;

  //! Initialization flag
  bool Initialized;

  //!Scenario render window
  vtkRenderWindow * RenderWindow;
  //!Scenario renderer
  vtkRenderer * Renderer;

  //!Collection of the scenario objects
  vtkScenarioObjectCollection * Objects;

  //!Enable collision detection
  bool Collision;

private:

  vtkScenario(const vtkScenario&);
  void operator=(const vtkScenario&);

};

#endif
