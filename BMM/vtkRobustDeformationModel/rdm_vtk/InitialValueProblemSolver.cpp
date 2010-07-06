#include "InitialValueProblemSolver.h"

#include <iostream>

InitialValueProblemSolver::InitialValueProblemSolver(vtkRDM* derivatives) {
	
	this->derivatives = derivatives;

	unsigned int numPoints = this->derivatives->GetNumberOfPoints();

	this->dx = new double[numPoints*3];
	this->dv = new double[numPoints*3];

	using namespace std;
	}

InitialValueProblemSolver::~InitialValueProblemSolver(void) {
	
	delete[] this->dx;
	delete[] this->dv;
	}






													
