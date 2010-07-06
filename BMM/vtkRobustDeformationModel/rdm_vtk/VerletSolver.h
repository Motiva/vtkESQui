#pragma once
#include "InitialValueProblemSolver.h"

class VerletSolver :
	public InitialValueProblemSolver
	{
	public:
		VerletSolver(vtkRDM* derivatives);
		~VerletSolver(void);


		void ComputeNextStep(double* posNext, 
					double* speedNext,
					/*double* posCurr,
					double* speedCurr,*/
					unsigned int numPoints,
					double dt);



	};
