#include "vtkGetFEMInterface.h"

#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"


vtkCxxRevisionMacro(vtkGetFEMInterface, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkGetFEMInterface);

//----------------------------------------------------------------------------
vtkGetFEMInterface::vtkGetFEMInterface()
{
}

//----------------------------------------------------------------------------
vtkGetFEMInterface::~vtkGetFEMInterface()
{
}

void vtkGetFEMInterface::SetPolyData(vtkPolyData * data)
{
  this->Data = data;
}

void vtkGetFEMInterface::SelectBoundaries()
{
  size_type N = this->Mesh->dim();
  getfem::mesh_region border_faces;
  getfem::outer_faces_of_mesh(*(this->Mesh), border_faces);
  for (getfem::mr_visitor i(border_faces); !i.finished(); ++i) {
    base_node un = this->Mesh->normal_of_face_of_convex(i.cv(), i.f());
    un /= gmm::vect_norm2(un);
    if (gmm::abs(un[N-1] - 1.0) < 0.5)
    {
      cout << "Contact Point: "<< i.cv()  <<"\n";
      this->Mesh->region(CONTACT_BOUNDARY).add(i.cv(), i.f());
    }
    else
    {
      if(this->Dirichlet)
      {
        this->Mesh->region(DIRICHLET_BOUNDARY).add(i.cv(), i.f());
      }
    }
  }
  std::cout << "CONTACT: " << this->Mesh->region(CONTACT_BOUNDARY).nb_convex() << std::endl;
  std::cout << "DIRICHLET: " << this->Mesh->region(DIRICHLET_BOUNDARY).nb_convex() << std::endl;
}

void vtkGetFEMInterface::Init()
{
  //Create new mesh object
  this->Mesh = new getfem::mesh();

  //Default parameters
  std::string MESH_FILE = "structured:GT=\"GT_PK(3,1)\";SIZES=[1,1,1];NOISED=0";
  std::string FEM_TYPE  = "FEM_PK(3,2)";
  std::string FEM_TYPE_CONT  = "FEM_PK(3,1)";
  std::string INTEGRATION = "IM_TETRAHEDRON(6)";

  cout << "MESH_FILE=" << MESH_FILE << "\n";
  cout << "FEM_TYPE="  << FEM_TYPE << "\n";
  cout << "INTEGRATION=" << INTEGRATION << "\n";

  size_type NX = 10;
  size_type N = 3;

  if(this->FileName)
  {
    std::cout << this->FileName << std::endl;
    getfem::import_mesh(this->FileName,"gmsh",*(this->Mesh));
  }
  else
  {
    std::stringstream filename; filename << MESH_FILE;
    if ((MESH_FILE.compare(0,11,"structured:") == 0) && NX > 0) {
      filename << ";NSUBDIV=[" << NX;
      for (size_type i = 1; i < N; ++i) filename << "," << NX;
      filename << "];";
    }
    getfem::import_mesh(filename.str(), *(this->Mesh));
  }

  this->MeshDimension = this->Mesh->dim();
  this->Mesh->optimize_structure();

  std::cout << "Mesh Dimension:" << static_cast<int>(this->Mesh->dim()) << "\n";
  std::cout << "Mesh nb points:" << this->Mesh->nb_points() << "\n";
  std::cout << "Mesh nb convex:" << this->Mesh->nb_convex() << "\n";

  //Init MF_U, MF_RHS and MF_VM
  this->MF_U = new getfem::mesh_fem(*(this->Mesh));
  this->MF_VM = new getfem::mesh_fem(*(this->Mesh));
  this->MF_RHS = new getfem::mesh_fem(*(this->Mesh));
  this->MF_CONT = new getfem::mesh_fem(*(this->Mesh));

  this->MF_U->set_qdim(dim_type(N));
  this->MF_VM->set_qdim(dim_type(N));

  //Init the integration method
  this->MIM = new getfem::mesh_im(*(this->Mesh));
  getfem::pfem pf_u = getfem::fem_descriptor(FEM_TYPE);
  getfem::pfem pf_mult_cont = getfem::fem_descriptor(FEM_TYPE_CONT);
  getfem::pintegration_method ppi = getfem::int_method_descriptor(INTEGRATION);

  this->MIM->set_integration_method(this->Mesh->convex_index(), ppi);

  //Set the finite element on the solution
  this->MF_U->set_finite_element(this->Mesh->convex_index(), pf_u);

  //Set the finite element on the VonMisses stress mesh
  this->MF_VM->set_classical_discontinuous_finite_element(1);

  //Set the finite element on the contact multiplier
  this->MF_CONT->set_finite_element(this->Mesh->convex_index(), pf_mult_cont);

  //Set the finite element on rhs
  if(!pf_u->is_lagrange())
  {
    exit(0);
  }
  else
  {
    this->MF_RHS->set_finite_element(this->Mesh->convex_index(), pf_u);
  }

  //Set boundary conditions
  this->SelectBoundaries();

  //Init DOFs
  this->MF_U->nb_dof();
  this->MF_RHS->nb_dof();
  this->MF_VM->nb_dof();

  //Init FEM Model
  this->Model = new getfem::model();

  std::cout << "FEM Elastodynamic problem has been correctly initialized\n";
}

//----------------------------------------------------------------------------
void vtkGetFEMInterface::Solve()
{
  dal::bit_vector transient_bricks;

  bgeot::size_type N = this->Mesh->dim();
  bgeot::size_type numberOfDOFsRHS = this->MF_RHS->nb_dof();

  GMM_ASSERT1(!this->MF_U->is_reduced(), "To be adapted");
  GMM_ASSERT1(!this->MF_RHS->is_reduced(), "To be adapted");

  size_type ref_dof = 0;
  for (size_type i = 1; i < this->MF_U->nb_dof(); ++i)
    if (this->MF_U->point_of_basic_dof(i)[N-1] < this->MF_U->point_of_basic_dof(ref_dof)[N-1]) ref_dof = i;
  cout << "ref_dof selected : " << this->MF_U->point_of_basic_dof(ref_dof) << endl;

  // Main unknown of the problem
  this->Model->add_fem_variable("u",*(this->MF_U), 2);

  //Linear elasticity brick
  //Lame parameters (lambda & mu)
  this->Model->add_initialized_scalar_data("lambda", this->Lambda);
  this->Model->add_initialized_scalar_data("mu", this->Mu);

  //Add a linear elasticity brick to the model
  transient_bricks.add(getfem::add_isotropic_linearized_elasticity_brick(*(this->Model), *(this->MIM), "u", "lambda", "mu"));

  // Volumic source term.
  plain_vector F(numberOfDOFsRHS * N);
  plain_vector f(N); f[N-1] = -(this->Rho*this->PG); //Set Gravity on Z
  for (size_type i = 0; i < numberOfDOFsRHS; ++i) {
    gmm::copy(f, gmm::sub_vector(F, gmm::sub_interval(i*N, N)));
  }

  this->Model->add_initialized_fem_data("VolumicData", *(this->MF_RHS), F);
  getfem::add_source_term_brick(*(this->Model), *(this->MIM), "u", "VolumicData");

  //Dirichlet condition
  gmm::clear(F);
  if(this->Dirichlet)
  {
    for (size_type i = 0; i < numberOfDOFsRHS; ++i){
      F[(i+1)*N-1] = this->DirichletRatio * this->MF_RHS->point_of_basic_dof(i)[N-1];
    }
    this->Model->add_initialized_fem_data("DirichletData", *(this->MF_RHS), F);
    getfem::add_Dirichlet_condition_with_multipliers(*(this->Model), *(this->MIM), "u", *(this->MF_U), DIRICHLET_BOUNDARY, "DirichletData");
  }

  //Contact Condition
  dal::bit_vector cn = this->MF_U->basic_dof_on_region(CONTACT_BOUNDARY);
  cout << "cn = " << cn << endl;
  getfem::CONTACT_B_MATRIX BN(cn.card()/N, this->MF_U->nb_dof());
  //sparse_matrix BT((N-1)*cn.card()/N, this->MF_U.nb_dof());
  plain_vector gap(cn.card()/N);
  size_type jj = 0;
  for (dal::bv_visitor i(cn); !i.finished(); ++i)
  {
    if (i % N == 0) {
      BN(jj, i+N-1) = -1.;
      gap[jj] = this->MF_U->point_of_basic_dof(i)[N-1];
      //for (size_type k = 0; k < N-1; ++k) BT((N-1)*jj+k, i+k) = 1.;
      ++jj;
    }
  }

  // Contact multiplier
  this->Model->add_fem_variable("Mult", *(this->MF_CONT));
  this->Model->add_initialized_fixed_size_data("Gap", gap);
  this->Model->add_initialized_scalar_data("r", this->R); //Augmentation Parameter
  getfem::add_basic_contact_brick(*(this->Model),"u", "Mult", "r", BN, "Gap");

  //Transient
  this->Model->add_fem_data("v", *(this->MF_U), 1, 2);
  this->Model->add_initialized_scalar_data("dt", this->TimeStep);
  size_type ibddt;
  scalar_type alpha;

  switch (this->Scheme) {
  case 0://Theta
    this->Model->add_initialized_scalar_data("theta", this->Theta);
    ibddt = getfem::add_basic_d2_on_dt2_brick(*(this->Model), *(this->MIM), "u", "v", "dt", "theta");
    getfem::add_theta_method_dispatcher(*(this->Model), transient_bricks, "theta");
    alpha = this->Theta;
    break;
  case 1://Newmark
    this->Model->add_initialized_scalar_data("beta", this->Beta);
    this->Model->add_initialized_scalar_data("gamma", this->Gamma);
    ibddt = getfem::add_basic_d2_on_dt2_brick(*(this->Model), *(this->MIM), "u", "v", "dt", "twobeta");
    getfem::add_theta_method_dispatcher(*(this->Model), transient_bricks, "twobeta");
    alpha = 2.*this->Beta;
    break;
  case 2://Midpoint
    this->Model->add_initialized_scalar_data("alpha", 0.5);
    ibddt = getfem::add_basic_d2_on_dt2_brick(*(this->Model), *(this->MIM), "u", "v", "dt", "alpha");
    getfem::add_midpoint_dispatcher(*(this->Model), transient_bricks);
    alpha = 0.5;
    break;
  default:
    break;
  }

  gmm::iteration iter(this->Residual, 0, 40000);

  this->Model->first_iter();

  // Initial conditions.
  gmm::resize(this->U, this->MF_U->nb_dof());
  gmm::clear(this->U);
  gmm::copy(this->U, this->Model->set_real_variable("u", 1));
  //getfem::interpolation_function(this->MR_RHS, U, sol_dtu);
  gmm::copy(this->U, this->Model->set_real_variable("v", 1));

  cout << "MF_CONT: " << this->MF_CONT->nb_dof() << endl;
  cout << "MF_U: " << this->MF_U->nb_dof() << endl;

  int T = 2;
  scalar_type sol_t = 0;
  for (scalar_type t = 0; t < T; t += this->TimeStep)
  {
    sol_t = t+this->TimeStep;

    /*gmm::resize(F, mf_rhs.nb_dof()*N);
    getfem::interpolation_function(mf_rhs, F, sol_grad, NEUMANN_BOUNDARY_NUM);
    gmm::scale(F, gmm::sqr(sol_c));
    gmm::copy(F, model.set_real_variable("NeumannData"));*/

    cout << "solving for t = " << sol_t << endl;
    iter.init();
    getfem::standard_solve(*(this->Model), iter);
    switch (this->Scheme)
    {
    case 1 : // Theta-method
      getfem::velocity_update_for_order_two_theta_method(*(this->Model), "u", "v", "dt", "theta");
      break;
    case 2 : // Midpoint
      getfem::velocity_update_for_order_two_theta_method(*(this->Model), "u", "v", "dt", "alpha");
      break;
    case 3 : // Newmark
      getfem::velocity_update_for_Newmark_scheme(*(this->Model), ibddt, "u", "v", "dt", "twobeta", "gamma");
      break;
    }

  }

  gmm::copy(this->Model->real_variable("u"), U);

}

//----------------------------------------------------------------------------
void vtkGetFEMInterface::Write()
{
  //TODO: Create an Importer/Exporter of getFEM mesh file format
  this->Mesh->write_to_file("getFEM.mesh");
}

//----------------------------------------------------------------------------
void vtkGetFEMInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
