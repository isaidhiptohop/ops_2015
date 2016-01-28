#ifndef ANT_COLONY_LANDSCAPE_H
#define ANT_COLONY_LANDSCAPE_H

#include<memory>
#include<sstream>
#include "interfaces.h"
#include "pathiterator.h"
#include "exceptions.h"

namespace ant_colony {

	class Landscape : public ILandscape {
		std::vector<std::shared_ptr<Path>> adjacence;
		double ** nextPheromonUpdate;
		int size;
		std::vector<int> destinations;
		double updateFactor, decayFactor;
		
		void deallocNextPheromonUpdate();
		
		public:
		Landscape();
		Landscape(const double *const* adjacence, int size, int colonyPostion, int foodPostion, double updateFactor = 1, double decayFactor = 0.02, double ** pheromone = nullptr);
		void setAdjacence(const double *const* adjacence, int size, double ** pheromone = nullptr);
		IPathIterator* getPaths(int position) const override;
		void update() override;
		bool movedPath(int from, int to, bool addPheromon = true) override;
		unsigned int getDestinationNum() const override;
		std::vector<std::vector<double>> getPheromone() override;
		
		unsigned int getSize() const override;
		
		virtual ~Landscape();
	};
}

#endif
