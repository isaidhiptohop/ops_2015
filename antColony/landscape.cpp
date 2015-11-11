#include "landscape.h"

namespace ant_colony {

	Landscape::Landscape() : Landscape(nullptr, 0, -1,-1) {
	}
	
	Landscape::Landscape(const double *const* adjacence, int size, int colonyPosition, int foodPosition, double ** pheromone) 
						:adjacence{}, nextPheromonUpdate{nullptr}, size{size}, destinations{colonyPosition, foodPosition} {
		if(adjacence) 
			setAdjacence(adjacence, size);
	}
	
	Landscape::~Landscape() {
		this->deallocNextPheromonUpdate();
	}
	
	void Landscape::deallocNextPheromonUpdate() {
		if(nextPheromonUpdate) {
			delete[] nextPheromonUpdate[0];
			delete[] nextPheromonUpdate;
		}
	}
	
	
	void Landscape::setAdjacence(const double *const* adjacence, int size, double ** pheromone) {
		this->adjacence.clear(); //delete previous adjacence matrix
		this->deallocNextPheromonUpdate(); //delete previous pheromone update matrix
		this->nextPheromonUpdate = new double*[size]; //recreate new matrix
		this->nextPheromonUpdate[0] = new double[size * size](); //recreate new matrix
		
		//initialize adjacence matrix and pheromon matrix
		for(int i = 0; i < size; ++i) { 
			if(i != 0) { //initialize adresses of pheromon matrix
				this->nextPheromonUpdate[i] = this->nextPheromonUpdate[i - 1] + size;
			}
			//initialize adjacence matrix
			Path * list = nullptr; //new linked-list
			for(int n = 0; n < size; ++n) { //for every row in adjacence matrix
				if(i != n && adjacence[i][n] > 0) { //if valid path is given
					list = new Path(adjacence[i][n], pheromone ? pheromone[i][n] : 0, i, n, list); //add path to list
					
				}
			} //end row for
			this->adjacence.push_back(std::shared_ptr<Path>(list)); //add linked list to adjacence matrix
		}
		this->size = size; //save new size
	}
	
	IPathIterator* Landscape::getPaths(int position) const {
		return new PathIterator(adjacence[position].get());
	}
	
	void Landscape::update() {
		for(int i = 0; i < size; ++i) {
			IPathIterator& it = *(this->getPaths(i));
			for(;it; ++it) {
				IPath& p = *it;
				p.pheromon+= this->nextPheromonUpdate[i][p.destination];
				this->nextPheromonUpdate[i][p.destination] = 0;
			}
			delete &it;
		}
		//TODO: pheromone decay
	}
	
	int Landscape::movedPath(int from, int to) {
		//TODO: update correctly
		this->nextPheromonUpdate[from][to] += 1;
		this->nextPheromonUpdate[to][from] += 1;
		int ret = -1;
		for (unsigned int i = 0; i< destinations.size(); ++i) {
			if(destinations[i] == to) {
				ret = destinations[(i+1)%destinations.size()];
				break;
			}
		}
		return ret;
	}
	
	unsigned int Landscape::getSize() const {
		return this->size;
	}
	
	std::vector<std::vector<double>> Landscape::getPheromone() {
		std::vector<std::vector<double>> pheromones(size, std::vector<double>(size, 0));
		for(unsigned int i = 0; i < adjacence.size(); ++i) {
			IPathIterator * it = this->getPaths(i);
			for(; *it; ++(*it)) {
				pheromones[i][(**it).destination] = (**it).pheromon;
			}
			delete it;
		}
		return pheromones;
	}
	
}
