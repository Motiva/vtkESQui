// .NAME vtkMSSWin32Header - manage Windows system differences
// .SECTION Description
// The vtkMSSWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkMSSWin32Header_h
#define __vtkMSSWin32Header_h

#include <vtkMSSConfigure.h>

#if defined(_WIN32)
  #if defined(vtkMSS_EXPORTS)
    #define VTK_vtkMSS_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkMSS_EXPORT
	#define VTK_vtkMSS_EXPORT __declspec( dllimport ) 
  #endif

#else
  #define VTK_vtkMSS_EXPORT
#endif

#endif
