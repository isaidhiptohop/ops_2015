#ifndef ANT_COLONY_LANDSCAPE_H
#define ANT_COLONY_LANDSCAPE_H

#include "interfaces.h"

namespace ant_colony {

	class Landscape : public ILandscape {
		public:
		Landscape();
		IPathIterator* getPaths(int position) const override;
		void update() override;
		void setPheromonUpdate(int from, int to) override;
		
		virtual ~Landscape();
	};
}

#endif
