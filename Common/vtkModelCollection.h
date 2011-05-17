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
#ifndef __vtkModelCollection_h
#define __vtkModelCollection_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkCollection.h"

class vtkModel;

//! Collection of scenario object models.
/*!
 * vtkModelCollection is an object for creating and manipulating lists
 * of scenario element models. The lists are unsorted and allow duplicate
 * entries.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkModelCollection : public vtkCollection
{
public:
	//!Type revision macro
	vtkTypeRevisionMacro(vtkModelCollection,vtkCollection);
	//! Create new model storage collection
	static vtkModelCollection *New() {return new vtkModelCollection;}
	//! Return classname
	const char *GetClassName() {return "vtkModelCollection";}
	//! Print collection values
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	//!Insert a model into the collection
	/*!
	 * \param id index of the list where the model will be added
	 * \param model vtkModel object to be inserted
	 * \sa InsertNextModel(vtkModel * model);
	 */
	void InsertModel(vtkIdType id, vtkModel * model);

	//!Insert model at the end of the collection
	/*!
	 * \param model vtkModel object to be inserted
	 * \sa InsertModel(vtkIdType id, vtkModel * model)
	 */
	void AddModel(vtkModel * model);

	//!Return model located the specified id
	/*!
	 * NULL is returned when the collection is exhausted.
	 * \return model in the list with the position identifier.
	 * \param id list index where the model is stored
	 */
	vtkModel * GetModel(vtkIdType id);

	//!Return model on the collection
	/*!
	 * Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
	 * \return model at the pointer position.
	 */
	vtkModel * GetNextModel();

	//!Return number of models in the collection
	vtkIdType GetNumberOfModels();

protected:
	vtkModelCollection() {};
	~vtkModelCollection() {};

private:
	vtkModelCollection(const vtkModelCollection&);  // Not implemented.
	void operator=(const vtkModelCollection&);  // Not implemented.
};


#endif
