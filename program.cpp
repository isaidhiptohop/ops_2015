#include<iostream>
#include<iomanip>

#include "antColony/ants.h"

//program for testing antColony Namespace

using namespace antC;
using namespace std;

void printNum(double num) {

	cout << setprecision(3) << setw(11) << setfill(' ') << num;
}

void printLandscape(ILandscape & landscape) {

	auto ph = landscape.getPheromone();
	cout << "     ";
	for(unsigned int i = 0; i < ph.size(); ++i) {
		printNum(i);
	}
	cout << endl << endl;
	unsigned int i = 0;
	for(auto &row : ph) {
		cout << setw(2) << i << ":  ";
		++i;
		for(auto &el : row) {
			printNum(el);
		}
		cout << endl;
	}
}

int main() {
	std::cout << "main()" << std::endl;
	/*double  arr[][4] = {{0,1,1,1},
						{1,0,0,1},
						{1,0,0,1},
						{1,1,1,0}};
	double * arr1[] = {arr[0],arr[1],arr[2],arr[3]};
	double ** matrix = arr1;*/

	/**
	double arr [15][15] = { {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
							{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
							{1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
							{0,1,1,0,0,0,0,0,1,0,1,0,0,0,0},
							{0,0,1,0,0,1,1,0,0,0,0,0,0,0,0},
							{0,0,0,0,1,0,1,1,0,0,0,0,0,0,0},
							{0,0,0,0,1,1,0,0,1,0,0,0,0,0,0},
							{0,0,0,0,0,1,0,0,1,1,0,0,0,0,0},
							{0,0,0,1,0,0,1,1,0,1,1,1,0,0,0},
							{0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
							{0,0,0,1,0,0,0,0,1,0,0,1,0,1,0},
							{0,0,0,0,0,0,0,0,1,1,1,0,1,1,1},
							{0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
							{0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
							{0,0,0,0,0,0,0,0,0,0,0,1,1,1,0} }; */

	double arr [3][3] = { {0,0,1},
							{0,0,1},
							{1,1,0} };
	
	double * arr1[3];
	for(int i = 0; i < 3;++i)
		arr1[i] = arr[i];
	
	double ** matrix = arr1;

	IAntColony * col = new AntColony(40, matrix, 3, 0, 1, 40, 0.05);

	col->setPheromonWeight(2.5);
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


