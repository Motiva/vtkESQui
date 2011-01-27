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
#ifndef __vtkPSSInterface_h
#define __vtkPSSInterface_h

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkBioMechanicalModel.h"

#include "vtkParticleSpringSystem.h"

//! Interface to the abstract vtkBiomechanicalModel class for a particle-spring system

class VTK_ESQUI_BIOMECHANICS_EXPORT vtkPSSInterface : public vtkBioMechanicalModel
{
public:
	vtkTypeRevisionMacro(vtkPSSInterface, vtkBioMechanicalModel);
	//! Create a new PSS Interface
	static vtkPSSInterface* New();
	const char *GetClassName() {return "vtkPSSInterface";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Process the algorithm request (Update).
	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	//!Initialize the Biomechanical Model
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
	//! Set the neighborhood size
	vtkSetMacro(RigidityFactor, int);
	//! Set motion equation solver type.
	vtkSetMacro(SolverType, vtkParticleSpringSystem::MotionEquationSolverType);

protected:
	vtkPSSInterface();
	~vtkPSSInterface();

private:
	vtkPSSInterface(const vtkPSSInterface&);  // Not implemented.
	void operator=(const vtkPSSInterface&);  // Not implemented.

	//!Particle-spring system mesh
	vtkParticleSpringSystem * ParticleSpringSystem;

	// PSS specific parameters
	//! Distance coefficient.
	double DistanceCoefficient;
	//! Damping coefficient.
	/*!
	 * < 1 Under-damped. The system oscillates (with a slightly different frequency than the undamped case) with the amplitude gradually decreasing to zero
	 * = 1 Critically Damped. The system returns to equilibrium as quickly as possible without oscillating
	 * > 1 Over-Damped. The system returns (exponentially decays) to equilibrium without oscillating
	 */
	double DampingCoefficient;
	//! Spring Coefficient K
	double SpringCoefficient;
	//! Mass value on each point
	double Mass;
	//! calculation time step
	double DeltaT;
	//! Neighborhood size factor
	int RigidityFactor;
	//! Motion equation solver type
	//BTX
	vtkParticleSpringSystem::MotionEquationSolverType SolverType;
	//ETX
};

#endif
