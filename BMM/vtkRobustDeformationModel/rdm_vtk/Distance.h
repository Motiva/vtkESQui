#pragma once

#include "UGridAccelerator.h"
#include "vtkTetra.h"	// just including that at the moment to have vtkIdType defined - what is the right include?
#include <iostream>

class Distance
	{
	public:
		Distance(vtkIdType p1, vtkIdType p2, double kd, double damping);
		Distance(const Distance& dist);

		~Distance(void);
		
		bool operator < (const Distance& d) const;

		/*!
		 * Inits the original distance to the current distance
		 */
		void initDistanceOriginal(const double* positions);


		/*!
		 *   Precomputes some value that will be needed multiple
		 * times by the force computation
		 */
		void initForceComputation(const double* positions, const double* speeds);

		/*!
		 *   Computes the force resulting from the distance preservation constraint
		 * \param result double array to store the resulting force into
		 * \param number of the point to compute the force for, maybe 0 or 1
		 */
		void computeForce(double result[3], const double* positions, const double* speeds, unsigned int point);

		/*!
		 * Returns the vtk id of an point (the id a point has in the
		 * underlying vtkUnstrucutredGrid).
		 * \param point may be 0 or 1. Indicates the first or second point in this Distance
		 * \return id of the desired point in the underlying vtkUnstructuredGrid
		 */
		vtkIdType getPointId(unsigned int point) const;

	private:

		vtkIdType point1;
		vtkIdType point2;
		double distanceOriginal;
		double distanceCurrent;
		double kd;
		double damping;

		double grad[6];
		
		/*!
		 * Computes the distance between the two points
		 * \return distance between the points
		 */
		double getDistance(const double* positions) const;

		/*!
		 * Computes the distance preservation constraint. The constraint
		 * is defined as the ration between the current distance and the original distance -1
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