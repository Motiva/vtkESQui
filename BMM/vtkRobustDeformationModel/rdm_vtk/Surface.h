#pragma once

#include "vtkTetra.h"	// just including that at the moment to have vtkIdType defined - what is the right include?

class UGridAccelerator;

class Surface
	{
	public:

		Surface(vtkIdType p1, vtkIdType p2, vtkIdType p3, double ks);
		~Surface(void);

		bool operator < (const Surface& s) const;

		void initSurfaceOriginal(const double* positions);

		/*!
		 * Computes the force resulting from the surface preservation constraint.
		 * \param result double array to store the resulting force into
		 * \param number of the point to compute the force for, maybe 0, 1 or 2
		 */
		void computeForce(double result[3], const double* positions, unsigned int point);

		/*!
		 * Returns the vtk id of an point (the id a point has in the
		 * underlying vtkUnstrucutredGrid).
		 * \param point may be 0, 1 or 2. Indicates the first, second or third point in this Surface
		 * \return id of the desired point in the underlying vtkUnstructuredGrid
		 */
		vtkIdType getPointId(unsigned int point) const;


	private:

		vtkIdType point1;
		vtkIdType point2;
		vtkIdType point3;

		double surfaceOriginal;
		double surfaceCurrent;
		double ks;

		//UGridAccelerator* accel;

		double getSurface(const double* positions) const;

		double getSurfaceConstraint() const;

		/*!
		 * Computes the gradient of the constraint 
		 */
		void getGradient(double result[3], const double* positions, unsigned int point) const;
	};
