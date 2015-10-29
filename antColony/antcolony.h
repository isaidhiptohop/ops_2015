#ifndef ANT_COLONY_ANT_COLONY_H
#define ANT_COLONY_ANT_COLONY_H

#include<vector>
#include<memory>
#include "interfaces.h"
#include "landscape.h"

namespace ant_colony {

	class AntColony : public IAntColony {
		//std::vector<std::unique_ptr<int>> ants;
		ILandscape * landscape;
		std::vector<std::vector<int>> history;
		public:
		AntColony();
		std::vector<std::vector<int>> nextStep(unsigned int steps = 1) override;
		std::vector<std::vector<int>> getAntMoves() const override;
		ILandscape& getLandscape() const override;
		
		virtual ~AntColony();
	};

}

#endif
