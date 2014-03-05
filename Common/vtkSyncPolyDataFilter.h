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
#ifndef __vtkSyncPolyDataFilter_h
#define __vtkSyncPolyDataFilter_h

#include "vtkESQuiCommonWin32Header.h"
#include "vtkPolyDataAlgorithm.h"

class vtkXMLPolyDataReader;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkIdList;
class vtkPointLocator;
class vtkSmoothPolyDataFilter;

//!This class represents an abstract model on a scenario element.
/*!
 * It acts as a base class for visualization, collision and deformation models.
 * vtkSyncPolyDataFilter class has 2 inputs. By default the input data will be read from a
 * vtp file specified in FileName attribute.
 * A second optional parameter is available, the source mesh. Main input will
 * be adapted to the source mesh (point positions will be synchronized)
 * The output displays the transformed mesh according to transformation
 * matrix values.
 */

class VTK_ESQUI_COMMON_EXPORT vtkSyncPolyDataFilter : public vtkPolyDataAlgorithm {

public:
  //! Type revision macro
  vtkTypeRevisionMacro(vtkSyncPolyDataFilter,vtkPolyDataAlgorithm);

  //! Create new model that will be used as a data container
  static vtkSyncPolyDataFilter *New();
  //! Print model info
  void PrintSelf(ostream& os, vtkIndent indent);
  //! Return class name
  const char *GetClassName() {return "vtkSyncPolyDataFilter";};

  //! Specify the source object for mesh synchronization purposes
  /*!
   * This is an optional input. If source is set, the model primary mesh
   * will be synchronized to this source mesh whenever a change is made on it.
   * \sa vtkPolyData* GetSource()
   */
  virtual void SetSource(vtkPolyData *source);

  //! Return the source object for mesh synchronization purposes
  /*!
   * \sa SetSource(vtkPolyData *)
   */
  virtual vtkPolyData *GetSource();

  //! Set filter smoothing
  /*!
   * \sa double GetSmoothing()
   */
  vtkSetMacro(Smoothing, bool);

  //! Get filter visibilty
  /*!
   * \sa double GetSmoothing()
   */
  vtkGetMacro(Smoothing, bool);

  vtkBooleanMacro(Smoothing, bool);

protected:
  vtkSyncPolyDataFilter();
  ~vtkSyncPolyDataFilter();

  virtual int FillOutputPortInformation( int port, vtkInformation* info );
  virtual int FillInputPortInformation(int port, vtkInformation *info);
  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  //! Virtual initialization function
  /*!
   * The model has to be initialized in order to be updated. At least one parameter must be previously defined: \n
   * - Input: vtkPolyData object \n
   */
  void Initialize();

  //! Initialization flag
  bool Initialized;

  //! Point identifier hashmap
  vtkIdList * HashMap;

  //! Point locator
  vtkPointLocator * Locator;

  //! Enable output smoothing
  bool Smoothing;

  //! Smoothing filter
  vtkSmoothPolyDataFilter * SmoothFilter;

private:

  vtkSyncPolyDataFilter (const vtkSyncPolyDataFilter &);//NotImplemented
  void operator =(const vtkSyncPolyDataFilter &);//Not Implemented

};
#endif
