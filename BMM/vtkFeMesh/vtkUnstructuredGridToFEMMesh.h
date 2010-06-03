#ifndef __vtkUnstructuredGridToFEMMesh_h
#define __vtkUnstructuredGridToFEMMesh_h

#include "vtkObject.h"
#include "vtkFeMeshWin32Header.h"

//
// ITK Headers
// 
#include "itkMesh.h"
#include "itkTriangleCell.h"
#include "itkQuadrilateralCell.h"
#include "itkTetrahedronCell.h"
#include "itkCellInterface.h"
#include "itkFEM.h"
#include "itkFEMLinearSystemWrapperVNL.h"
#include "itkAutomaticTopologyMeshSource.h"

//
// VTK headers
//
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"

// Code extracted from InsightApplications/Auxiliary/vtk
// This define is needed to deal with double/float changes in VTK
#ifndef vtkFloatingPointType
#define vtkFloatingPointType float
#endif

//Point & Cell Dimensions for tetrahedral mesh
const unsigned int PointDimension   = 3;
const unsigned int MaxCellDimension = 3;

//Type Definitions

//ITKMesh Type Definitions
typedef itk::DefaultStaticMeshTraits<
                      vtkFloatingPointType,
                      PointDimension,
                      MaxCellDimension,
                      vtkFloatingPointType,
                      vtkFloatingPointType  >       MeshTraits;
 
typedef itk::Mesh<
                      vtkFloatingPointType,
                      PointDimension,
                      MeshTraits              >     MeshType;


typedef MeshType::Pointer                        MeshTypePointer;
typedef MeshType::CellType                       CellType;
typedef MeshType::PointType                      PointType;
typedef MeshType::CellPixelType                  CellPixelType;
typedef MeshType::CellsContainerPointer          CellsContainerPointer;
typedef MeshType::CellsContainerIterator         CellsContainerIterator;

typedef itk::AutomaticTopologyMeshSource< MeshType >   MeshSourceType;
typedef MeshSourceType::IdentifierType 								 IdentifierType;
typedef std::set<IdentifierType>       					 NeighborSet;


//FEM Solver Type Definitions
typedef itk::fem::Solver                                  SolverType;
typedef SolverType*                                       SolverPointerType;
typedef itk::fem::MaterialLinearElasticity                MaterialType;
typedef itk::fem::Element3DC0LinearTetrahedronStrain      ElementType;
typedef ElementType::Node                           	NodeType;

//Triangle Cell Type Definition
typedef itk::CellInterface<vtkFloatingPointType, MeshType::CellTraits> CellInterfaceType;
typedef itk::TriangleCell<CellInterfaceType> TriangleCellType;

//Quadrilateral Cell Type Definitions
typedef itk::CellInterface<vtkFloatingPointType, MeshType::CellTraits> CellInterfaceType;
typedef itk::QuadrilateralCell<CellInterfaceType> QuadrilateralCellType;

//Tetrahedra Cell Type Definitions
typedef itk::CellInterface<vtkFloatingPointType, MeshType::CellTraits> CellInterfaceType;
typedef itk::TetrahedronCell<CellInterfaceType> TetrahedronCellType;

typedef vnl_vector<double> DoubleVectorType;

class VTK_vtkFeMesh_EXPORT vtkUnstructuredGridToFEMMesh : public vtkObject
{
public:
	
	static vtkUnstructuredGridToFEMMesh* New();
  vtkTypeRevisionMacro(vtkUnstructuredGridToFEMMesh, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

	void SetInput(vtkUnstructuredGrid* data);
	void SetFileName(const char *);
	SolverType* GetOutput();
	void Update();

protected:
  vtkUnstructuredGridToFEMMesh();
  ~vtkUnstructuredGridToFEMMesh();

private:
  vtkUnstructuredGridToFEMMesh(const vtkUnstructuredGridToFEMMesh&);  // Not implemented.
  void operator=(const vtkUnstructuredGridToFEMMesh&);  // Not implemented.

	void SetInputData(vtkUnstructuredGrid* data);

	void MeshFromUnstructuredGrid(vtkUnstructuredGrid* grid);

	void FEMSolverFromMesh(MeshType::Pointer itkmesh);

	vtkUnstructuredGrid* MeshToUnstructuredGrid(MeshType::Pointer grid);

	MeshType::Pointer itkMesh;
	SolverPointerType solver;
	vtkUnstructuredGrid * grid;

};

#endif
