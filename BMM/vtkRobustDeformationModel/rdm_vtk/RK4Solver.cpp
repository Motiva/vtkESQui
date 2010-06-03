#include "RK4Solver.h"

RK4Solver::RK4Solver(vtkRDM* derivatives) : InitialValueProblemSolver(derivatives)	{
	
	unsigned int numPoints = this->derivatives->GetNumberOfPoints();

	this->tk = new double[numPoints*3];
	this->tj = new double[numPoints*3];

	this->k1 = new double[numPoints*3];
	this->k2 = new double[numPoints*3];
	this->k3 = new double[numPoints*3];
	this->k4 = new double[numPoints*3];	
	this->j1 = new double[numPoints*3];
	this->j2 = new double[numPoints*3];
	this->j3 = new double[numPoints*3];
	this->j4 = new double[numPoints*3];
	}

RK4Solver::~RK4Solver(void) {
	
	delete[] this->tk;
	delete[] this->tj;

	delete[] this->k1;
	delete[] this->k2;
	delete[] this->k3;
	delete[] this->k4;
	delete[] this->j1;
	delete[] this->j2;
	delete[] this->j3;
	delete[] this->j4;
	}

void RK4Solver::ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt) 
	{

	// define some optimizations
	double dt05 = 0.5 * dt;
	double dt1_6 = dt / 6;

	// evaluate derivatives at time t
	this->derivatives->evalSpeed(this->k1, posNext, speedNext);
	this->derivatives->evalAcceleration(this->j1, posNext, speedNext);

	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		tk[i3_0] = posNext[i3_0] + dt05 * k1[i3_0];
		tk[i3_1] = posNext[i3_1] + dt05 * k1[i3_1];
		tk[i3_2] = posNext[i3_2] + dt05 * k1[i3_2];

 		tj[i3_0] = speedNext[i3_0] + dt05 * j1[i3_0];
		tj[i3_1] = speedNext[i3_1] + dt05 * j1[i3_1];
		tj[i3_2] = speedNext[i3_2] + dt05 * j1[i3_2];
		}

	this->derivatives->evalSpeed(this->k2, tk, tj);
	this->derivatives->evalAcceleration(this->j2, tk, tj);

	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		tk[i3_0] = posNext[i3_0] + dt05 * k2[i3_0];
		tk[i3_1] = posNext[i3_1] + dt05 * k2[i3_1];
		tk[i3_2] = posNext[i3_2] + dt05 * k2[i3_2];

 		tj[i3_0] = speedNext[i3_0] + dt05 * j2[i3_0];
		tj[i3_1] = speedNext[i3_1] + dt05 * j2[i3_1];
		tj[i3_2] = speedNext[i3_2] + dt05 * j2[i3_2];
		}
 	
	this->derivatives->evalSpeed(this->k3, tk, tj);
	this->derivatives->evalAcceleration(this->j3, tk, tj);
	
	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

		tk[i3_0] = posNext[i3_0] + dt * k3[i3_0];
		tk[i3_1] = posNext[i3_1] + dt * k3[i3_1];
		tk[i3_2] = posNext[i3_2] + dt * k3[i3_2];

 		tj[i3_0] = speedNext[i3_0] + dt * j3[i3_0];
		tj[i3_1] = speedNext[i3_1] + dt * j3[i3_1];
		tj[i3_2] = speedNext[i3_2] + dt * j3[i3_2];
		}

	this->derivatives->evalSpeed(this->k4, tk, tj);
	this->derivatives->evalAcceleration(this->j4, tk, tj);

 	for (unsigned int i = 0; i < numPoints; i++) {

		vtkIdType i3_0 = i*3;
		vtkIdType i3_1 = i*3 + 1;
		vtkIdType i3_2 = i*3 + 2;

 		posNext[i3_0] = posNext[i3_0] + dt1_6 * (k1[i3_0] + 2*k2[i3_0] + 2*k3[i3_0] + k4[i3_0]);
		posNext[i3_1] = posNext[i3_1] + dt1_6 * (k1[i3_1] + 2*k2[i3_1] + 2*k3[i3_1] + k4[i3_1]);
		posNext[i3_2] = posNext[i3_2] + dt1_6 * (k1[i3_2] + 2*k2[i3_2] + 2*k3[i3_2] + k4[i3_2]);

		speedNext[i3_0] = speedNext[i3_0] + dt1_6 * (j1[i3_0] + 2*j2[i3_0] + 2*j3[i3_0] + j4[i3_0]);
		speedNext[i3_1] = speedNext[i3_1] + dt1_6 * (j1[i3_1] + 2*j2[i3_1] + 2*j3[i3_1] + j4[i3_1]);
		speedNext[i3_2] = speedNext[i3_2] + dt1_6 * (j1[i3_2] + 2*j2[i3_2] + 2*j3[i3_2] + j4[i3_2]);
		}
	}