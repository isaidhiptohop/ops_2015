#include "population.h"
#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)

std::default_random_engine generator 
    (std::chrono::system_clock::now().time_since_epoch().count());

Population<N>::Population (unsigned int N, int size, 
                           double mutation_probability, int alpha) {
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
bool Population<N>::iterate (int iterations) {
    for (int i = 0; i < iterations; i++) {
        Individual<N> * fittest = getFittest ();
        if (fittest) {
            fittest->print ();
            return true;
        }
        else {
            print ();
            Individual<N> * winners = tournament ();
            mutation (winners);
            Individual<N> * children = crossOver (winners);
            merge (winners, children);
            return false;
        }
    }
    
    return false;
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
    
    std::cout << "mutations: " << mutations << std::endl;
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

