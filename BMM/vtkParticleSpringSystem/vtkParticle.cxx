#include "vtkParticle.h"

#include "vtkObjectFactory.h"
#include "vtkCollection.h"
#include "vtkUnstructuredGrid.h"
#include "vtkIdList.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkCellArray.h"
#include "vtkMath.h"

#include "vtkSpring.h"
#include "vtkSpringCollection.h"

vtkCxxRevisionMacro(vtkParticle, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkParticle);

//----------------------------------------------------------------------------
vtkParticle::vtkParticle()
{
	this->Id = -1;
	this->Mass = 0.0;
	this->InverseMass = 100000.0;
	this->Fixed = 0;
	this->Position[0] = 0.0;
	this->Position[1] = 0.0;
	this->Position[2] = 0.0;
	this->Velocity[0] = 0.0;
	this->Velocity[1] = 0.0;
	this->Velocity[2] = 0.0;
	this->Acceleration[0] = 0.0;
	this->Acceleration[1] = 0.0;
	this->Acceleration[2] = 0.0;
	this->Force[0] = 0.0;
	this->Force[1] = 0.0;
	this->Force[2] = 0.0;
	this->Springs = vtkSpringCollection::New();
}

//----------------------------------------------------------------------------
vtkParticle::~vtkParticle()
{
}

//----------------------------------------------------------------------------
void vtkParticle::SetMass(double mass)
{
	this->Mass = mass;
	if (mass > 0)
	{
		this->InverseMass = (1/mass);
	}
}
//----------------------------------------------------------------------------
void vtkParticle::Update()
{
	this->Acceleration[0] = this->InverseMass*this->Force[0];
	this->Acceleration[1] = this->InverseMass*this->Force[1];
	this->Acceleration[2] = this->InverseMass*this->Force[2];
}

//----------------------------------------------------------------------------
void vtkParticle::AddPosition(double x, double y, double z)
{
	if(!this->Fixed)
	{
		this->Position[0] += x;
		this->Position[1] += y;
		this->Position[2] += z;
	}
}

//----------------------------------------------------------------------------
void vtkParticle::AddForce(double x, double y, double z)
{
	if(!this->Fixed)
	{
		this->Force[0] += x;
		this->Force[1] += y;
		this->Force[2] += z;
	}
}

//----------------------------------------------------------------------------
void vtkParticle::SetSpring(vtkIdType id, vtkSpring * spring)
{
	this->Springs->InsertSpring(id, spring);
}

//----------------------------------------------------------------------------
void vtkParticle::InsertNextSpring(vtkSpring * spring)
{
	this->Springs->InsertNextSpring(spring);
}

//----------------------------------------------------------------------------
vtkSpring * vtkParticle::GetSpring(vtkIdType id)
{
	return this->Springs->GetSpring(id);
}

//----------------------------------------------------------------------------
bool vtkParticle::ContainsSpring(vtkSpring * spring)
{
	for(vtkIdType i = 0; i < this->Springs->GetNumberOfItems(); i++)
	{
		vtkSpring * local = this->GetSpring(i);
		vtkParticle * p0 = spring->GetParticle(0);
		vtkParticle * p1 = spring->GetParticle(1);
		if(local && local->ContainsParticle(p0) && local->ContainsParticle(p1)){
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------
void vtkParticle::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent << "Id: " << this->Id << "\n";
	os << indent << "Mass: " << this->Mass << "\n";
	os << indent << "Position: (" << this->Position[0] << ", "
									<< this->Position[1] << ", "
									<< this->Position[2] << ")\n";
	os << indent << "Velocity: (" << this->Velocity[0] << ", "
										<< this->Velocity[1] << ", "
										<< this->Velocity[2] << ")\n";
	os << indent << "Acceleration: (" << this->Acceleration[0] << ", "
										<< this->Acceleration[1] << ", "
										<< this->Acceleration[2] << ")\n";
	os << indent << "Force: (" << this->Force[0] << ", "
										<< this->Force[1] << ", "
										<< this->Force[2] << ")\n";
}
