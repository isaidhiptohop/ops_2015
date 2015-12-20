#include "population.h"
#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)

int main () {
    const int N = 5;
    Population<N> * pop = new Population<N> (10, 0.05);
    pop->print ();
    FILE_LINE_OUT ();
    Individual<N> * winners = pop->tournament ();
    FILE_LINE_OUT ();
    pop->print (winners);
    pop->mutation (winners);
    Individual<N> * children = pop->crossOver (winners);
    pop->merge (winners, children);
    pop->print ();
    
    return 0;
}
