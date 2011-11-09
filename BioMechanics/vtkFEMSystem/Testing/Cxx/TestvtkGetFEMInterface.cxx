#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkDataSetMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkIdList.h"

#include "vtkGetFEMInterface.h"



int main(int argc, char * argv[])
{
  vtkGetFEMInterface * getFEM = vtkGetFEMInterface::New();

  getFEM->SetFileName("/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/3D/cylinder.gmsh");
  getFEM->SetDirichlet(0); //Disable Dirichlet Boundary
  getFEM->SetDirichletRatio(0.2); //Dirichlet Ratio

  getFEM->SetLambda(0.5); //Lambda Lamé
  getFEM->SetMu(0.5); //Mu Lamé
  getFEM->SetRho(1e-6); //Density
  getFEM->SetR(0.1); //Augmentation
  getFEM->SetContactMass(1);// Supress Contact Point Mass

  getFEM->SetPG(9810); //Gravity Coefficient (mm/s^2)
  getFEM->SetFriction_coef(0.1); //Friction Coefficient

  getFEM->SetResidual(3e-7); //Residual Error
  getFEM->SetDeltaT(1e-6); //DeltaT

  //Time Scheme Parameters
  /*
   * 0 = theta method
   * 1 = Newmark
   * 2 = midpoint
   * 3 = midpoint + separate contact forces
   * */
  getFEM->SetScheme(2); //Solver Time Scheme (midpoint)
  getFEM->SetTheta(0.5); //Theta;
  getFEM->SetBeta(0.5); //Beta (Newmark)
  getFEM->SetGamma(0.5); //Gamma (Newmark)
  getFEM->SetRestitution(0.6); //Restitution parameter (Paoli-Schaltzmann)

  //Initialize
  getFEM->Initialize();

  //Solve
  getFEM->Solve();
  //getFEM->Write();
  getFEM->Delete();

  return 0;
}

