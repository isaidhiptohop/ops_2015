#ifndef ANT_COLONY_PATH_H
#define ANT_COLONY_PATH_H

#include "interfaces.h"

namespace ant_colony {
	class Path : public IPath {
		public:
		double cost;
		double pheromon;
		int source;
		int destination;
		Path * next;
		
		
		Path(double cost, double pheromon, int source, int destination);
		Path(double cost, double pheromon, int source, int destination, Path * next);
		
		virtual ~Path();
	};
}

#endif
