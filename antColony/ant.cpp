#include"ant.h"
#include <math.h>
#include <random>

namespace ant_colony {
	Ant::Ant() : position{0},destination{-1},lastPosition{-1} {}
	
	Ant::~Ant() {}
	
	int Ant::move(ILandscape& landscape) {
		bool pathToLast = false;
		int pheromonWeighting = 4;
		int pathWeighting = 4;

		//std::vector<IPath> p; //Markus: du kannst kein vector mit dem typ IPath erstellen
				
		IPathIterator & iterator=*(landscape.getPaths(position));
		
		//save the number of optinions
		int optinions = 0;

		for(;iterator; ++iterator){
			optinions++;
			if((*iterator).destination==lastPosition)
				pathToLast = true;
			else
				;//p.push_back(*iterator);
		}

		//list of weightingCost to other knots
		double *weightingCost = new double[optinions];
		//sum of Costs
		double sumWeightingCosts = 0;
		int count =0;

		for(;iterator; ++iterator){
			//temp weightingCost for this knot
			double wCost = pow((*iterator).pheromon,pheromonWeighting)*pow((*iterator).cost,pathWeighting);
			//add to list
			weightingCost[count++]=wCost;
			//add to sum
			sumWeightingCosts+=wCost;
		}		
		//list of probablility
		double *probablilityKnotList = new double[optinions];
		for(int i = 0; i<optinions;i++){
			//probability like formula
			probablilityKnotList[i]=weightingCost[i]/sumWeightingCosts;
		}

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
		//delete things
		delete[] probablilityKnotList;
		delete[] weightingCost;
		return choosenKnot;
	}
	
	//setter with not negativ test
	void Ant::setDestination(int dest){
		if(dest>=0)
			destination = dest;
		else
			throw 0; // todo fehlermeldung
	}
}
