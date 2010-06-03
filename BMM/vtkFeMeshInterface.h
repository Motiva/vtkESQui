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

//! Implementation of the generic Biomechanical Model interface for a Finite Element Model based deformation system

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
