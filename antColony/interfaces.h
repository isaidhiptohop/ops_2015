#ifndef ANT_COLONY_INTERFACES_H
#define ANT_COLONY_INTERFACES_H

#include<vector>

namespace ant_colony {
	class IAnt;
	class ILandscape;
	class IAntColony;
	class IPath;
	class IPathIterator;


	class IAnt {
		protected:
		IAnt();
		public:
		virtual int move(ILandscape& landscape) = 0;
		virtual void setDestination(int) = 0;
		virtual ~IAnt();
	};

	class ILandscape {
		protected:
		ILandscape();
		public:
		virtual IPathIterator* getPaths(int position) const = 0;
		virtual void update() = 0;
		virtual void setPheromonUpdate(int from, int to) = 0;
		virtual ~ILandscape();
	};

	class IAntColony {
		protected:
		IAntColony();
		public:
		virtual std::vector<std::vector<int>> nextStep(unsigned int steps = 1) = 0;
		virtual std::vector<std::vector<int>> getAntMoves() const = 0;
		virtual ILandscape& getLandscape() const = 0;
		virtual ~IAntColony();
	};

	class IPath {
		protected:
		IPath();
		public:
		double cost;
		double pheromon;
		int source;
		int destination;
		
		virtual ~IPath() = 0;
	};

	class IPathIterator {
		protected:
		IPathIterator();
		public:
		virtual IPathIterator& operator++() = 0;
		virtual IPath& operator*() const = 0;
		virtual operator bool() const = 0;
		virtual ~IPathIterator();
	};
	
}

namespace antC = ant_colony;
#endif
