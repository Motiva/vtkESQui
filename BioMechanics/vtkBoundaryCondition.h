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
#ifndef __vtkBoundaryCondition_h
#define __vtkBoundaryCondition_h

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkObject.h"

class vtkPoints;
class vtkIdList;

//!This class acts as data container storing all the useful information of aboundary condition.

class VTK_ESQUI_BIOMECHANICS_EXPORT vtkBoundaryCondition : public vtkObject {
	
public:
	//! Type revision macro
	vtkTypeRevisionMacro(vtkBoundaryCondition,vtkObject);

	//! Create new contact that will be used as a data container
	static vtkBoundaryCondition *New();
	//! Print contact info
	void PrintSelf(ostream& os, vtkIndent indent);
	//! Return class name
	const char *GetClassName() {return "vtkBoundaryCondition";};

	
	//!Condition type definition
	enum vtkBoundaryConditionType{
		Dirichlet = 0,
		Neumann = 1,
		Cauchy = 2
	};
	

	//!Set condition type
	vtkSetMacro(Type, vtkBoundaryCondition::vtkBoundaryConditionType);
	//!Return condition type
	vtkGetMacro(Type, vtkBoundaryCondition::vtkBoundaryConditionType);

	//!Set condition id
	vtkSetMacro(Id, vtkIdType);
	//!Return condition id
	vtkGetMacro(Id, vtkIdType);

	//!Set condition value
	vtkSetMacro(Value, double);
	//!Return condition value
	vtkGetMacro(Value, double);

	//!Set condition value
	vtkSetMacro(PointId, vtkIdType);
	//!Return condition value
	vtkGetMacro(PointId, vtkIdType);

	//!Set condition point coordinates
	vtkSetVector3Macro(Point, double);
	//!Return condition point coordinates
	vtkGetVector3Macro(Point, double);

	//! Performs a full detailed copy of the contact
	void DeepCopy(vtkBoundaryCondition *info);

protected:
	vtkBoundaryCondition();
	~vtkBoundaryCondition();
private:

	//! Condition Type
	vtkBoundaryConditionType Type;

	//! Condition Id
	vtkIdType Id;

	//! Condition point coordinates
	double Point[3];

	//! Condition point id
	vtkIdType PointId;

	//! Condition value
	double Value;

	vtkBoundaryCondition (const vtkBoundaryCondition &);//NotImplemented
	void operator =(const vtkBoundaryCondition &);//Not Implemented
};
#endif
