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
#ifndef __vtkRK4Solver_h
#define __vtkRK4Solver_h

#include "vtkParticleSpringSystemWin32Header.h"

#include "vtkObjectFactory.h"
#include "vtkIdList.h"
#include "vtkDoubleArray.h"
#include "vtkParticleCollection.h"
#include "vtkParticle.h"

#include "vtkParticleSpringSystem.h"

#include "vtkMath.h"

//! Implementation of the mass-spring deformation model

class VTK_vtkParticleSpringSystem_EXPORT vtkRK4Solver : public vtkObject {
public:


	vtkTypeRevisionMacro(vtkRK4Solver, vtkObject);
	static vtkRK4Solver * New();
	void PrintSelf(ostream& os, vtkIndent indent);

	vtkSetMacro(NumberOfParticles, double);		// NumberOfParticles
	vtkSetObjectMacro(DeformationModel, vtkParticleSpringSystem);		// NumberOfParticles

protected:
	vtkRK4Solver();
	~vtkRK4Solver();

	void Init();
	//void ComputeNextStep(vtkDoubleArray * positions, vtkDoubleArray * velocities, double deltaT);
	void ComputeNextStep(vtkParticleCollection * particles, double deltaT);

private:
	vtkRK4Solver(const vtkRK4Solver&);            // Not implemented.
	void operator=(const vtkRK4Solver&);           // Not implemented.

	vtkParticleSpringSystem * DeformationModel;
	double NumberOfParticles;

	vtkDoubleArray * dx;
	vtkDoubleArray * dv;

	vtkDoubleArray * dx1;
	vtkDoubleArray * dv1;
	vtkDoubleArray * dx2;
	vtkDoubleArray * dv2;
	vtkDoubleArray * dx3;
	vtkDoubleArray * dv3;

	void Evaluate(vtkParticle * p, double * dX, double * dV, double deltaT);

	void Reset();
};

#endif

