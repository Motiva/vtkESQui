#ifndef __vtkESQuiHapticsWin32Header_h
#define __vtkESQuiHapticsWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32) &&  !defined(VTKESQUI_STATIC)
#if defined(vtkESQuiHaptics_EXPORTS)
    #define VTK_ESQUI_HAPTICS_EXPORT __declspec(dllexport)
  #else
    #define VTK_ESQUI_HAPTICS_EXPORT __declspec(dllimport)
  #endif
#else
#define VTK_ESQUI_HAPTICS_EXPORT
#endif

#endif
