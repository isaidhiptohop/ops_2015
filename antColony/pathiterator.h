#ifndef ANT_COLONY_PATHITERATOR_H
#define ANT_COLONY_PATHITERATOR_H

#include "interfaces.h"
#include "path.h"

namespace ant_colony {
	
	class PathIterator : public IPathIterator {
		Path * path;
		public:
		PathIterator(Path * path);
		IPathIterator& operator++() override;
		const IPath& operator*() const override;
		operator bool() const override;
		~PathIterator();
	};
}

#endif
