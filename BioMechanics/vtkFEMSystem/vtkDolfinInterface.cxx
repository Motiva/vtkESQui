#include "vtkDolfinInterface.h"

#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"
#include "FERegion.h"


vtkCxxRevisionMacro(vtkDolfinInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkDolfinInterface);

//----------------------------------------------------------------------------
vtkDolfinInterface::vtkDolfinInterface()
{

}

//----------------------------------------------------------------------------
vtkDolfinInterface::~vtkDolfinInterface()
{

}

void vtkDolfinInterface::Init()
{
  //Parameters
  dolfin::parameters["mesh_partitioner"] = "SCOTCH";

  // Load tetrahedral mesh from file
  this->FEMesh = new dolfin::Mesh(this->FileName);

  std::cout << this->FEMesh->str(1) << endl;

  //Order FE Mesh
  this->FEMesh->order();

  //Create function space
  Elasticity::FunctionSpace V(*(this->FEMesh));

  // Create right hand side function (constant zero force)
  dolfin::Constant f (0,0,0);

  //Set up boundary conditions on region 0 (static)
  FERegion left;

  //Set up boundary conditions on region 1

  //Collect boundary condtions

}

//----------------------------------------------------------------------------
void vtkDolfinInterface::Write()
{
  //TODO: Create an Importer/Exporter of getFEM mesh file format
}

//----------------------------------------------------------------------------
void vtkDolfinInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
