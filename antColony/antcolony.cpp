#include"antcolony.h"

namespace ant_colony {
	AntColony::AntColony() : landscape(new Landscape()), history{{std::vector<int>()}} {}
	
	AntColony::~AntColony() {
		delete landscape;
	}
	
	std::vector<std::vector<int>> AntColony::nextStep(unsigned int steps) {
		return history; //TODO
	}
	
	std::vector<std::vector<int>> AntColony::getAntMoves() const {
		return history;
	}
	
	ILandscape& AntColony::getLandscape() const {
		return *landscape;
	}
	
}
