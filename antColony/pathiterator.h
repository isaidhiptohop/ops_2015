#ifndef ANT_COLONY_PATHITERATOR_H
#define ANT_COLONY_PATHITERATOR_H

#include "interfaces.h"
#include "path.h"

namespace ant_colony {
	
	class PathIterator : public IPathIterator {
		Path * path;
		public:
		PathIterator(Path * path);
		virtual IPathIterator& operator++();
		virtual IPath& operator*() const;
		virtual operator bool() const;
		virtual ~PathIterator();
	};
}

#endif
