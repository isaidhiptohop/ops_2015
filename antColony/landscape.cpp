#include "landscape.h"

namespace ant_colony {

	Landscape::Landscape() : Landscape(nullptr, 0, -1,-1) {
	}
	
	Landscape::Landscape(const double *const* adjacence, int size, int colonyPosition, int foodPosition, double updateFactor, double decayFactor, double ** pheromone) 
						:adjacence{}, nextPheromonUpdate{nullptr}, size{size}, destinations{colonyPosition, foodPosition}, updateFactor{updateFactor}, decayFactor{decayFactor} {
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
				p.pheromon *= (1 - decayFactor);
				p.pheromon+= this->nextPheromonUpdate[i][p.destination];
				this->nextPheromonUpdate[i][p.destination] = 0;
			}
			delete &it;
		}
	}
	
	bool Landscape::movedPath(int from, int to) {
		//TODO: make more performant
		double cost = 0; 
		IPathIterator * it = this->getPaths(from);
		for(; *it; ++(*it)) {
			if((**it).destination == to) {
				cost = (**it).cost;
				break;
			}
		}
		delete it;
		
		if(cost == 0) 
			throw AntException("Path not found. Matrix maybe corrupted.");
		
		double diff = updateFactor / cost;
		this->nextPheromonUpdate[from][to] += diff;
		this->nextPheromonUpdate[to][from] += diff;
		int ret = false;
		for (unsigned int i = 0; i< destinations.size(); ++i) {
			if(destinations[i] == to) {
				ret = true;
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
