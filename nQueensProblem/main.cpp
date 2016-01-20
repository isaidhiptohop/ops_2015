#include <iostream>
#include <random>

#include "individual.h"
#include "population.h"

char help [] = "\n./nqueens <N> <POPULATION_SIZE> [<M_P>] [<ALPHA>]\n\n"
              "N ................. problem size.\n"
              "POPULATION SIZE ... size of your population. if the population\n"
              "                    is not divisible by 4, the size will be\n"
              "                    round up to the next number thats\n"
              "                    divisible by four.\n"
              "M_P ............... mutation probability. optional. default\n"
              "                    default value is 0\n"
              "ALPHA ............. additional weight for the tournament\n"
              "                    decision. optional. default value is 1\n\n";

int main (int argc, char * argv []) {
    int N = 1;
    int size = 1;
    double m_p = 0;
    int alpha = 1;
    
    std::string arg_1;
    
    if (argc == 2) {
        arg_1.assign (argv [1]);
    }
    
    if (arg_1.compare("help") == 0) {
        std::cout << help;
        return -1;
    } else if (argc == 3) {
        N = std::stoi (argv [1]);
        size = std::stoi (argv [2]);
    } else if (argc == 4) {
        N = std::stoi (argv [1]);
        size = std::stoi (argv [2]);
        m_p = std::stod (argv [3]);
    } else if (argc == 5) {
        N = std::stoi (argv [1]);
        size = std::stoi (argv [2]);
        m_p = std::stod (argv [3]);
        alpha = std::stoi (argv [4]);
    } else if (argc < 2 || argc > 5) {
        std::cout << "invalid amount of arguments. try again." << std::endl;
        return 1;
    }
    
    Population pop (N, size, m_p, alpha);
//    int i = 1;
    Individual * fittest = nullptr;
    do {
//        std::cout << i++ << std::endl;
        fittest = pop.iterate(1);
        pop.saveStats ();
//        std::cout << fittest->getFitness () << std::endl;
    } while (fittest->getFitness () != 0);
    
    fittest->printBoard ();
    
    return 0;
}

