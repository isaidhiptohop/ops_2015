#include"ant.h"

namespace ant_colony {
	Ant::Ant() : position{0},destination{-1},lastPosition{-1}, distribution{std::uniform_real_distribution<double>(0.0,1.0)} {}
	
	Ant::~Ant() {}
	
	int Ant::move(ILandscape& landscape, double pheromonWeighting, double pathWeighting) {
		bool pathToLast = false;
		//as parameters
		//double pheromonWeighting = 4;
		//double pathWeighting = 4;

				
		IPathIterator & iterator=*(landscape.getPaths(position));
		
		//save the number of optinions
		//int optinions = 0;
		
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
			else {
				//temp weightingCost for this knot, NOT WORKING! always calculates 0
				//double wCost = pow((*iterator).pheromon,pheromonWeighting)*pow((*iterator).cost,pathWeighting);
				double wCost = 1;
				//add to sum
				sumWeightingCosts+=wCost;
				//add to list
				weightingCost.push_back(wCost);
				//add destination to list
				destinations.push_back(dest);
			}
		}
		

		//list of weightingCost to other knots
		//double *weightingCost = new double[optinions];
		//sum of Costs
		//double sumWeightingCosts = 0;
		//int count =0;

		/*for(;iterator; ++iterator){
			//temp weightingCost for this knot
			double wCost = pow((*iterator).pheromon,pheromonWeighting)*pow((*iterator).cost,pathWeighting);
			//add to list
			weightingCost[count++]=wCost;
			//add to sum
			sumWeightingCosts+=wCost;
		}*/		
		//list of probablility
		//double *probablilityKnotList = new double[weightingCost.size()];
		
		//choose next knot
		double random=distribution(generator); //create random 0 <= r < 1
		double searchIntervall = 0;
		int choosenKnot=-1; //0 is a valid knot, -1 however not
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
		/*
		//choose next knot
		double random=(double)rand();
		
		int choosenKnot=0;

		//reset count		
		count =0;
		//set Intervall to firste probability (0-firstP)
		double searchIntervall = probablilityKnotList[0];
		//go throw the paths
		for(;iterator; ++iterator){
			//check that the random is in the intervall
			if(random<searchIntervall){
				//check if its the first intervall 
				if(choosenKnot==0){
					//make the choosenKnot
					choosenKnot = (*iterator).destination;
				}
			}
			else{
				//make the next intervall
				searchIntervall+=probablilityKnotList[count++];
			}
		}
		*/
		//delete things
		delete &iterator;
		if (choosenKnot == -1) {
			if (pathToLast) {
				choosenKnot = this->lastPosition;
			} else {
				choosenKnot = position;
				lastPosition = -1;
			}
		}
		int ret = landscape.movedPath(position,choosenKnot);
		if(ret > 0) {
			destination = ret;
			lastPosition = -1;
		}
		position = choosenKnot;
		return choosenKnot;
	}
	
	//setter with not negativ test
	void Ant::setDestination(int dest){
		if(dest>=0)
			destination = dest;
		else
			throw AntException("no valid destination given"); // todo fehlermeldung
	}
}
