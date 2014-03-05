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
#ifndef __vtkParticleCollection_h
#define __vtkParticleCollection_h

#include "vtkCollection.h"
#include "vtkParticleSpringSystemWin32Header.h"

class vtkParticle;

//!  Collection of Particles
/*! 
vtkParticleCollection represents and provides methods to manipulate a list of
Particles (i.e., vtkParticle and subclasses). The list is unsorted and duplicate
entries are not prevented.
\sa vtkCollection, vtkParticle
*/
class VTK_vtkParticleSpringSystem_EXPORT vtkParticleCollection : public vtkCollection
{
public:
  //!Create new object
  static vtkParticleCollection *New();
  //!Type Revision Macro
  vtkTypeRevisionMacro(vtkParticleCollection,vtkCollection);
  //!Print Class Values
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!Insert an Particle to the collection
  /*!
  * The Particle will be inserted at the end of the list
  * \param particle vtkParticle object to be inserted
  * \sa InsertParticle(vtkIdType id, vtkParticle *Particle)
  */
  void InsertNextParticle(vtkParticle *particle);

  //!Insert an Particle at the specified index
  /*!
  * \param id index of the list where the Particle will be inserted
  * \param particle vtkParticle object to be inserted
  * \sa InsertNextParticle(vtkParticle *Particle)
  */
  void InsertParticle(vtkIdType id, vtkParticle *particle);

  //!Return an Particle in specified position of the list.
  /*!
  *   Get the Particle in the list with the position id. NULL is returned when the collection is exhausted.
  * \param id index of the Particle to be returned
  */
  vtkParticle *GetParticle(vtkIdType id);

  //!Return the next Particleism on the list.
  /*!
  * Get the Particle at the pointer position. Beware of calling InitTraversal() to init the pointer. NULL is returned when the collection is exhausted.
  */
  vtkParticle * GetNextParticle();

  //!Return whether the collection contains a particle or not
  bool ContainsParticle(vtkParticle * particle);

  //!Return the number of particles in the collection
  int GetNumberOfParticles();

protected:
  vtkParticleCollection() {};
  ~vtkParticleCollection() {};

private:
  vtkParticleCollection(const vtkParticleCollection&);  // Not implemented.
  void operator=(const vtkParticleCollection&);          // Not implemented.
};

#endif
