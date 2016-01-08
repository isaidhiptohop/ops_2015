#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)

//std::default_random_engine generator 
//    (std::chrono::system_clock::now().time_since_epoch().count());

Individual::Individual (int * chromosom) {
    this->chromosom = new int [N];
    
    if (chromosom) {
        for (unsigned int i = 0; i < N; i++) {
            this->chromosom [i] = chromosom [i];
        }
    }
    else {
        std::uniform_int_distribution<int> distribution (0, N-1);
        
        for (unsigned int i = 0; i < N; i++) {
            this->chromosom [i] = distribution (generator);
        }
    }
}

Individual::Individual (const Individual & individual) {
    for (unsigned int i = 0; i < N; i++)
    {
        this->chromosom [i] = individual.chromosom [i];
    }
}

Individual & Individual::operator= (const Individual & individual) {
    for (unsigned int i = 0; i < N; i++)
    {
        this->chromosom [i] = individual.chromosom [i];
    }
    
    return * this;
}

Individual::~Individual () { delete [] chromosom; }

void Individual::setChromosom (int * array) {
    for (int i = 0; i < N; i++) {
        chromosom [i] = array [i];
    }
}

void Individual::getChromosom (int * array) {
    for (int i = 0; i < N; i++) {
        array [i] = chromosom [i];
    }
}

int Individual::getChromosom (int i) const {
    return chromosom [i];
}

int Individual::getFitness ()
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

void Individual::mutate () {
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

void Individual::print ()
{
    for (int i = 0; i < N; i++) {
        std::cout << "[" << chromosom [i] << "]\n";
    }
}

void Individual::crossOver (const Individual * parent_1, 
                               const Individual * parent_2,
                               Individual & children_1, 
                               Individual & children_2) {
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

