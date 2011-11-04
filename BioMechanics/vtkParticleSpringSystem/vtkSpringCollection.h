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
#ifndef __vtkSpringCollection_h
#define __vtkSpringCollection_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkCollection.h"

class vtkSpring;

//!  Collection of Springs
/*! 
vtkSpringCollection represents and provides methods to manipulate a list of
Springs (i.e., vtkSpring and subclasses). The list is unsorted and duplicate
entries are not prevented.
\sa vtkCollection, vtkSpring
*/
class VTK_vtkParticleSpringSystem_EXPORT vtkSpringCollection : public vtkCollection
{
public:
  //!Create new object
  static vtkSpringCollection *New();
  //!Type Revision Macro
  vtkTypeRevisionMacro(vtkSpringCollection,vtkCollection);
  //!Print Class Values
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!Insert an Spring to the collection
  /*!
  * The Spring will be inserted at the end of the list
  * \param Spring vtkSpring object to be inserted
  * \sa InsertSpring(vtkIdType id, vtkSpring *Spring)
  */
  void InsertNextSpring(vtkSpring *Spring);

  //!Insert an Spring at the specified index
  /*!
  * \param id index of the list where the Spring will be inserted
  * \param Spring vtkSpring object to be inserted
  * \sa InsertNextSpring(vtkSpring *Spring)
  */
  void InsertSpring(vtkIdType id, vtkSpring *Spring);

  //!Return an Spring in specified position of the list.
  /*!
  *   Get the Spring in the list with the position id. NULL is returned when the collection is exhausted.
  * \param id index of the Spring to be returned
  */
  vtkSpring *GetSpring(vtkIdType id);

  //!Return the next Spring on the list.
  /*!
  * Get the Spring at the pointer position. Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
  */
  vtkSpring * GetNextSpring();

  //!Return whether the collection contains a Spring or not
  bool ContainsSpring(vtkSpring * Spring);


protected:
  vtkSpringCollection() {};
  ~vtkSpringCollection() {};

private:
  vtkSpringCollection(const vtkSpringCollection&);  // Not implemented.
  void operator=(const vtkSpringCollection&);          // Not implemented.
};

#endif
