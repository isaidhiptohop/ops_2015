#include <iostream>
#include <random>
#include <chrono>

#include "individual.h"

std::default_random_engine generator 
    (std::chrono::system_clock::now().time_since_epoch().count());

/***** STATIC VARIBALE INITIALIZATION *****/

int Individual::N;



/***** PUBLIC INDIVIDUAL FUNCTIONS *****/

// CONSTRUCTORS

Individual::Individual (int * chromosom) {
    this->chromosom = new int [N];
    
    if (chromosom) {
        for (int i = 0; i < N; i++) {
            this->chromosom [i] = chromosom [i];
        }
    } else {
        std::uniform_int_distribution<int> distribution (0, N-1);
        
        for (int i = 0; i < N; i++) {
            this->chromosom [i] = distribution (generator);
        }
    }

    this->calcFitness ();
}

Individual::Individual (const Individual & individual) {
    this->chromosom = new int [N];
    
    for (int i = 0; i < N; i++) {
        this->chromosom [i] = individual.chromosom [i];
    }
    
    this->fitness = individual.fitness;
}

Individual & Individual::operator= (const Individual & individual) {
    this->chromosom = new int [N];
    
    for (int i = 0; i < N; i++) {
        this->chromosom [i] = individual.chromosom [i];
    }
    
    this->fitness = individual.fitness;
    
    return * this;
}


// DESTRUCTOR

Individual::~Individual () {
    delete [] this->chromosom;
}

/*
void setChromosom (int * array);
void getChromosom (int * array);
int getChromosom (int i) const;
int getFitness ();
*/


// GETTER & SETTER METHODS

void Individual::setChromosom (int * array) {
    for (int i = 0; i < N; i++) {
        chromosom [i] = array [i];
    }
}

int Individual::getChromosom (int i) const {
    return this->chromosom [i];
}

int Individual::getFitness () {
    calcFitness ();
    return this->fitness;
}


// BEHAVIOUR METHODS

void Individual::mutate () {
    std::uniform_int_distribution<int> distribution (0, N-1);
    
    int mutations = distribution (generator);
    
    for (int i = 0; i < mutations; i++) {
        this->chromosom [distribution (generator)] = distribution (generator);
    }
    
    this->calcFitness ();
}


// OUTPUT METHODS

void Individual::print () {
    for (int i = 0; i < N; i++) {
        std::cout << "[" << chromosom [i] << "]\n";
    }
}

void Individual::printBoard () {
    std::cout << "+";    
    for (int i = 0; i < N; i++) {
        std::cout << "--+";
    }    
    std::cout << std::endl;

    for (int i = 0; i < N; i++) {
        std::cout << "|";
        for (int j = 0; j < N; j++) {
            if (j != this->chromosom [i]) {
                std::cout << "  |";
            } else {
                std::cout << "XX|";
            }
        }
        
        std::cout << std::endl << "+";    
        for (int i = 0; i < N; i++) {
            std::cout << "--+";
        }    
        std::cout << std::endl;
    }
}



/***** PRIVATE INDIVIDUAL FUNCTIONS *****/

void Individual::calcFitness () {
    int fitness = 0;
    
    for (int i = 0; i < N; i++) {
        int buf = 0;

        for (int j = 0; j < N; j++) {
            if (i != j) {
                if (chromosom [i] == chromosom [j]) {
                    buf--;
                }
                else if (chromosom [i] + abs (i - j) == chromosom [j]) {
                    buf--;
                }
                else if (chromosom [i] - abs (i - j) == chromosom [j]) {
                    buf--;
                }
            }
        }

        if (buf < fitness) fitness = buf;
    }
    
    this->fitness = fitness;
}

