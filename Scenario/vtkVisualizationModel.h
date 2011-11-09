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
#ifndef __vtkVisualizationModel
#define __vtkVisualizationModel

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkModel.h"

class vtkTexture;
class vtkJPEGReader;
class vtkTextureMapToSphere;
class vtkTransformTextureCoords;


//! vtkVisualizationModel object defines a visualization mesh (vtkPolyData)
/*!
 * This class inherits from vtkModel base class. As it is specified in vtkModel,
 * at least an input mesh should be defined. Optionally a source mesh for
 * synchronization purposes may be defined.
 * To enhance visualization user experience a mesh texture can be specified
 * through the TextureFileName attribute.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkVisualizationModel: public vtkModel {

public:

  //! Type revision macro
  vtkTypeRevisionMacro(vtkVisualizationModel, vtkModel);
  //! Create a new Scenario object
  static vtkVisualizationModel * New();

  //! Return the class name
  const char *GetClassName() {return "vtkVisualizationModel";}

  //! Print class values
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Assign texture model filename
  /*!
   *\sa GetTextureFileName()
   */
  vtkSetStringMacro(TextureFileName);

  //! Return model texture filename
  /*!
   *\sa SetTextureFileName(const char * name)
   */
  vtkGetStringMacro(TextureFileName);

  //! Abstract initialization function
  /*!
   * This method initializes the tool physical values, scale, position, etc...
   */
  virtual void Initialize();

protected:

  vtkVisualizationModel();
  ~vtkVisualizationModel();

  //! Process the algorithm request (Update).
  //virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkVisualizationModel (const vtkVisualizationModel &);
  void operator =(const vtkVisualizationModel &);

  //! Visualization texture filename
  char * TextureFileName;

  //! Visualization mesh texture
  vtkTexture * Texture;

  //! Texture file reader
  vtkJPEGReader * TextureReader;

  //! Texture mapper
  vtkTextureMapToSphere * TextureMap;

  //! Transformation of texture coords
  vtkTransformTextureCoords * TextureCoords;

};

#endif
