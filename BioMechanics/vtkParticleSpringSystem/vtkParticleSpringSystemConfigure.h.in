#ifndef __vtkParticleSpringSystemConfigure_h
#define __vtkParticleSpringSystemConfigure_h

// Define vtkParticleSpringSystem_SHARED if the library was built shared.
#if 1
# define vtkParticleSpringSystem_SHARED
#endif

// Disable warning caused from static VTK and shared @PROJECT_NAME@.
#if defined(_MSC_VER) && defined(vtkParticleSpringSystem_SHARED)
# pragma warning (disable: 4275) /* non-DLL-interface base class used */
# pragma warning ( disable : 4996 )
#endif

// Setup export/import macro for DLL.  The symbol
// "@PROJECT_NAME@_EXPORTS" is defined by CMake when building source
// files for a shared library named "@PROJECT_NAME@".  For these
// sources we should export if building a shared library.  For other
// sources we should import if using a shared library.
#if defined(_WIN32) && defined(vtkParticleSpringSystem_SHARED)
# if defined(vtkParticleSpringSystem_EXPORTS)
#  define VTK_vtkParticleSpringSystem_EXPORT __declspec(dllexport)
# else
#  define VTK_vtkParticleSpringSystem_EXPORT __declspec(dllimport)
# endif
#else
# define VTK_vtkParticleSpringSystem_EXPORT
#endif

#endif // __vtkParticleSpringSystemConfigure_h
