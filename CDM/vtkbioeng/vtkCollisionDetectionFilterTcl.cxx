// tcl wrapper for vtkCollisionDetectionFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkCollisionDetectionFilter.h"

#include "vtkTclUtil.h"
#include <vtkstd/stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkCollisionDetectionFilterNewCommand()
{
  vtkCollisionDetectionFilter *temp = vtkCollisionDetectionFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataAlgorithmCppCommand(vtkPolyDataAlgorithm *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkCollisionDetectionFilterCppCommand(vtkCollisionDetectionFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkCollisionDetectionFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkCollisionDetectionFilterCppCommand(static_cast<vtkCollisionDetectionFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkCollisionDetectionFilterCppCommand(vtkCollisionDetectionFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkCollisionDetectionFilter",argv[1]))
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
    vtkCollisionDetectionFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkCollisionDetectionFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkCollisionDetectionFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkCollisionDetectionFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetCollisionMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCollisionMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCollisionModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCollisionModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCollisionModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCollisionModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCollisionMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCollisionMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCollisionModeToAllContacts",argv[1]))&&(argc == 2))
    {
    op->SetCollisionModeToAllContacts();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetCollisionModeToFirstContact",argv[1]))&&(argc == 2))
    {
    op->SetCollisionModeToFirstContact();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetCollisionModeToHalfContacts",argv[1]))&&(argc == 2))
    {
    op->SetCollisionModeToHalfContacts();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetCollisionModeAsString",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetCollisionModeAsString();
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
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkCollisionDetectionFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkCollisionDetectionFilter");
    return TCL_OK;
    }
  if ((!strcmp("SetInput",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkPolyData  *temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetInput(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInput",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkPolyData  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetInput(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetContactCells",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkIdTypeArray  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetContactCells(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkIdTypeArray");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetContactsOutputPort",argv[1]))&&(argc == 2))
    {
    vtkAlgorithmOutput  *temp20;
    temp20 = (op)->GetContactsOutputPort();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAlgorithmOutput");
    return TCL_OK;
    }
  if ((!strcmp("GetContactsOutput",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetContactsOutput();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("SetTransform",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkLinearTransform  *temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkLinearTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkLinearTransform"),interp,error));
    if (!error)
    {
    op->SetTransform(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransform",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkLinearTransform  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetTransform(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkLinearTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMatrix",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkMatrix4x4  *temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkMatrix4x4 *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkMatrix4x4"),interp,error));
    if (!error)
    {
    op->SetMatrix(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMatrix",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkMatrix4x4  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetMatrix(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkMatrix4x4");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetBoxTolerance",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetBoxTolerance(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBoxTolerance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBoxTolerance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCellTolerance",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetCellTolerance(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCellTolerance",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetCellTolerance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetGenerateScalars",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetGenerateScalars(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGenerateScalars",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetGenerateScalars();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GenerateScalarsOn",argv[1]))&&(argc == 2))
    {
    op->GenerateScalarsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GenerateScalarsOff",argv[1]))&&(argc == 2))
    {
    op->GenerateScalarsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfContacts",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfContacts();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfBoxTests",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBoxTests();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfCellsPerBucket",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfCellsPerBucket(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfCellsPerBucket",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfCellsPerBucket();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOpacity",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOpacity(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOpacityMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOpacityMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOpacityMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOpacityMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOpacity",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOpacity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMTime",argv[1]))&&(argc == 2))
    {
    unsigned long     temp20;
    temp20 = (op)->GetMTime();
    char tempResult[1024];
    sprintf(tempResult,"%lu",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkCollisionDetectionFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkCollisionDetectionFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetCollisionMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCollisionModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCollisionModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCollisionMode\n",NULL);
    Tcl_AppendResult(interp,"  SetCollisionModeToAllContacts\n",NULL);
    Tcl_AppendResult(interp,"  SetCollisionModeToFirstContact\n",NULL);
    Tcl_AppendResult(interp,"  SetCollisionModeToHalfContacts\n",NULL);
    Tcl_AppendResult(interp,"  GetCollisionModeAsString\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetInput\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetInput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetContactCells\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetContactsOutputPort\n",NULL);
    Tcl_AppendResult(interp,"  GetContactsOutput\n",NULL);
    Tcl_AppendResult(interp,"  SetTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMatrix\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetMatrix\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetBoxTolerance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBoxTolerance\n",NULL);
    Tcl_AppendResult(interp,"  SetCellTolerance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCellTolerance\n",NULL);
    Tcl_AppendResult(interp,"  SetGenerateScalars\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetGenerateScalars\n",NULL);
    Tcl_AppendResult(interp,"  GenerateScalarsOn\n",NULL);
    Tcl_AppendResult(interp,"  GenerateScalarsOff\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfContacts\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBoxTests\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfCellsPerBucket\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfCellsPerBucket\n",NULL);
    Tcl_AppendResult(interp,"  SetOpacity\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOpacityMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOpacityMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOpacity\n",NULL);
    Tcl_AppendResult(interp,"  GetMTime\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetCollisionMode" );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCollisionMode" );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToAllContacts" );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToFirstContact" );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToHalfContacts" );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeAsString" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "SetInput" );
    Tcl_DStringAppendElement ( &dString, "GetInput" );
    Tcl_DStringAppendElement ( &dString, "GetContactCells" );
    Tcl_DStringAppendElement ( &dString, "GetContactsOutputPort" );
    Tcl_DStringAppendElement ( &dString, "GetContactsOutput" );
    Tcl_DStringAppendElement ( &dString, "SetTransform" );
    Tcl_DStringAppendElement ( &dString, "GetTransform" );
    Tcl_DStringAppendElement ( &dString, "SetMatrix" );
    Tcl_DStringAppendElement ( &dString, "GetMatrix" );
    Tcl_DStringAppendElement ( &dString, "SetBoxTolerance" );
    Tcl_DStringAppendElement ( &dString, "GetBoxTolerance" );
    Tcl_DStringAppendElement ( &dString, "SetCellTolerance" );
    Tcl_DStringAppendElement ( &dString, "GetCellTolerance" );
    Tcl_DStringAppendElement ( &dString, "SetGenerateScalars" );
    Tcl_DStringAppendElement ( &dString, "GetGenerateScalars" );
    Tcl_DStringAppendElement ( &dString, "GenerateScalarsOn" );
    Tcl_DStringAppendElement ( &dString, "GenerateScalarsOff" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfContacts" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBoxTests" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfCellsPerBucket" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfCellsPerBucket" );
    Tcl_DStringAppendElement ( &dString, "SetOpacity" );
    Tcl_DStringAppendElement ( &dString, "GetOpacityMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetOpacityMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetOpacity" );
    Tcl_DStringAppendElement ( &dString, "GetMTime" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter *NewInstance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter *SafeDownCast (vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCollisionMode */
    if ( strcmp ( argv[2], "SetCollisionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCollisionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCollisionMode */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCollisionMode (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetCollisionMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollisionModeMinValue */
    if ( strcmp ( argv[2], "GetCollisionModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollisionModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCollisionModeMinValue ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetCollisionModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollisionModeMaxValue */
    if ( strcmp ( argv[2], "GetCollisionModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollisionModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCollisionModeMaxValue ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetCollisionModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollisionMode */
    if ( strcmp ( argv[2], "GetCollisionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollisionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollisionMode */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCollisionMode ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetCollisionMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCollisionModeToAllContacts */
    if ( strcmp ( argv[2], "SetCollisionModeToAllContacts" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToAllContacts" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCollisionModeToAllContacts */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCollisionModeToAllContacts ();this SetCollisionMode VTK_ALL_CONTACTS " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetCollisionModeToAllContacts */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCollisionModeToFirstContact */
    if ( strcmp ( argv[2], "SetCollisionModeToFirstContact" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToFirstContact" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCollisionModeToFirstContact */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCollisionModeToFirstContact ();this SetCollisionMode VTK_FIRST_CONTACT " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetCollisionModeToFirstContact */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCollisionModeToHalfContacts */
    if ( strcmp ( argv[2], "SetCollisionModeToHalfContacts" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCollisionModeToHalfContacts" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCollisionModeToHalfContacts */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCollisionModeToHalfContacts ();this SetCollisionMode VTK_HALF_CONTACTS " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetCollisionModeToHalfContacts */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollisionModeAsString */
    if ( strcmp ( argv[2], "GetCollisionModeAsString" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollisionModeAsString" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollisionModeAsString */
    Tcl_DStringAppendElement ( &dString, " Set the collision mode to VTK_ALL_CONTACTS to find all the contacting cell pairs with\n two points per collision, or VTK_HALF_CONTACTS to find all the contacting cell pairs\n with one point per collision, or VTK_FIRST_CONTACT to quickly find the first contact\n point.\n" );
    Tcl_DStringAppendElement ( &dString, "const char *GetCollisionModeAsString ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetCollisionModeAsString */

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
    Tcl_DStringAppendElement ( &dString, " Constructs with initial values.\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkCollisionDetectionFilter *New ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInput */
    if ( strcmp ( argv[2], "SetInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInput */
    Tcl_DStringAppendElement ( &dString, " Set and Get the input vtk polydata models\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInput (int i, vtkPolyData *model);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInput */
    if ( strcmp ( argv[2], "GetInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInput */
    Tcl_DStringAppendElement ( &dString, " Set and Get the input vtk polydata models\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetInput (int i);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetContactCells */
    if ( strcmp ( argv[2], "GetContactCells" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetContactCells" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetContactCells */
    Tcl_DStringAppendElement ( &dString, " Get an array of the contacting cells. This is a convenience method to access \n the \"ContactCells\" field array in outputs 0 and 1. These arrays index contacting\n cells (eg) index 50 of array 0 points to a cell (triangle) which contacts/intersects\n a cell at index 50 of array 1. This method is equivalent to \n GetOutput(i)->GetFieldData()->GetArray(\"ContactCells\")\n" );
    Tcl_DStringAppendElement ( &dString, "vtkIdTypeArray *GetContactCells (int i);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetContactCells */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetContactsOutputPort */
    if ( strcmp ( argv[2], "GetContactsOutputPort" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetContactsOutputPort" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetContactsOutputPort */
    Tcl_DStringAppendElement ( &dString, " Get the output with the points where the contacting cells intersect. This method is\n is equivalent to GetOutputPort(2)/GetOutput(2) \n" );
    Tcl_DStringAppendElement ( &dString, "vtkAlgorithmOutput *GetContactsOutputPort ();return this GetOutputPort " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetContactsOutputPort */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetContactsOutput */
    if ( strcmp ( argv[2], "GetContactsOutput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetContactsOutput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetContactsOutput */
    Tcl_DStringAppendElement ( &dString, " Specify the transform object used to transform models. Alternatively, matrices\n can be set instead.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetContactsOutput ();return this GetOutput " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetContactsOutput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransform */
    if ( strcmp ( argv[2], "SetTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkLinearTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransform */
    Tcl_DStringAppendElement ( &dString, " Specify the transform object used to transform models. Alternatively, matrices\n can be set instead.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTransform (int i, vtkLinearTransform *transform);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransform */
    if ( strcmp ( argv[2], "GetTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransform */
    Tcl_DStringAppendElement ( &dString, " Specify the matrix object used to transform models.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkLinearTransform *GetTransform (int i);return this Transform i " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatrix */
    if ( strcmp ( argv[2], "SetMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkMatrix4x4" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatrix */
    Tcl_DStringAppendElement ( &dString, " Specify the matrix object used to transform models.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMatrix (int i, vtkMatrix4x4 *matrix);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatrix */
    if ( strcmp ( argv[2], "GetMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatrix */
    Tcl_DStringAppendElement ( &dString, " Specify the matrix object used to transform models.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkMatrix4x4 *GetMatrix (int i);" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBoxTolerance */
    if ( strcmp ( argv[2], "SetBoxTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBoxTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBoxTolerance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBoxTolerance (float );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetBoxTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBoxTolerance */
    if ( strcmp ( argv[2], "GetBoxTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBoxTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBoxTolerance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetBoxTolerance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetBoxTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCellTolerance */
    if ( strcmp ( argv[2], "SetCellTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCellTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCellTolerance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCellTolerance (double );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetCellTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCellTolerance */
    if ( strcmp ( argv[2], "GetCellTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCellTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCellTolerance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetCellTolerance ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetCellTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGenerateScalars */
    if ( strcmp ( argv[2], "SetGenerateScalars" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGenerateScalars" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGenerateScalars */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetGenerateScalars (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetGenerateScalars */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGenerateScalars */
    if ( strcmp ( argv[2], "GetGenerateScalars" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGenerateScalars" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGenerateScalars */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetGenerateScalars ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetGenerateScalars */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateScalarsOn */
    if ( strcmp ( argv[2], "GenerateScalarsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateScalarsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateScalarsOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void GenerateScalarsOn ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GenerateScalarsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateScalarsOff */
    if ( strcmp ( argv[2], "GenerateScalarsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateScalarsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateScalarsOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void GenerateScalarsOff ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GenerateScalarsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfContacts */
    if ( strcmp ( argv[2], "GetNumberOfContacts" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfContacts" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfContacts */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfContacts ();return this GetOutput GetFieldData GetArray GetNumberOfTuples " );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetNumberOfContacts */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBoxTests */
    if ( strcmp ( argv[2], "GetNumberOfBoxTests" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBoxTests" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBoxTests */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBoxTests ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetNumberOfBoxTests */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfCellsPerBucket */
    if ( strcmp ( argv[2], "SetNumberOfCellsPerBucket" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfCellsPerBucket" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfCellsPerBucket */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfCellsPerBucket (int );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetNumberOfCellsPerBucket */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfCellsPerBucket */
    if ( strcmp ( argv[2], "GetNumberOfCellsPerBucket" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfCellsPerBucket" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfCellsPerBucket */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfCellsPerBucket ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetNumberOfCellsPerBucket */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOpacity */
    if ( strcmp ( argv[2], "SetOpacity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOpacity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOpacity */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOpacity (float );" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for SetOpacity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOpacityMinValue */
    if ( strcmp ( argv[2], "GetOpacityMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOpacityMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOpacityMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOpacityMinValue ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetOpacityMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOpacityMaxValue */
    if ( strcmp ( argv[2], "GetOpacityMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOpacityMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOpacityMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOpacityMaxValue ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetOpacityMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOpacity */
    if ( strcmp ( argv[2], "GetOpacity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOpacity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOpacity */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOpacity ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetOpacity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMTime */
    if ( strcmp ( argv[2], "GetMTime" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMTime" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMTime */
    Tcl_DStringAppendElement ( &dString, " Return the MTime also considering the transform.\n" );
    Tcl_DStringAppendElement ( &dString, "unsigned long GetMTime ();" );
    Tcl_DStringAppendElement ( &dString, "vtkCollisionDetectionFilter" );
    /* Closing for GetMTime */

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
