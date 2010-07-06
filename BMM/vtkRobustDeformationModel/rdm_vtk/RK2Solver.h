#pragma once
#include "InitialValueProblemSolver.h"

class vtkRDM;

class RK2Solver :
	public InitialValueProblemSolver
	{
	public:
		RK2Solver(vtkRDM* derivatives);
		~RK2Solver(void);

		void ComputeNextStep(double* posNext, 
					double* speedNext,
					/*double* posCurr,
					double* speedCurr,*/
					unsigned int numPoints,
					double dt);

	private:

		double* k1;
		double* k2;
		double* j1;
		double* j2;
	};
