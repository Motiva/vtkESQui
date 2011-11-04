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
#ifndef __vtkRK4Solver_h
#define __vtkRK4Solver_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkMotionEquationSolver.h"

class vtkParticleSpringSystem;
class vtkParticleCollection;
class vtkParticle;

//! Implementation of the Runge-Kutta solver.

class VTK_vtkParticleSpringSystem_EXPORT vtkRK4Solver : public vtkMotionEquationSolver {
public:

  //! Type revision macro
  vtkTypeRevisionMacro(vtkRK4Solver, vtkObject);
  //! Create new Runge-Kutta Solver
  static vtkRK4Solver * New();
  //! Print object info
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Initialize solver
  virtual void Init();

  //! Compute next step for every particle
  /*!
  * \param particles collection of particles
  * \param deltaT time step
  */
  virtual void ComputeNextStep(vtkParticleCollection * particles, double deltaT);

protected:
  vtkRK4Solver();
  ~vtkRK4Solver();

private:
  vtkRK4Solver(const vtkRK4Solver&);            // Not implemented.
  void operator=(const vtkRK4Solver&);           // Not implemented.

  //! 1st order derivative x
  vtkDoubleArray * dx1;
  //! 1st order derivative v
  vtkDoubleArray * dv1;
  //! 2nd order derivative x
  vtkDoubleArray * dx2;
  //! 2nd order derivative v
  vtkDoubleArray * dv2;
  //! 3rd order derivative x
  vtkDoubleArray * dx3;
  //! 3rd order derivative v
  vtkDoubleArray * dv3;
  //! 3rd order derivative x
  vtkDoubleArray * dx4;
  //! 3rd order derivative v
  vtkDoubleArray * dv4;

  //! Evaluate derivatives on each step
  void Evaluate(vtkParticleCollection * particles, vtkDoubleArray * dX, vtkDoubleArray * dV, double deltaT);

  //! Reset equation motion solver
  void Reset();
};

#endif

