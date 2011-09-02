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
#ifndef __vtkCUDARK4Solver_h
#define __vtkCUDARK4Solver_h

#include "vtkCUDAParticleSystemWin32Header.h"
#include "vtkCUDAMotionEquationSolver.h"

class vtkCUDAParticleSystem;

//! Implementation of the Runge-Kutta solver.

class VTK_vtkCUDAParticleSystem_EXPORT vtkCUDARK4Solver : public vtkCUDAMotionEquationSolver {
public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkCUDARK4Solver, vtkObject);
	//! Create new Runge-Kutta Solver
	static vtkCUDARK4Solver * New();
	//! Print object info
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Initialize equation motion solver
	virtual void Init();

	//! Compute next step for every particle
	/*!
	 * \param p particle position vector
	 * \param v particle velocity vector
	 * \param a particle acceleration vector
	*/
	virtual void ComputeNextStep(float *p, float *v, float *a);

protected:
	vtkCUDARK4Solver();
	~vtkCUDARK4Solver();

private:
	vtkCUDARK4Solver(const vtkCUDARK4Solver&);            // Not implemented.
	void operator=(const vtkCUDARK4Solver&);           // Not implemented.

	//! Position 1st order derivative
	float * dx1;
	//! Velocity 1st order derivative
	float * dv1;
	//! 2nd order derivative x
	float * dx2;
	//! 2nd order derivative v
	float * dv2;
	//! 3rd order derivative x
	float * dx3;
	//! 3rd order derivative v
	float * dv3;
	//! 4rd order derivative x
	float * dx4;
	//! 4rd order derivative v
	float * dv4;

	//! Evaluate derivatives on each step
	void Evaluate(float *p, float *v, float *a, double deltaT, int order);

};

#endif

