#ifndef __vtkRobustDeformationModelWin32Header_h
#define __vtkRobustDeformationModelWin32Header_h

#include <vtkRobustDeformationModelConfigure.h>

#if defined(_WIN32)
  #if defined(vtkRobustDeformationModel_EXPORTS)
    #define VTK_vtkRobustDeformationModel_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkRobustDeformationModel_EXPORT
	#define VTK_vtkRobustDeformationModel_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_vtkRobustDeformationModel_EXPORT
#endif

#endif
