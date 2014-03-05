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
#ifndef __vtkCollisionModel
#define __vtkCollisionModel

#include "vtkESQuiCollisionDetectionWin32Header.h"
#include "vtkModel.h"

class vtkPolyDataNormals;
class vtkCollision;
class vtkCollisionCollection;
class vtkSphereSource;
class vtkGlyph3D;
class vtkFloatArray;

//! vtkCollisionModel object defines a collision model based on a mesh (vtkPolyData).
/*!
 * This class inherits from vtkModel base class. As it is specified in vtkModel,
 * at least an input mesh should be defined. Optionally a source mesh for
 * synchronization purposes may be defined.
 * Collision model must be synchronized with deformation model if present.
 */

class VTK_ESQUI_COLLISIONDETECTION_EXPORT vtkCollisionModel: public vtkModel {

public:

  //!Type revision macro
  vtkTypeRevisionMacro(vtkCollisionModel, vtkModel);
  //!Create a new Collision Model object
  static vtkCollisionModel * New();

  //!Return the class name
  const char *GetClassName() {return "vtkCollisionModel";}

  //!Print class values
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  //! Collision model object type
  enum vtkCollsionObjectType
  {
    Tool = 0,
    Organ = 1
  };
  //ETX

  //!Virtual initialization function
  /*!
   * The model has to be initialized in order to be updated. At least one parameter must be previously defined: \n
   * - Input: vtkPolyData object \n
   */
  virtual void Initialize();

  //!Set the model direction (WXYZ)
  vtkSetVector3Macro(Direction, double);
  //!Get the model direction (WXYZ)
  vtkGetVector3Macro(Direction, double);
  //!Set the model velocity (WXYZ)
  vtkSetVector3Macro(Velocity, double);
  //!Get the model velocity (WXYZ)
  vtkGetVector3Macro(Velocity, double);
  //!Set the model acceleration (WXYZ)
  vtkSetVector3Macro(Acceleration, double);
  //!Get the model acceleration (WXYZ)
  vtkGetVector3Macro(Acceleration, double);
  //!Set the model delta T
  vtkSetMacro(DeltaT, double);
  //!Get the model delta T
  vtkGetMacro(DeltaT, double);

  //!Set the visualization sphere radius
  vtkSetMacro(Radius, double);
  //!Get the visualization sphere radius
  vtkGetMacro(Radius, double);

  //!Set display of collision by color
  vtkSetMacro(DisplayCollisions, bool);
  //!Get display of collision by color
  vtkGetMacro(DisplayCollisions, bool);

  vtkBooleanMacro(DisplayCollisions, bool);

  //!Set detected collisions on the model
  /*!
   * Any previously added collision will be removed from the collection
   * \param collisions collection of collisions
   * \sa vtkCollisionCollection * GetCollisions()
   */
  void SetCollisions(vtkCollisionCollection * collisions);
  //!Get detected collisions on the model
  /*!
   * \return pointer to collection of collisions
   * \sa SetCollisions(vtkCollisionCollection * collisions);
   */
  vtkCollisionCollection * GetCollisions();

  //!Add a single collision to the model
  /*!
   * Previously added collisions remain in the collection
   * \param c collision to be added to the collection
   * \sa RemoveCollision(vtkIdType id)
   */
  void AddCollision(vtkCollision * c);

  //!Remove the collision at the specified identifier
  /*!
   * \param id collision identifier
   * \sa AddCollision(vtkCollision * c)
   */
  void RemoveCollision(vtkIdType id);

  //!Remove all collisions from the model
  void RemoveAllCollisions();

  //! Return total number of model collisions
  int GetNumberOfCollisions();

  //!Return cell normals of a collision model
  vtkFloatArray * GetCellNormals();

  //!Return point normals of a collision model
  vtkFloatArray * GetPointNormals();

protected:

  vtkCollisionModel();
  ~vtkCollisionModel();

  //! Process the algorithm request (Update).
  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  //! Unit direction vector
  double Direction[3];
  //! Velocity vector
  double Velocity[3];
  //! Acceleration vector
  double Acceleration[3];
  //! Simulation time step
  double DeltaT;

  //! Glyph sphere radius
  double Radius;

  //! Display collision points
  bool DisplayCollisions;

  //! Surface normals for distance field computation
  vtkPolyDataNormals *Normals;

  //! Sphere source for glyphs
  vtkSphereSource * Sphere;

  //! Glyphs display of points
  vtkGlyph3D * Glyphs;

  //! Point scalars
  vtkDoubleArray * Scalars;
  
  //! Color lookup table
  vtkLookupTable * LUT;

  //! Collection of collisions
  vtkCollisionCollection * Collisions;

  //BTX
  //! Object type
  vtkCollsionObjectType * ObjectType;
  //ETX

private:
  vtkCollisionModel (const vtkCollisionModel &);
  void operator =(const vtkCollisionModel &);

};

#endif
