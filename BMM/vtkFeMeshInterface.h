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
#ifndef __vtkFeMeshInterface_h
#define __vtkFeMeshInterface_h

#include "vtkESQuiBMMWin32Header.h"

#include "vtkBioMechanicalModel.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkIdList.h"
#include "vtkSphere.h"
#include "vtkExtractGeometry.h"
#include "vtkExtractPolyDataGeometry.h"
#include "vtkPointLocator.h"

#include "vtkFeMesh.h"
#include "vtkContact.h"
#include "vtkContactCollection.h"

//! Interface to the abstract vtkBiomechanicalModel class for a FEM based system

class VTK_ESQUI_BMM_EXPORT vtkFeMeshInterface : public vtkBioMechanicalModel
{
public:
  
	vtkTypeRevisionMacro(vtkFeMeshInterface, vtkBioMechanicalModel);
	static vtkFeMeshInterface* New();
	const char *GetClassName() {return "vtkFeMeshInterface";};
	//! Print class object values
	void PrintSelf(ostream& os, vtkIndent indent);
	
	//!Set input data (organ mesh) to the FEM-based deformation model
	/*!
	* A vtkUnstructuredGrid is required due to 3D deformation
	* \param data vtkUnstructured 3d organ mesh data
	*/
	void SetInput(vtkUnstructuredGrid * data);

	//!Perform an update of the FEM-based deformation model
	/*!
	 * Once collision detection process has been performed, if any contact between a tool and organ has occured the organ must be updated, so FEM system.
	 */
	void Update();

	//!Initialize the Biomechanical Model
	virtual void Init();

	//! Return updated organ mesh (data) after updating the biomechanical model
	virtual vtkUnstructuredGrid * GetOutput();

protected:
  vtkFeMeshInterface();
  ~vtkFeMeshInterface();

private:
  vtkFeMeshInterface(const vtkFeMeshInterface&);  // Not implemented.
  void operator=(const vtkFeMeshInterface&);  // Not implemented.

  //! Clips mesh to extract the ROI, region of interest (an sphere around the contact point)
  void ClipMesh();

  //! In order to use the itk::fem package an itk tetrahedral mesh is required.
  void GenerateTetrahedralMesh();

  //! Synchronize point ids between itk and vtk meshes
  void ComputePointIds();

  //! Synchronize point cells between itk and vtk meshes
  void ComputePointCells();

  //! Synchronize point ids between itk and vtk meshes
  void ComputeBoundCells();

  //! Reconstruction of the original unstructured grid mesh after fem deformation process.
  void RebuildMesh();

  //3D Surface Input
  vtkUnstructuredGrid * ClippedMesh;

  //! Mesh bounds
  vtkPoints * Bounds;
  //! Mesh cell bounds
  vtkIdList * BoundCells;
  //! Mesh cell degrees of freedom
  vtkDoubleArray * Dofs;

  //Array used to synchronize point Ids (i,j) where:
  // 	* i: original mesh point Id,
  //	* j: deformed (clipped) mesh point Id,
  vtkIntArray * SyncIds;

  //! Biomechanical model mesh
  vtkFeMesh * FeMesh;

  //! Updated flag
  vtkIdType updated;

};

#endif
