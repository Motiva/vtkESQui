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
#ifndef _vtkOrgan_h_
#define _vtkOrgan_h_

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkScenarioObject.h"

#include "vtkActor.h"
#include "vtkPolyDataMapper.h"

class vtkPoints;
class vtkCell;
class vtkIdList;
class vtkXMLPolyDataReader;
class vtkPolyData;
class vtkTransformPolyDataFilter;
class vtkTransform;
class vtkTexture;

class vtkBioMechanicalModel;
class vtkCollisionModel;

//! Implementation class for scenario organ definition.
/*!
 * In vtkESQui an organ is considered an scenario object, so vtkScenarioObject
 * serves as its base class.\n
 * As a vtkModel based class, at least a visualization model has to be defined.
 *  If the organ is deformable, a deformation and a collision model must also
 * be defined.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkOrgan: public vtkScenarioObject
{
public:
  //! Type revision macro
  vtkTypeRevisionMacro(vtkOrgan, vtkScenarioObject);
  //! Create new organ
  static vtkOrgan * New();
  //! Return class name
  const char *GetClassName() {return "vtkOrgan";}

  virtual void PrintSelf(ostream &os, vtkIndent indent);

  //!Set force factor.
  /*!
   * Used for estimating haptic force process
   * \sa GetForceFactor()
   */
  vtkSetMacro(ForceFactor, double);

  //!GetfForce factor.
  /*!
   * \sa SetForceFactor(double value)
   */
  vtkGetMacro(ForceFactor, double);

protected:

  //!Force factor estimation
  double ForceFactor;

  vtkOrgan();
  ~vtkOrgan();

private:

  vtkOrgan(const vtkOrgan &);
  void operator=(const vtkOrgan &);

};

#endif

