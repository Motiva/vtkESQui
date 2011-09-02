// .NAME vtkCUDAParticleSystemWin32Header - manage Windows system differences
// .SECTION Description
// The vtkCUDAParticleSystemWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkCUDAParticleSystemWin32Header_h
#define __vtkCUDAParticleSystemWin32Header_h

#include <vtkCUDAParticleSystemConfigure.h>

#if defined(_WIN32)
  #if defined(vtkCUDAParticleSystem_EXPORTS)
    #define VTK_vtkCUDAParticleSystem_EXPORT __declspec(dllexport)
  #else
	#undef VTK_vtkCUDAParticleSystem_EXPORT
	#define VTK_vtkCUDAParticleSystem_EXPORT __declspec( dllimport )
  #endif

#else
  #define VTK_vtkCUDAParticleSystem_EXPORT
#endif

#endif
