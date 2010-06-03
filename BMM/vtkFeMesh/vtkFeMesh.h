#ifndef __vtkFeMesh_h
#define __vtkFeMesh_h

#include "vtkFeMeshConfigure.h" // Include configuration header.
#include "vtkObject.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkUnstructuredGridToFEMMesh.h"

class VTK_vtkFeMesh_EXPORT vtkFeMesh : public vtkObject
{
public:
  
	//typedef itkMeshToFEMMesh <MeshType> itkToFEM;
	
	static vtkFeMesh* New();
  vtkTypeRevisionMacro(vtkFeMesh, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

	void SetInput(vtkUnstructuredGrid * data);

	vtkUnstructuredGrid* GetOutput();

	void Update();

	void SetExternalForces(vtkIdList * cells, vtkDoubleArray * forces);

	void SetBoundaryConditions(vtkIdList * cells, vtkDoubleArray * dofs);
	
protected:
  vtkFeMesh();
  ~vtkFeMesh();

private:
  vtkFeMesh(const vtkFeMesh&);  // Not implemented.
  void operator=(const vtkFeMesh&);  // Not implemented.
	
	void InitSolver();
	
	//void ApplyBoundaryConditions();

	//void ApplyForces();

	vtkUnstructuredGridToFEMMesh * vtk2fem;

	vtkUnstructuredGrid * grid;

	SolverType * solver;

};

#endif
