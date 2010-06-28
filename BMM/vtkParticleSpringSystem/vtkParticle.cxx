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
	this->DampingCoefficient = 0.0;
	this->DistanceCoefficient = 0.0;
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
void vtkParticle::SetPosition(double x, double y, double z)
{
	this->Position[0] = x;
	this->Position[1] = y;
	this->Position[2] = z;
}

//----------------------------------------------------------------------------
void vtkParticle::SetPosition(double * position)
{
	this->SetPosition(position[0], position[1], position[2]);
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
double * vtkParticle::GetPosition()
{
	return this->Position;
}

//----------------------------------------------------------------------------
void vtkParticle::GetPosition(double position[3])
{
	position[0] = this->Position[0];
	position[1] = this->Position[1];
	position[2] = this->Position[2];
}

//----------------------------------------------------------------------------
void vtkParticle::SetVelocity(double x, double y, double z)
{
	this->Velocity[0] = x;
	this->Velocity[1] = y;
	this->Velocity[2] = z;
}

//----------------------------------------------------------------------------
void vtkParticle::SetVelocity(double * Velocity)
{
	this->SetVelocity(Velocity[0], Velocity[1], Velocity[2]);
}

//----------------------------------------------------------------------------
double * vtkParticle::GetVelocity()
{
	return this->Velocity;
}

//----------------------------------------------------------------------------
void vtkParticle::GetVelocity(double Velocity[3])
{
	Velocity[0] = this->Velocity[0];
	Velocity[1] = this->Velocity[1];
	Velocity[2] = this->Velocity[2];
}

//----------------------------------------------------------------------------
void vtkParticle::SetAcceleration(double x, double y, double z)
{
	this->Acceleration[0] = x;
	this->Acceleration[1] = y;
	this->Acceleration[2] = z;
}

//----------------------------------------------------------------------------
void vtkParticle::SetAcceleration(double * Acceleration)
{
	this->SetAcceleration(Acceleration[0], Acceleration[1], Acceleration[2]);
}

//----------------------------------------------------------------------------
double * vtkParticle::GetAcceleration()
{
	return this->Acceleration;
}

//----------------------------------------------------------------------------
void vtkParticle::GetAcceleration(double Acceleration[3])
{
	Acceleration[0] = this->Acceleration[0];
	Acceleration[1] = this->Acceleration[1];
	Acceleration[2] = this->Acceleration[2];
}

//----------------------------------------------------------------------------
void vtkParticle::Update()
{
	this->Acceleration[0] = this->InverseMass*this->Force[0];
	this->Acceleration[1] = this->InverseMass*this->Force[1];
	this->Acceleration[2] = this->InverseMass*this->Force[2];
}

//----------------------------------------------------------------------------
void vtkParticle::SetForce(double x, double y, double z)
{
	this->Force[0] = x;
	this->Force[1] = y;
	this->Force[2] = z;
}

//----------------------------------------------------------------------------
void vtkParticle::SetForce(double * Force)
{
	this->SetForce(Force[0], Force[1], Force[2]);
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
double * vtkParticle::GetForce()
{
	return this->Force;
}

//----------------------------------------------------------------------------
void vtkParticle::GetForce(double Force[3])
{
	Force[0] = this->Force[0];
	Force[1] = this->Force[1];
	Force[2] = this->Force[2];
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
