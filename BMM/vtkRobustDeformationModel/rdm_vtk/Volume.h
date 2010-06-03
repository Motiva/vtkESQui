#pragma once

#include "UGridAccelerator.h"
#include "vtkTetra.h"

#include <vector>
#include <algorithm>

class Volume
	{
	public:

		Volume(vtkIdType p1, vtkIdType p2, vtkIdType p3, vtkIdType p4, double kv);

		~Volume(void);

		void initVolumeOriginal(const double* positions);

		/*!
		 *   Precomputes some value that will be needed multiple
		 * times by the force computation
		 */
		void initForceComputation(const double* positions);

		/*!
		 * Computes the force resulting from the volume preservation constraint.
		 * \param result double array to store the resulting force into
		 * \param number of the point to compute the force for, maybe 0, 1, 2 or 3
		 */
		void computeForce(double result[3], const double* positions, unsigned int point);

		/*!
		 * Returns the vtk id of an point (the id a point has in the
		 * underlying vtkUnstrucutredGrid).
		 * \param point may be 0, 1, 2 or 3. Indicates the first, second, third or fourth point in this Volume
		 * \return id of the desired point in the underlying vtkUnstructuredGrid
		 */
		vtkIdType getPointId(unsigned int point) const;



	private:

		vtkIdType point1;
		vtkIdType point2;
		vtkIdType point3;
		vtkIdType point4;

		double volumeOriginal;
		double volumeCurrent;

		double kv;
		
		double grad[12];

		//! Computes the volume of this tetrahedron
		double getVolume(const double* positions) const;

		double getVolumeConstraint() const;
		
		/*!
		 * Computes the gradient of the constraint 
		 */
		void getGradient(double result[3], const double* positions, unsigned int point) const;
	};
