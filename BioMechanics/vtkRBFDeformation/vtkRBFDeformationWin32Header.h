// .NAME vtkRBFDeformationWin32Header - manage Windows system differences
// .SECTION Description
// The vtkRBFDeformationWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkRBFDeformationWin32Header_h
#define __vtkRBFDeformationWin32Header_h

#include <vtkRBFDeformationConfigure.h>

#if defined(_WIN32)
  #if defined(vtkRBFDeformation_EXPORTS)
    #define VTK_vtkRBFDeformation_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkRBFDeformation_EXPORT
	#define VTK_vtkRBFDeformation_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_vtkRBFDeformation_EXPORT
#endif

#endif
