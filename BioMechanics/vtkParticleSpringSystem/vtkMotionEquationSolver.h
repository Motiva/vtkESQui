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

#pragma once
#ifndef __vtkMotionEquationSolver_h
#define __vtkMotionEquationSolver_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkObject.h"

class vtkDoubleArray;

class vtkParticleSpringSystem;
class vtkParticleCollection;
class vtkParticle;

//! Implementation of the abstract motion equation solver

class VTK_vtkParticleSpringSystem_EXPORT vtkMotionEquationSolver : public vtkObject {
public:

  //! Type revision macro
  vtkTypeRevisionMacro(vtkMotionEquationSolver, vtkObject);
  //! Print solver info
  void PrintSelf(ostream& os, vtkIndent indent);

  //!Enumeration of solver types
  enum MotionEquationSolverType{
    Euler = 0,
    ModifiedEuler = 1,
    VelocityVerlet = 2,
    RungeKutta4 = 3
  };

  //! Set number of particles
  vtkSetMacro(NumberOfParticles, double);// NumberOfParticles

  //! Set residual error
  vtkSetMacro(Residual, double);// NumberOfParticles

  //! Set Deformation model
  void SetDeformationModel(vtkParticleSpringSystem * model);

  //! Initialize solver.
  /*!
   * Pure virtual method must be implemented in subclasses
   */
  virtual void Initialize() = 0;

  //! Compute next step for every particle
  /*!
   * Pure virtual method must be implemented in subclasses
   * \param particles collection of particles
   * \param dt time step
   */
  virtual void ComputeNextStep(vtkParticleCollection * particles, double dt) = 0;

protected:
  vtkMotionEquationSolver();
  ~vtkMotionEquationSolver();

  //! Solver deformation model
  vtkParticleSpringSystem * DeformationModel;
  //! Number of particles
  double NumberOfParticles;

  //!Residual Error
  double Residual;
  //! Velocity derivative
  vtkDoubleArray * dv;
  //! Position derivative
  vtkDoubleArray * dx;

private:
  vtkMotionEquationSolver(const vtkMotionEquationSolver&);            // Not implemented.
  void operator=(const vtkMotionEquationSolver&);           // Not implemented.
};

#endif

