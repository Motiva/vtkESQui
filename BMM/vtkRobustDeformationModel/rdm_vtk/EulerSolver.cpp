#include "EulerSolver.h"
#include "InitialValueProblemSolver.h"
#include "vtkRDM.h"


EulerSolver::EulerSolver(vtkRDM* derivatives) : InitialValueProblemSolver(derivatives) {		
	}
	

EulerSolver::~EulerSolver(void) {
	}



void EulerSolver::ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt) 
	{


	this->derivatives->evalSpeed(this->dx, posNext, speedNext);
	this->derivatives->evalAcceleration(this->dv, posNext, speedNext);

	for (unsigned int i = 0; i < numPoints; i++) {

		posNext[i*3 + 0] = posNext[i*3 + 0] + dt * this->dx[i*3 + 0];
		posNext[i*3 + 1] = posNext[i*3 + 1] + dt * this->dx[i*3 + 1];
		posNext[i*3 + 2] = posNext[i*3 + 2] + dt * this->dx[i*3 + 2];
		
		speedNext[i*3 + 0] = speedNext[i*3 + 0] + dt * this->dv[i*3 + 0];
		speedNext[i*3 + 1] = speedNext[i*3 + 1] + dt * this->dv[i*3 + 1];
		speedNext[i*3 + 2] = speedNext[i*3 + 2] + dt * this->dv[i*3 + 2];
		}
	}