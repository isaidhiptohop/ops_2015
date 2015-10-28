#include"ant.h"

namespace ant_colony {
	Ant::Ant() : position{0},destination{-1},lastPosition{-1} {}
	
	Ant::~Ant() {}
	
	int Ant::move(ILandscape& landscape) {
		bool pathToLast = false;
		std::vector<IPath> p;
				
		IPathIterator &iterator=*(landscape.getPaths(position));
		for(;iterator; ++iterator){
			if(*iterator.destination==lastPosition)
				pathToLast = true;
			else
				p.push_back(*iterator);
		}

		return 0;
	}
	
	//setter with not negativ test
	void Ant::setDestination(int dest){
		if(dest>=0)
			destination = dest;
		else
			throw 0; // todo fehlermeldung
	}
}
