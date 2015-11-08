#ifndef ANT_COLONY_ANT_COLONY_H
#define ANT_COLONY_ANT_COLONY_H

#include<vector>
#include<memory>
#include "interfaces.h"
#include "landscape.h"
#include "ant.h"

namespace ant_colony {

	class AntColony : public IAntColony {
		std::vector<std::unique_ptr<IAnt>> ants;
		ILandscape * landscape;
		std::vector<std::vector<int>> history;
		public:
		AntColony(int antNumber);
		AntColony(int antNumber, const double *const* adjacence, int size, int colonyPosition, int foodPosition);
		std::vector<std::vector<int>> nextStep(unsigned int steps = 1) override;
		std::vector<std::vector<int>> getAntMoves() const override;
		ILandscape& getLandscape() const override;
		
		virtual ~AntColony();
	};

}

#endif
