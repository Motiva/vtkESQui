#include "Volume.h"
#include "vtkTetra.h"

#include "VectorMath.h"

Volume::Volume(vtkIdType p1, vtkIdType p2, vtkIdType p3, vtkIdType p4, double kv) {
	
	this->kv = kv;

	this->point1 = p1;
	this->point2 = p2;
	this->point3 = p3;
	this->point4 = p4;
	}

Volume::~Volume(void) {
	// nothing to do here, no dynamic data
	}


double Volume::getVolume(const double* positions) const {

	const double* pi = positions + this->point1*3;//const_cast<double*>(this->accel->getPoint(point1));
	const double* pj = positions + this->point2*3;//const_cast<double*>(this->accel->getPoint(point2));
	const double* pk = positions + this->point3*3;//const_cast<double*>(this->accel->getPoint(point3));
	
	// this is the top of the tetra
	const double* pl = positions + this->point4*3;//const_cast<double*>(this->accel->getPoint(point4));
	
	//double volumeVtk = vtkTetra::ComputeVolume(pi, pj, pk, pl);


	double a[3];
	double b[3];
	double c[3];
	double crossResult[3];

	// a
	a[0] = pj[0] - pl[0];
	a[1] = pj[1] - pl[1];
	a[2] = pj[2] - pl[2];

	// b
	b[0] = pi[0] - pl[0];
	b[1] = pi[1] - pl[1];
	b[2] = pi[2] - pl[2];

	// c
	c[0] = pk[0] - pl[0];
	c[1] = pk[1] - pl[1];
	c[2] = pk[2] - pl[2];


	VectorMath::CrossProduct(b, c, crossResult);
	double result = VectorMath::DotProduct(a, crossResult);
	return result / 6.0;
	}


void Volume::computeForce(double result[3], const double* positions, unsigned int point) {

	double scalar = - this->kv * this->getVolumeConstraint();
	double* grad = this->grad + point*3;

	result[0] = scalar * grad[0];
	result[1] = scalar * grad[1];
	result[2] = scalar * grad[2];
	}


void Volume::getGradient(double result[3], const double* positions, unsigned int point) const {

	const double* pi;
	const double* pj;
	const double* pk;
	const double* pl;

	if (point == 0) {   // orientation ok
		pi = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pj = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pk = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pl = positions + this->point4*3;//this->accel->getPoint(this->point4);
		}
	else if (point == 1) {	// orientation ok						
		pi = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pj = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pk = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pl = positions + this->point4*3;//this->accel->getPoint(this->point4);
		}
	else if (point == 2) {	// orientation ok
		pi = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pj = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pk = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pl = positions + this->point4*3;//this->accel->getPoint(this->point4);
		}
	else if (point == 3) {	// orientation ok, corrected from 4, 1,2,3
		
		// maybe original
		pi = positions + this->point4*3;//this->accel->getPoint(this->point4);
		pj = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pk = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pl = positions + this->point1*3;//this->accel->getPoint(this->point1);
		}
		// TODO defined the order of the points in the tetrahedron correctly
		// maybe when building the volumes check for positive volume of tetrahedron
	else
		std::cerr << "Error in Volume.h getGradient()" << std::endl;

	double scalar = 1 / (6 * this->volumeOriginal);

	result[0] = scalar * (pk[1]*(pj[2] - pl[2]) + pl[1]*(pk[2] - pj[2]) + pj[1]*(pl[2] - pk[2]));
	result[1] = scalar * (pj[0]*(pk[2] - pl[2]) + pk[0]*(pl[2] - pj[2]) + pl[0]*(pj[2] - pk[2]));
	result[2] = scalar * (pk[1]*(pl[0] - pj[0]) + pl[1]*(pj[0] - pk[0]) + pj[1]*(pk[0] - pl[0]));
	}

double Volume::getVolumeConstraint() const {

	return (this->volumeCurrent - this->volumeOriginal) / this->volumeOriginal;
	}


vtkIdType Volume::getPointId(unsigned int point) const {

	if (point == 0) return this->point1;
	else if (point == 1) return this->point2;
	else if (point == 2) return this->point3;
	else if (point == 3) return this->point4;

	// if provided with a wrong parameter this will for sure cause a program error
	return -1;
	}


void Volume::initVolumeOriginal(const double* positions) {
	
	double volume = this->getVolume(positions);
	if (volume == 0) {
		cout << "Problem: initial volume of a tetrahedron is zero, please chose another meshing algorithm." << endl;
		volume = 0.00001;
		}

	this->volumeOriginal = this->getVolume(positions);
	}

void Volume::initForceComputation(const double* positions) {

	this->volumeCurrent = this->getVolume(positions);
	this->getGradient(this->grad, positions, 0);
	this->getGradient(this->grad + 3, positions, 1);
	this->getGradient(this->grad + 6, positions, 2);
	this->getGradient(this->grad + 9, positions, 3);
	}