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
#ifndef __vtkToolCollection_h
#define __vtkToolCollection_h

#include "vtkCollection.h"
#include "vtkESQuiScenarioWin32Header.h"

class vtkTool;

//! Collection of tools 
/*!
 * Acts as container for tools. Used in the scenario tool management.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkToolCollection : public vtkCollection
{
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkToolCollection,vtkCollection);
	//! Create a new colection for tool storage
	static vtkToolCollection *New() {return new vtkToolCollection;}
	//! Return class name
	const char *GetClassName() {return "vtkToolCollection";}
	//! Printa object values
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert tool into the collection at the specified id
	/*!
	 * \param id identifier key of tool
	 * \param tool vtkTool object to be inserted
	 */
	void InsertTool(vtkIdType id, vtkTool * tool);
	//!Insert tool at the last position of the collection
	/*!
	 * \param tool vtkTool object to be inserted
	 */
	void InsertNextTool(vtkTool * tool);
	//!Return tool at the specified id
	/*!
	 * \param id identifier key of the tool
	 */
	vtkTool * GetTool(vtkIdType id);
	//!Return the next tool
	/*!
	 * Return the tool located at the pointer position + 1
	 * \sa GetTool(vtkIdType id)
	 */
	vtkTool * GetNextTool();

protected:
	//   vtkIdType NumeroDeHerramientas;
	vtkToolCollection() {};
	~vtkToolCollection() {};

private:
	vtkToolCollection(const vtkToolCollection&);  // Not implemented.
	void operator=(const vtkToolCollection&);  // Not implemented.
};


#endif
