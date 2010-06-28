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

#include "vtkObjectFactory.h"
#include "vtkCollection.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkCellArray.h"

#include "vtkMath.h"

//! Implementation of the mass-spring deformation model

class VTK_vtkParticleSpringSystem_EXPORT vtkParticle : public vtkObject {
public:


	vtkTypeRevisionMacro(vtkParticle, vtkObject);
	static vtkParticle * New();
	void PrintSelf(ostream& os, vtkIndent indent);

	vtkSetMacro(Id, vtkIdType);						// Id
	vtkGetMacro(Id, vtkIdType);
	vtkSetMacro(DeltaT, double);					// dt for every step
	//vtkSetMacro(Mass, double);
	vtkGetMacro(Mass, double);						// Mass of the particle
	vtkGetMacro(InverseMass, double);
	vtkSetMacro(DistanceCoefficient, double);		// Distance constraint coefficient
	vtkSetMacro(DampingCoefficient, double);		// Damping coefficient
	vtkSetMacro(Fixed, bool);						// Fix particle
	vtkSetMacro(Contacted, bool);					// Contacted particle
	vtkGetMacro(Contacted, bool);

	void SetMass(double mass);

	void SetPosition(double x, double y, double z);

	void SetPosition(double * position);

	void AddPosition(double x, double y, double z);

	double * GetPosition();

	void GetPosition(double position[3]);

	void SetVelocity(double x, double y, double z);

	void SetVelocity(double * Velocity);

	double * GetVelocity();

	void GetVelocity(double Velocity[3]);

	void SetAcceleration(double x, double y, double z);

	void SetAcceleration(double * Acceleration);

	double * GetAcceleration();

	void GetAcceleration(double Acceleration[3]);

	void Update();

	void SetForce(double x, double y, double z);

	void SetForce(double * Force);

	void AddForce(double x, double y, double z);

	double * GetForce();

	void GetForce(double Force[3]);

protected:
	vtkParticle();
	~vtkParticle();

	vtkIdType Id;
	double Position[3];
	double Velocity[3];
	double Acceleration[3];
	double Force[3];

	double DeltaT;
	double DistanceCoefficient;
	double DampingCoefficient;
	double Mass;
	double InverseMass;
	bool Fixed;
	bool Contacted;

private:
	vtkParticle(const vtkParticle&);            // Not implemented.
	void operator=(const vtkParticle&);           // Not implemented.

};

#endif

