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
#ifndef __vtkDeformationModel
#define __vtkDeformationModel

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkModel.h"

class vtkPolyData;

class vtkCollision;
class vtkCollisionCollection;
class vtkBoundaryCondition;
class vtkBoundaryConditionCollection;

//! Class vtkDeformationModel defines a visualization mesh (PolyData) with its texture. Inherits from vtkModel

class VTK_ESQUI_SCENARIO_EXPORT vtkDeformationModel: public vtkModel {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkDeformationModel, vtkModel);

	//!Create a new Scenario object
	//static vtkDeformationModel * New();

	//!Return the class name
	const char *GetClassName() {return "vtkDeformationModel";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Initialize the Deformation Model
	virtual void Init();

	//!Set the detected collisions
	void SetCollisions(vtkCollisionCollection * collisions);

	//!Get the detected contacts
	vtkCollisionCollection * GetCollisions();

	//!Add a collision to the model
	void AddCollision(vtkCollision * c);

	//!Remove the collision at the specified id
	void RemoveCollision(vtkIdType id);

	//!Remove all collisions from the model
	void RemoveAllCollisions();

	//!Insert a condition into the deformation model
	virtual void InsertNextBoundaryCondition(vtkBoundaryCondition * condition) ;

	//!Insert a collection of conditions into the deformation model
	virtual void InsertBoundaryConditions(vtkBoundaryConditionCollection * collection);

	//!Remove conditions of the deformation model
	virtual void DeleteBoundaryConditions();

	//! Set Gravity Force (m/s2)
	vtkSetVector3Macro(Gravity, double);
	//! Get Gravity Force (m/s2)
	vtkGetVector3Macro(Gravity, double);

protected:

	vtkDeformationModel();
	~vtkDeformationModel();

	//! Gravitation force
	double Gravity[3];

	//!Model collisions.
	/*!
	 * This acts as pointer to the collision model collection of collisions
	 */
	vtkCollisionCollection * Collisions;

	//!Boundary conditions
	vtkBoundaryConditionCollection * BoundaryConditions;

	//! Process the algorithm request (Update).
	//virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
	vtkDeformationModel (const vtkDeformationModel &);
	void operator =(const vtkDeformationModel &);

};

#endif
