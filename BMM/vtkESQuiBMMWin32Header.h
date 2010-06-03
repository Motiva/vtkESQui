// .NAME vtkmyCommonWin32Header - manage Windows system differences
// .SECTION Description
// The vtkmyCommonWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkesquiBMMWin32Header_h
#define __vtkesquiBMMWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32)&& !defined(VTKESQUI_STATIC)
  #if defined(vtkESQuiBMM_EXPORTS)
    #define VTK_ESQUI_BMM_EXPORT __declspec(dllexport)
  #else
	#undef VTK_ESQUI_BMM_EXPORT
	#define VTK_ESQUI_BMM_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_ESQUI_BMM_EXPORT
#endif

#endif
