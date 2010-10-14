// .NAME vtkFEMSystemWin32Header - manage Windows system differences
// .SECTION Description
// The vtkFEMSystemWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkFEMSystemWin32Header_h
#define __vtkFEMSystemWin32Header_h

#include <vtkFEMSystemConfigure.h>

#if defined(_WIN32)
  #if defined(vtkFEMSystem_EXPORTS)
    #define VTK_vtkFEMSystem_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkFEMSystem_EXPORT
	#define VTK_vtkFEMSystem_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_vtkFEMSystem_EXPORT
#endif

#endif
