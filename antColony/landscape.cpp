#include "landscape.h"

namespace ant_colony {

	Landscape::Landscape() : adjacence{}, nextPheromonUpdate{nullptr}, size{0} {
	}
	
	Landscape::Landscape(double ** adjacence, int size) :Landscape() {
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
	
	
	void Landscape::setAdjacence(double ** adjacence, int size, double ** pheromone) {
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
			this->adjacence.push_back(std::shared_ptr<IPath>(list)); //add linked list to adjacence matrix
		}
		this->size = size; //save new size
	}
	
	IPathIterator* Landscape::getPaths(int position) const {
		return new PathIterator(static_cast<Path*>(adjacence[position].get()));
	}
	
	void Landscape::update() {
	}
	
	void Landscape::setPheromonUpdate(int from, int to) {
		
	}
	
}
