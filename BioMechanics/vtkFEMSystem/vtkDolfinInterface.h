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
#ifndef __vtkDolfinInterface_h
#define __vtkDolfinInterface_h

#include "vtkFEMSystemWin32Header.h"
#include "vtkObject.h"

class vtkUnstructuredGrid;

#include <dolfin.h>
#include "Elasticity.h"


//! Implementation of the Dolfin FEM library interface
/*!
 *
 */

class VTK_vtkFEMSystem_EXPORT vtkDolfinInterface : public vtkObject
{
public:

  //!Type revision macro
  vtkTypeRevisionMacro(vtkDolfinInterface,vtkObject);
  //! Create new dolfin library interface object
  static vtkDolfinInterface * New();
  //! Print system info
  void PrintSelf(ostream& os, vtkIndent indent);

  void SetMesh(vtkUnstructuredGrid * mesh);

  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  //vtkSetMacro(Mesh, vtkUnstructuredGrid);
  //vtkGetMacro(Mesh, vtkUnstructuredGrid);

  void Init();

  void Write();

protected:
  vtkDolfinInterface();
  ~vtkDolfinInterface();

  //!Configuration filename
  char * FileName;

  //! Lambda elasticity coefficient
  double Lambda;

  //!Mu elasticity coefficient
  double Mu;

  //!Tetrahedral mesh
  dolfin::Mesh * FEMesh;


  //vtkUnstructuredGrid * Mesh;

private:
  vtkDolfinInterface(const vtkDolfinInterface&);            // Not implemented.
  void operator=(const vtkDolfinInterface&);           // Not implemented.

  void GenerateTopologyMesh();
  void GenerateFEMMesh();
};

#endif

