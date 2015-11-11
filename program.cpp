#include<iostream>

#include "antColony/ants.h"

//program for testing antColony Namespace

using namespace antC;
using namespace std;

int main() {
	std::cout << "main()" << std::endl;
	double  arr[][4] = {{0,1,1,1},
						{1,0,0,1},
						{1,0,0,1},
						{1,1,1,0}};
	double * arr1[] = {arr[0],arr[1],arr[2],arr[3]};
	double ** matrix = arr1;			
	IAntColony * col = new AntColony(40, matrix, 4, 0, 3);
	col->nextStep(20);
	auto ph = col->getLandscape().getPheromone();
	for(auto &row : ph) {
		for(auto &el : row) {
			cout << el << " ";
		}
		cout << endl;
	}
	
	delete col;		
	/*for(int i = 0; i< 4; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;*/
	return 0;
}
