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
#ifndef _vtkEsquiHaptics_h
#define _vtkEsquiHaptics_h

#include "vtkESQuiHapticsWin32Header.h"
#include "vtkHaptic.h"

class vtkLSWTool;

class ImmSurgicalWorkstation;


static const unsigned VTK_CORRECT_LSW = 1;
static const unsigned VTK_DETECT_FAILURE_LSW = 0;
static const unsigned VTK_CALIBRATE_FAILURE_LSW = -1;

//! vtkLSW class wraps the access to the LSW haptic device.
/*!
 * This interface enables the interaction with the Immersion LSW haptic
 * device.\n
 * In a cyclic process the haptic is synchronously checked for changes.
 * Device physical attributes are saved and then sent to the simulation
 * process.\n
 * Access method to device initialization has also been implemented.
 */

class VTK_ESQUI_HAPTICS_EXPORT vtkLSW:public vtkHaptic
{

public:
  //! Type revision macro
  vtkTypeRevisionMacro(vtkLSW, vtkHaptic);
  //! Create new vtkLSW object
  static vtkLSW *New();
  //! Return class name
  const char *GetClassName() { return "vtkLSW"; }
  //! Print object info
  void PrintSelf(ostream& os, vtkIndent indent);

  //! Set up the LSW
  virtual int Init();

  //!Update the device
  virtual void Update();

  //! Close the communication and turn off the LSW
  virtual void Release();

  //!Update the device
  void UpdateDevice();

  //! Get the tool depth
  double GetToolDepth(int id);
  //! Get the tool yaw
  double GetToolYaw(int id);
  //! Get the tool pitch
  double GetToolPitch(int id);
  //! Get the tool aperture
  double GetToolOpening(int id);
  //! Get the tool twist
  double GetToolRoll(int id);

  //! Get the left tool depth
  double GetLeftToolDepth();
  //! Get the left tool yaw
  double GetLeftToolYaw();
  //! Get the left tool pitch
  double GetLeftToolPitch();
  //! Get the left tool aperture
  double GetLeftToolOpening();
  //! Get the left tool twist
  double GetLeftToolRoll();

  //! Get the right tool depth
  double GetRightToolDepth();
  //! Get the right tool yaw
  double GetRightToolYaw();
  //! Get the right tool pitch
  double GetRightToolPitch();
  //! Get the right tool aperture
  double GetRightToolOpening();
  //! Get the right tool twist
  double GetRightToolRoll();

  //! Set the specified tool contact force
  void SetToolForce(int id, float X, float Y, float Z);
  //! Set the right tool contact force
  void SetRightToolForce(float X, float Y, float Z);
  //! Set the left tool contact force
  void SetLeftToolForce(float X, float Y, float Z);

  //!Set data into the specified tool
  void SetToolData(int id, int Type, double k, int flag);
  //!Set data into the left tool
  void SetLeftToolData(int Type, double k, int flag);
  //!Set data into the left tool
  void SetRightToolData(int Type, double k, int flag);
private:

  bool StopFlag;
  bool ForceCalculationMethodActive;

  //! vtkLSWTool object to access to the LSW's right tool
  vtkLSWTool* LSWRightTool;

  //! vtkLSWTool object to access to the LSW's left tool
  vtkLSWTool* LSWLeftTool;

  //! ImmSurgicalWorkstation object.
  ImmSurgicalWorkstation* LSWDevice;

protected:

  vtkLSW();
  ~vtkLSW();
};

#endif
