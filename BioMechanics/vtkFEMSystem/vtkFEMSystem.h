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
#ifndef __vtkFEMSystem_h
#define __vtkFEMSystem_h

#include "vtkFEMSystemWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

#include "vtkGetFEMInterface.h"

class vtkMassProperties;

//! Implementation of the particle-spring deformation model
/*!
 * The system is compounded of several collections of elements: springs, particles, links.
 * There are some system properties to define material behavior. All these properties must be set prior to the initialization Initialize()
 */

class VTK_vtkFEMSystem_EXPORT vtkFEMSystem : public vtkPolyDataAlgorithm
{
public:

  //!Type revision macro
  vtkTypeRevisionMacro(vtkFEMSystem,vtkPolyDataAlgorithm);
  //! Create new particle spring system object
  static vtkFEMSystem * New();
  //! Print system info
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Initialize the particle spring system
  /*!
   * Create springs, particles and the links between them.
   */
  void Init();

  //! Compute one step of the motion equation solver
  /*!
   * The execution of the solver will be done through omputeNextStep() method. This method is executed on every time step to perform mesh deformation.
   */
  void Step();

  //Parameters for Particle-Spring System
  //! Set spring coefficient
  vtkSetMacro(SpringCoefficient, double);    // Spring coefficient
  //! Set distance coefficient. Percentage
  vtkSetMacro(DistanceCoefficient, double);  // Distance constraint coefficient
  //! Set damping coefficient
  vtkSetMacro(DampingCoefficient, double);  // Damping coefficient
  //! Set time step
  vtkSetMacro(TimeStep, double);        // dt for every step
  //! Set mass of system particles
  vtkSetMacro(Mass, double);          // Mass of all points (future work: specify different masses for different "layers")
  //! Set rigidity factor.
  vtkSetMacro(RigidityFactor, vtkIdType);    // Rigidity Factor -> neighborhood size

  //! Set particle-spring system contacts
  /*!
   * \param ids List of particle ids
   * \param directions Array containing displacement directions
   */
  void SetContacts(vtkIdList * ids, vtkDoubleArray * directions);

protected:
  vtkFEMSystem();
  ~vtkFEMSystem();

  //! VTK method for system update. Must be implemented
  int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);

  //Model Parameters
  //! Spring stiffness k.
  double SpringCoefficient;
  //! Distance coefficient. Maximum Percentage of elongation
  double DistanceCoefficient;
  //! Damping coefficient
  double DampingCoefficient;
  //! Equation time step
  double TimeStep;
  //! Mass for each system particle
  double Mass;
  //! Size of particle neighborhood. The big
  vtkIdType RigidityFactor;


  //Contact data
  //! List of contact identifiers
  vtkIdList* ContactIds;
  //! List of contact directions
  vtkDoubleArray* ContactDisplacements;

  vtkGetFEMInterface * getFEM;

private:
  vtkFEMSystem(const vtkFEMSystem&);            // Not implemented.
  void operator=(const vtkFEMSystem&);           // Not implemented.
};

#endif

