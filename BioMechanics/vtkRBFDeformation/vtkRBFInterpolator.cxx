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
#include "vtkRBFInterpolator.h"


#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPolyData.h"
#include "vtkTupleInterpolator.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"

vtkCxxRevisionMacro(vtkRBFInterpolator, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkRBFInterpolator);

//--------------------------------------------------------------------------
vtkRBFInterpolator::vtkRBFInterpolator()
{
  this->Initialized = false;
  this->NumberOfControlPoints = 0;
}

//--------------------------------------------------------------------------
vtkRBFInterpolator::~vtkRBFInterpolator()
{
}

//----------------------------------------------------------------------------
void vtkRBFInterpolator::Init()
{
  if(!this->Initialized)
  {
    vtkPolyData * input = vtkPolyData::SafeDownCast(this->GetInput());

    if(this->NumberOfControlPoints > 0 &&
        this->NumberOfControlPoints < input->GetNumberOfPoints())
    {
      this->Interpolator = vtkTupleInterpolator::New();
      this->Interpolator->SetNumberOfComponents(3);
      this->Interpolator->SetInterpolationTypeToSpline();

      this->controlPoints = vtkDoubleArray::New();
      this->controlPoints->SetNumberOfComponents(3);
      this->controlPoints->SetNumberOfTuples(this->NumberOfControlPoints);

      this->controlPointDisplacements = vtkDoubleArray::New();
      this->controlPointDisplacements->SetNumberOfComponents(3);
      this->controlPointDisplacements->SetNumberOfTuples(this->NumberOfControlPoints);

      int vertsPerPoint = vtkMath::Round(input->GetNumberOfPoints()/this->NumberOfControlPoints);
      //TODO: Select input points from user input
      double p0[3];
      p0[0] = 0; p0[1] = 0; p0[2] = 0;
      for(int i=0; i<this->NumberOfControlPoints; i++)
      {
        double * p = input->GetPoint(i*vertsPerPoint);
        this->controlPoints->InsertNextTuple(p);
        this->controlPointDisplacements->InsertNextTuple(p0);
        //this->Interpolator->AddTuple()
      }

    }
  }
}

//----------------------------------------------------------------------------
int vtkRBFInterpolator::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  //Get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  //Output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  cout << input->GetNumberOfPoints() << endl;

  output->ShallowCopy(input);

  return 1;
}

//--------------------------------------------------------------------------
void vtkRBFInterpolator::PrintSelf(ostream&os, vtkIndent indent)
{
}
