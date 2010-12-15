#ifndef __vtkBioengInstantiator_h
#define __vtkBioengInstantiator_h

#include "vtkInstantiator.h"

#include "vtkBioengConfigure.h"


class VTK_BIOENG_EXPORT vtkBioengInstantiator
{
  public:
  vtkBioengInstantiator();
  ~vtkBioengInstantiator();
  private:
  static void ClassInitialize();
  static void ClassFinalize();
  static unsigned int Count;
}; 

static vtkBioengInstantiator vtkBioengInstantiatorInitializer;

#endif
