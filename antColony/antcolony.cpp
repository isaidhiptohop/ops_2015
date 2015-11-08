#include"antcolony.h"

namespace ant_colony {
	AntColony::AntColony(int antNumber) : AntColony(antNumber, nullptr, 0, -1, -1) {
	}
	
	AntColony::AntColony(int antNumber, const double *const* adjacence, int size, int colonyPosition, int foodPosition) : ants(antNumber), landscape(new Landscape(adjacence, size, colonyPosition, foodPosition)), history{{std::vector<int>()}} {
		for(int i = 0; i < antNumber; ++i) {
			ants[i] = std::unique_ptr<IAnt>(new Ant());
		}
	}
	
	AntColony::~AntColony() {
		delete landscape;
	}
	
	std::vector<std::vector<int>> AntColony::nextStep(unsigned int steps) {
		std::vector<std::vector<int>> hChange = {};
		for(unsigned int i = 0; i < steps; ++i) {
			std::vector<int> current(landscape->getSize(), 0);
			for(auto it = ants.begin(); it != ants.end(); ++it) {
				int pos = (*it)->move(*landscape,1,1);
				++current[pos];
			}
			landscape->update();
			hChange.push_back(current);
		}
		history.insert(history.end(),hChange.begin(), hChange.end());
		return hChange;
	}
	
	std::vector<std::vector<int>> AntColony::getAntMoves() const {
		return history;
	}
	
	ILandscape& AntColony::getLandscape() const {
		return *landscape;
	}
	
}
