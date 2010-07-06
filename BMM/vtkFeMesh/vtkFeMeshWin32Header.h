// .NAME vtkmyCommonWin32Header - manage Windows system differences
// .SECTION Description
// The vtkmyCommonWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkFeMeshWin32Header_h
#define __vtkFeMeshWin32Header_h

#include <vtkFeMeshConfigure.h>

#if defined(_WIN32)
  #if defined(vtkFeMesh_EXPORTS)
    #define VTK_vtkFeMesh_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkFeMesh_EXPORT
	#define VTK_vtkFeMesh_EXPORT __declspec( dllimport ) 
  #endif

#else
  #define VTK_vtkFeMesh_EXPORT
#endif

#endif
