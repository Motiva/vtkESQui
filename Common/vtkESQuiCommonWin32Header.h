#ifndef __vtkEsquiCommonWin32Header_h
#define __vtkEsquiCommonWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32) && !defined(VTKESQUI_STATIC)
  #if defined(vtkESQuiCommon_EXPORTS)
    #define VTK_ESQUI_COMMON_EXPORT __declspec(dllexport)
  #else
    #define VTK_ESQUI_COMMON_EXPORT __declspec(dllimport) 
  #endif
#else
  #define VTK_ESQUI_COMMON_EXPORT
#endif

#endif
