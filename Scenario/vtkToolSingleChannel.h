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
#ifndef __vtkToolSingleChannel
#define __vtkToolSingleChannel

#include "vtkESQuiScenarioWin32Header.h"
#include "vtkTool.h"

//! vtkToolSingleChannel class implements a surgical rigid endoscopy tool.
/*!
 * This class inherits from vtkTool, and serves as base class for specific
 * rigid endoscopy technique tools.
 * A few rigid endoscopy specific movements (rotation, insertion, opening etc...)
 * has been defined to control the tool.
 */
class VTK_ESQUI_SCENARIO_EXPORT vtkToolSingleChannel: public vtkTool {
  
public:

  //! Type revision macro
  vtkTypeRevisionMacro(vtkToolSingleChannel, vtkTool);

  //! Create new vtkToolSingleChannel object
  static vtkToolSingleChannel *New();

  //! Return the class name
  const char *GetClassName() {return "vtkToolSingleChannel";}

  //! Print class values
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Tool type definition
  enum vtkToolSingleChannelModel{
    Cauterizer = 0,
    Brush,
    Cutter,
    Camera
  };

  //! Set tool model
  void SetToolModel(unsigned int Value);
  //vtkSetMacro(ToolModel, vtkToolSingleChannel::vtkToolSingleChannelModel);

  //! Return tool model
  unsigned int GetToolModel();
  //vtkGetMacro(ToolModel, vtkToolSingleChannel::vtkToolSingleChannelModel);

  //! Set Attached flag
  vtkSetMacro(Attached, bool);
  //! Return Attached flag
  vtkGetMacro(Attached, bool);

  //! Initialization method
  /*!
   * This method initializes the tool physical values, scale, position, etc...
   */
  virtual void Initialize();

  //! Assign the haptic force as vector
  /*!
   * \param Force force vector of components
   */
  void SetForceFeedback(float Force[]);

  //! Assign the haptic force components individually
  /*!
   * \param x x component of the force
   * \param y y component of the force
   * \param z z component of the force
   */
  void SetForceFeedback(float x, float y, float z) {
    this->ForceFeedback[0] = x;
    this->ForceFeedback[1] = y;
    this->ForceFeedback[2] = z;
  }

  //! Return the haptic force in float vector
  /*!
   * \sa SetForceFeedback(float Force[])
   * \sa SetForceFeedback(float x, float y, float z)
   */
  void GetForceFeedback( float Force[] ) const
  {
    Force[0] = this->ForceFeedback[0];
    Force[1] = this->ForceFeedback[1];
    Force[2] = this->ForceFeedback[2];
  }

  //! Return the x component of the haptic force
  /*!
   * \sa SetForceFeedback(float Force[])
   * \sa SetForceFeedback(float x, float y, float z)
   */
  float GetXFeedbackForce() {return this->ForceFeedback[0];}
  //! Return the y component of the haptic force
  /*!
   * \sa SetForceFeedback(float Force[])
   * \sa SetForceFeedback(float x, float y, float z)
   */
  float GetYFeedbackForce() {return this->ForceFeedback[1];}
  //! Return the z component of the haptic force
  /*!
   * \sa SetForceFeedback(float Force[])
   * \sa SetForceFeedback(float x, float y, float z)
   */
  float GetZFeedbackForce() {return this->ForceFeedback[2];}

  //! Return tool contact force
  double * GetContactForceValue();

  //! Pull back tool
  /*!
   * Set the tool depth position back.
   */
  void Pull();

  //! Pull back tool
  /*!
   * Set the tool depth position forward.
   */
  void Push();

  //! Sets the tool's depth in its own coordinate system
  void SetDepth(double depth);

  //!Return the tool's depth
  double GetDepth();

  //! Rotate the tool in degrees from current orientation about the X axis using the right hand rule.
  void RotateX(double angle);

  //! Rotate the tool in degrees from current orientation about the Y axis using the right hand rule.
  void RotateY(double angle);

  //! Rotate the tool in degrees from current orientation about the Z axis using the right hand rule.
  void RotateZ(double angle);

  //! Rotates the tool orientation angX, angY and angZ degrees.
  void RotateXYZ(double angX, double angY, double angZ);

  //! Sets the tool yaw angle
  void Yaw(double angle);

  //! Sets the tool pitch angle
  void Pitch(double angle);

  //! Sets the tool roll angle
  void Roll(double angle);

  //! Sets the tool orientation.
  void RollPitchYaw(double NewRoll, double NewPitch, double NewYaw);

protected:

  //! Update tool orientation with these diferential angles.
  void UpdateTool(double dPitch, double dYaw, double dRoll);

  vtkToolSingleChannel();
  ~vtkToolSingleChannel();

  //! Tool Type
  vtkToolSingleChannelModel ToolModel;

  //! Force Feedabck unit vector
  double ForceFeedback[3];
  //! Tool depth (vtk z-axis direction)
  double Depth;
  //! Tool Yaw Angle. VTK Y-axis about rotation
  double YawAngle;
  //! Tool Pitch Angle. VTK X-axis about rotation
  double PitchAngle;
  //! Tool Roll Angle. VTK Z-axis about rotation
  double RollAngle;

  //! Determines if the tool rolls free or attached to the camera
  bool Attached;
  //! Stores the difference between camera roll angle and tool roll angle.
  double DiffRoll;

private:
  vtkToolSingleChannel (const vtkToolSingleChannel &);
  void operator =(const vtkToolSingleChannel &);
};

#endif
