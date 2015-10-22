#include"ant.h"

namespace ant_colony {
	Ant::Ant() : position{0},destination{-1},lastPosition{-1} {}
	
	Ant::~Ant() {}
	
	int Ant::move(ILandscape& landscape) {
		return 0;
	}
	
	void Ant::setDestination(int) {
	}
}
