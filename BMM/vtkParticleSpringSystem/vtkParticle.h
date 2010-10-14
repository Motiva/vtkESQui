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
#ifndef __vtkParticle_h
#define __vtkParticle_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkObject.h"

class vtkSpring;
class vtkSpringCollection;

//! Implementation of a system particle
/*!
 * Contains all the specific info for each system particle: position, velocity, acceleration, etc...
 *
 */

class VTK_vtkParticleSpringSystem_EXPORT vtkParticle : public vtkObject {
public:

	//! Type revision macro
	vtkTypeRevisionMacro(vtkParticle, vtkObject);
	//! Create new particle
	static vtkParticle * New();
	//! Print particle info
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set particle identifier
	vtkSetMacro(Id, vtkIdType);						// Id
	//! get particle identifier
	vtkGetMacro(Id, vtkIdType);
	//! Set particle mass
	void SetMass(double mass);
	//! Get particle mass
	vtkGetMacro(Mass, double);						// Mass of the particle
	//! Get particle inverse mass
	vtkGetMacro(InverseMass, double);
	//! Set is particle status as fixed
	vtkSetMacro(Status, bool);						// Fix particle
	//! Set is particle status as fixed
	vtkGetMacro(Status, bool);
	//! Set particle as contacted
	vtkSetMacro(Contacted, bool);					// Contacted particle
	//! Get if particle is contacted
	vtkGetMacro(Contacted, bool);
	//! Enable/Disable contacted particle
	vtkBooleanMacro(Contacted, bool);

	//! Set particle position
	vtkSetVector3Macro(Position, double);
	//! Get particle position
	vtkGetVector3Macro(Position, double);
	//! Add offset to particle position
	void AddPosition(double x, double y, double z);

	//! Set particle velocity
	vtkSetVector3Macro(Velocity, double);
	//! Get particle velocity
	vtkGetVector3Macro(Velocity, double);

	//! Set particle acceleration
	vtkSetVector3Macro(Acceleration, double);
	//! Get particle acceleration
	vtkGetVector3Macro(Acceleration, double);

	//! Set particle force
	vtkSetVector3Macro(Force, double);
	//! Get particle force
	vtkGetVector3Macro(Force, double);
	//! Add offset to particle force
	void AddForce(double x, double y, double z);

	//! Insert a new spring in the particle spring collection
	void InsertNextSpring(vtkSpring * spring);

	//! Set a spring at a specified id
	void SetSpring(vtkIdType id, vtkSpring * spring);

	//! Get spring with the specified id
	vtkSpring * GetSpring(vtkIdType id);

	//! Check whether the particle contains a spring
	bool ContainsSpring(vtkSpring * spring);

	//! Update particle properties: position, velocity, accel...
	/*!
	 * All the particle properties are updated on each step. The particle will be ignored if is set as fixed
	 */
	void Update();


protected:
	vtkParticle();
	~vtkParticle();

	//! Particle identifier
	vtkIdType Id;
	//! 3D position
	double Position[3];
	//! Particle velocity. updated on every step
	double Velocity[3];
	//! Particle acceleration. updated on every step
	double Acceleration[3];
	//! particle force. f=m·a
	double Force[3];

	//! Mass of the particle
	double Mass;
	//! Inverse mass. optimizes calculation
	double InverseMass;
	//! Particle is free/fixed (0/1)
	bool Status;
	//! Particle is contacted
	bool Contacted;

	//! Collection of particle springs
	vtkSpringCollection * Springs;

private:
	vtkParticle(const vtkParticle&);            // Not implemented.
	void operator=(const vtkParticle&);           // Not implemented.

};

#endif

