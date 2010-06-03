#pragma once
#include "InitialValueProblemSolver.h"

class vtkRDM;

class EulerSolver :
	public InitialValueProblemSolver
	{
	public:
		EulerSolver(vtkRDM* derivatives);
		~EulerSolver(void);

		void ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt);
	};
