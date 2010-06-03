#ifndef __vtkesquiScenarioWin32Header_h
#define __vtkesquiScenarioWin32Header_h

#include "vtkESQuiConfigure.h"

#if defined(WIN32)&& !defined(VTKESQUI_STATIC)
  #if defined(vtkESQuiScenario_EXPORTS)
    #define VTK_ESQUI_SCENARIO_EXPORT __declspec(dllexport)
  #else
	#undef VTK_ESQUI_SCENARIO_EXPORT
	#define VTK_ESQUI_SCENARIO_EXPORT __declspec( dllimport ) 
  #endif

#else
  #define VTK_ESQUI_SCENARIO_EXPORT
#endif

#endif
