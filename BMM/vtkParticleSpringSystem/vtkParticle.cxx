#include "vtkParticle.h"

#include "vtkObjectFactory.h"

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