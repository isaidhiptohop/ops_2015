#include"antcolony.h"

namespace ant_colony {
	AntColony::AntColony(int antNumber) : AntColony(antNumber, nullptr, 0, -1, -1) {
	}
	
	AntColony::AntColony(int antNumber, const double *const* adjacence, int size, int colonyPosition, int foodPosition, double updateFactor, double decayFactor) : ants(antNumber), landscape(new Landscape(adjacence, size, colonyPosition, foodPosition, updateFactor, decayFactor)), history{{std::vector<int>()}}, generator(std::chrono::system_clock::now().time_since_epoch().count()), pheromonWeight{1}, costWeight{-1} {
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
				int pos = (*it)->move(*landscape, generator, pheromonWeight, costWeight); //TODO: Add weights
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
	
	void AntColony::setCostWeight(double value) {
		if(value <= -1) 
			costWeight = value;
	}
	void AntColony::setPheromonWeight(double value) {
		if(value >= 0) 
			pheromonWeight = value;
	}
}
