#ifndef __DistanceMSS_h__
#define __DistanceMSS_h__

#include "UGridAcceleratorMSS.h"
#include "vtkTetra.h"
#include <iostream>
class DistanceMSS {
public:
    DistanceMSS(vtkIdType p1, vtkIdType p2, double kd, double damping);
    DistanceMSS(const DistanceMSS& dist);
    ~DistanceMSS(void);
    bool operator < (const DistanceMSS& d) const;
    /*!
    * Inits the original DistanceMSS to the current DistanceMSS
    */
    void initDistanceOriginal(const double* positions);
    /*!
    *   Precomputes some value that will be needed multiple
    * times by the force computation
    */
    void initForceComputation(const double* positions, const double* speeds);
    /*!
    *   Computes the force resulting from the DistanceMSS preservation constraint
    * \param result double array to store the resulting force into
    * \param number of the point to compute the force for, maybe 0 or 1
    */
    void computeForce(double result[3], const double* positions, const double* speeds, unsigned int point);
    /*!
    * Returns the vtk id of an point (the id a point has in the
    * underlying vtkUnstrucutredGrid).
    * \param point may be 0 or 1. Indicates the first or second point in this DistanceMSS
    * \return id of the desired point in the underlying vtkUnstructuredGrid
    */
    vtkIdType getPointId(unsigned int point) const;
private:
    vtkIdType point1;
    vtkIdType point2;
    double DistanceOriginal;
    double DistanceCurrent;
    double kd;
    double damping;
    double grad[6];
    /*!
    * Computes the DistanceMSS between the two points
    * \return DistanceMSS between the points
    */
    double getDistance(const double* positions) const;
    /*!
    * Computes the DistanceMSS preservation constraint. The constraint
    * is defined as the ration between the current DistanceMSS and the original DistanceMSS -1
    * \return the constraint scalar
    */
    double getConstraint() const;
    /*!
    * Computes the gradient of the constraint
    */
    void getGradient(double result[3], const double* positions, unsigned int point) const;
    //! Computes the damping scalar at point i
    double getDamping(const double* positions, const double* speeds, unsigned int point);
};

#endif

