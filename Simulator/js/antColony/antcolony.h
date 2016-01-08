#ifndef ANT_COLONY_ANT_COLONY_H
#define ANT_COLONY_ANT_COLONY_H

#include<vector>
#include<memory>
#include<random>
#include<chrono>
#include "interfaces.h"
#include "landscape.h"
#include "ant.h"

namespace ant_colony {

	class AntColony : public IAntColony {
		std::vector<std::unique_ptr<IAnt>> ants;
		ILandscape * landscape;
		std::vector<std::vector<int>> history;
		std::default_random_engine generator;
		
		double pheromonWeight, costWeight;
		
		public:
		AntColony(int antNumber);
		AntColony(int antNumber, const double *const* adjacence, int size, int colonyPosition, int foodPosition, double updateFactor = 1, double decayFactor = 0.02);
		std::vector<std::vector<int>> nextStep(unsigned int steps = 1) override;
		std::vector<std::vector<int>> getAntMoves() const override;
		ILandscape& getLandscape() const override;
		
		void setCostWeight(double value) override;
		void setPheromonWeight(double value) override;
		
		virtual ~AntColony();
	};

}

#endif
