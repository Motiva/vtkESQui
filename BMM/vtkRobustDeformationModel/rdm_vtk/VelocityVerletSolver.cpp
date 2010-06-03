#include "VelocityVerletSolver.h"

VelocityVerletSolver::VelocityVerletSolver(vtkRDM* derivatives) : InitialValueProblemSolver(derivatives) {

	vtkIdType numPoints = derivatives->GetNumberOfPoints();
	this->v1_2 = new double[numPoints*3];

	// initialized the derivatives to be used in the
	// very first step to zero - is that correct???
	for (vtkIdType i = 0; i < numPoints*3; i++)
		this->dv[i] = 0;
	}

VelocityVerletSolver::~VelocityVerletSolver(void) {
	delete[] this->v1_2;
	}


void VelocityVerletSolver::ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt) 
	{


	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		v1_2[i3_0] = speedNext[i3_0] + 0.5 * dt * dv[i3_0];
		v1_2[i3_1] = speedNext[i3_1] + 0.5 * dt * dv[i3_1];
		v1_2[i3_2] = speedNext[i3_2] + 0.5 * dt * dv[i3_2];


		posNext[i3_0] = posNext[i3_0] + dt * v1_2[i3_0];
		posNext[i3_1] = posNext[i3_1] + dt * v1_2[i3_1];
		posNext[i3_2] = posNext[i3_2] + dt * v1_2[i3_2];
		}

	//this->derivatives->evalSpeed(this->dx, posNext, v1_2);
	this->derivatives->evalAcceleration(this->dv, posNext, v1_2);

	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		speedNext[i3_0] = v1_2[i3_0] + 0.5 * dt * dv[i3_0];
		speedNext[i3_1] = v1_2[i3_1] + 0.5 * dt * dv[i3_1];
		speedNext[i3_2] = v1_2[i3_2] + 0.5 * dt * dv[i3_2];
		}
	}
