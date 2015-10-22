#include "interfaces.h"

namespace ant_colony {
	IAnt::IAnt() {}
	IAnt::~IAnt() {}
	
	ILandscape::ILandscape() {}
	ILandscape::~ILandscape() {}
	
	IAntColony::IAntColony() {}
	IAntColony::~IAntColony() {}
	
	IPath::IPath() : cost{0}, pheromon{0}, source{0}, destination{0} {}
	IPath::~IPath() {}
	
	IPathIterator::IPathIterator() {}
	IPathIterator::~IPathIterator() {}
}
