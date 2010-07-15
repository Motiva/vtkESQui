#include "vtkSpring.h"


vtkCxxRevisionMacro(vtkSpring, "$Revision: 0.1 $");
vtkStandardNewMacro(vtkSpring);

//----------------------------------------------------------------------------
vtkSpring::vtkSpring()
{
	this->SpringCoefficient = 0.0;
	this->DistanceCoefficient = 0.0;
	this->DampingCoefficient = 0.0;
	this->DeltaT = 0.0;
	this->RestLength = 0.0;
	this->Direction[0] = 0.0;
	this->Direction[1] = 0.0;
	this->Direction[2] = 0.0;
	this->Particles = vtkParticleCollection::New();
}

//----------------------------------------------------------------------------
vtkSpring::~vtkSpring()
{
	if(this->Particles) this->Particles->Delete();
}

//----------------------------------------------------------------------------
double * vtkSpring::GetDirection()
{
	return this->Direction;
}

//----------------------------------------------------------------------------
void vtkSpring::GetDirection(double Direction[3])
{
	Direction[0] = this->Direction[0];
	Direction[1] = this->Direction[1];
	Direction[2] = this->Direction[2];
}

//----------------------------------------------------------------------------
void vtkSpring::SetParticle(vtkIdType id, vtkParticle * particle)
{
	this->Particles->SetParticle(id, particle);
}

//----------------------------------------------------------------------------
void vtkSpring::InsertNextParticle(vtkParticle * particle)
{
	this->Particles->InsertNextParticle(particle);
}

//----------------------------------------------------------------------------
vtkParticle * vtkSpring::GetParticle(vtkIdType id)
{
	return this->Particles->GetParticle(id);
}

//----------------------------------------------------------------------------
bool vtkSpring::ContainsParticle(vtkParticle * particle)
{
	for(vtkIdType i = 0; i < this->Particles->GetNumberOfItems(); i++)
	{
		vtkParticle * local = this->GetParticle(i);
		if(local && (local->GetId() == particle->GetId())){
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------
void vtkSpring::Init()
{
	double * p0 = this->GetParticle(0)->GetPosition();
	double * p1 = this->GetParticle(1)->GetPosition();
	//Note vtkMath returns squared-distance
	this->RestLength = sqrt(vtkMath::Distance2BetweenPoints(p0, p1));
	vtkMath::Subtract(p0, p1, this->Direction);
	vtkMath::Normalize(this->Direction);
}

//----------------------------------------------------------------------------
void vtkSpring::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent << "Id: " << this->Id << "\n";
	os << indent << "DistanceCoefficient: " << this->DistanceCoefficient << "\n";
	os << indent << "DampingCoefficient: " << this->DampingCoefficient << "\n";
	os << indent << "Distance: " << this->RestLength << "\n";
	os << indent << "Direction: (" << this->Direction[0] << ", "
									<< this->Direction[1] << ", "
									<< this->Direction[2] << ")\n\n";
	os << indent << "Particle0Id: " << this->Particles->GetParticle(0)->GetId() << "\n";
	os << indent << "Particle1Id: " << this->Particles->GetParticle(1)->GetId() << "\n";
}
