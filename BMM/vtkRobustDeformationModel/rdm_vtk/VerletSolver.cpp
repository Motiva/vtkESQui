#include "VerletSolver.h"

VerletSolver::VerletSolver(vtkRDM* derivatives) : InitialValueProblemSolver(derivatives) {
	}

VerletSolver::~VerletSolver(void) {
	}


void VerletSolver::ComputeNextStep(double* posNext, 
			double* speedNext,
			/*double* posCurr,
			double* speedCurr,*/
			unsigned int numPoints,
			double dt) 
			{


			double dtdt = dt*dt;
			double dt2 = 2 * dt;

			// get the old positions
			double* oldPos = this->derivatives->GetOldPositions();

			//this->derivatives->evalSpeed(this->dx, posCurr, speedCurr);
			this->derivatives->evalAcceleration(this->dv, posNext, speedNext);

			for (unsigned int i = 0; i < numPoints; i++) {

				vtkIdType i3_0 = i*3;
				vtkIdType i3_1 = i*3 + 1;
				vtkIdType i3_2 = i*3 + 2;


				posNext[i3_0] = 2 * posNext[i3_0] - oldPos[i3_0] + dtdt * this->dv[i3_0];
				posNext[i3_1] = 2 * posNext[i3_1] - oldPos[i3_1] + dtdt * this->dv[i3_1];
				posNext[i3_2] = 2 * posNext[i3_2] - oldPos[i3_2] + dtdt * this->dv[i3_2];

				speedNext[i3_0] = (posNext[i3_0] - oldPos[i3_0]) / (dt2);
				speedNext[i3_1] = (posNext[i3_1] - oldPos[i3_1]) / (dt2);
				speedNext[i3_2] = (posNext[i3_2] - oldPos[i3_2]) / (dt2);
				}
			}
