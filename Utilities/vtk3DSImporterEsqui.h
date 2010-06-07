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
/*=========================================================================*/
// .NAME vtk3DSImporterEsqui - imports 3D Studio files for Esqui purpouses.
// .SECTION Description
// vtk3DSImporterEsqui imports 3D Studio files into vtk for ESQUI purpouses.

// .SECTION See Also
// vtk3DSImporter, vtkImporter


#ifndef __vtk3DSImporterEsqui_h
#define __vtk3DSImporterEsqui_h

#include "vtk3DSImporter.h"
#include "vtkESQuiUtilitiesWin32Header.h"
#include "vtkPolyDataCollection.h"
#include "vtkPolyData.h"
#include "vtkActorCollection.h"
#include "vtkTransformPolyDataFilter.h"

//! Import 3DS files to VTK data structure
class VTK_ESQUI_UTILITIES_EXPORT vtk3DSImporterEsqui : public vtk3DSImporter
{

public:
  
  vtkTypeRevisionMacro(vtk3DSImporterEsqui,vtk3DSImporter);
  static vtk3DSImporterEsqui *New();
  const char *GetClassName() {return "vtk3DSImporterEsqui";};

  // Devuelve una coleccion de actores del fichero 3DS importado
  //! Return a collection of actors from the imported 3DS file
  vtkActorCollection *GetActorCollection();

  //!Set scale factor of the imported objects
  /*!
   * \param scale object scale
   * \sa GetScale()
   */
  void SetScale(float scale);

  //!Return scale factor
  /*!
   * \sa SetScaleFactor(float scale)
   */
  float GetScale();

protected:
  vtk3DSImporterEsqui();
  ~vtk3DSImporterEsqui() {};

  //!Scale of the imported objects
  float Scale;

  //!PolyData actors filter
  vtkTransformPolyDataFilter *Filter;

  //!Collection of Actors
  vtkActorCollection *Actors;
// Importa los actores del fichero 3DS
//! Import the actors from the 3DS file
  virtual void ImportActors (vtkRenderer *renderer);
// Interpreta el fichero 3DS
//! Read the 3DS file
  virtual void ReadData();
private:
  vtk3DSImporterEsqui(const vtk3DSImporterEsqui &);  // Not implemented.
  void operator=(const vtk3DSImporterEsqui &);  // Not implemented.
};

#endif

