#include "interfaces.h"

namespace ant_colony {
	IAnt::IAnt() {}
	IAnt::~IAnt() {}
	
	ILandscape::ILandscape() {}
	ILandscape::~ILandscape() {}
	
	IAntColony::IAntColony() {}
	IAntColony::~IAntColony() {}
	
	IPath::IPath() : IPath(0,0,0,0) {}
	IPath::IPath(double cost, double pheromon, int source, int destination) : cost{cost}, pheromon{pheromon}, source{source}, destination{destination} {}
	IPath::~IPath() {}
	
	IPathIterator::IPathIterator() {}
	IPathIterator::~IPathIterator() {}
}
