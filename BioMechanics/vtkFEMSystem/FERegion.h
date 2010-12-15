/*
 * FERegion.h
 *
 *  Created on: Nov 25, 2010
 *      Author: jballesteros
 */

#ifndef FEREGION_H_
#define FEREGION_H_

#include "dolfin.h"

class FERegion : public dolfin::SubDomain
{
public:
	bool inside(const dolfin::Array<double>& x, bool on_boundary) const
	{
		return x[0] < this->X && on_boundary;
	}
	void SetX(double x){this->X=x;}
	void SetY(double y){this->Y=y;}
	void SetZ(double z){this->Z=z;}

private:

	double X;
	double Y;
	double Z;
};


#endif /* FEREGION_H_ */
