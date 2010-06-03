// tcl wrapper for vtkCellsFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkCellsFilter.h"

#include "vtkTclUtil.h"
#include <vtkstd/stdexcept>
#include <vtksys/ios/sstream>
int vtkPolyDataAlgorithmCppCommand(vtkPolyDataAlgorithm *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkCellsFilterCppCommand(vtkCellsFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkCellsFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkCellsFilterCppCommand(static_cast<vtkCellsFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkCellsFilterCppCommand(vtkCellsFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkCellsFilter",argv[1]))
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
    vtkCellsFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkCellsFilter");
    return TCL_OK;
    }
  if ((!strcmp("MarkCell",argv[1]))&&(argc == 3))
    {
    vtkIdType   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->MarkCell(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("UnmarkCell",argv[1]))&&(argc == 3))
    {
    vtkIdType   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->UnmarkCell(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ToggleCell",argv[1]))&&(argc == 3))
    {
    vtkIdType   temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->ToggleCell(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("UndoMarks",argv[1]))&&(argc == 2))
    {
    op->UndoMarks();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMarkedColor",argv[1]))&&(argc == 5))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetMarkedColor(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMarkedColor",argv[1]))&&(argc == 5))
    {
    double temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->SetMarkedColor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMarkedColor",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetMarkedColor();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      char converted[1024];
      *converted = '\0';
      Tcl_PrintDouble(interp,temp20[0], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[1], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[2], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("SetUnmarkedColor",argv[1]))&&(argc == 5))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetUnmarkedColor(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetUnmarkedColor",argv[1]))&&(argc == 5))
    {
    double temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->SetUnmarkedColor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUnmarkedColor",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetUnmarkedColor();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      char converted[1024];
      *converted = '\0';
      Tcl_PrintDouble(interp,temp20[0], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[1], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[2], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("SetMarkedOpacity",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMarkedOpacity(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMarkedOpacity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMarkedOpacity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkCellsFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkCellsFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  MarkCell\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  UnmarkCell\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ToggleCell\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  UndoMarks\n",NULL);
    Tcl_AppendResult(interp,"  SetMarkedColor\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetMarkedColor\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetMarkedColor\n",NULL);
    Tcl_AppendResult(interp,"  SetUnmarkedColor\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetUnmarkedColor\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetUnmarkedColor\n",NULL);
    Tcl_AppendResult(interp,"  SetMarkedOpacity\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMarkedOpacity\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "MarkCell" );
    Tcl_DStringAppendElement ( &dString, "UnmarkCell" );
    Tcl_DStringAppendElement ( &dString, "ToggleCell" );
    Tcl_DStringAppendElement ( &dString, "UndoMarks" );
    Tcl_DStringAppendElement ( &dString, "SetMarkedColor" );
    Tcl_DStringAppendElement ( &dString, "SetMarkedColor" );
    Tcl_DStringAppendElement ( &dString, "GetMarkedColor" );
    Tcl_DStringAppendElement ( &dString, "SetUnmarkedColor" );
    Tcl_DStringAppendElement ( &dString, "SetUnmarkedColor" );
    Tcl_DStringAppendElement ( &dString, "GetUnmarkedColor" );
    Tcl_DStringAppendElement ( &dString, "SetMarkedOpacity" );
    Tcl_DStringAppendElement ( &dString, "GetMarkedOpacity" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter *NewInstance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MarkCell */
    if ( strcmp ( argv[2], "MarkCell" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MarkCell" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MarkCell */
    Tcl_DStringAppendElement ( &dString, " Mark a cell to be removed by RemoveMarkedCells later.  \n" );
    Tcl_DStringAppendElement ( &dString, "void MarkCell (vtkIdType cellid);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for MarkCell */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UnmarkCell */
    if ( strcmp ( argv[2], "UnmarkCell" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UnmarkCell" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UnmarkCell */
    Tcl_DStringAppendElement ( &dString, " Unmark any cell that has been marked already\n" );
    Tcl_DStringAppendElement ( &dString, "void UnmarkCell (vtkIdType cellid);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for UnmarkCell */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ToggleCell */
    if ( strcmp ( argv[2], "ToggleCell" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ToggleCell" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ToggleCell */
    Tcl_DStringAppendElement ( &dString, " If a cells is unmarked- mark it... if its marked- unmark it.\n" );
    Tcl_DStringAppendElement ( &dString, "void ToggleCell (vtkIdType cellid);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for ToggleCell */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UndoMarks */
    if ( strcmp ( argv[2], "UndoMarks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UndoMarks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UndoMarks */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked color... default is red\n" );
    Tcl_DStringAppendElement ( &dString, "void UndoMarks ();this InitializeScalars this MarkedCellIdList Reset this Scalars Modified " );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for UndoMarks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMarkedColor */
    if ( strcmp ( argv[2], "SetMarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked color... default is red\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMarkedColor (double r, double g, double b);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for SetMarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMarkedColor */
    if ( strcmp ( argv[2], "SetMarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked color... default is red\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMarkedColor (double rgb\\[3\\]);this SetMarkedColor rgb rgb rgb " );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for SetMarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMarkedColor */
    if ( strcmp ( argv[2], "GetMarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked color... default is red\n" );
    Tcl_DStringAppendElement ( &dString, "double  *GetMarkedColor ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for GetMarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUnmarkedColor */
    if ( strcmp ( argv[2], "SetUnmarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUnmarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUnmarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the umarked color... default is white. You could set this to the same color as the vtkProperty\n and users won't know they are seeing scalars.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUnmarkedColor (double r, double g, double b);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for SetUnmarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUnmarkedColor */
    if ( strcmp ( argv[2], "SetUnmarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUnmarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUnmarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the umarked color... default is white. You could set this to the same color as the vtkProperty\n and users won't know they are seeing scalars.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUnmarkedColor (double rgb\\[3\\]);this SetUnmarkedColor rgb rgb rgb " );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for SetUnmarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUnmarkedColor */
    if ( strcmp ( argv[2], "GetUnmarkedColor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUnmarkedColor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUnmarkedColor */
    Tcl_DStringAppendElement ( &dString, " Set/Get the umarked color... default is white. You could set this to the same color as the vtkProperty\n and users won't know they are seeing scalars.\n" );
    Tcl_DStringAppendElement ( &dString, "double  *GetUnmarkedColor ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for GetUnmarkedColor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMarkedOpacity */
    if ( strcmp ( argv[2], "SetMarkedOpacity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMarkedOpacity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMarkedOpacity */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked opacity... default is 1.0 By setting this to 0.0 you can temporally remove the cells\n and then permanently remove them with RemoveMarkedCells\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMarkedOpacity (double opacity);" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for SetMarkedOpacity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMarkedOpacity */
    if ( strcmp ( argv[2], "GetMarkedOpacity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMarkedOpacity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMarkedOpacity */
    Tcl_DStringAppendElement ( &dString, " Set/Get the marked opacity... default is 1.0 By setting this to 0.0 you can temporally remove the cells\n and then permanently remove them with RemoveMarkedCells\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMarkedOpacity ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCellsFilter" );
    /* Closing for GetMarkedOpacity */

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
