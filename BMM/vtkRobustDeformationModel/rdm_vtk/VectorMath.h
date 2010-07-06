#pragma once

#include <math.h>

class VectorMath
	{
	public:
		VectorMath(void);
		~VectorMath(void);


		inline static void CrossProduct(const double p1[3], const double p2[3], double result[3]) {

			/*
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x);
			*/
			result[0] = p1[1] * p2[2] - p1[2] * p2[1];
			result[1] = p1[2] * p2[0] - p1[0] * p2[2];
			result[2] = p1[0] * p2[1] - p1[1] * p2[0];
			}

		inline static double DotProduct(const double p1[3], const double p2[3]) {

			return	p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2];
			}

		inline static double length(const double p[3]) {

			return sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
			}
	};
