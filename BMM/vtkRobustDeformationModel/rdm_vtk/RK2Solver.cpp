#include "InitialValueProblemSolver.h"
#include "RK2Solver.h"

inline bool my_isnan(double x) {
	return x != x;
	}

RK2Solver::RK2Solver(vtkRDM* derivatives) : InitialValueProblemSolver(derivatives)	{
	
	unsigned int numPoints = this->derivatives->GetNumberOfPoints();

	this->k1 = new double[numPoints*3];
	this->k2 = new double[numPoints*3];
	this->j1 = new double[numPoints*3];
	this->j2 = new double[numPoints*3];
	}

RK2Solver::~RK2Solver(void) {

	delete[] this->k1;
	delete[] this->k2;
	delete[] this->j1;
	delete[] this->j2;
	}


void RK2Solver::ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt) 
	{

	double dt05 = 0.5 * dt;

	// evaluate derivatives at time t
	this->derivatives->evalSpeed(this->dx, posNext, speedNext);
	this->derivatives->evalAcceleration(this->dv, posNext, speedNext);


	// compute positions and speeds at the midpoint
	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;


		// this is actually pos(t) + 0.5 * dt * dx[t]
		k1[i3_0] = posNext[i3_0] + dt05 * this->dx[i3_0];
		k1[i3_1] = posNext[i3_1] + dt05 * this->dx[i3_1];
		k1[i3_2] = posNext[i3_2] + dt05 * this->dx[i3_2];
		
		j1[i3_0] = speedNext[i3_0] + dt05 * this->dv[i3_0];
		j1[i3_1] = speedNext[i3_1] + dt05 * this->dv[i3_1];
		j1[i3_2] = speedNext[i3_2] + dt05 * this->dv[i3_2];
		}
		

	// evaluate derivatives at time t + dt/2
	// actually computes k2 which is then inside dv and dx
	this->derivatives->evalSpeed(this->k2, k1, j1);
	this->derivatives->evalAcceleration(this->j2, k1, j1);


	// compute results at time t + dt
	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		posNext[i3_0] = posNext[i3_0] + dt * this->k2[i3_0];
		posNext[i3_1] = posNext[i3_1] + dt * this->k2[i3_1];
		posNext[i3_2] = posNext[i3_2] + dt * this->k2[i3_2];
		
		speedNext[i3_0] = speedNext[i3_0] + dt * this->j2[i3_0];
		speedNext[i3_1] = speedNext[i3_1] + dt * this->j2[i3_1];
		speedNext[i3_2] = speedNext[i3_2] + dt * this->j2[i3_2];
		}
	}