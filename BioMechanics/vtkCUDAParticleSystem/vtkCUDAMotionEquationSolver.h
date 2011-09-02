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
#ifndef __vtkCUDAMotionEquationSolver_h
#define __vtkCUDAMotionEquationSolver_h

#include "vtkCUDAParticleSystemWin32Header.h"
#include "vtkObject.h"

class vtkCUDAParticleSystem;

//! Implementation of the abstract motion equation solver

class VTK_vtkCUDAParticleSystem_EXPORT vtkCUDAMotionEquationSolver : public vtkObject {
public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkCUDAMotionEquationSolver, vtkObject);
	//! Print solver info
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Enumeration of solver types
	enum CUDAMotionEquationSolverType{
		Euler = 0,
		ModifiedEuler = 1,
		VelocityVerlet = 2,
		MidPoint = 3,
		RungeKutta4 = 4
	};

	//! Set number of particles
	vtkSetMacro(NumberOfParticles, double);// NumberOfParticles

	//! Set time step
	vtkSetMacro(DeltaTime, double);

	//! Set residual error
	vtkSetMacro(Residual, double);// NumberOfParticles

	//! Set Deformation model
	void SetDeformationModel(vtkCUDAParticleSystem * model);

	//! Initialize solver.
	/*!
	 * Pure virtual method must be implemented in subclasses
	 */
	virtual void Init() = 0;

	//! Compute next step for every particle
	/*!
	 * Pure virtual method must be implemented in subclasses
	 * \param p particle position vector
	 * \param v particle velocity vector
	 * \param a particle acceleration vector
	 */
	virtual void ComputeNextStep(float *p, float *v, float *a) = 0;

protected:
	vtkCUDAMotionEquationSolver();
	~vtkCUDAMotionEquationSolver();

	//!Initialize flag
	bool Initialized;

	//! Solver deformation model
	vtkCUDAParticleSystem * DeformationModel;

	//! Number of particles
	int NumberOfParticles;

	//! Time step
	double DeltaTime;

	//! Residual Error
	double Residual;

	//! Particle position device vector
	float * dPos;
	//! Particle velocity device vector
	float * dVel;
	//! Particle acceleration device vector
	float * dAcc;

private:
	vtkCUDAMotionEquationSolver(const vtkCUDAMotionEquationSolver&);            // Not implemented.
	void operator=(const vtkCUDAMotionEquationSolver&);           // Not implemented.

};

#endif

