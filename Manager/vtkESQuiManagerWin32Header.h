#ifndef __vtkesquiManagerWin32Header_h
#define __vtkesquiManagerWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32)&& !defined(VTKESQUI_STATIC)

#if defined(vtkESQuiManager_EXPORTS)
//	#undef VTK_ESQUI_MANAGER_EXPORT
#define VTK_ESQUI_MANAGER_EXPORT __declspec(dllexport)
#else
//	#undef VTK_ESQUI_MANAGER_EXPORT
#define VTK_ESQUI_MANAGER_EXPORT __declspec(dllimport)
#endif

#else
#define VTK_ESQUI_MANAGER_EXPORT
#endif

#endif

