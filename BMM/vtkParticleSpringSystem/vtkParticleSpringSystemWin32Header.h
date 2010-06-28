// .NAME vtkParticleSpringSystemWin32Header - manage Windows system differences
// .SECTION Description
// The vtkParticleSpringSystemWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkParticleSpringSystemWin32Header_h
#define __vtkParticleSpringSystemWin32Header_h

#include <vtkParticleSpringSystemConfigure.h>

#if defined(_WIN32)
  #if defined(vtkParticleSpringSystem_EXPORTS)
    #define VTK_vtkParticleSpringSystem_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkParticleSpringSystem_EXPORT
	#define VTK_vtkParticleSpringSystem_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_vtkParticleSpringSystem_EXPORT
#endif

#endif
