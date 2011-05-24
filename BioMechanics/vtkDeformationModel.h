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

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkModel.h"

class vtkPolyData;

class vtkCollision;
class vtkCollisionCollection;
class vtkBoundaryCondition;
class vtkBoundaryConditionCollection;

//! vtkDeformationModel object defines a deformation model based on a mesh (vtkPolyData).
/*!
 * This class inherits from vtkModel base class. As it is specified in vtkModel,
 * at least an input mesh should be defined. Optionally a source mesh for
 * synchronization purposes may be defined.
 * An interface based on this class must be implemented to access available
 * deformation models: vtkParticleSpringSystem, vtkExplicitDeformableModel, etc...
 */

class VTK_ESQUI_BIOMECHANICS_EXPORT vtkDeformationModel: public vtkModel {

public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkDeformationModel, vtkModel);

	//!Return the class name
	const char *GetClassName() {return "vtkDeformationModel";}

	//!Print class values
	void PrintSelf(ostream& os, vtkIndent indent);

	//!Initialize the deformation model
	/*!
	 * The model has to be initialized in order to be updated. At least one parameter must be previously defined: \n
	 * - Input: vtkPolyData object \n
	 */
	virtual void Init();

	//!Set the detected collisions
	/*!
	 * Set a collection of collisions to the model. Any collision previously added will be removed.
	 * \param collisions collection of collisions
	 * \sa vtkCollisionCollection * GetCollisions()
	 */
	void SetCollisions(vtkCollisionCollection * collisions);

	//!Get the detected contacts
	/*!
	 * \return collection of collisions
	 * \sa SetCollisions(vtkCollisionCollection * collisions)
	 */
	vtkCollisionCollection * GetCollisions();

	//!Add a collision to the model
	/*!
	 * Add a single collision to the model. Previously added collisions are kept.
	 * \param c collision to be added
	 * \sa RemoveCollision(vtkIdType id)
	 */
	void AddCollision(vtkCollision * c);

	//!Remove the collision at the specified id
	/*!
	 * Remove a single collision from the model at the specified id
	 * \param id collision identifier
	 * \sa AddCollision(vtkCollision * c)
	 */
	void RemoveCollision(vtkIdType id);

	//!Remove all collisions from the model
	/*!
	 * All previously defined collisions are removed from the model. Collision collection object is not deleted.
	 */
	void RemoveAllCollisions();

	//!Insert a condition into the deformation model
	/*!
	 * Add a single condition to the model at the end of the collection
	 * \param condition condition to be added
	 * \sa InsertBoundaryConditions(vtkBoundaryConditionCollection * collection);
	 */
	virtual void InsertNextBoundaryCondition(vtkBoundaryCondition * condition) ;

	//!Insert a collection of conditions into the deformation model
	/*!
	 * Add a collection of conditions to the model.
	 * \param collection collection of conditions to be added
	 * \sa InsertNextBoundaryCondition(vtkBoundaryCondition * condition) ;
	 */
	virtual void InsertBoundaryConditions(vtkBoundaryConditionCollection * collection);

	//!Remove conditions of the deformation model
	/*!
	 * All previously defined conditions are removed from the model. Condition collection object is not deleted.
	 */
	virtual void RemoveBoundaryConditions();

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
	 * This acts as pointer to the deformation model collection of collisions
	 */
	vtkCollisionCollection * Collisions;

	//!Boundary conditions
	/*!
	 * This acts as pointer to the deformation model collection of conditions
	 */
	vtkBoundaryConditionCollection * BoundaryConditions;

private:
	vtkDeformationModel (const vtkDeformationModel &);
	void operator =(const vtkDeformationModel &);

};

#endif
