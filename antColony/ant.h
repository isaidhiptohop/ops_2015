#ifndef ANT_COLONY_ANT_H
#define ANT_COLONY_ANT_H

#include"interfaces.h"
#include<vector>
#include<random>
#include <math.h>
#include "exceptions.h"

namespace ant_colony {

	class Ant : public IAnt {
		int position, lastPosition;
  		std::uniform_real_distribution<double> distribution;
		public:
		Ant();
		int move(ILandscape& landscape, std::default_random_engine& generator, double pheromonWeighting = 1, double pathWeighting = 1) override;
		
		virtual ~Ant();
	};
}

#endif
