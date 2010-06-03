#include "vtkUnstructuredGridToFEMMesh.h"

#include "vtkObjectFactory.h"


vtkCxxRevisionMacro(vtkUnstructuredGridToFEMMesh, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkUnstructuredGridToFEMMesh);

//----------------------------------------------------------------------------
vtkUnstructuredGridToFEMMesh::vtkUnstructuredGridToFEMMesh()
{
	///Constructor. All required objects are created.
	this->itkMesh = MeshType::New();
	this->solver = new SolverType;
}

//----------------------------------------------------------------------------
vtkUnstructuredGridToFEMMesh::~vtkUnstructuredGridToFEMMesh()
{
	///Destructor. Created objects are purged.
	delete this->solver;
	//MeshType Object is not deleted because it is a smartPointer.
}

//----------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::SetFileName(const char* file)
{
	///Input Data can be assigned as an vtk input file.
	
	//this->reader->SetFileName(file);
	//this->reader->Update();
	//this->SetInputData(this->reader->GetOutput());
}

//----------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::SetInput(vtkUnstructuredGrid * data)
{
	/// Input Data assigned as a vtkUnstructuredGrid object
	this->SetInputData(data);
}

//----------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::SetInputData(vtkUnstructuredGrid * data)
{
	///Grid object is assigned either by SetInput or SetFileName method.
	std::cout << "[vtkUnstructuredGridToFEMMesh] Input data # points: " << data->GetNumberOfPoints() << "\n";
	std::cout << "[vtkUnstructuredGridToFEMMesh] Input data # cells: " << data->GetNumberOfCells() << "\n";
	this->grid = data;
}

//----------------------------------------------------------------------------
SolverType * vtkUnstructuredGridToFEMMesh::GetOutput()
{
	///Return FEM solver
	return this->solver;
}

//----------------------------------------------------------------------------

void vtkUnstructuredGridToFEMMesh::Update()
{
	///Conversion sequence required to perform transalation.
	std::cout << "Begin Conversion vtk -> itkmesh"  << std::endl;
	MeshFromUnstructuredGrid(this->grid);
	std::cout << "End of Conversion vtk -> itkmesh" << std::endl;
	
	std::cout << "Begin Conversion itkmesh -> FEMmesh"  << std::endl;
	FEMSolverFromMesh(this->itkMesh);
	std::cout << "End of Conversion itk -> FEMmesh" << std::endl;
	
}

//-------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::MeshFromUnstructuredGrid(vtkUnstructuredGrid* grid)
{
	/// An itk mesh object is obtained by converting vtkUnstructuredGrid
	/// data information to itk	mesh object data types.

	//Get the points from vtk
	vtkPoints * vtkpoints = grid->GetPoints();
	vtkIdType numberOfPoints = vtkpoints->GetNumberOfPoints();

	//Create a compatible point container for the mesh
	MeshType::PointsContainer::Pointer points = 
	MeshType::PointsContainer::New();

	//Reserve points space
	points->Reserve(numberOfPoints);
	
	//Assign point container to the mesh
	this->itkMesh->SetPoints(points);
	for (int i = 0; i < numberOfPoints; i++)
	{
		vtkFloatingPointType * apoints = vtkpoints->GetPoint(i);
		this->itkMesh->SetPoint(i, MeshType::PointType(apoints));
	}

	//Get cells from the mesh
	vtkCellArray * vtkcells = grid->GetCells();
	vtkIdType numberOfCells = vtkcells->GetNumberOfCells();

	//Create a compatible cell container for the mesh
	MeshType::CellsContainerPointer cells = 
	MeshType::CellsContainer::New();
	this->itkMesh->SetCells(cells);
	
	//Reserve cells space
	cells->Reserve(numberOfCells);

	//Set Cells in new mesh
	vtkIdType cellId = 0;
	vtkIdType cellType;
	vtkIdType cellNumberOfPoints;
	vtkIdType* cellPoints;

	//Cells iterator is initialized
	vtkcells->InitTraversal();
	while(vtkcells->GetNextCell(cellNumberOfPoints, cellPoints))
	{
		MeshType::CellAutoPointer c;
		cellType = grid->GetCellType(cellId);
		switch(cellType)
		{
			case VTK_TRIANGLE:
			{
				TriangleCellType * t = new TriangleCellType;
				t->SetPointIds((unsigned long*)cellPoints);
				c.TakeOwnership( t );
				break;
			}
			case VTK_QUAD:
			{
				QuadrilateralCellType * t = new QuadrilateralCellType;
				t->SetPointIds((unsigned long*)cellPoints);
				c.TakeOwnership( t );
				break;
			}
			case VTK_TETRA:
			{
				TetrahedronCellType * t = new TetrahedronCellType;
				t->SetPointIds((unsigned long*)cellPoints);
				c.TakeOwnership( t );
				break;
			}
			default:
				std::cerr << "Warning. Cell Type not supported" << std::endl;
		}
		this->itkMesh->SetCell(cellId,c);
		cellId++;
	}
}

//-------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::FEMSolverFromMesh(MeshType::Pointer itkmesh)
{
	/// Obtains the FEM Solver from the ITK Mesh object

	//Clear solver from previous executions
	this->solver->Clear();

	//TODO: Read Material Properties from SRML File
	// Create dummy material
	MaterialType::Pointer mat = MaterialType::New();
	mat->GN = 0;
	mat->E = 20;
	mat->A = 1.0;
	mat->h = 1.0;
	mat->I = 1.0;
	mat->nu = 0;
	mat->RhoC = 1.0;

	// Create element type
	ElementType::Pointer e = ElementType::New();
	e->m_mat = dynamic_cast<MaterialType*>( mat );

	// Convert mesh points to FEM nodes
	PointType* ptr;
	PointType pt;
	ptr = &pt;

	for (unsigned int k=0; k < itkmesh->GetNumberOfPoints(); k++) 
	{
		itkmesh->GetPoint(k, ptr);
		NodeType::Pointer nd;
		nd = new NodeType(pt[0], pt[1], pt[2]);
		nd->GN = k;
		this->solver->node.push_back(itk::fem::FEMP<NodeType>(*&nd));
	}

	// Convert mesh cells to FEM Solver elements
	CellsContainerPointer cellList = itkmesh->GetCells();
	CellsContainerIterator cells = cellList->Begin();
	ElementType::Pointer e2;

	for (unsigned int k=0; k < itkmesh->GetNumberOfCells(); k++) 
	{
		//TODO: Conversion is currently working for Tetrahedron cell-based 
		//meshes. A further implementation could be extended with new 
		//geometries (Triangle, Hexaedron, etc).

		//Check cell Geometry in order to assign
		const unsigned long *tp;
		CellType* cellPtr = cells.Value();
		CellType::CellGeometry cellGeo = cells.Value()->GetType();
		tp = cellPtr->GetPointIds();
		cells++;
	
		NeighborSet cellSet;	

//		std::cout << "Neighbors: " << itkmesh->GetCellBoundaryFeatureNeighbors(0,k, &cellSet) << "\n";

		e2 = dynamic_cast<ElementType*> (e->Clone());
		e2->SetNode(0, this->solver->node.Find( tp[0] ));
		e2->SetNode(1, this->solver->node.Find( tp[1] ));
		e2->SetNode(2, this->solver->node.Find( tp[2] ));
		
		if(cellGeo == CellType::TETRAHEDRON_CELL)
		{
			e2->SetNode(3, this->solver->node.Find( tp[3] ));
		}
		
		e2->GN = k;
		this->solver->el.push_back(itk::fem::FEMP<itk::fem::Element>(*&e2));
	}

}

//-------------------------------------------------------------------------
vtkUnstructuredGrid* vtkUnstructuredGridToFEMMesh::MeshToUnstructuredGrid(MeshType::Pointer mesh)
{
	///Inverted Conversion is made from itk Mesh to vtkUnstructuredGrid

	//TODO: Finish transform inversion method. Now inversion is done by asigning points + displacements to the existing grid.
	vtkIdType numberOfPoints =  mesh->GetNumberOfPoints();
				  
	if (numberOfPoints == 0)
	{
		mesh->Print(std::cerr);
		printf( "Aborting: No Points in GRID\n");
		exit(-1); 
	}

	//Create a vtkUnstructured Grid
	vtkUnstructuredGrid* vtkgrid = vtkUnstructuredGrid::New();

	//Create the vtkPoints and set number of points
	vtkPoints * vtkpoints = vtkPoints::New();
	vtkpoints->SetNumberOfPoints(numberOfPoints);

	//Iterate over the mesh points and store them in the vtkpoints object
	MeshType::PointsContainer::Pointer points = mesh->GetPoints();
	for(MeshType::PointsContainer::Iterator i = points->Begin(); i != points->End(); i++)
	{
		//Retrieve the point index
		int idx = i->Index();
		//Store the point in the vtkpoints object. ITK returns a const pointer, but vtk does not, so a const_cast is used
		vtkpoints->SetPoint(idx, const_cast<vtkFloatingPointType*>(i->Value().GetDataPointer()));
	}
	vtkgrid->SetPoints(vtkpoints);

	//Next step is to insert the cells

	return vtkgrid;
}

//----------------------------------------------------------------------------
void vtkUnstructuredGridToFEMMesh::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
