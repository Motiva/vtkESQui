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
#ifndef __vtkSpring_h
#define __vtkSpring_h

#include "vtkParticleSpringSystemWin32Header.h"
#include "vtkObject.h"

class vtkParticle;
class vtkParticleCollection;

//! Implementation of a system spring
/*!
 * Every system particle is connected at least to 1 spring. One spring always connects two particles of the system.
 * This class contains all the specific info for each spring: rest length, stiffness, damping, direction, etc...
 */

class VTK_vtkParticleSpringSystem_EXPORT vtkSpring : public vtkObject {
public:

	//! Type Revision Macro
	vtkTypeRevisionMacro(vtkSpring, vtkObject);
	//! Create new spring object
	static vtkSpring * New();
	//! Print object info
	void PrintSelf(ostream& os, vtkIndent indent);

	//! Set spring identifier
	vtkSetMacro(Id, vtkIdType);
	//! Get spring identifier
	vtkGetMacro(Id, vtkIdType);
	//! Set spring coefficient
	vtkSetMacro(K, double);
	//! Get spring coefficient
	vtkGetMacro(K, double);
	//! Set damping coefficient
	vtkSetMacro(Damping, double);
	//! Get damping coefficient
	vtkGetMacro(Damping, double);
	//! Set distance coefficient. Percentage
	vtkSetMacro(Distance, double);
	//! Get distance coefficient. Percentage
	vtkGetMacro(Distance, double);
	//! Set time step. (ms)
	vtkSetMacro(TimeStep, double);					// dt for every step
	//! Get time step. (ms)
	vtkGetMacro(TimeStep, double);
	//! Set rest length
	vtkGetMacro(RestLength, double);					// Distance between particles

	//! Get spring direction.
	double * GetDirection();

	//! Get spring direction.
	void GetDirection(double Direction[3]);

	//! Insert a new particle in the spring particle collection
	void InsertNextParticle(vtkParticle * particle);

	//! Set a particle at a specified id
	void SetParticle(vtkIdType id, vtkParticle * particle);

	//! Get particle with the specified id
	vtkParticle * GetParticle(vtkIdType id);

	//! Check whether the spring contains a particle
	bool ContainsParticle(vtkParticle * particle);

	//! Initialize the spring values
	void Init();

protected:
	vtkSpring();
	~vtkSpring();

	//! Spring Identifier
	vtkIdType Id;
	//! Spring Coefficient, stiffness. k; F = -kx
	double K;
	//! Damping Coefficient
	double Damping;
	//! Distance Coefficient
	double Distance;
	//! Time step,
	double TimeStep;

	//! Spring rest length
	double RestLength;
	//! spring direction
	double Direction[3];

	//! Spring particles
	vtkParticleCollection * Particles;

private:
	vtkSpring(const vtkSpring&);            // Not implemented.
	void operator=(const vtkSpring&);           // Not implemented.

};

#endif

