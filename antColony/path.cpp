#include "path.h"

namespace ant_colony {
	Path::Path(double cost, double pheromon, int source, int destination, Path * next) : IPath(cost, pheromon, source, destination), next{next} {}
	
	Path::Path(double cost, double pheromon, int source, int destination) : Path(cost, pheromon, source, destination, nullptr){}
	
	Path::~Path() {
		if(this->next) {
			delete this->next;
		}
	}
}
