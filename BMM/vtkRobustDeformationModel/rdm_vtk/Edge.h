#pragma once
#include "vtkTetra.h"	// just including that at the moment to have vtkIdType defined - what is the right include?
class Edge
	{
	public:
		Edge(vtkIdType p1, vtkIdType p2) {
			
			// order the points, bigger values first
			this->p1 = std::max(p1, p2);
			this->p2 = std::min(p1, p2);
			}
		~Edge(void){};
		vtkIdType p1;
		vtkIdType p2;
		bool operator < (const Edge& e) const {
			using namespace std;
			return (p1 < e.p1 || p2 < e.p2);
			}
	};
