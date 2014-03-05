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
#ifndef __vtkSingleChannelInteractorStyle_h
#define __vtkSingleChannelInteractorStyle_h

#include "vtkESQuiInteractionWin32Header.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkTransform.h"

class vtkIdList;
class vtkCamera;
class vtkScenario;

//! Implementation of an interactor style for rigid endoscopy through single channel.
/*!
 * Defines a standard keyboard layout for simulation control in rigid 
 * endoscopy techniques through single channel.
 * Mouse and keyboard events are handled and further actions, such as
 * rotations or translations, implemented.
 */
class VTK_ESQUI_INTERACTION_EXPORT vtkSingleChannelInteractorStyle : public vtkInteractorStyleTrackballCamera {

public:
  //! Type revision macro
  vtkTypeRevisionMacro(vtkSingleChannelInteractorStyle,vtkInteractorStyleTrackballCamera);

  //! Create new vtkSingleChannelInteractorStyle object
  static vtkSingleChannelInteractorStyle *New();

  //! Return class name
  const char *GetClassName() {return "vtkSingleChannelInteractorStyle";};

  //! Initialization of the simulation style
  void Initialize();

  //! Handle onKeyPress event
  virtual void OnKeyPress();

  //! Handle onMouseMove event
  virtual void OnMouseMove();

  //! Handle onLeftButtonDown event
  virtual void OnLeftButtonDown();
  //! Handle onLeftButtonUp event
  virtual void OnLeftButtonUp();

  //! Handle onMiddleButtonDown event
  virtual void OnMiddleButtonDown();
  //! Handle onMiddleButtonUp event
  virtual void OnMiddleButtonUp();

  //! Handle onRightButtonDown event
  virtual void OnRightButtonDown();
  //! Handle onRightButtonUp event
  virtual void OnRightButtonUp();

  //! Handle OnMouseWheelForward event
  virtual void OnMouseWheelForward();
  //! Handle OnMouseWheelBackward event
  virtual void OnMouseWheelBackward();
  
  //! Change tool
  //\returns True if tool changed succesfully.
  bool ChangeTool(int ToolId);

  //! Set interactor scenario
  /*!
   * \sa SetScenario SetScenario(vtkScenario *)
   */
  vtkSetMacro(Scenario, vtkScenario *);

  //! Get interactor scenario
  /*!
   * \sa vtkScenario * GetScenario()
   */
  vtkGetMacro(Scenario, vtkScenario *);

  //! Set currently active tool id
  /*!
   * \sa int GetActiveToolId()
   */
  vtkSetMacro(ActiveToolId, vtkIdType);
  //! Get currently active tool id
  /*!
   * \sa SetActiveToolId(int)
   */
  vtkGetMacro(ActiveToolId, vtkIdType);

  //! Set movement scale factor
  /*!
   * \sa double GetScale()
   */
  vtkSetMacro(Scale, double);
  //! Get movement scale
  /*!
   * \sa SetScale(double)
   */
  vtkGetMacro(Scale, double);

  //! Set lens inclination
  /*!
   * \sa double GetLensAngle()
   */
  void SetLensAngle(double angle);
  //! Get lens inclination
  /*!
   * \sa SetLensAngle(double)
   */
  vtkGetMacro(LensAngle, double);

protected:
  vtkSingleChannelInteractorStyle();
  ~vtkSingleChannelInteractorStyle();

  // Returns stroed tool identifier
  int GetToolId(int id);

  //! Number of tools to be handled
  int GetNumberOfTools();

  //! Scenario to be controled
  vtkScenario * Scenario;

  //! Scenario camera
  vtkCamera * Camera;

  //! Last mouse event position
  int PreviousPosition[2];

  //! Movement scale
  double Scale;

  //! Left button pressed flag
  bool LeftButtonPressed;
  //! Right button pressed flag
  bool RightButtonPressed;

  //! Tool id list
  vtkIdList * ToolIds;
  //! Active tool identifier
  vtkIdType ActiveToolId;
  //! Initialization flag
  bool Initialized;
  
  //! Id of Camera-tool object
  int CamToolId;
  //! Camera depth
  double CamDepth;
  //! Camera lens Roll
  double Roll;
  //! Camera/Tool Pitch
  double Pitch;
  //! Camera/Tool Yaw
  double Yaw;
  //! Camera rotation axis
  double *CamOrigin;
  //! Lens angle in degrees
  double LensAngle;
  
  //! Performs the same translation on every tool
  void TranslateAllTools(double step, bool shift);
  //! Performs the same Z-rotation on every tool
  void RotateZAllTools(double angle);
  //! Update position of every tool, including Camera-Tool
  void UpdateAllTools(double YawStep, double PitchStep);
  //! Rotate the lens to the desired position
  void RotateLens(double angle);  

private:
  vtkSingleChannelInteractorStyle (const vtkSingleChannelInteractorStyle &);//NotImplemented
  void operator =(const vtkSingleChannelInteractorStyle &);//Not Implemented
};
#endif
