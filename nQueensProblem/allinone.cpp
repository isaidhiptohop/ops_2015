#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

#define FILE_LINE_OUT() (std::cout << __FILE__ << ":" << __LINE__ << std::endl)

bool OUTPUT = false;

extern std::default_random_engine generator;

template <int N>
class Individual {
        int * chromosom = nullptr;
    
    public:
        Individual (int * chromosom = nullptr);
        Individual (const Individual & individual);
        Individual & operator= (const Individual & individual);
        ~Individual ();
        
        void setChromosom (int * array);
        void getChromosom (int * array);
        int getChromosom (int i) const;
        int getFitness ();
        void mutate ();
        void print ();

        static void crossOver (const Individual<N> * parent_1, 
                               const Individual<N> * parent_2,
                               Individual<N> & children_1, 
                               Individual<N> & children_2);
};

template <int N>
class Population {
        int size;
        double mutation_probability;
        Individual<N> * individuals;
        
    public:
        Population (int size, double mutation_probability);
        Individual<N> * iterate (int iterations = 1);
        Individual<N> * getFittest ();
        void print (Individual<N> * gene_pool);
        void print ();
        
//    private:
        Individual<N> * tournament ();
        void mutation (Individual<N> * gene_pool);
        Individual<N> * crossOver (Individual<N> * gene_pool);
        void merge (Individual<N> * pool_1, Individual<N> * pool_2);

        void dice (int iterations = 1);
        void swap (int index_1, int index_2);
};

/*
template <int N>
void printBoard (Individual<N> * individual);
*/
std::default_random_engine generator 
    (std::chrono::system_clock::now().time_since_epoch().count());

int main () {
    const int N = 5;
    Population<N> * pop = new Population<N> (100, 0.5);
    Individual<N> * fittest = nullptr;
//    pop->print ();
    while (!fittest) {
        fittest = pop->iterate();
/*        Individual<N> * winners = pop->tournament ();
        pop->mutation (winners);
        Individual<N> * children = pop->crossOver (winners);
        pop->merge (winners, children);
        fittest = pop->getFittest (); */
//        if (fittest) printBoard (fittest);
//        else pop->print ();
//        std::cin >> k;
    }
    
    fittest->print();
    
//    pop->getFittest ()->print();
    
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

template <int N>
Population<N>::Population (int size, double mutation_probability) {
    this->mutation_probability = mutation_probability;
    
    if (size % 4 != 0) {
        this->size = size + (4 - size % 4);
        individuals = new Individual<N> [this->size];
    }
    else {
        this->size = size;
        individuals = new Individual<N> [this->size];
    }
}

template <int N>
Individual<N> * Population<N>::iterate (int iterations) {
    for (int i = 0; i < iterations; i++) {
        Individual<N> * fittest = getFittest ();
        if (fittest) {
            return fittest;
        }
        else {
//            print ();
            Individual<N> * winners = tournament ();
            mutation (winners);
            Individual<N> * children = crossOver (winners);
            merge (winners, children);
            return fittest;
        }
    }
    
    return nullptr;
}

template <int N>
void Population<N>::print (Individual<N> * gene_pool) {
    for (int k = 0; k < size/2; k+=10) {
        int k_up = k + 10;
        if (k_up > size/2) k_up = size/2;
        
        for (int i = 0; i < N; i++) {
            for (int j = k; j < k_up; j++) {
                std::cout << "[" << gene_pool [j].getChromosom (i) 
                          << "]  ";
            }
            
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
    }
}

template <int N>
void Population<N>::print () {
    std::cout << "============================================" 
              << std::endl << "Population size: " << size 
              << std::endl << std::endl;
    
    for (int k = 0; k < size; k+=10) {
        int k_up = k + 10;
        if (k_up > size) k_up = size;
        
        for (int i = 0; i < N; i++) {
            for (int j = k; j < k_up; j++) {
                std::cout << "[" << individuals [j].getChromosom (i) 
                          << "]  ";
            }
            
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
    }
}

//    private:
template <int N>
Individual<N> * Population<N>::getFittest () {
    for (int i = 0; i < size; i++) {
        if (individuals [i].getFitness() == 0) {
            return & individuals [i];
        }
    }
    
    return nullptr;
}

template <int N>
Individual<N> * Population<N>::tournament () {
    std::uniform_real_distribution<double> distribution (0, 1);
    
    Individual<N> * winners = new Individual<N> [size/2];
    
    for (int i = 0; i < size; i+=2) {
        int fitness_1 = individuals [i].getFitness();
        int fitness_2 = individuals [i + 1].getFitness();
        
        double probability;
        if (fitness_1 > fitness_2) {
            probability = fitness_2 / (fitness_1 + fitness_2);
        }
        else {
            probability = fitness_1 / (fitness_1 + fitness_2);
        }
        
        double pivot = distribution (generator);
        if (pivot <= probability) {
            winners [i/2] = individuals [i];
        }
        else {
            winners [i/2] = individuals [i + 1];
        }
    }
                
    return winners;
}

template <int N>
void Population<N>::mutation (Individual<N> * gene_pool) {
    std::uniform_real_distribution<double> 
        distribution_1 (0, mutation_probability * (size/2));
    
    int pivot = distribution_1 (generator);

    std::uniform_real_distribution<double> 
        distribution_2 (0, (size/2)-1);

    int mutations = 0;
    
    for (int i = 0; i < pivot; i++) {
        int index = distribution_2 (generator);
        gene_pool [index].mutate();
        mutations++;
    }
    
    if (OUTPUT) std::cout << "mutations: " << mutations << std::endl;
}

template <int N>
Individual<N> * Population<N>::crossOver (Individual<N> * gene_pool) {
    Individual<N> * children = new Individual<N> [size/2];
    for (int i = 0; i < size/2; i+=2) {
        Individual<N> children_1;
        Individual<N> children_2;
        
        Individual<N>::crossOver (& gene_pool [i], & gene_pool [i + 1],
                                  children_1, children_2);
        
        children [i] = children_1;
        children [i + 1] = children_2;
    }
    
    return children;
}

template <int N>
void Population<N>::merge (Individual<N> * pool_1, Individual<N> * pool_2) {
    for (int i = 0; i < size/2; i++) {
        individuals [i] = pool_1 [i];
    }
    for (int i = 0; i < size/2; i++) {
        individuals [i + size/2] = pool_2 [i];
    }
                
    dice(5);
}


template <int N>
void Population<N>::dice (int iterations) {
    for (int i = 0; i < iterations; i++) {
        std::uniform_real_distribution<double> distribution (0, size-1);

        for (int i = 0; i < size/2; i++) {
            int random_index_1 = distribution (generator);
            int random_index_2 = distribution (generator);
            swap (random_index_1, random_index_2);
        }
    }
}

template <int N>
void Population<N>::swap (int index_1, int index_2) {
    Individual<N> * help = & individuals [index_1];
    individuals [index_1] = individuals [index_2];
    individuals [index_2] = * help;
}

