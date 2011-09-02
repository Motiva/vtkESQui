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
#ifndef __vtkCUDAParticleSystem_h
#define __vtkCUDAParticleSystem_h

#include "vtkCUDAParticleSystemWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

#include "vtkCUDAMotionEquationSolver.h"

class vtkMassProperties;

//! Implementation of the particle-spring deformation model
/*!
 * The system is compounded of several collections of elements: springs, particles, links.
 * There are some system properties to define material behavior. All these properties must be set prior to the initialization Init()
 */

class VTK_vtkCUDAParticleSystem_EXPORT vtkCUDAParticleSystem : public vtkPolyDataAlgorithm {
public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkCUDAParticleSystem,vtkPolyDataAlgorithm);
	//! Create new particle spring system object
	static vtkCUDAParticleSystem * New();
	//! Print system info
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Initialize the particle spring system
	/*!
	 * Create springs, particles and the links between them.
	 */
	void Init();

	//Parameters for Particle-Spring System
	//! Set spring coefficient
	vtkSetMacro(SpringCoefficient, double);		// Spring coefficient
	//! Set distance coefficient. Percentage
	vtkSetMacro(DistanceCoefficient, double);	// Distance constraint coefficient
	//! Set damping coefficient
	vtkSetMacro(DampingCoefficient, double);	// Damping coefficient
	//! Set time step
	vtkSetMacro(DeltaTime, double);				// dt for every step
	//! Set mass of system particles
	vtkSetMacro(Mass, double);					// Mass of all points (future work: specify different masses for different "layers")
	//! Set residual error
	vtkSetMacro(Residual, double);					// Residual error tolerance on displacement calculus
	//! Set motion equation solver type.
	vtkSetMacro(SolverType, vtkCUDAMotionEquationSolver::CUDAMotionEquationSolverType);		// Motion equation solver type

	//! Set Gravity Force (m/s2)
	vtkSetVector3Macro(Gravity, double);
	//! Get Gravity Force (m/s2)
	vtkGetVector3Macro(Gravity, double);

	//! Insert a collision in the particle-spring system
	void InsertCollision(vtkIdType id, double * displacement);

	//! Insert collisions particle-spring system
	/*!
	 * \param ids List of particle ids
	 * \param displacements Array containing displacement displacements
	 */
	void SetCollisions(vtkIdList * ids, vtkDoubleArray * displacements);

	//! Compute the forces attending to the collisions
	/*!
	 * The solver is executed every step, in order to compute the force propagation along the whole system
	 */
	void ComputeForces();

protected:
	vtkCUDAParticleSystem();
	~vtkCUDAParticleSystem();

	//! VTK method for system update. Must be implemented
	int RequestData(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector);

	// Model Parameters
	//! Init flag
	bool Initialized;
	//! System total number of Particles
	int NumberOfParticles;
	//! System total number of Spring
	int NumberOfSprings;

	// Host Data Vectors
	// Particles
	//! Particle position host vector
	float * hPos;
	//! Particle velocity host vector
	float * hVel;
	//! Particle acceleration host vector
	float * hAcc;
	//! Particle force host vector
	float * hFor;
	//! Particle mass host vector
	float * hMss;

	// Springs
	//! Spring particle identifiers
	int * hIds; //particle Ids
	//! Spring length. Distance between particles.
	float * hLength;

	// Device Data
	//! Particle position device vector
	float * dPos;
	//! Particle velocity device vector
	float * dVel;
	//! Particle acceleration device vector
	float * dAcc;

	//! Equation time step
	double DeltaTime;

	// System Parameters
	//! Spring stiffness k.
	double SpringCoefficient;
	//! Distance coefficient. Maximum Percentage of elongation
	double DistanceCoefficient;
	//! Damping coefficient
	double DampingCoefficient;
	//! Mass for each system particle
	double Mass;
	//! Residual error tolerance
	double Residual;

	//! Gravitational force vector
	double Gravity[3];

	//! Motion equation solver
	vtkCUDAMotionEquationSolver * Solver;
	//! Motion equation solver type
	vtkCUDAMotionEquationSolver::CUDAMotionEquationSolverType SolverType;

	//Contact data
	//! Contact Point Ids
	vtkIdList * CollisionIds;
	//! Contact Point Displacements
	vtkDoubleArray * CollisionDisplacements;

private:
	vtkCUDAParticleSystem(const vtkCUDAParticleSystem&);            // Not implemented.
	void operator=(const vtkCUDAParticleSystem&);           // Not implemented.

	//! Compute inserted collisions
	void ComputeCollisions();

	//! Enumeration of vector types
	enum VectorType{
		Position,
		Velocity,
		Acceleration,
		Force
	};

	//
	float * GetParticleVector(VectorType t, vtkIdType id);

	//
	void DisplayParticleVectors();

};

#endif

