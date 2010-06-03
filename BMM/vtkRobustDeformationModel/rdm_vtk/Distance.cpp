#include "Distance.h"
#include <iostream>
#include "VectorMath.h"
#include "vtkRDM.h"

inline bool my_isnan(double x) {
	return x != x;
	}

Distance::Distance(vtkIdType p1, vtkIdType p2, double kd, double damping) {

	// order the points, bigger values first
	// for an easy implementation of the < operator
	this->point1 = std::max(p1,p2);
	this->point2 = std::min(p1,p2);

	// some error catching code
	if (p1 == p2 || p1 < 0 || p2 < 0) {
		std::cerr << "Problem in Distance::Distance() problem with point ids" << endl;
		std::cerr << "Points are: " << p1 << "," << p2 << endl;
		}

	this->kd = kd;
	//this->accel = accel;
	this->damping = damping;
	}


Distance::Distance(const Distance& dist) {

	this->point1 = dist.point1;
	this->point2 = dist.point2;
	this->distanceOriginal = dist.distanceOriginal;
	this->distanceCurrent = dist.distanceCurrent;
	this->kd = dist.kd;
	//this->accel = dist.accel;
	this->damping = dist.damping;
	}


Distance::~Distance(void) {
	// nothing to do here, no dynamic data
	}


bool Distance::operator < (const Distance& d) const {
	return (this->point1 < d.point1 || this->point2 < d.point2);
	}


void Distance::initDistanceOriginal(const double* positions) {
	this->distanceOriginal = this->getDistance(positions);
	}


void Distance::initForceComputation(const double* positions, const double* speeds) {

   	//##########################################
	// Here go all calls that precompute stuff 
	// that is needed multiple times during one
	// call of computeForce().
	// other methods in this class may rely on it!!
	this->distanceCurrent = this->getDistance(positions);
	this->getGradient(this->grad, positions, 0);
	this->getGradient(this->grad+3, positions, 1);
	}


void Distance::computeForce(double result[3], const double* positions, const double* speeds, unsigned int point) {

	double* grad = this->grad + point*3;

	double scalarForce = - this->kd * this->getConstraint();
	double scalarDamping = this->getDamping(positions, speeds, point);

	//if (my_isnan(scalarForce)) cout << "NAN" << endl;
	//if (my_isnan(scalarDamping)) cout << "NAN" << endl;

	double scalar = scalarForce + scalarDamping;

	result[0] = scalar * grad[0];
	result[1] = scalar * grad[1];
	result[2] = scalar * grad[2];

	// compute some external damping
	/*
	vtkIdType p = this->getPointId(point);
	const double* speed = speeds + p*3;
	result[0] -= 0.0001 * speed[0];
	result[1] -= 0.0001 * speed[1];
	result[2] -= 0.0001 * speed[2];
	*/
	}



double Distance::getDistance(const double* positions) const {
	const double* p1 = positions + this->point1*3;//this->accel->getPoint(this->point1);
	const double* p2 = positions + this->point2*3;//this->accel->getPoint(this->point2);
	double distance = sqrt(pow(p2[0] - p1[0],2.0) + pow(p2[1] - p1[1], 2.0) + pow(p2[2] - p1[2],2.0));
	if (distance == 0) cout << "Problem: Distance = 0" << endl;
	return distance;
	}

double Distance::getConstraint() const {
	double constraint = (this->distanceCurrent - distanceOriginal) / distanceOriginal;
	return constraint;
	}

void Distance::getGradient(double result[3], const double* positions, unsigned int point) const {

	const double* pi;
	const double* pj;

	if (point == 0) {
		pi = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pj = positions + this->point2*3;//this->accel->getPoint(this->point2);
		}
	else if (point == 1) {
		pi = positions + this->point2*3;//;this->accel->getPoint(this->point2);
		pj = positions + this->point1*3;//this->accel->getPoint(this->point1);
		}
	else {
		std::cerr << "Error in Distance.h getGradient()" << std::endl;
		std::cerr << "Point is " << point << ", but allowed is 0 or 1" << endl;
		}


	double denom = this->distanceOriginal * this->distanceCurrent;
	result[0] = (pi[0] - pj[0]) / denom;
	result[1] = (pi[1] - pj[1]) / denom;
	result[2] = (pi[2] - pj[2]) / denom;
	}

double Distance::getDamping(const double* positions, const double* speeds, unsigned int point) {


	vtkIdType i;
	vtkIdType j;
	double* gradI;
	double* gradJ;

	if (point == 0) {
		gradI = this->grad;
		gradJ = this->grad + 3;
		i = this->point1;
		j = this->point2;
		}

	else if (point == 1) {
		gradI = this->grad + 3;
		gradJ = this->grad;
		i = this->point2;
		j = this->point1;
		}

	const double* vi = speeds + i*3;
	const double* vj = speeds + j*3;

	double dotI = VectorMath::DotProduct(gradI, vi);
	double dotJ = VectorMath::DotProduct(gradJ, vj);

	double scalar = - this->damping * (dotI + dotJ);
	return scalar;
	}

vtkIdType Distance::getPointId(unsigned int point) const {

	if (point == 0) return this->point1;
	else if (point == 1) return this->point2;
	
	// if provided with a wrong parameter this will for sure cause a program error
	return -1;
	}