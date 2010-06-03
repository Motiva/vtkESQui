/*=========================================================================*/
// .NAME vtk3DSImporterEsqui - imports 3D Studio files for Esqui purpouses.
// .SECTION Description
// vtk3DSImporterEsqui imports 3D Studio files into vtk for ESQUI purpouses.

// .SECTION See Also
// vtk3DSImporter, vtkImporter


#ifndef __vtk3DSImporterEsqui_h
#define __vtk3DSImporterEsqui_h

#include "vtk3DSImporter.h"
#include "vtkESQuiUtilitiesWin32Header.h"
#include "vtkPolyDataCollection.h"
#include "vtkPolyData.h"
#include "vtkActorCollection.h"
#include "vtkTransformPolyDataFilter.h"
// Importa ficheros 3DS para usarlos en el simulador ESQUI

//! Import 3DS files to VTK data structure
class VTK_ESQUI_UTILITIES_EXPORT vtk3DSImporterEsqui : public vtk3DSImporter
{

public:
  
  vtkTypeRevisionMacro(vtk3DSImporterEsqui,vtk3DSImporter);
  static vtk3DSImporterEsqui *New();
  const char *GetClassName() {return "vtk3DSImporterEsqui";};

  // Devuelve una coleccion de actores del fichero 3DS importado
  //! Return a collection of actors from the imported 3DS file
  vtkActorCollection *GetActorCollection();

  //!Set scale factor of the imported objects
  /*!
   * \param scale object scale
   * \sa GetScale()
   */
  void SetScale(float scale);

  //!Return scale factor
  /*!
   * \sa SetScaleFactor(float scale)
   */
  float GetScale();

protected:
  vtk3DSImporterEsqui();
  ~vtk3DSImporterEsqui() {};

  //!Scale of the imported objects
  float Scale;

  //!PolyData actors filter
  vtkTransformPolyDataFilter *Filter;

  //!Collection of Actors
  vtkActorCollection *Actors;
// Importa los actores del fichero 3DS
//! Import the actors from the 3DS file
  virtual void ImportActors (vtkRenderer *renderer);
// Interpreta el fichero 3DS
//! Read the 3DS file
  virtual void ReadData();
private:
  vtk3DSImporterEsqui(const vtk3DSImporterEsqui &);  // Not implemented.
  void operator=(const vtk3DSImporterEsqui &);  // Not implemented.
};

#endif

