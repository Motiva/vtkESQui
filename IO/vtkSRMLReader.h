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

#ifndef __vtkSRMLReader_h
#define __vtkSRMLReader_h

#include "vtkESQuiIOWin32Header.h"
#include "vtkXMLDataParser.h"

#include "vtkSimulation.h"
#include "vtkScenario.h"
#include "vtkScenarioObject.h"
#include "vtkScenarioElement.h"
#include "vtkModel.h"
#include "vtkStdString.h"

#ifndef VTKESQUI_USE_NO_HAPTICS
#include "vtkHaptic.h"
#endif

//! SRML reader
/*!
 * Reader to reconstruct a simulation state from a SRML(XML) file.
 */
class VTK_ESQUI_IO_EXPORT vtkSRMLReader: public vtkXMLDataParser
{
public:

  //! Type revision macro
  vtkTypeRevisionMacro(vtkSRMLReader, vtkXMLDataParser);
  //! Create new vtkSRMLReader object
  static vtkSRMLReader *New();

  //! Return class name
  const char *GetClassName() { return "vtkSRMLReader"; }

  //! Return the vtkSimulation object
  /*!
   * The FileName must have been set.
   */
  vtkSimulation *ConstructSimulation();

  //! Print the attributes value
  void PrintSelf(ostream& os, vtkIndent indent);

#ifdef VTKESQUI_USE_HAPTICS
// Anything here?
#endif

protected:

  vtkSRMLReader();
  ~vtkSRMLReader();

	// Auxiliary Functions
	bool CheckStructure();
	void InternalConstructSimulation();
	void ImportCamera(vtkXMLDataElement *c);
	void ImportLight(vtkXMLDataElement *l);
	void ImportObject(vtkXMLDataElement *o);
	void ImportElement(vtkScenarioObject *object, vtkXMLDataElement *e);
	void ImportModel(vtkScenarioElement *element, vtkXMLDataElement *m);

  //! This is the simulation object built out from the SRML.
  vtkSimulation *Simulation;
  //! Scenario asocciated with the simulation in construction.
  vtkScenario   *Scenario;
	//! Data path
	vtkStdString DataPath;
	//! XML Root
	vtkXMLDataElement *Root;
};
#endif

