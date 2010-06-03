// tcl wrapper for vtkBufferOutputWindow object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkBufferOutputWindow.h"

#include "vtkTclUtil.h"
#include <vtkstd/stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkBufferOutputWindowNewCommand()
{
  vtkBufferOutputWindow *temp = vtkBufferOutputWindow::New();
  return static_cast<ClientData>(temp);
}

int vtkOutputWindowCppCommand(vtkOutputWindow *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkBufferOutputWindowCppCommand(vtkBufferOutputWindow *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkBufferOutputWindowCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkBufferOutputWindowCppCommand(static_cast<vtkBufferOutputWindow *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkBufferOutputWindowCppCommand(vtkBufferOutputWindow *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkBufferOutputWindow",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkOutputWindowCppCommand(static_cast<vtkOutputWindow *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkOutputWindow"), TCL_VOLATILE);
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
    vtkBufferOutputWindow  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkBufferOutputWindow");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkBufferOutputWindow  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkBufferOutputWindow");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkBufferOutputWindow  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkBufferOutputWindow");
    return TCL_OK;
    }
  if ((!strcmp("GetLastOutputTextType",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLastOutputTextType();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("DisplayText",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->DisplayText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("DisplayErrorText",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->DisplayErrorText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("DisplayWarningText",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->DisplayWarningText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("DisplayGenericWarningText",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->DisplayGenericWarningText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("DisplayDebugText",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->DisplayDebugText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLastOutputText",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetLastOutputText();
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
  if ((!strcmp("GetLastOutputTextTypeAsString",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetLastOutputTextTypeAsString();
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkBufferOutputWindowCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkOutputWindowCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkBufferOutputWindow:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetLastOutputTextType\n",NULL);
    Tcl_AppendResult(interp,"  DisplayText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DisplayErrorText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DisplayWarningText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DisplayGenericWarningText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DisplayDebugText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLastOutputText\n",NULL);
    Tcl_AppendResult(interp,"  GetLastOutputTextTypeAsString\n",NULL);
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
    vtkOutputWindowCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputTextType" );
    Tcl_DStringAppendElement ( &dString, "DisplayText" );
    Tcl_DStringAppendElement ( &dString, "DisplayErrorText" );
    Tcl_DStringAppendElement ( &dString, "DisplayWarningText" );
    Tcl_DStringAppendElement ( &dString, "DisplayGenericWarningText" );
    Tcl_DStringAppendElement ( &dString, "DisplayDebugText" );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputText" );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputTextTypeAsString" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkOutputWindowCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
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
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
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
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow *NewInstance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
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
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow *SafeDownCast (vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkBufferOutputWindow *New ();" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastOutputTextType */
    if ( strcmp ( argv[2], "GetLastOutputTextType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputTextType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastOutputTextType */
    Tcl_DStringAppendElement ( &dString, " Get the last text type: 0 = TEXT; 1 = WARNING_TEXT; 2 = ERROR_TEXT; \n 3 = GENERIC_WARNING_TEXT; 4 = DEBUG_TEXT\n" );
    Tcl_DStringAppendElement ( &dString, "int GetLastOutputTextType ();" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for GetLastOutputTextType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DisplayText */
    if ( strcmp ( argv[2], "DisplayText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DisplayText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DisplayText */
    Tcl_DStringAppendElement ( &dString, " the output window api\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void DisplayText (const char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for DisplayText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DisplayErrorText */
    if ( strcmp ( argv[2], "DisplayErrorText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DisplayErrorText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DisplayErrorText */
    Tcl_DStringAppendElement ( &dString, " the output window api\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void DisplayErrorText (const char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for DisplayErrorText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DisplayWarningText */
    if ( strcmp ( argv[2], "DisplayWarningText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DisplayWarningText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DisplayWarningText */
    Tcl_DStringAppendElement ( &dString, " the output window api\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void DisplayWarningText (const char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for DisplayWarningText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DisplayGenericWarningText */
    if ( strcmp ( argv[2], "DisplayGenericWarningText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DisplayGenericWarningText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DisplayGenericWarningText */
    Tcl_DStringAppendElement ( &dString, " the output window api\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void DisplayGenericWarningText (const char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for DisplayGenericWarningText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DisplayDebugText */
    if ( strcmp ( argv[2], "DisplayDebugText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DisplayDebugText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DisplayDebugText */
    Tcl_DStringAppendElement ( &dString, " the output window api\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void DisplayDebugText (const char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for DisplayDebugText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastOutputText */
    if ( strcmp ( argv[2], "GetLastOutputText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastOutputText */
    Tcl_DStringAppendElement ( &dString, " Get the last message\n" );
    Tcl_DStringAppendElement ( &dString, "char *GetLastOutputText ();" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for GetLastOutputText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastOutputTextTypeAsString */
    if ( strcmp ( argv[2], "GetLastOutputTextTypeAsString" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastOutputTextTypeAsString" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastOutputTextTypeAsString */
    Tcl_DStringAppendElement ( &dString, " Get the last message type... error, warning etc.\n" );
    Tcl_DStringAppendElement ( &dString, "char *GetLastOutputTextTypeAsString ();" );
    Tcl_DStringAppendElement ( &dString, "vtkBufferOutputWindow" );
    /* Closing for GetLastOutputTextTypeAsString */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkOutputWindowCppCommand(static_cast<vtkOutputWindow *>(op),interp,argc,argv) == TCL_OK)
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
