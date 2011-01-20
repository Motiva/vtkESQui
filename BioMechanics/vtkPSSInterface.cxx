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
#include "vtkPSSInterface.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"

#include "vtkContact.h"
#include "vtkContactCollection.h"

vtkCxxRevisionMacro(vtkPSSInterface, "$Revision: 1.2 $");
vtkStandardNewMacro(vtkPSSInterface);

//----------------------------------------------------------------------------
vtkPSSInterface::vtkPSSInterface()
{
	this->ParticleSpringSystem = vtkParticleSpringSystem::New();
	this->DistanceCoefficient = 0;
	this->SpringCoefficient = 0;
	this->DampingCoefficient = 0;
	this->Mass = 0;
	this->DeltaT = 0;
	this->SolverType = vtkParticleSpringSystem::VelocityVerlet;
}

//----------------------------------------------------------------------------
vtkPSSInterface::~vtkPSSInterface()
{
	this->ParticleSpringSystem->Delete();
}

//--------------------------------------------------------------------------
void vtkPSSInterface::Init()
{
	std::cout << "vtkPSSInterface::Init()\n";
	//Set pss input data
	this->ParticleSpringSystem->SetInput(this->GetInput());
	//Set Mass-Spring System parameters
	this->ParticleSpringSystem->SetDistanceCoefficient(this->DistanceCoefficient);
	this->ParticleSpringSystem->SetSpringCoefficient(this->SpringCoefficient);//Friction
	this->ParticleSpringSystem->SetDampingCoefficient(this->DampingCoefficient);//Friction
	this->ParticleSpringSystem->SetMass(this->Mass);
	this->ParticleSpringSystem->SetDeltaT(this->DeltaT);
	this->ParticleSpringSystem->SetRigidityFactor(this->RigidityFactor);
	this->ParticleSpringSystem->SetSolverType(this->SolverType);
	//Initialize system
	this->ParticleSpringSystem->Init();
}

// VTK specific method: This method is called when the pipeline is calculated.
//----------------------------------------------------------------------------
int vtkPSSInterface::RequestData(
		vtkInformation *vtkNotUsed(request),
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector) {

	cout << "vtkPSSInterface::RequestData\n";
	// Get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// Get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

	this->ParticleSpringSystem->SetInput(input);

	this->Contacts->InitTraversal();
	while(vtkContact * contact = this->Contacts->GetNextContact())
	{
		if(contact->GetContactType() == vtkContact::ToolOrgan)
		{
			contact->Print(cout);
			this->ParticleSpringSystem->InsertContact(contact->GetPointId(1), contact->GetDisplacement());
		}
	}

	this->ParticleSpringSystem->Modified();
	this->ParticleSpringSystem->Update();

	output->ShallowCopy(this->ParticleSpringSystem->GetOutput());

	this->DeleteContacts();

	return 1;
}

//--------------------------------------------------------------------------
void vtkPSSInterface::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent <<  "SpringCoefficient: " << this->SpringCoefficient << endl;
  os << indent <<  "DampingCoefficient: " << this->DampingCoefficient << endl;
  os << indent <<  "DistanceCoefficient: " << this->DistanceCoefficient << endl;
  os << indent <<  "Mass: " << this->Mass << endl;
  os << indent <<  "DeltaT: " << this->DeltaT << endl;
}
