#include "landscape.h"

namespace ant_colony {

	Landscape::Landscape() {
	}
	
	Landscape::~Landscape() {}
	
	IPathIterator* Landscape::getPaths(int position) const {
		return new PathIterator(static_cast<Path*>(adjacence[position].get()));
	}
	
	void Landscape::update() {
	}
	
	void Landscape::setPheromonUpdate(int from, int to) {
	}
	
}
