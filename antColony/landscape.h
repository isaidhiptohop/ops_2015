#ifndef ANT_COLONY_LANDSCAPE_H
#define ANT_COLONY_LANDSCAPE_H

#include<memory>
#include "interfaces.h"
#include "pathiterator.h"

namespace ant_colony {

	class Landscape : public ILandscape {
		std::vector<std::shared_ptr<IPath>> adjacence;
		double ** nextPheromonUpdate;
		int size;
		
		void deallocNextPheromonUpdate();
		
		public:
		Landscape();
		Landscape(double ** adjacence, int size);
		void setAdjacence(double ** adjacence, int size, double ** pheromone = nullptr);
		IPathIterator* getPaths(int position) const override;
		void update() override;
		void setPheromonUpdate(int from, int to) override;
		
		virtual ~Landscape();
	};
}

#endif
