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
#include "vtkMSSInterface.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkDelaunay3D.h"

#include "vtkMSS.h"

vtkCxxRevisionMacro(vtkMSSInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkMSSInterface);

//----------------------------------------------------------------------------
vtkMSSInterface::vtkMSSInterface()
{
	this->MSSMesh = vtkMSS::New();
	this->DistanceCoefficient = 0;
	this->DampingCoefficient = 0;
	this->Mass = 0;
	this->DeltaT = 0;
	this->Steps = 0;
}

//----------------------------------------------------------------------------
vtkMSSInterface::~vtkMSSInterface()
{
	this->MSSMesh->Delete();
	this->Mesh->Delete();
}

//TODO: Rewrite method to adapt to vtkBiomechanicalModel (vtkPolyDataAlgorithm) class

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkMSSInterface::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	output->DeepCopy(input);

	if (input->GetPoints()->GetData()->GetDataType() != VTK_DOUBLE)
	{
		//Set input mesh where deformation will be calculated
		vtkPoints * ps = vtkPoints::New();
		ps->SetDataTypeToDouble();
		ps->DeepCopy(input->GetPoints());
		input->SetPoints(ps);
		input->Update();
	}

	this->MSSMesh->SetContacts(this->ContactPointIds, this->ContactDisplacements);
	this->MSSMesh->Update();

	vtkPoints * points = this->MSSMesh->GetOutput()->GetPoints();

	output->SetPoints(points);
	output->Modified();

	this->Clear();

	return 1;
}

//--------------------------------------------------------------------------
void vtkMSSInterface::Init()
{
	//Set Mass-Spring System parameters
	this->MSSMesh->SetDistanceCoefficient(this->DistanceCoefficient);
	this->MSSMesh->SetDampingCoefficient(this->DampingCoefficient);//Friction
	this->MSSMesh->SetMass(this->Mass);
	this->MSSMesh->SetDeltaT(this->DeltaT);
	this->MSSMesh->SetSteps(this->Steps);

	//Tetrahedralize mesh
	vtkDelaunay3D * del = vtkDelaunay3D::New();
	del->SetInput(vtkPolyData::SafeDownCast(this->GetInput()));
	del->Update();

	this->MSSMesh->SetInput(del->GetOutput());

	//this->Print(cout);
}

//--------------------------------------------------------------------------
void vtkMSSInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent <<  "DistanceCoefficient: " << this->DistanceCoefficient << endl;
  os << indent <<  "DampingCoefficient: " << this->DampingCoefficient << endl;
  os << indent <<  "Mass: " << this->Mass << endl;
  os << indent <<  "DeltaT: " << this->DeltaT << endl;
  os << indent <<  "Steps: " << this->Steps << endl;
}
