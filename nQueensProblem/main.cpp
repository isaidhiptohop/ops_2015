#include "population.h"
#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ << ":" << __LINE__ << std::endl)
/*
template <int N>
void printBoard (Individual<N> * individual);
*/
int main () {
    const int N = 5;
    Population<N> * pop = new Population<N> (100, 0.5);
//    Individual<N> * fittest = nullptr;
//    pop->print ();
    while (pop->iterate()) {
        
/*        Individual<N> * winners = pop->tournament ();
        pop->mutation (winners);
        Individual<N> * children = pop->crossOver (winners);
        pop->merge (winners, children);
        fittest = pop->getFittest (); */
//        if (fittest) printBoard (fittest);
//        else pop->print ();
//        std::cin >> k;
    }
    
    return 0;
}
/*
template <int N>
void printBoard (Individual<N> * individual) {
    for (int j = 0; j < 2*N + 1; j++) {
        if (j % 2) {
            std::cout << "+";
        }
        else {
            std::cout << "-";
        }
    }
    
    std::cout << std::endl;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (individual->getChromosom (i) == j) {
                std::cout << "|X";
            }
            else {
                std::cout << "| ";
            }
        }
        
        std::cout << "|" << std::endl;
        
        for (int j = 0; j < 2*N + 1; j++) {
            if (j % 2) {
                std::cout << "+";
            }
            else {
                std::cout << "-";
            }
        }
        std::cout << std::endl;
    }
}
*/
