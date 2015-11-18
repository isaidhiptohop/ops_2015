#include<iostream>

#include "antColony/ants.h"

//program for testing antColony Namespace

using namespace antC;
using namespace std;

void printLandscape(ILandscape & landscape) {

	auto ph = landscape.getPheromone();
	for(auto &row : ph) {
		for(auto &el : row) {
			cout << el << "\t";
		}
		cout << endl;
	}
}

int main() {
	std::cout << "main()" << std::endl;
	double  arr[][4] = {{0,1,1,1},
						{1,0,0,1},
						{1,0,0,1},
						{1,1,1,0}};
	double * arr1[] = {arr[0],arr[1],arr[2],arr[3]};
	double ** matrix = arr1;			
	IAntColony * col = new AntColony(20, matrix, 4, 0, 3, 1, 0.2);
	col->setPheromonWeight(2);
	col->setCostWeight(-1);
	while(true) {
		int steps;
		cout << "enter steps: ";
		cin >> steps;
		if(steps == 0)
			break;
		col->nextStep(steps);
		printLandscape(col->getLandscape());
		cout << endl;
	}
	delete col;		
	/*for(int i = 0; i< 4; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;*/
	return 0;
}


