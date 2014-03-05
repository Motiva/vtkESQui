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
#include "vtkHaptic.h"
#include "vtkObjectFactory.h"

#include "vtkScenarioObjectCollection.h"
#include "vtkScenarioObject.h"
#include "vtkTool.h"
#include "vtkToolSingleChannel.h"

vtkCxxRevisionMacro(vtkHaptic, "$Revision: 0.1 $");

//--------------------------------------------------------------------------
vtkHaptic::vtkHaptic() {
	this->Name = "";
	this->Tools = vtkScenarioObjectCollection::New();
	this->NumberOfTools = 0;
	this->SingleChannel = false;
	this->Camera = NULL;
	this->CameraObject = NULL;
	this->LastRoll = this->LastPitch = this->LastYaw = 0.0;
	this->CameraOrigin = new double[3];
	this->CameraOrigin[0] = this->CameraOrigin[1] = this->CameraOrigin[2] = 0.0;
	this->LensAngle = 0.0;
}

//--------------------------------------------------------------------------
vtkHaptic::~vtkHaptic() {
	this->Tools->Delete();
	delete[] this->CameraOrigin;
}

//----------------------------------------------------------------------------
void vtkHaptic::SetTools(vtkScenarioObjectCollection * collection)
{
	this->Tools->RemoveAllItems();

	collection->InitTraversal();
	while(vtkScenarioObject * tool = collection->GetNextObject())
	{
		if(tool->GetType() == vtkScenarioObject::Tool)
		{
			this->AddTool(vtkTool::SafeDownCast(tool));
		}
	}

}

//----------------------------------------------------------------------------
vtkScenarioObjectCollection * vtkHaptic::GetTools()
{
	return this->Tools;
}

//----------------------------------------------------------------------------
void vtkHaptic::AddTool(vtkTool * tool)
{
	this->Tools->AddObject(tool);
}

//----------------------------------------------------------------------------
void vtkHaptic::ReplaceTool(vtkIdType index, vtkTool* tool)
{
	this->Tools->ReplaceObject(index, tool);
}

//----------------------------------------------------------------------------
void vtkHaptic::RemoveTool(vtkIdType index)
{
	this->Tools->RemoveItem(index);
}

//----------------------------------------------------------------------------
vtkIdType vtkHaptic::GetNumberOfTools()
{
	return this->Tools->GetNumberOfItems();
}

//----------------------------------------------------------------------------
vtkTool * vtkHaptic::GetTool(vtkIdType id)
{
	return vtkTool::SafeDownCast(this->Tools->GetObject(id));
}

//----------------------------------------------------------------------------
void vtkHaptic::SetCameraObject(vtkToolSingleChannel *object)
{
	if(object != NULL && object->GetToolModel() == vtkToolSingleChannel::Camera)
	{
		this->CameraObject = object;
	}
	else
	{
		vtkErrorMacro("object is not a vtkToolSingleChannel::Camera object");
	}
}

//----------------------------------------------------------------------------
void vtkHaptic::SetLensAngle(double angle)
{
	if (angle >= 0 && angle <= 90)
	{
		this->LensAngle = angle;
	}
}

//--------------------------------------------------------------------------
void vtkHaptic::PrintSelf(ostream& os,vtkIndent indent) {

	this->Superclass::PrintSelf(os, indent);

	os << indent << "Name: " << this->Name << std::endl;

}
