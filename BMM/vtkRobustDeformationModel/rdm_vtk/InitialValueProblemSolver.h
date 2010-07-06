#pragma once

#include "vtkRDM.h"


/**
 *   A pretty specific initial value prolem solver for efficiently
 * advancing a deformable model simulation.
 * It solves the system of two intial value problems, namely:
 * x' = F(x,v)
 * v' = G(x,v)
 * The value of the two derivatives x' and v' is already computed 
 * and directly passed into the solver
 */
class InitialValueProblemSolver
	{
	public:
		InitialValueProblemSolver(vtkRDM* derivatives);
		virtual ~InitialValueProblemSolver(void);


		virtual void ComputeNextStep(double* posNext, 
							double* speedNext,
							/*double* posCurr,
							double* speedCurr,*/
							unsigned int numPoints,
							double dt) = 0;


	protected:

		vtkRDM* derivatives;
		double* dx;
		double* dv;

	};
