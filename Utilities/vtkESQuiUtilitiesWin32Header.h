#ifndef __vtkesquiUtilitiesWin32Header_h
#define __vtkesquiUtilitiesWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32)&& !defined(VTKESQUI_STATIC)
  #if defined(vtkESQuiUtilities_EXPORTS)
    #define VTK_ESQUI_UTILITIES_EXPORT __declspec(dllexport)
  #else
	#undef VTK_ESQUI_UTILITIES_EXPORT
	#define VTK_ESQUI_UTILITIES_EXPORT __declspec( dllimport ) 
  #endif

#else
  #define VTK_ESQUI_UTILITIES_EXPORT
#endif

#endif
