#include<iostream>

#include "antColony/ants.h"

//program for testing antColony Namespace

using namespace ant_colony;

int main() {
	std::cout << "main()" << std::endl;
	Ant a = Ant();
	AntColony c = AntColony();
	Landscape l = Landscape();
	Path p = Path(0,0,0,0);
	PathIterator pi = PathIterator(&p);
	
	return 0;
}
