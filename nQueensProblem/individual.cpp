#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)

//std::default_random_engine generator 
//    (std::chrono::system_clock::now().time_since_epoch().count());

template <int N>
Individual<N>::Individual (int * chromosom) {
    this->chromosom = new int [N];
    
    if (chromosom) {
        for (int i = 0; i < N; i++) {
            this->chromosom [i] = chromosom [i];
        }
    }
    else {
        std::uniform_int_distribution<int> distribution (0, N-1);
        
        for (int i = 0; i < N; i++) {
            this->chromosom [i] = distribution (generator);
        }
    }
}

template <int N>
Individual<N>::Individual (const Individual & individual) {
    for (int i = 0; i < N; i++)
    {
        this->chromosom [i] = individual.chromosom [i];
    }
}

template <int N>
Individual<N> & Individual<N>::operator= (const Individual & individual) {
    for (int i = 0; i < N; i++)
    {
        this->chromosom [i] = individual.chromosom [i];
    }
    
    return * this;
}

template <int N>
Individual<N>::~Individual () { delete [] chromosom; }

template <int N>
void Individual<N>::setChromosom (int * array) {
    for (int i = 0; i < N; i++) {
        chromosom [i] = array [i];
    }
}

template <int N>
void Individual<N>::getChromosom (int * array) {
    for (int i = 0; i < N; i++) {
        array [i] = chromosom [i];
    }
}

template <int N>
int Individual<N>::getChromosom (int i) const {
    return chromosom [i];
}

template <int N>
int Individual<N>::getFitness ()
{
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
    
    return fitness;
}

template <int N>
void Individual<N>::mutate () {
    std::uniform_int_distribution<int> distribution (0, N-1);
    
    int mutations_amount = distribution (generator);
    int * indices = new int [mutations_amount];
    
    for (int i = 0; i < mutations_amount; i++) {
        indices [i] = distribution (generator);
    }
    
    for (int i = 0; i < mutations_amount; i++) {
        chromosom [indices [i]] = distribution (generator);
    }
    
    delete [] indices;
}

template <int N>
void Individual<N>::print ()
{
    for (int i = 0; i < N; i++) {
        std::cout << "[" << chromosom [i] << "]\n";
    }
}

template <int N>
void Individual<N>::crossOver (const Individual<N> * parent_1, 
                               const Individual<N> * parent_2,
                               Individual<N> & children_1, 
                               Individual<N> & children_2) {
    std::uniform_int_distribution<int> distribution (1, N-1);
    
    int pivot = distribution (generator);
    
    int chromosom_1 [N];
    int chromosom_2 [N];
    
    for (int i = 0; i < pivot; i++) {
        chromosom_1 [i] = parent_1->getChromosom (i);
        chromosom_2 [i] = parent_2->getChromosom (i);
    }
    for (int i = pivot; i < N; i++) {
        chromosom_1 [i] = parent_2->getChromosom (i);
        chromosom_2 [i] = parent_1->getChromosom (i);
    }
    
    children_1.setChromosom (chromosom_1);
    children_2.setChromosom (chromosom_2);
}

