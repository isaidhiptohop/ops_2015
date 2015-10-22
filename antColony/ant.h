#ifndef ANT_COLONY_ANT_H
#define ANT_COLONY_ANT_H

#include"interfaces.h"

namespace ant_colony {

	class Ant : public IAnt {
		int position, destination, lastPosition;
		public:
		Ant();
		int move(ILandscape& landscape) override;
		void setDestination(int) override;
		
		virtual ~Ant();
	};
}

#endif
