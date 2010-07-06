#include "vtkTclUtil.h"
#include "vtkVersion.h"
#define VTK_TCL_TO_STRING(x) VTK_TCL_TO_STRING0(x)
#define VTK_TCL_TO_STRING0(x) #x
extern "C"
{
#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION >= 4)
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, CONST84 char *[]);
#else
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, char *[]);
#endif
}

int vtkBufferOutputWindowCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkBufferOutputWindowNewCommand();
int vtkCollisionDetectionFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkCollisionDetectionFilterNewCommand();
int vtkRemoveCellsFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkRemoveCellsFilterNewCommand();
int vtkSelectCellsFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkSelectCellsFilterNewCommand();
int vtkTesselateBoundaryLoopsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkTesselateBoundaryLoopsNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkbioengtcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkbioengtcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkbioengtcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkbioengtcl_Init(interp);
}


int VTK_EXPORT Vtkbioengtcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkBufferOutputWindow"), vtkBufferOutputWindowNewCommand,
                  vtkBufferOutputWindowCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkCollisionDetectionFilter"), vtkCollisionDetectionFilterNewCommand,
                  vtkCollisionDetectionFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkRemoveCellsFilter"), vtkRemoveCellsFilterNewCommand,
                  vtkRemoveCellsFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkSelectCellsFilter"), vtkSelectCellsFilterNewCommand,
                  vtkSelectCellsFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkTesselateBoundaryLoops"), vtkTesselateBoundaryLoopsNewCommand,
                  vtkTesselateBoundaryLoopsCommand);
  char pkgName[]="vtkBioengTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
