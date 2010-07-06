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
#include "vtkRDMInterface.h"

vtkCxxRevisionMacro(vtkRDMInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkRDMInterface);

//----------------------------------------------------------------------------
vtkRDMInterface::vtkRDMInterface()
{
	this->RDMMesh = vtkRobustDeformationModel::New();
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->Mass = 0;
	this->DeltaT = 0;
}

//----------------------------------------------------------------------------
vtkRDMInterface::~vtkRDMInterface()
{
	this->RDMMesh->Delete();
	this->Mesh->Delete();
}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkRDMInterface::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {

    // Get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // Get the input and output
    vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
                                     inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
                                      outInfo->Get(vtkDataObject::DATA_OBJECT()));

    //std::cout << "vtkRDMInterface::RequestData" << endl;

    if (input->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE)
    {
    	//Set input mesh where deformation will be calculated
    	vtkPoints * ps = vtkPoints::New();
    	ps->SetDataTypeToDouble();
    	ps->DeepCopy(input->GetPoints());
    	input->SetPoints(ps);
    	input->Update();
    }

    this->RDMMesh->SetInput(input);
    this->RDMMesh->SetContacts(this->ContactPointIds, this->ContactDirections);

    this->RDMMesh->Update();

    output->DeepCopy(this->RDMMesh->GetOutput());

    this->Clear();

    return 1;
}

//--------------------------------------------------------------------------
void vtkRDMInterface::Init()
{
	//Set Mass-Spring System parameters
	this->RDMMesh->SetDistanceForceCoefficient(this->DistanceCoefficient);
	this->RDMMesh->SetDamping(this->DampingCoefficient);//Friction
	this->RDMMesh->SetSurfaceForceCoefficient(this->SurfaceCoefficient);
	this->RDMMesh->SetVolumeForceCoefficient(this->VolumeCoefficient);
	this->RDMMesh->SetInput(vtkUnstructuredGrid::SafeDownCast(this->GetInput()));
	this->RDMMesh->Init();

	this->Print(cout);
}

//--------------------------------------------------------------------------
void vtkRDMInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent <<  "DistanceCoefficient: " << this->DistanceCoefficient << endl;
  os << indent <<  "DampingCoefficient: " << this->DampingCoefficient << endl;
  os << indent <<  "Mass: " << this->Mass << endl;
  os << indent <<  "DeltaT: " << this->DeltaT << endl;
}
