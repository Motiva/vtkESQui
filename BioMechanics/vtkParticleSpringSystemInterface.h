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
#ifndef __vtkParticleSpringSystemInterface_h
#define __vtkParticleSpringSystemInterface_h

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkDeformationModel.h"

#include "vtkParticleSpringSystem.h"

//! Interface class for a particle-spring deformation system.
/*!
 * This class, based in vtkDeformationModel class, adapts the access to the
 * external vtkParticleSpringSystem package.
 */

class VTK_ESQUI_BIOMECHANICS_EXPORT vtkParticleSpringSystemInterface : public vtkDeformationModel
{
public:
	//! Type Revision Macro
	vtkTypeRevisionMacro(vtkParticleSpringSystemInterface, vtkDeformationModel);
	//! Create a new PSS Interface
	static vtkParticleSpringSystemInterface* New();
	const char *GetClassName() {return "vtkParticleSpringSystemInterface";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//!Initialize the Deformation Model
	/*!
	 * The model has to be initialized in order to be updated. Some parameters
	 * must be previously defined:\n
	 * - Input: vtkPolyData object
	 * - DistanceCoefficient: Maximum distance between points
	 * - DampingCoefficient: (d) oscillation coefficient
	 * - SpringCoefficient: (K) spring coefficient
	 * - Mass: unit mass of each particle
	 * - RigidityFactor: n-neighborhood connectivity
	 * - DeltaT: time step of deformation process
	 * - SolverType: motion equation solver type
	 */
	virtual void Init();

	//! Set the distance coefficient
	vtkSetMacro(DistanceCoefficient, double);
	//! Set the damping coefficient
	vtkSetMacro(DampingCoefficient, double);
	//! Set the damping coefficient
	vtkSetMacro(SpringCoefficient, double);
	//! Set the mass value
	vtkSetMacro(Mass, double);
	//! Set the time step
	vtkSetMacro(DeltaT, double);
	//! Set motion equation solver type.
	vtkSetMacro(SolverType, vtkMotionEquationSolver::MotionEquationSolverType);

	//!Add displacement at the specified point.
	/*!
	 *  The displacement shall not be greater than a 1/10 of the input mesh radius
	 * \param pointId Point identifier
	 * \param vector displacement vector
	 */
	virtual void AddDisplacement(vtkIdType pointId, double * vector);

protected:
	vtkParticleSpringSystemInterface();
	~vtkParticleSpringSystemInterface();


	//!Particle-spring system mesh
	vtkParticleSpringSystem * ParticleSpringSystem;

	// PSS specific parameters
	//! Distance coefficient.
	double DistanceCoefficient;
	//! Damping coefficient.
	/*!
	 * Damping coefficient behaviour:\n
	 * - < 1 Under-damped. The system oscillates (with a slightly different frequency than the undamped case) with the amplitude gradually decreasing to zero
	 * - = 1 Critically Damped. The system returns to equilibrium as quickly as possible without oscillating
	 * - > 1 Over-Damped. The system returns (exponentially decays) to equilibrium without oscillating
	 */
	double DampingCoefficient;
	//! Spring Coefficient
	/*!
	 * Commonly known as K, defines the spring rigidity.
	 */
	double SpringCoefficient;
	//! Mass of each particle
	/*!
	 * Mass value for every particle of the system. All particles must be set to the same value.
	 * By default is set to 1.
	 */
	double Mass;
	//! Calculation time step
	/*!
	 * Deformation process time interval. The configuration is crucial, for greater values the system
	 * could become unstable.
	 */
	double DeltaT;

	//! Motion equation solver type
	/*!
	 * There are several motion equation solvers available:
	 * - Euler
	 * - Modified Euler
	 * - Verlet
	 * - Runge-Kutta 4-5
	 */
	vtkMotionEquationSolver::MotionEquationSolverType SolverType;
	
private:
	vtkParticleSpringSystemInterface(const vtkParticleSpringSystemInterface&);  // Not implemented.
	void operator=(const vtkParticleSpringSystemInterface&);  // Not implemented.
};

#endif
