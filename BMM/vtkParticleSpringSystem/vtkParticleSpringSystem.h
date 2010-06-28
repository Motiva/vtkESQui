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
#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include "vtkPolyDataAlgorithm.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"

#include "vtkMassProperties.h"
#include "vtkSpring.h"
#include "vtkSpringCollection.h"
#include "vtkParticle.h"
#include "vtkParticleCollection.h"
#include "vtkCellLinks.h"
#include "vtkCell.h"


//BTX
class vtkMotionEquationSolver;
class vtkVelocityVerletSolver;
//ETX

//! Implementation of the mass-spring deformation model

class VTK_vtkParticleSpringSystem_EXPORT vtkParticleSpringSystem : public vtkPolyDataAlgorithm {
public:

	//BTX
	//!Enumeration of solver types
	enum MotionEquationSolverType{
		Euler = 0,
		ModifiedEuler = 1,
		VelocityVerlet = 2,
		RungeKutta4 = 3
	};
	//ETX

	vtkTypeRevisionMacro(vtkParticleSpringSystem,vtkPolyDataAlgorithm);
	static vtkParticleSpringSystem * New();
	void PrintSelf(ostream& os, vtkIndent indent);

	void Init();

	void Step();

	//get parameters for Mass Spring System
	vtkSetMacro(SpringCoefficient, double);		// Spring coefficient
	vtkSetMacro(DistanceCoefficient, double);	// Distance constraint coefficient
	vtkSetMacro(DampingCoefficient, double);	// Damping coefficient
	vtkSetMacro(DeltaT, double);				// dt for every step
	vtkSetMacro(Mass, double);					// Mass of all points (future work: specify different masses for different "layers")
	vtkSetMacro(RigidityFactor, vtkIdType);		// Rigidity Factor -> neighborhood size
	vtkSetMacro(SolverType, MotionEquationSolverType);		// Motion equation solver type

	void SetContacts(vtkIdList * ids, vtkDoubleArray * directions);

	void ComputeForces();

protected:
	vtkParticleSpringSystem();
	~vtkParticleSpringSystem();

	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//Particle-Spring System
	vtkSpringCollection * Springs;
	vtkParticleCollection * Particles;
	vtkCellLinks * Links;

	//Model Parameters
	double SpringCoefficient;
	double DistanceCoefficient;
	double DampingCoefficient;
	double DeltaT;
	double Mass;
	vtkIdType RigidityFactor;

	//System Properties
	vtkMassProperties * SystemProperties;
	double Volume;

	//Motion equation solver
	vtkMotionEquationSolver * Solver;
	MotionEquationSolverType SolverType;

	//Contact data
	vtkIdList* ContactIds;
	vtkDoubleArray* ContactDirections;

private:
	vtkParticleSpringSystem(const vtkParticleSpringSystem&);            // Not implemented.
	void operator=(const vtkParticleSpringSystem&);           // Not implemented.

	void ComputeContacts();

};

#endif

