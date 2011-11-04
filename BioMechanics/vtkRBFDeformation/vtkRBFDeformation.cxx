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
#include "vtkRBFDeformation.h"


#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPolyData.h"
#include "vtkTupleInterpolator.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkIdList.h"
#include "vtkMath.h"

vtkCxxRevisionMacro(vtkRBFDeformation, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkRBFDeformation);

//--------------------------------------------------------------------------
vtkRBFDeformation::vtkRBFDeformation()
{
  this->Initialized = false;
  this->NumberOfControlPoints = 0;
  this->RelaxationFactor = 0.0;

  this->controlPointIds = NULL;
  this->controlPointStatus = NULL;
}

//--------------------------------------------------------------------------
vtkRBFDeformation::~vtkRBFDeformation()
{
}

//----------------------------------------------------------------------------
void vtkRBFDeformation::Init()
{
  if(!this->Initialized)
  {
    vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());

    if(this->NumberOfControlPoints > 0 &&
        this->NumberOfControlPoints < input->GetNumberOfPoints())
    {

      //this->controlPoints = vtkDoubleArray::New();
      //this->controlPoints->SetNumberOfComponents(3);
      //this->controlPoints->SetNumberOfTuples(this->NumberOfControlPoints);

      //this->controlPointDisplacements = vtkDoubleArray::New();
      //this->controlPointDisplacements->SetNumberOfComponents(3);
      //this->controlPointDisplacements->SetNumberOfTuples(this->NumberOfControlPoints);

      this->controlPointIds = vtkIdList::New();
      this->controlPointStatus = vtkIntArray::New();

      int vertsPerPoint = input->GetNumberOfPoints()/this->NumberOfControlPoints;
      //TODO: Select input points from user input
      //double p0[3];
      //p0[0] = 0; p0[1] = 0; p0[2] = 0;
      for(int i=0; i<this->NumberOfControlPoints; i++)
      {
        int id = i*vertsPerPoint;
        double * p = input->GetPoint(id);
        this->controlPointIds->InsertNextId(id);
        this->controlPointStatus->InsertNextTuple1(0);
        //this->controlPoints->InsertNextTuple(p);
        //this->controlPointDisplacements->InsertNextTuple(p0);
        //Set control point positions
        this->controlPointX.push_back(p[0]);
        this->controlPointY.push_back(p[1]);
        this->controlPointZ.push_back(p[2]);
        //Initialize control point displacements to 0
        this->controlPointDisplacementX.push_back(0.0);
        this->controlPointDisplacementY.push_back(0.0);
        this->controlPointDisplacementZ.push_back(0.0);
      }

      this->rbfX = new RBFInterpolator(controlPointX, controlPointY, controlPointZ, controlPointDisplacementX);
      this->rbfY = new RBFInterpolator(controlPointX, controlPointY, controlPointZ, controlPointDisplacementY);
      this->rbfZ = new RBFInterpolator(controlPointX, controlPointY, controlPointZ, controlPointDisplacementZ);

    }
  }
}

//----------------------------------------------------------------------------
int vtkRBFDeformation::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  //Get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  //Output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  output->DeepCopy(input);

  // update the control points based on the new control point positions
  this->rbfX->UpdateFunctionValues(controlPointDisplacementX);
  this->rbfY->UpdateFunctionValues(controlPointDisplacementY);
  this->rbfZ->UpdateFunctionValues(controlPointDisplacementZ);

  double po[3];
  vtkPoints * points = output->GetPoints();
  for(int i=0; i<output->GetNumberOfPoints(); i++)
  {
    double * pi = input->GetPoint(i);
    double * po = points->GetPoint(i);
    //Po = Pi + interpolate(i)
    points->SetPoint(i, pi[0] + this->rbfX->interpolate(pi[0], pi[1], pi[2]),
        pi[1] + this->rbfY->interpolate(pi[0], pi[1], pi[2]),
        pi[2] + this->rbfZ->interpolate(pi[0], pi[1], pi[2]));

    //if (i == 24) {
    //  cout << pi[0] << ", " << pi[1] << ", " << pi[2] << endl;
    //  cout << po[0] << ", " << po[1] << ", " << po[2] << endl;
    //}
  }

  for(int i=0; i<this->NumberOfControlPoints; i++)
  {
    //Check if point is in contact
    if(this->controlPointStatus->GetValue(i) == 0)
    {
      //Apply relaxation factor
      double f = (1 - this->RelaxationFactor);
      //if(i==2) {
      //  cout << this->controlPointDisplacementY[i] << " f:" << f << endl;
      //}
      this->controlPointDisplacementX[i] *= f;
      this->controlPointDisplacementY[i] *= f;
      this->controlPointDisplacementZ[i] *= f;
    }
    else //Reset to 0
      this->controlPointStatus->SetValue(i, 0);
  }
  return 1;
}

//--------------------------------------------------------------------------
void vtkRBFDeformation::SetDisplacement(int i, double * d)
{
  //Adjust displacement to original point position
  if (i < this->GetNumberOfControlPoints())
  {
    //output mesh point position
    int id = this->controlPointIds->GetId(i);
    double * p = this->GetOutput()->GetPoint(id);

    //Add actual point position to displacement
    this->controlPointDisplacementX[i] = d[0] + (p[0] - this->controlPointX[i]);
    this->controlPointDisplacementY[i] = d[1] + (p[1] - this->controlPointY[i]);
    this->controlPointDisplacementZ[i] = d[2] + (p[2] - this->controlPointZ[i]);

    //Set point as contacted
    this->controlPointStatus->SetValue(i, 1);

    /*cout << i << " -> d: " << d[0] << ", " << d[1] << ", " << d[2] << "\ts: "
      << controlPointDisplacementX[i] << ", "
      << controlPointDisplacementY[i] << ", "
      << controlPointDisplacementZ[i] << endl;*/

    //Raise update event
    this->Modified();
  }
}

//--------------------------------------------------------------------------
void vtkRBFDeformation::PrintSelf(ostream&os, vtkIndent indent)
{
}
