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
#ifndef __vtkGetFEMInterface_h
#define __vtkGetFEMInterface_h

#include "vtkFEMSystemWin32Header.h"
#include "vtkObject.h"

#include "bgeot_mesh_structure.h"
#include "getfem_mesh.h"
#include "getfem_mesh_fem.h"
#include "getfem/getfem_config.h"
#include "getfem/getfem_assembling.h" /* import assembly methods (and norms comp.) */
#include "getfem/getfem_import.h"
#include "getfem/getfem_export.h"   /* export functions (save solution in a file)  */
#include "getfem/getfem_regular_meshes.h"
#include "getfem/getfem_model_solvers.h"
#include "getfem/getfem_Coulomb_friction.h"
#include "gmm/gmm.h"

class vtkPolyData;

/*Some GetFEM types*/
using bgeot::base_small_vector; /* special class for small (dim<16) vectors */
using bgeot::base_node;  /* geometrical nodes(derived from base_small_vector)*/
using bgeot::scalar_type; /* = double */
using bgeot::size_type;   /* = unsigned long */
using bgeot::dim_type;
using bgeot::base_matrix; /* small dense matrix. */
using bgeot::pgeometric_trans;

/*Some Typedefs*/
typedef bgeot::base_node getFEMPoint;
typedef bgeot::base_small_vector getFEMVector;

/* definition of some matrix/vector types.
 * default types of getfem_model_solvers.h
 */
typedef getfem::modeling_standard_sparse_vector sparse_vector;
typedef getfem::modeling_standard_sparse_matrix sparse_matrix;
typedef getfem::modeling_standard_plain_vector  plain_vector;

//! Implementation of th
/*!
 *
 */

class VTK_vtkFEMSystem_EXPORT vtkGetFEMInterface : public vtkObject
{
public:

	//!Type revision macro
	vtkTypeRevisionMacro(vtkGetFEMInterface,vtkObject);
	//! Create new particle spring system object
	static vtkGetFEMInterface * New();
	//! Print system info
	void PrintSelf(ostream& os, vtkIndent indent);

	void SetPolyData(vtkPolyData * data);

	//!Set model filename
	vtkSetStringMacro(FileName);
	vtkGetStringMacro(FileName);

	//!Set Dirichlet condition status
	vtkSetMacro(Dirichlet, bool);
	vtkGetMacro(Dirichlet, bool);
	vtkBooleanMacro(Dirichlet, bool);
	//!Set Dirichlet Ratio
	vtkSetMacro(DirichletRatio, scalar_type);
	vtkGetMacro(DirichletRatio, scalar_type);

	//Material

	//!Set Lambda Coefficient
	vtkSetMacro(Lambda, scalar_type);
	vtkGetMacro(Lambda, scalar_type);

	//!Set Mu Coefficient
	vtkSetMacro(Mu, scalar_type);
	vtkGetMacro(Mu, scalar_type);

	//!Set Rho Density
	vtkSetMacro(Rho, scalar_type);
	vtkGetMacro(Rho, scalar_type);

	//!Set PG Gravity
	vtkSetMacro(PG, scalar_type);
	vtkGetMacro(PG, scalar_type);

	//!Set Friction Coefficient
	vtkSetMacro(Friction_coef, scalar_type);
	vtkGetMacro(Friction_coef, scalar_type);

	//!Set Contact Nodes Mass
	vtkSetMacro(ContactMass, size_type);
	vtkGetMacro(ContactMass, size_type);

	//!Set Augmentation
	vtkSetMacro(R, scalar_type);
	vtkGetMacro(R, scalar_type);

	//Time Scheme
	//!Set Time Scheme
	vtkSetMacro(Scheme, size_type);
	vtkGetMacro(Scheme, size_type);

	//!Set Theta Coefficient
	vtkSetMacro(Theta, scalar_type);
	vtkGetMacro(Theta, scalar_type);

	//!Set Beta Coefficient (Newmark)
	vtkSetMacro(Beta, scalar_type);
	vtkGetMacro(Beta, scalar_type);

	//!Set Gamma Coefficient (Newmark)
	vtkSetMacro(Gamma, scalar_type);
	vtkGetMacro(Gamma, scalar_type);

	//!Set Restitution
	vtkSetMacro(Restitution, scalar_type);
	vtkGetMacro(Restitution, scalar_type);

	//!Set Delta T
	vtkSetMacro(DeltaT, scalar_type);
	vtkGetMacro(DeltaT, scalar_type);

	//!Set Residual Error
	vtkSetMacro(Residual, scalar_type);
	vtkGetMacro(Residual, scalar_type);

	void Init();

	void Write();

	void Solve();

protected:
	vtkGetFEMInterface();
	~vtkGetFEMInterface();

	void SelectBoundaries();

	enum {DIRICHLET_BOUNDARY = 0, CONTACT_BOUNDARY = 1};

	//! Biomechanical model mesh Filename
	char * FileName;

	//!Surface mesh
	vtkPolyData * Data;

	//Mesh
	//!Biomechanical Model mesh
	getfem::mesh * Mesh;

	//FEM Meshes
	//!Integration method
	getfem::mesh_im * MIM;
	//!PDE solution. Displacements after force apply
	getfem::mesh_fem * MF_U;
	//!VonMisses stress values
	getfem::mesh_fem * MF_VM;
	//!Right hand side values
	getfem::mesh_fem * MF_RHS; /*Right Hand-side force*/
	//!Contact multiplier
	getfem::mesh_fem * MF_CONT;

	pgeometric_trans GeometricTransformation;

	plain_vector U;

	//Boundary Conditions
	//!Enable/Disable dirichlet condition
	bool Dirichlet;
	//!Dirichlet Ratio
	scalar_type DirichletRatio;

	//Brick Model
	//!Brick model for PDE solving
	getfem::model * Model;

	//Mesh Parameters
	//!Mesh Dimension
	size_type MeshDimension;
	//!Mesh Element Dimension
	size_type ElementDimension;

	//Material Parameters
	//!Lambda Lamé Coefficient
	scalar_type Lambda;
	//!Mu Lamé Coefficient
	scalar_type Mu;
	//!Density
	scalar_type Rho;
	//!Contact Point Mass
	size_type ContactMass;
	//!Augmentation
	scalar_type R;

	//Environment Parameters
	//!Gravity Coefficient (mm/s^2)
	scalar_type PG;
	//!Friction Coefficient
	scalar_type Friction_coef;

	//Solver Parameters
	//!Residual Error for iterative solver
	scalar_type Residual;
	//!DeltaT
	scalar_type DeltaT;

	//Time Scheme Parameters
	//!Solver Time Scheme
	/*!
	 * 0 = theta method
	 * 1 = Newmark
	 * 2 = midpoint
	 * 3 = midpoint + separate contact forces
	 * */
	size_type Scheme;
	//!Theta value.
	/*!
	 * Only applies if theta method is selected
	 */
	scalar_type Theta;
	//!Beta value.
	/*!
	 * Only applies if Newmark method is selected
	 */
	scalar_type Beta;
	//!Gamma value.
	/*!
	 * Only applies if Newmark method is selected
	 */
	scalar_type Gamma;
	//!Restitution parameter
	scalar_type Restitution;



private:
	vtkGetFEMInterface(const vtkGetFEMInterface&);            // Not implemented.
	void operator=(const vtkGetFEMInterface&);           // Not implemented.

	void GenerateTopologyMesh();
	void GenerateFEMMesh();
};

#endif

