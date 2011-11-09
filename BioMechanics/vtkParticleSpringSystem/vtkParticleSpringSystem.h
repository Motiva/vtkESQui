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
#ifndef __vtkParticleSpringSystem_h
#define __vtkParticleSpringSystem_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

#include "vtkMotionEquationSolver.h"

class vtkMassProperties;

class vtkSpring;
class vtkSpringCollection;
class vtkParticle;
class vtkParticleCollection;

//! Implementation of the particle-spring deformation model
/*!
 * The system is compounded of several collections of elements: springs, particles, links.
 * There are some system properties to define material behavior. All these properties must be set prior to the initialization Init()
 */

class VTK_vtkParticleSpringSystem_EXPORT vtkParticleSpringSystem : public vtkPolyDataAlgorithm {
public:

  //!Type revision macro
  vtkTypeRevisionMacro(vtkParticleSpringSystem,vtkPolyDataAlgorithm);
  //! Create new particle spring system object
  static vtkParticleSpringSystem * New();
  //! Print system info
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Initialize the particle spring system
  /*!
   * Create springs, particles and the links between them.
   */
  void Initialize();

  //Parameters for Particle-Spring System
  //! Set spring coefficient
  vtkSetMacro(Spring, double);    // Spring coefficient
  //! Set distance coefficient. Percentage
  vtkSetMacro(Distance, double);  // Distance constraint coefficient
  //! Set damping coefficient
  vtkSetMacro(Damping, double);  // Damping coefficient
  //! Set time step
  vtkSetMacro(TimeStep, double);        // dt for every step
  //! Set mass of system particles
  vtkSetMacro(Mass, double);          // Mass of all points (future work: specify different masses for different "layers")
  //! Set residual error
  vtkSetMacro(Residual, double);          // Residual error tolerance on displacement calculus
  //! Set motion equation solver type.
  vtkSetMacro(SolverType, vtkMotionEquationSolver::MotionEquationSolverType);    // Motion equation solver type

  //! Set Gravity Force (m/s2)
  vtkSetVector3Macro(Gravity, double);
  //! Get Gravity Force (m/s2)
  vtkGetVector3Macro(Gravity, double);

  //! Set particle status
  /*!
   * 0: Fixed
   * 1: Free
   */
  void SetParticleStatus(vtkIdType id, bool status);

  //! Insert a displacement in the particle-spring system
  void InsertDisplacement(vtkIdType id, double x, double y, double z);

  //! Insert a displacement to a particle
  void InsertDisplacement(vtkIdType id, double * displacement);

  //! Insert displacements to particles
  /*!
   * \param ids List of particle ids
   * \param displacements Array containing displacements per particle
   */
  void SetDisplacements(vtkIdList * ids, vtkDoubleArray * displacements);

  //! Compute the forces attending to the collisions
  /*!
   * The solver is executed every step, in order to compute the force propagation along the whole system
   */
  void ComputeForces();

protected:
  vtkParticleSpringSystem();
  ~vtkParticleSpringSystem();

  //! VTK method for system update. Must be implemented
  int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);

  //! Initialized flag
  bool Initialized;

  //Particle-Spring System
  //! Collection of springs
  vtkSpringCollection * Springs;
  //! Collection of particles
  vtkParticleCollection * Particles;

  //Model Parameters
  //! Spring stiffness k.
  /*!
   * Spring stiffness coefficient: resistance offered by the spring to a
   * deformation. F[p(i)] = -(K*dNorm-L)*(d(p[i])/dNorm);
   */
  double Spring;
  //! Distance coefficient.
  /*!
   * Maximum distance of linear elongation. For instance, a distance
   * coefficient of 2, means the elongation of a spring is linear elastic
   * until it reaches 2 times its initial length. For higher values, the
   * stiffness coefficient is modified dynamically.
   */
  double Distance;
  //! Damping coefficient
  double Damping;
  //! Equation time step
  double TimeStep;
  //! Mass for each system particle
  double Mass;
  //! Residual error tolerance
  double Residual;

  //!Gravity
  double Gravity[3];

  //! Motion equation solver
  vtkMotionEquationSolver * Solver;
  //! Motion equation solver type
  vtkMotionEquationSolver::MotionEquationSolverType SolverType;

private:
  vtkParticleSpringSystem(const vtkParticleSpringSystem&);            // Not implemented.
  void operator=(const vtkParticleSpringSystem&);           // Not implemented.

  //! Create a new spring between two particles
  void CreateSpring(vtkParticle * p0, vtkParticle * p1);
};

#endif

