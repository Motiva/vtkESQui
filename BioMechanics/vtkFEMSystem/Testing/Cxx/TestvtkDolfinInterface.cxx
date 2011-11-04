#include "vtkPolyData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include "vtkDolfinInterface.h"



int main(int argc, char * argv[])
{
  const char * filename = "/home/jballesteros/Workspace/data/vtkESQuiData/Scenario/Meshes/3D/cylinder.xml";

  if (argc > 1)
  {
    filename = argv[1];
  }

  vtkDolfinInterface * Dolfin = vtkDolfinInterface::New();

  Dolfin->SetFileName(filename);
  Dolfin->Init();

  return 0;
}

