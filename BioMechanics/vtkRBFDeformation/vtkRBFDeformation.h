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

#ifndef __vtkRBFDeformation_h
#define __vtkRBFDeformation_h

#include "vtkRBFDeformationWin32Header.h"
#include "vtkPolyDataAlgorithm.h"
#include "RBFInterpolator.h"

class vtkIdList;
class vtkIntArray;
class vtkDoubleArray;
class vtkTupleInterpolator;

class VTK_vtkRBFDeformation_EXPORT vtkRBFDeformation : public vtkPolyDataAlgorithm {
public:
	//!Type revision macro
	vtkTypeRevisionMacro(vtkRBFDeformation,vtkPolyDataAlgorithm);

	//!Create new object
	static vtkRBFDeformation *New();
	//!Print object info
	void PrintSelf(ostream& os, vtkIndent indent);
	//!Return class name
	const char *GetClassName() {return "vtkRBFDeformation";};

	//!Initialize class
	void Init();

	//!Set the number of control points
	vtkSetMacro(NumberOfControlPoints, int);
	//!Get the number of control points
	vtkGetMacro(NumberOfControlPoints, int);

	//!Set the time step
	vtkSetMacro(DeltaT, double);
	//!Get the time step
	vtkGetMacro(DeltaT, double);

	//!Set the relaxation factor
	vtkSetClampMacro(RelaxationFactor, double, 0.0, 1.0);
	//!Get the relaxation factor
	vtkGetMacro(RelaxationFactor, double);

	void SetDisplacement(int i, double * d);

protected:
	vtkRBFDeformation();
	~vtkRBFDeformation();

	virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:

	vtkRBFDeformation (const vtkRBFDeformation &);//NotImplemented
	void operator =(const vtkRBFDeformation &);//Not Implemented

	bool Initialized;

	int NumberOfControlPoints;

	double DeltaT;

	double RelaxationFactor;

	vtkDoubleArray * controlPoints;
	vtkDoubleArray * controlPointDisplacements;

	vtkIdList * controlPointIds;
	vtkIntArray * controlPointStatus;

	vector<float> controlPointX;										// X-coordinates of control points
	vector<float> controlPointY;										// Y-coordinates of control points
	vector<float> controlPointZ;										// Z-coordinates of control points

	vector<float> controlPointDisplacementX;							// Displacement of control points in the X-direction
	vector<float> controlPointDisplacementY;							// Displacement of control points in the Y-direction
	vector<float> controlPointDisplacementZ;							// Displacement of control points in the Z-direction

	RBFInterpolator * rbfX;
	RBFInterpolator * rbfY;
	RBFInterpolator * rbfZ;

};

#endif
