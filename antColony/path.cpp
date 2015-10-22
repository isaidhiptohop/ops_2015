#include "path.h"

namespace ant_colony {
	Path::Path(double cost, double pheromon, int source, int destination, Path * next) : cost{cost}, pheromon{pheromon}, source{source}, destination{destination}, next{next} {}
	
	Path::Path(double cost, double pheromon, int source, int destination) : Path(cost, pheromon, source, destination, nullptr){}
	
	Path::~Path() {}
}
