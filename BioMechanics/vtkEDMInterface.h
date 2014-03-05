/*==========================================================================
--------------------------------------------------
Program: ESQUI
Info and Bugs: {marf,jballesteros}@itccanarias.org
url: http://motivando.me 
--------------------------------------------------

Copyright (c) 2006-2007, Center for Technology in Medicine (CTM), 
University of Las Palmas de Gran Canaria (ULPGC), Canary Islands, Spain.
Copyright (c) 2007-2010, Institute of Technology at CanaryIslands (ITC),
Canary Islands, Spain.

This software is free software; you can redistribute it and/or modify it 
under the terms of the GNU Lesser General Public License (LGPL) as published
by the Free Software Foundation, either version 3 of the License, or (at 
your option) any later version.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2) Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

You should have received a copy of the GNU Lesser General Public License 
along with this program.  If not, see <http://www.gnu.org/licenses/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
==========================================================================*/
#ifndef __vtkEDMInterface_h
#define __vtkEDMInterface_h

#include "vtkESQuiBioMechanicsWin32Header.h"
#include "vtkDeformationModel.h"

#include "vtkDeformableMesh.h"

class vtkPolyDataToImageStencil;
class vtkImageStencil;
class vtkImageMagnitude;
class vtkImageSobel3D;
class vtkDoubleArray;

//! Interface class for a explicit deformation system.
/*!
 * This deformation model uses a level set technique adapting the input mesh
 * to a vtkImageData input. Input data meshes shall be closed surfaces.\n
 * The vtkEDMInterface class, based in vtkDeformationModel class, adapts the access to the
 * external vtkExplicitDeformableSystem package.
 */
class VTK_ESQUI_BIOMECHANICS_EXPORT vtkEDMInterface : public vtkDeformationModel
{
public:
  //! Type Revision Macro
  vtkTypeRevisionMacro(vtkEDMInterface, vtkDeformationModel);
  //! Create a new EDM Interface
  static vtkEDMInterface* New();
  const char *GetClassName() {return "vtkEDMInterface";};
  //! Print class object values
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Process the algorithm request (Update).
  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  //!Initialize the Deformation Model
  /*!
   * The model has to be initialized in order to be updated. Some parameters must be previously defined: \n
   * - Input: vtkPolyData object
   * - NumberOfIterations: Maximum number of iterations
   * - WarpScaleFactor: Deformation scale factor
   */
  virtual void Initialize();

  //!Set maximum number of iterations
  /*!
   * This parameter sets the number of iterations the model will be executed until it reaches an stationary state.
   * \sa int GetNumberOfIterations()
   */
  vtkSetMacro(NumberOfIterations, int);
  //!Get maximum number of iterations
  /*!
   * \sa SetNumberOfIterations(int)
   */
  vtkGetMacro(NumberOfIterations, int);

  //!Set the warp scale factor
  /*!
   * This parameter sets how fast the model recovers its original shape after being deformed.\n
   * Warp scale factor shall be at least 1/100000 the value of spacing or the system will become unstable\n
   * \sa double GetWarpScaleFactor()
   */
  vtkSetMacro(WarpScaleFactor, double);
  //!Get the warp scale factor
  /*!
   * \sa SetWarpScaleFactor(double)
   */
  vtkGetMacro(WarpScaleFactor, double);

  //!Set the image spacing (resolution)
  /*!
   * This parameter sets the image data spacing that sets the resolution.
   * \sa double GetImageSpacing()
   */
  vtkSetVector3Macro(ImageSpacing, double);
  //!Set the image spacing (resolution)
  /*!
   * Image spacing must be at least 1/10 of the object radius
   * \sa SetImageSpacing()
   */
  vtkGetVector3Macro(ImageSpacing, double);

  //!Set homogeneus image spacing (resolution)
  void SetImageSpacing(double spacing);

  //!Set the image source
  /*!
   * Optional input
   */
  void SetImageSource(vtkImageData * image){this->ImageSource = image;};

  //!Get the image source
  vtkImageData * GetImageSource(){return this->ImageSource;};

  //!Add displacement at the specified point.
  /*!
   * The displacement shall not be greater than a 1/10 of the input mesh radius
   * \param id Point identifier
   * \param x displacement x component
   * \param y displacement y component
   * \param z displacement z component
   */
  virtual void InsertDisplacement(vtkIdType id, double x, double y, double z);

  //!Insert a displacement to the model in the specified position
  /*!
   * \param id point identifier
   * \param vector displacement vector
   */
  virtual void InsertDisplacement(vtkIdType id, double * vector);

protected:
  vtkEDMInterface();
  ~vtkEDMInterface();

  //!Explicit deformable mesh filter
  vtkDeformableMesh * DeformableMesh;

  //!Source mesh polydata
  vtkPolyData * Deformed;

  //!Force vector computation
  vtkImageSobel3D * ForceFilter;

  //!Magnitude filter
  vtkImageMagnitude * MagnitudeFilter;

  //!Sobel filter
  vtkImageSobel3D * SobelFilter;

  //!Image data
  vtkImageData * ImageData;

  //!Generated Image Source
  vtkImageData * ImageSource;

  //!Image stencil filter
  vtkPolyDataToImageStencil * ImageStencilFilter;

  //!Image stencil
  vtkImageStencil * Stencil;

  //!Image origin
  double ImageOrigin[3];

  //!Image spacing
  double ImageSpacing[3];

  //!Image extent
  int ImageExtent[6];

  //! Mesh reset flag
  bool Reset;

  //! Number of iterations
  int NumberOfIterations;

  //! Warp scaling factor
  double WarpScaleFactor;

private:
  vtkEDMInterface(const vtkEDMInterface&);  // Not implemented.
  void operator=(const vtkEDMInterface&);  // Not implemented.
  
};

#endif
