#ifndef ANT_COLONY_ANT_H
#define ANT_COLONY_ANT_H

#include"interfaces.h"
#include<vector>
#include<random>
#include <math.h>
#include "exceptions.h"
#include<stack>

namespace ant_colony {

	class Ant : public IAnt {
		int position, lastPosition;
		bool search;
		std::vector<int> visitedDestinations;
  		std::uniform_real_distribution<double> distribution;
  		std::stack<int> path;
  		std::deque<std::string> debugStack;
  		
  		bool visited(int pos);
		public:
		Ant();
		int move(ILandscape& landscape, std::default_random_engine& generator, double pheromonWeighting = 1, double pathWeighting = 1) override;
		
		virtual ~Ant();
	};
}

#endif
