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

#include "vtkObjectFactory.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkCellArray.h"

#include "vtkParticleCollection.h"
#include "vtkParticle.h"

#include "vtkMath.h"

//! Implementation of the mass-spring deformation model

class VTK_vtkParticleSpringSystem_EXPORT vtkSpring : public vtkObject {
public:


	vtkTypeRevisionMacro(vtkSpring, vtkObject);
	static vtkSpring * New();
	void PrintSelf(ostream& os, vtkIndent indent);

	vtkSetMacro(Id, vtkIdType);						// Id
	vtkGetMacro(Id, vtkIdType);
	vtkSetMacro(SpringCoefficient, double);		// Spring coefficient
	vtkGetMacro(SpringCoefficient, double);
	vtkSetMacro(DampingCoefficient, double);		// Damping coefficient
	vtkGetMacro(DampingCoefficient, double);
	vtkSetMacro(DistanceCoefficient, double);		// Distance constraint coefficient
	vtkGetMacro(DistanceCoefficient, double);
	vtkSetMacro(DeltaT, double);					// dt for every step
	vtkGetMacro(DeltaT, double);
	vtkGetMacro(Distance, double);					// Distance between particles
	vtkSetObjectMacro(Particles, vtkParticleCollection);	// Neighbor particles

	double * GetDirection();

	void GetDirection(double Direction[3]);

	void InsertNextParticle(vtkParticle * particle);

	void SetParticle(vtkIdType id, vtkParticle * particle);

	vtkParticle * GetParticle(vtkIdType id);

	bool ContainsParticle(vtkParticle * particle);

	void Init();

protected:
	vtkSpring();
	~vtkSpring();

	vtkIdType Id;
	double SpringCoefficient;
	double DampingCoefficient;
	double DistanceCoefficient;
	double DeltaT;

	double Distance;
	double Direction[3];

	vtkParticleCollection * Particles;

private:
	vtkSpring(const vtkSpring&);            // Not implemented.
	void operator=(const vtkSpring&);           // Not implemented.

};

#endif

