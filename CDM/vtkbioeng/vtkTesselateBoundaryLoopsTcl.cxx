// tcl wrapper for vtkTesselateBoundaryLoops object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkTesselateBoundaryLoops.h"

#include "vtkTclUtil.h"
#include <vtkstd/stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkTesselateBoundaryLoopsNewCommand()
{
  vtkTesselateBoundaryLoops *temp = vtkTesselateBoundaryLoops::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataAlgorithmCppCommand(vtkPolyDataAlgorithm *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkTesselateBoundaryLoopsCppCommand(vtkTesselateBoundaryLoops *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkTesselateBoundaryLoopsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkTesselateBoundaryLoopsCppCommand(static_cast<vtkTesselateBoundaryLoops *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkTesselateBoundaryLoopsCppCommand(vtkTesselateBoundaryLoops *op, Tcl_Interp *interp,
             int argc, char *argv[])
{
  int    tempi;
  double tempd;
  static char temps[80];
  int    error;

  error = 0; error = error;
  tempi = 0; tempi = tempi;
  tempd = 0; tempd = tempd;
  temps[0] = 0; temps[0] = temps[0];

  if (argc < 2)
    {
    Tcl_SetResult(interp,const_cast<char *>("Could not find requested method."), TCL_VOLATILE);
    return TCL_ERROR;
    }
  if (!interp)
    {
    if (!strcmp("DoTypecasting",argv[0]))
      {
      if (!strcmp("vtkTesselateBoundaryLoops",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkPolyDataAlgorithmCppCommand(static_cast<vtkPolyDataAlgorithm *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkPolyDataAlgorithm"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("GetClassName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetClassName();
    if (temp20)
      {
      Tcl_SetResult(interp, const_cast<char *>(temp20), TCL_VOLATILE);
      }
    else
      {
      Tcl_ResetResult(interp);
      }
    return TCL_OK;
    }
  if ((!strcmp("IsA",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->IsA(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("NewInstance",argv[1]))&&(argc == 2))
    {
    vtkTesselateBoundaryLoops  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkTesselateBoundaryLoops");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkTesselateBoundaryLoops  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkTesselateBoundaryLoops");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkTesselateBoundaryLoops  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkTesselateBoundaryLoops");
    return TCL_OK;
    }
  if ((!strcmp("SetTolerance",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetTolerance(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTolerance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTolerance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaximumLength",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMaximumLength(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaximumLength",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMaximumLength();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAppendTesselationToInput",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAppendTesselationToInput(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAppendTesselationToInput",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAppendTesselationToInput();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AppendTesselationToInputOn",argv[1]))&&(argc == 2))
    {
    op->AppendTesselationToInputOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("AppendTesselationToInputOff",argv[1]))&&(argc == 2))
    {
    op->AppendTesselationToInputOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetReverseSense",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetReverseSense(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReverseSense",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetReverseSense();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ReverseSenseOn",argv[1]))&&(argc == 2))
    {
    op->ReverseSenseOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ReverseSenseOff",argv[1]))&&(argc == 2))
    {
    op->ReverseSenseOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetNonManifoldEdges",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNonManifoldEdges(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNonManifoldEdges",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNonManifoldEdges();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("NonManifoldEdgesOn",argv[1]))&&(argc == 2))
    {
    op->NonManifoldEdgesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("NonManifoldEdgesOff",argv[1]))&&(argc == 2))
    {
    op->NonManifoldEdgesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfLoops",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfLoops();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkTesselateBoundaryLoopsCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkTesselateBoundaryLoops:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetTolerance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTolerance\n",NULL);
    Tcl_AppendResult(interp,"  SetMaximumLength\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaximumLength\n",NULL);
    Tcl_AppendResult(interp,"  SetAppendTesselationToInput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAppendTesselationToInput\n",NULL);
    Tcl_AppendResult(interp,"  AppendTesselationToInputOn\n",NULL);
    Tcl_AppendResult(interp,"  AppendTesselationToInputOff\n",NULL);
    Tcl_AppendResult(interp,"  SetReverseSense\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReverseSense\n",NULL);
    Tcl_AppendResult(interp,"  ReverseSenseOn\n",NULL);
    Tcl_AppendResult(interp,"  ReverseSenseOff\n",NULL);
    Tcl_AppendResult(interp,"  SetNonManifoldEdges\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNonManifoldEdges\n",NULL);
    Tcl_AppendResult(interp,"  NonManifoldEdgesOn\n",NULL);
    Tcl_AppendResult(interp,"  NonManifoldEdgesOff\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfLoops\n",NULL);
    return TCL_OK;
    }

  if (!strcmp("DescribeMethods",argv[1]))
    {
    if(argc>3) {
      Tcl_SetResult ( interp, const_cast<char*>("Wrong number of arguments: object DescribeMethods <MethodName>"), TCL_VOLATILE ); 
      return TCL_ERROR;
 }
    if(argc==2) {

  Tcl_DString dString, dStringParent;

  Tcl_DStringInit ( &dString );

  Tcl_DStringInit ( &dStringParent );
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "SetTolerance" );
    Tcl_DStringAppendElement ( &dString, "GetTolerance" );
    Tcl_DStringAppendElement ( &dString, "SetMaximumLength" );
    Tcl_DStringAppendElement ( &dString, "GetMaximumLength" );
    Tcl_DStringAppendElement ( &dString, "SetAppendTesselationToInput" );
    Tcl_DStringAppendElement ( &dString, "GetAppendTesselationToInput" );
    Tcl_DStringAppendElement ( &dString, "AppendTesselationToInputOn" );
    Tcl_DStringAppendElement ( &dString, "AppendTesselationToInputOff" );
    Tcl_DStringAppendElement ( &dString, "SetReverseSense" );
    Tcl_DStringAppendElement ( &dString, "GetReverseSense" );
    Tcl_DStringAppendElement ( &dString, "ReverseSenseOn" );
    Tcl_DStringAppendElement ( &dString, "ReverseSenseOff" );
    Tcl_DStringAppendElement ( &dString, "SetNonManifoldEdges" );
    Tcl_DStringAppendElement ( &dString, "GetNonManifoldEdges" );
    Tcl_DStringAppendElement ( &dString, "NonManifoldEdgesOn" );
    Tcl_DStringAppendElement ( &dString, "NonManifoldEdgesOff" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLoops" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
    /* Starting function: GetClassName */
    if ( strcmp ( argv[2], "GetClassName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClassName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetClassName ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetClassName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IsA */
    if ( strcmp ( argv[2], "IsA" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IsA */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int IsA (const char *name);" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for IsA */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NewInstance */
    if ( strcmp ( argv[2], "NewInstance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NewInstance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops *NewInstance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SafeDownCast */
    if ( strcmp ( argv[2], "SafeDownCast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SafeDownCast */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops *SafeDownCast (vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, " Constructor\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkTesselateBoundaryLoops *New ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTolerance */
    if ( strcmp ( argv[2], "SetTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTolerance */
    Tcl_DStringAppendElement ( &dString, " Set the tolerance for point merging of line verts... if line verts aren't exactly coincident\n they wont be joined up into a loop. With a tolerance they can be incouraged to be coincident.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTolerance (float );" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SetTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTolerance */
    if ( strcmp ( argv[2], "GetTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTolerance */
    Tcl_DStringAppendElement ( &dString, " Set the tolerance for point merging of line verts... if line verts aren't exactly coincident\n they wont be joined up into a loop. With a tolerance they can be incouraged to be coincident.\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTolerance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaximumLength */
    if ( strcmp ( argv[2], "SetMaximumLength" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaximumLength" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaximumLength */
    Tcl_DStringAppendElement ( &dString, " Set the maximum number of lines in a loop. Default is 1000;\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaximumLength (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SetMaximumLength */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaximumLength */
    if ( strcmp ( argv[2], "GetMaximumLength" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaximumLength" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaximumLength */
    Tcl_DStringAppendElement ( &dString, " Set the maximum number of lines in a loop. Default is 1000;\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMaximumLength ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetMaximumLength */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAppendTesselationToInput */
    if ( strcmp ( argv[2], "SetAppendTesselationToInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAppendTesselationToInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAppendTesselationToInput */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetAppendTesselationToInput (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SetAppendTesselationToInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAppendTesselationToInput */
    if ( strcmp ( argv[2], "GetAppendTesselationToInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAppendTesselationToInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAppendTesselationToInput */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetAppendTesselationToInput ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetAppendTesselationToInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AppendTesselationToInputOn */
    if ( strcmp ( argv[2], "AppendTesselationToInputOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AppendTesselationToInputOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AppendTesselationToInputOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void AppendTesselationToInputOn ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for AppendTesselationToInputOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AppendTesselationToInputOff */
    if ( strcmp ( argv[2], "AppendTesselationToInputOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AppendTesselationToInputOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AppendTesselationToInputOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void AppendTesselationToInputOff ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for AppendTesselationToInputOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReverseSense */
    if ( strcmp ( argv[2], "SetReverseSense" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReverseSense" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReverseSense */
    Tcl_DStringAppendElement ( &dString, " Reverse the sense (front/back face) of the loops being tesselated\n" );
    Tcl_DStringAppendElement ( &dString, "void SetReverseSense (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SetReverseSense */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReverseSense */
    if ( strcmp ( argv[2], "GetReverseSense" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReverseSense" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReverseSense */
    Tcl_DStringAppendElement ( &dString, " Reverse the sense (front/back face) of the loops being tesselated\n" );
    Tcl_DStringAppendElement ( &dString, "int GetReverseSense ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetReverseSense */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ReverseSenseOn */
    if ( strcmp ( argv[2], "ReverseSenseOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ReverseSenseOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ReverseSenseOn */
    Tcl_DStringAppendElement ( &dString, " Reverse the sense (front/back face) of the loops being tesselated\n" );
    Tcl_DStringAppendElement ( &dString, "void ReverseSenseOn ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for ReverseSenseOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ReverseSenseOff */
    if ( strcmp ( argv[2], "ReverseSenseOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ReverseSenseOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ReverseSenseOff */
    Tcl_DStringAppendElement ( &dString, " Reverse the sense (front/back face) of the loops being tesselated\n" );
    Tcl_DStringAppendElement ( &dString, "void ReverseSenseOff ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for ReverseSenseOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNonManifoldEdges */
    if ( strcmp ( argv[2], "SetNonManifoldEdges" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNonManifoldEdges" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNonManifoldEdges */
    Tcl_DStringAppendElement ( &dString, " Sometimes boundary edges are non-manifold and need to be included\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNonManifoldEdges (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for SetNonManifoldEdges */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNonManifoldEdges */
    if ( strcmp ( argv[2], "GetNonManifoldEdges" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNonManifoldEdges" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNonManifoldEdges */
    Tcl_DStringAppendElement ( &dString, " Sometimes boundary edges are non-manifold and need to be included\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNonManifoldEdges ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetNonManifoldEdges */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NonManifoldEdgesOn */
    if ( strcmp ( argv[2], "NonManifoldEdgesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NonManifoldEdgesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NonManifoldEdgesOn */
    Tcl_DStringAppendElement ( &dString, " Sometimes boundary edges are non-manifold and need to be included\n" );
    Tcl_DStringAppendElement ( &dString, "void NonManifoldEdgesOn ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for NonManifoldEdgesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NonManifoldEdgesOff */
    if ( strcmp ( argv[2], "NonManifoldEdgesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NonManifoldEdgesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NonManifoldEdgesOff */
    Tcl_DStringAppendElement ( &dString, " Sometimes boundary edges are non-manifold and need to be included\n" );
    Tcl_DStringAppendElement ( &dString, "void NonManifoldEdgesOff ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for NonManifoldEdgesOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfLoops */
    if ( strcmp ( argv[2], "GetNumberOfLoops" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLoops" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfLoops */
    Tcl_DStringAppendElement ( &dString, " Get the number of boundary loops \n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfLoops ();" );
    Tcl_DStringAppendElement ( &dString, "vtkTesselateBoundaryLoops" );
    /* Closing for GetNumberOfLoops */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkPolyDataAlgorithmCppCommand(static_cast<vtkPolyDataAlgorithm *>(op),interp,argc,argv) == TCL_OK)
    {
    return TCL_OK;
    }
    }
  catch (vtkstd::exception &e)
    {
    Tcl_AppendResult(interp, "Uncaught exception: ",  e.what(), "\n", NULL);
    return TCL_ERROR;
    }
  return TCL_ERROR;
}
