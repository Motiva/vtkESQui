#include <set>
#include <iostream>
#include "Distance.h"

int main (int argc, char *argv[]) {

	using namespace std;
	set<Distance> distances;

	distances.insert(Distance(0,1,0.5,0.5));
	distances.insert(Distance(1,0,0.5,0.5));
	distances.insert(Distance(1,0,0.5,0.5));

	distances.insert(Distance(1,12,0.5,0.5));
	distances.insert(Distance(12,1,0.5,0.5));

	cout << distances.size() << endl;
	
	int i;
	cin >> i;
	return 0;
	}
