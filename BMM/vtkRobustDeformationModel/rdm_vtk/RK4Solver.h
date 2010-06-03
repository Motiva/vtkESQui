#pragma once
#include "InitialValueProblemSolver.h"

class RK4Solver :
	public InitialValueProblemSolver
	{
	public:
		RK4Solver(vtkRDM* derivatives);
		~RK4Solver(void);

		void ComputeNextStep(double* posNext, 
			double* speedNext,
			/*double* posCurr,
			double* speedCurr,*/
			unsigned int numPoints,
			double dt);

	private:

		double* tk;
		double* tj;

		double* k1;
		double* k2;
		double* k3;
		double* k4;
		double* j1;
		double* j2;
		double* j3;
		double* j4;
	};