#pragma once
#include "InitialValueProblemSolver.h"

class VelocityVerletSolver :
	public InitialValueProblemSolver
	{
	public:
		VelocityVerletSolver(vtkRDM* derivatives);
		~VelocityVerletSolver(void);

		void ComputeNextStep(double* posNext, 
					double* speedNext,
					/*double* posCurr,
					double* speedCurr,*/
					unsigned int numPoints,
					double dt);


	private:

		double* v1_2;

	};
