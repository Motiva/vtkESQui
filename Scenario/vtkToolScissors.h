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
#ifndef __vtkToolScissors_h
#define __vtkToolScissors_h

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkToolLaparoscopy.h"

//! Implementation class of surgical laparoscopic scissors.
/*!
 * This class, that inherits from vtkToolLaparascopy, defines surgical
 * scissors, inside the scenario.\n
 * Contains methods for position control of the tool in the scene and
 * collision detection.\n
 * Laparoscopic scissors are composed of 3 elements:\n
 *  - Stick
 *  - 2 Blades.
 */

class VTK_ESQUI_SCENARIO_EXPORT vtkToolScissors: public vtkToolLaparoscopy
{
public:

  //!Type revision macro
  vtkTypeRevisionMacro(vtkToolScissors,vtkToolLaparoscopy);

  //!Create new vtkToolScissors object
  static vtkToolScissors *New();

  //!Return class name
  const char *GetClassName() { return "vtkToolScissors"; }

  //!Print the attributes value
  void PrintSelf(ostream& os, vtkIndent indent);

  //!Initialize the tool from VTK file
  /*!
    This function initializes the tool whose mesh is described on a VTK file
   */
  virtual void Init();

  //! Set the tool's opening according with the value given in the "Opening" parameter
  /*!
   * This function set the tool's opening in function of the value given in the "Opening" parameter.
   * The opening is established modifying the tool's mesh position.
   * \sa double GetOpening()
   */
  void SetOpening(double opening);
  //!Get scissors opening
  /*!
   * \sa SetOpening(double)
   */
  vtkGetMacro(Opening, double);

  //! Open the grasper moving piece actors
  /*!
    Piece actors are transformed to perform tool opening
    Bounding boxes are recalculated.
   */
  void Open();

  //! Close the grasper moving piece actors
  /*!
    Piece actors are transformed to perform tool closing
    Bounding boxes are recalculated.
   */
  void Close();

  //!Return whether the grasper is closed or not
  bool IsClosed(){return (this->Opening <= 0.);}

  //! Sets the grasper' depth in its own coordinate system
  void SetDepth(double position);

  //! Rotate the grasper in degrees from current orientation about the X axis using the right hand rule.
  /*!
   * Scissors specific X-axis rotation.
   */
  void RotateX(double angle);

  //! Rotate the grasper in degrees from current orientation about the Y axis using the right hand rule.
  /*!
   * Scissors specific X-axis rotation.
   */
  void RotateY(double angle);

  //! Rotate the grasper in degrees from current orientation about the Y axis using the right hand rule.
  /*!
   * Scissors specific X-axis rotation.
   */
  void RotateZ(double angle);

  //! Sets the grasper yaw angle
  void Yaw(double angle);

  //! Sets the grasper pitch angle
  void Pitch(double angle);

  //! Sets the grasper roll angle
  void Roll(double angle);

protected:

  vtkToolScissors();
  ~vtkToolScissors();

private:

  vtkToolScissors (const vtkToolScissors &); //Not Implemented
  void operator =(const vtkToolScissors &); //Not Implemented

  //! Tool pieces opening
  /*!
   * The "Opening" parameter can take any value between [0,1]:
   * - "0" -> to close the tool
   * - "1" -> to open the tool
   */
  double Opening;

};
#endif

