#ifndef __vtkMSSConfigure_h
#define __vtkMSSConfigure_h

// Define vtkMSS_SHARED if the library was built shared.
#if 1
# define vtkMSS_SHARED
#endif

// Disable warning caused from static VTK and shared @PROJECT_NAME@.
#if defined(_MSC_VER) && defined(vtkMSS_SHARED)
# pragma warning (disable: 4275) /* non-DLL-interface base class used */
# pragma warning ( disable : 4996 )
#endif

// Setup export/import macro for DLL.  The symbol
// "@PROJECT_NAME@_EXPORTS" is defined by CMake when building source
// files for a shared library named "@PROJECT_NAME@".  For these
// sources we should export if building a shared library.  For other
// sources we should import if using a shared library.
#if defined(_WIN32) && defined(vtkMSS_SHARED)
# if defined(vtkMSS_EXPORTS)
#  define VTK_vtkMSS_EXPORT __declspec(dllexport)
# else
#  define VTK_vtkMSS_EXPORT __declspec(dllimport)
# endif
#else
# define VTK_vtkMSS_EXPORT
#endif

#endif // __vtkMSSConfigure_h
