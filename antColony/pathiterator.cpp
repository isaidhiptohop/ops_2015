#include "pathiterator.h"

namespace ant_colony {

		PathIterator::PathIterator(Path * path) : path{path} {}
		
		PathIterator::~PathIterator() {}
		
		IPathIterator& PathIterator::operator++() {
			path = path->next;
			return *this;
		}
		
		const IPath& PathIterator::operator*() const {
			return *path;
		}
		
		PathIterator::operator bool() const {
			return path != nullptr;
		}
}
