#include "landscape.h"

namespace ant_colony {

	Landscape::Landscape() {
	}
	
	Landscape::~Landscape() {}
	
	IPathIterator* Landscape::getPaths(int position) const {
		return new PathIterator(nullptr);
	}
	
	void Landscape::update() {
	}
	
	void Landscape::setPheromonUpdate(int from, int to) {
	}
	
}
