#include "DistanceMSS.h"
#include <iostream>
#include "VectorMathMSS.h"
DistanceMSS::DistanceMSS(vtkIdType p1, vtkIdType p2, double kd, double damping) {
    // order the points, bigger values first
    // for an easy implementation of the < operator
    this->point1 = std::max(p1,p2);
    this->point2 = std::min(p1,p2);
    // some error catching code
    if (p1 == p2 || p1 < 0 || p2 < 0) {
        std::cerr << "Problem in DistanceMSS::DistanceMSS() problem with point ids" << endl;
        std::cerr << "Points are: " << p1 << "," << p2 << endl;
    }
    this->kd = kd;
    this->damping = damping;
}
DistanceMSS::DistanceMSS(const DistanceMSS& dist) {
    this->point1 = dist.point1;
    this->point2 = dist.point2;
    this->DistanceOriginal = dist.DistanceOriginal;
    this->DistanceCurrent = dist.DistanceCurrent;
    this->kd = dist.kd;
    this->damping = dist.damping;
}
DistanceMSS::~DistanceMSS(void) {
}
bool DistanceMSS::operator < (const DistanceMSS& d) const {
    return (!(this->point1 >= d.point1) && !(this->point2 >= d.point2));
}
void DistanceMSS::initDistanceOriginal(const double* positions) {
    this->DistanceOriginal = this->getDistance(positions);
}
void DistanceMSS::initForceComputation(const double* positions, const double* speeds) {
    //##########################################
    // Here go all calls that precompute stuff
    // that is needed multiple times during one
    // call of computeForce().
    // other methods in this class may rely on it!!
    this->DistanceCurrent = this->getDistance(positions);
    this->getGradient(this->grad, positions, 0);
    this->getGradient(this->grad+3, positions, 1);
}
void DistanceMSS::computeForce(double result[3], const double* positions, const double* speeds, unsigned int point) {
    double* grad = this->grad + point*3;
    double scalarForce = - this->kd * this->getConstraint();
    double scalarDamping = this->getDamping(positions, speeds, point);
    double scalar = scalarForce + scalarDamping;
    result[0] = scalar * grad[0];
    result[1] = scalar * grad[1];
    result[2] = scalar * grad[2];
}
double DistanceMSS::getDistance(const double* positions) const {
    const double* p1 = positions + this->point1*3;//this->accel->getPoint(this->point1);
    const double* p2 = positions + this->point2*3;//this->accel->getPoint(this->point2);
    double DistanceMSS = sqrt(pow(p2[0] - p1[0],2.0) + pow(p2[1] - p1[1], 2.0) + pow(p2[2] - p1[2],2.0));
    if (DistanceMSS == 0) cout << "Problem: DistanceMSS = 0" << endl;
    return DistanceMSS;
}
double DistanceMSS::getConstraint() const {
    double constraint = (this->DistanceCurrent - DistanceOriginal) / DistanceOriginal;
    return constraint;
}
void DistanceMSS::getGradient(double result[3], const double* positions, unsigned int point) const {
    const double* pi;
    const double* pj;
    if (point == 0) {
        pi = positions + this->point1*3;//this->accel->getPoint(this->point1);
        pj = positions + this->point2*3;//this->accel->getPoint(this->point2);
    } else if (point == 1) {
        pi = positions + this->point2*3;//;this->accel->getPoint(this->point2);
        pj = positions + this->point1*3;//this->accel->getPoint(this->point1);
    } else {
        std::cerr << "Error in DistanceMSS.h getGradient()" << std::endl;
        std::cerr << "Point is " << point << ", but allowed is 0 or 1" << endl;
    }
    double denom = this->DistanceOriginal * this->DistanceCurrent;
    result[0] = (pi[0] - pj[0]) / denom;
    result[1] = (pi[1] - pj[1]) / denom;
    result[2] = (pi[2] - pj[2]) / denom;
}

double DistanceMSS::getDamping(const double* positions, const double* speeds, unsigned int point) {
    vtkIdType i;
    vtkIdType j;
    double* gradI;
    double* gradJ;
    if (point == 0) {
        gradI = this->grad;
        gradJ = this->grad + 3;
        i = this->point1;
        j = this->point2;
    } else if (point == 1) {
        gradI = this->grad + 3;
        gradJ = this->grad;
        i = this->point2;
        j = this->point1;
    }
    const double* vi = speeds + i*3;
    const double* vj = speeds + j*3;
    double dotI = VectorMathMSS::DotProduct(gradI, vi);
    double dotJ = VectorMathMSS::DotProduct(gradJ, vj);
    double scalar = - this->damping * (dotI + dotJ);
    return scalar;
}
vtkIdType DistanceMSS::getPointId(unsigned int point) const {
    if (point == 0) return this->point1;
    else if (point == 1) return this->point2;
    // if provided with a wrong parameter this will for sure cause a program error
    return -1;
}


