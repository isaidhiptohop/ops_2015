#include <iostream>
#include <random>

#include "individual.h"
#include "population.h"

int main (int argc, char * argv []) {
    int N = std::stoi (argv [1]);
    int size = std::stoi (argv [2]);
    double m_p = std::stod (argv [3]);
    int alpha = std::stoi (argv [4]);
    
    Population pop (N, size, m_p/10, alpha);
    pop.iterate(10000);
    pop.saveStats ();
    
    return 0;
}

