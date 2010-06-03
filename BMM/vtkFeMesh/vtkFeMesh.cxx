#include "vtkFeMesh.h"

//Debug Purposes
#include "itkFEMLoadBC.h"
#include "itkFEMLoadNode.h"
#include "itkFEMLinearSystemWrapperItpack.h"

#include "vtkObjectFactory.h"
#include "vtkTimerLog.h"

vtkCxxRevisionMacro(vtkFeMesh, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkFeMesh);

//----------------------------------------------------------------------------
vtkFeMesh::vtkFeMesh()
{
	this->vtk2fem = vtkUnstructuredGridToFEMMesh::New();
	this->grid = vtkUnstructuredGrid::New();
}

//----------------------------------------------------------------------------
vtkFeMesh::~vtkFeMesh()
{
	if(this->vtk2fem != NULL) this->vtk2fem->Delete();
	if(this->grid != NULL) this->grid->Delete();
}

//----------------------------------------------------------------------------
void vtkFeMesh::SetInput(vtkUnstructuredGrid * data)
{
	///Set input mesh as a vtkUnstructuredGrid object
	std::cout << GetClassName() << "::SetInput" << std::endl;

	//Create a local copy of the input data
	this->grid->DeepCopy(data);
	this->grid->Update();

	//Set FEM solver input data
	this->vtk2fem->SetInput(this->grid);
	this->vtk2fem->Update();

	//Retrieve FEM solver
	this->solver = this->vtk2fem->GetOutput();
	this->InitSolver();
}

//----------------------------------------------------------------------------
vtkUnstructuredGrid * vtkFeMesh::GetOutput()
{	
	///Return the deformed mesh as a vtkUnstrcturedGrid object

	vtkIdType pointId;
	vtkPoints * oldPoints = this->grid->GetPoints();
	vtkPoints * points = vtkPoints::New();
	points->SetNumberOfPoints(oldPoints->GetNumberOfPoints());
	
 	//Update displacements of all nodes in a system;
	//std::cout<<"\nNodal displacements:\n";
	for( ::itk::fem::Solver::NodeArray::iterator n = this->solver->node.begin(); n!=this->solver->node.end(); n++)
	{
		pointId = (*n)->GN;
		//std::cout<<"Node#: "<< pointId  <<": ";
		double * point = oldPoints->GetPoint(pointId);
		
		// For each DOF in the node... 
		for( unsigned int d=0, dof; (dof=(*n)->GetDegreeOfFreedom(d))!=::itk::fem::Element::InvalidDegreeOfFreedomID; d++ )
		{
			point[d] += this->solver->GetSolution(dof);
			//std::cout<< point[d];
			//std::cout<<",  ";
		}
		points->SetPoint(pointId, point);
		//std::cout<<"\b\b\b \b\n";
	}
	//cout<<"\n";
	
	this->grid->SetPoints(points);
	this->grid->BuildLinks();
	return this->grid;
}

//----------------------------------------------------------------------------
void vtkFeMesh::Update()
{	
	vtkTimerLog * timer = vtkTimerLog::New();

	timer->StartTimer();
	// Assemble the master force vector (from the applied loads).
	this->solver->AssembleF();
	timer->StopTimer();

	std::cout << "AssembleF: " << 1/timer->GetElapsedTime() << " Hz\n";

	timer->StartTimer();
	// Solve the system of equations for displacements (u=K^-1*F)
	this->solver->Solve();
	timer->StopTimer();

	std::cout << "Solve: " << 1/timer->GetElapsedTime() << " Hz\n";
}

//----------------------------------------------------------------------------
void vtkFeMesh::SetExternalForces(vtkIdList * cells, vtkDoubleArray * forces)
{
	///Set External Forces induced by the contacts
	DoubleVectorType uptCoords;
	itk::fem::LoadNode::Pointer ln;
	vtkIdType cellId;
	double * force;

	std::cout	<< "[vtkFeMesh] SetExternalForces\n";

	for(vtkIdType i = 0; i < cells->GetNumberOfIds(); i++)
	{
		cellId = cells->GetId(i);
		force = forces->GetTuple(i);
		std::cout << "cellId: " << cellId << " | force " << force[0] << "|"  << force[1] << "|" << force[2] << "\n";
		ln = itk::fem::LoadNode::New();
		ln->m_element = this->solver->el.Find(cellId);

		unsigned int Npts=ln->m_element->GetNumberOfNodes();
		std::cout << "GN: " << ln->m_element->GN << "\n";
		for(unsigned int ptCount = 0; ptCount < Npts; ptCount++)
		{
			uptCoords = ln->m_element->GetNode(ptCount)->GetCoordinates();
			std::cout << "pointId[" << ptCount << "]: " << uptCoords[0] << " , "<< uptCoords[1]<< " , " << uptCoords[2] <<std::endl;
		}
		
		ln->m_pt = 1;
		ln->F = vnl_vector<double>(3);
		ln->F[0] = force[0];
		ln->F[1] = force[1];
		ln->F[2] = force[2];
		this->solver->load.push_back(itk::fem::FEMP<itk::fem::Load>(*&ln));
	}
	
}

//----------------------------------------------------------------------------
void vtkFeMesh::InitSolver()
{
	/// FEM Solver initialization with default values.
	//itk::fem::LinearSystemWrapperVNL * ls = new itk::fem::LinearSystemWrapperVNL;
	//ls->SetMaximumNonZeroValuesInMatrix(1000,1000);
	//this->solver->SetLinearSystemWrapper(ls);
	//this->solver->SetTimeStep(5E-6);

	this->solver->InitializeLinearSystemWrapper();
	std::cout << "after InitializeLinearSystemWrapper" << std::endl;

 	// Assign a unique id (global freedom number - GFN)
 	// to every degree of freedom (DOF) in a system.
	this->solver->GenerateGFN();
	std::cout << "after GenerateGFN" << std::endl;
	
	// Assemble the master stiffness matrix. In order to do this
	// the GFN's should already be assigned to every DOF.
	this->solver->AssembleK();
	std::cout << "after AssembleK" << std::endl;
	
	// Perform any preprocessing on the master stiffness matrix.
	this->solver->DecomposeK();
	std::cout << "after GenerateGFN" << std::endl;

}
//----------------------------------------------------------------------------
void vtkFeMesh::SetBoundaryConditions(vtkIdList * cells, vtkDoubleArray * dofs)
{

	///Set Boundary Conditions and its dofs
	itk::fem::Element::Pointer e;
	vtkIdType cellId;
	double dof[3];

	std::cout	<< "[vtkFeMesh] SetBoundaryConditions\n";

	for(vtkIdType i = 0; i < cells->GetNumberOfIds(); i++)
	{
		cellId = cells->GetId(i);
		dofs->GetTupleValue(i, dof);
		e = solver->el.Find(cellId);

		//std::cout << "cellId (" << cellId  << ") DOFs: " << e->GetNumberOfDegreesOfFreedom() << "\n";
		for(vtkIdType j = 0; j < 4; j++)
		{
			// Set up the first degree of freedom (DOF) at the specified node with the m_value=0.0 (Fixed Node at X Direction). Note that the m_value is a vector
			itk::fem::LoadBC::Pointer bc1 = itk::fem::LoadBC::New();
			bc1->m_element = e;
			bc1->m_dof = 3*j;
			bc1->m_value.set_size(1);
			bc1->m_value = dof[0];
			//bc1->m_value = vnl_vector<double>(1);
			//bc1->m_value[0] = 0.0;
			this->solver->load.push_back( itk::fem::FEMP<itk::fem::Load>(*&bc1));
	
			// Set up the second DOF at the specified node with the m_value=dof[1] (Free Node at Y Direction). Note that the m_value is a vector
			itk::fem::LoadBC::Pointer bc2 = itk::fem::LoadBC::New();
			bc2->m_element = e;
			bc2->m_dof = 3*j+1;
			bc2->m_value.set_size(1);
			bc2->m_value = dof[1];
			this->solver->load.push_back(itk::fem::FEMP<itk::fem::Load>(*&bc2));

			// Set up the third DOF at the specified node with the m_value=dof[2] (Free Node at Y Direction). Note that the m_value is a vector
			itk::fem::LoadBC::Pointer bc3 = itk::fem::LoadBC::New();
			bc3->m_element = e;
			bc3->m_dof = 3*j+2;
			bc3->m_value.set_size(1);
			bc3->m_value = dof[2];
			this->solver->load.push_back(itk::fem::FEMP<itk::fem::Load>(*&bc3));
		}
	}
}

//----------------------------------------------------------------------------
void vtkFeMesh::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
