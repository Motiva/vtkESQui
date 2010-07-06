#include "Surface.h"

#include "UGridAccelerator.h"
#include "VectorMath.h"

Surface::Surface(vtkIdType p1, vtkIdType p2, vtkIdType p3, double ks) {

	this->ks = ks;
	//this->accel = accel;

	vtkIdType maximum = std::max(p1, std::max(p2,p3));
	vtkIdType minimum = std::min(p1, std::min(p2,p3));

	vtkIdType mid;
	if (p1 < maximum && p1 > minimum) mid = p1;
	else if (p2 < maximum && p2 > minimum) mid = p2;
	else if (p3 < maximum && p3 > minimum) mid = p3;

	this->point1 = maximum;
	this->point2 = mid;
	this->point3 = minimum;
	}

Surface::~Surface(void) {
	// nothing to do here, no dynamic data
	}


bool Surface::operator < (const Surface& s) const {
	return (this->point1 < s.point1
		|| this->point2 < s.point2
		|| this->point3 < s.point3);
	}


void Surface::initSurfaceOriginal(const double* positions) {
	this->surfaceOriginal = this->getSurface(positions);
	}

void  Surface::computeForce(double result[3], const double* positions, unsigned int point) {

	//##########################################
	// Here go all calls that precompute stuff 
	// that is needed multiple times during one
	// call of computeForce().
	// other methods in this class may rely on it!!
	this->surfaceCurrent = this->getSurface(positions);

	// TODO precompute gradients since they are
	// also used multiple times
	//##########################################


	double scalar = - this->getSurfaceConstraint() * this->ks;
	this->getGradient(result, positions, point);

	result[0] *= scalar;
	result[1] *= scalar;
	result[2] *= scalar;
	}




double Surface::getSurface(const double* positions) const {

	const double* pi = positions + point1*3;//this->accel->getPoint(point1);
	const double* pj = positions + point2*3;//this->accel->getPoint(point2);
	const double* pk = positions + point3*3;//this->accel->getPoint(point3);

	double pji[3];
	double pki[3];

	pji[0] = pj[0] - pi[0];
	pji[1] = pj[1] - pi[1];
	pji[2] = pj[2] - pi[2];

	pki[0] = pk[0] - pi[0];
	pki[1] = pk[1] - pi[1];
	pki[2] = pk[2] - pi[2];

	double cross[3];

	VectorMath::CrossProduct(pji, pki, cross);
	return 0.5 * VectorMath::length(cross);
	}


double Surface::getSurfaceConstraint() const {
	return (this->surfaceCurrent - this->surfaceOriginal) / this->surfaceOriginal;
	}

void Surface::getGradient(double result[3], const double* positions, unsigned int point) const {

	const double* pi;
	const double* pj;
	const double* pk;

	if (point == 0) {
		pi = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pj = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pk = positions + this->point3*3;//this->accel->getPoint(this->point3);
		}
	else if (point == 1) {
		pi = positions + this->point2*3;//this->accel->getPoint(this->point2);
		pj = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pk = positions + this->point1*3;//this->accel->getPoint(this->point1);
		}
	else if (point == 2) {
		pi = positions + this->point3*3;//this->accel->getPoint(this->point3);
		pj = positions + this->point1*3;//this->accel->getPoint(this->point1);
		pk = positions + this->point2*3;//this->accel->getPoint(this->point2);
		}
	else
		std::cerr << "Error in Distance.h getGradient()" << std::endl;


	// this term contains a 4 * because this->getSurface() only delivers
	// 0.5 * |(pj-pi) x (pk-pi)|
	double scalar = 1 / (4 * this->surfaceOriginal * this->surfaceCurrent);


	// we only need to compute those three terms since the entries in the gradient are
	// composed of each two of those terms.
	// This saves a lot of computation time
	double pjzpkx = (pj[2]*pk[0] - pj[2]*pi[0] - pi[2]*pk[0] - pj[0]*pk[2] + pj[0]*pi[2] + pi[0]*pk[2]);
	double pjxpky = (pj[0]*pk[1] - pj[0]*pi[1] - pi[0]*pk[1] - pj[1]*pk[0] + pj[1]*pi[0] + pi[1]*pk[0]);
	double pjypkz = (pj[1]*pk[2] - pj[1]*pi[2] - pi[1]*pk[2] - pj[2]*pk[1] + pj[2]*pi[1] + pi[2]*pk[1]);


	// x entry of the gradient:
	// (pjzpkx - pjzpix - pizpkx - pjxpkz + pjxpiz + pixpkz) (pkz - pjz)
	// +(pjxpky - pjxpiy - pixpky - pjypkx + pjypix + piypkx) (pjy - pky)
	double x = pjzpkx * (pk[2] - pj[2]) + pjxpky * (pj[1] - pk[1]);

	// y entry of the gradient	
	// (pjypkz - pjypiz - piypkz - pjzpky + pjzpiy + pizpky) (pjz - pkz)
	// +(pjxpky - pjxpiy - pixpky - pjypkx + pjypix + piypkx) (pkx - pjx)
	double y = pjypkz * (pk[0] - pj[0]) + pjxpky * (pk[0] - pj[0]);

	// z entry of the gradient
	// (pjypkz - pjypiz - piypkz - pjzpky + pjzpiy + pizpky) (pky - pjy)
	// +(pjzpkx - pjzpix - pizpkx - pjxpkz + pjxpiz + pixpkz) (pjx - pkx)
	double z = pjypkz * (pk[1] - pj[1]) + pjzpkx * (pj[0] - pk[0]);

	result[0] = x * scalar;
	result[1] = y * scalar;
	result[2] = z * scalar;
	}

vtkIdType Surface::getPointId(unsigned int point) const {

	if (point == 0) return this->point1;
	else if (point == 1) return this->point2;
	else if (point == 2) return this->point3;

	// if provided with a wrong parameter this will for sure cause a program error
	return -1;
	}