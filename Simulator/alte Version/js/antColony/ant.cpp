#include"ant.h"

namespace ant_colony {
	Ant::Ant() : position{0},lastPosition{-1},search{true},visitedDestinations{0}, distribution{std::uniform_real_distribution<double>(0.0,1.0)}, path{} {}
	
	Ant::~Ant() {}
	
	int Ant::move(ILandscape& landscape, std::default_random_engine& generator, double pheromonWeighting, double pathWeighting) {
		bool pathToLast = false;
		
		int choosenKnot=-1; //0 is a valid knot, -1 however not
		if(!search) {
			if(path.empty())
				throw AntException("unexpected empty stack");
				
			choosenKnot = path.top();
			path.pop();
		} else {
				
			IPathIterator & iterator=*(landscape.getPaths(position));
		
			//list of weightingCost to other knots
			std::vector<double> weightingCost = std::vector<double>();
			std::vector<int> destinations = std::vector<int>();
			//sum of Costs
			double sumWeightingCosts = 0;
		

			for(;iterator; ++iterator){
				//optinions++;
				int dest = (*iterator).destination;
				if(dest==lastPosition)
					pathToLast = true;
				else if (!visited(dest)){
					double wCost = pow((*iterator).pheromon + 1,pheromonWeighting)*pow((*iterator).cost,pathWeighting);
					//double wCost = 1;
					//add to sum
					sumWeightingCosts+=wCost;
					//add to list
					weightingCost.push_back(wCost);
					//add destination to list
					destinations.push_back(dest);
				}
			}
		
			//choose next knot
			double random=distribution(generator); //create random 0 <= r < 1
			double searchIntervall = 0;
			for(unsigned int i = 0; i<weightingCost.size();++i){
				//probability like formula
				searchIntervall +=weightingCost[i]/sumWeightingCosts;
			
				//check that the random is in the intervall
				if(random<searchIntervall){
					//make the choosenKnot
					choosenKnot = destinations[i];
					break; //exit loop if decision is made
				}
			}
			//delete things
			delete &iterator;
		
			
		}
		
		if (choosenKnot == -1) {
			if (pathToLast) {
				choosenKnot = this->lastPosition;
				path.pop();
			} else {
				choosenKnot = position;
				lastPosition = -1;
			}
		} else {
			lastPosition = position;
			if(search) {
				path.push(position);
			}
			bool ret = landscape.movedPath(position,choosenKnot, !search);
			if(ret) {
				search = !search;
				lastPosition = -1;
				if (visitedDestinations.size() == landscape.getDestinationNum() - 1)
					visitedDestinations.clear();
		
				visitedDestinations.push_back(position);
			}
		}
		position = choosenKnot;
		return choosenKnot;
	}
	
	bool Ant::visited(int pos) {
		for(auto p : visitedDestinations) {
			if(p == pos)
				return true;
		}
		return false;
	}
}
