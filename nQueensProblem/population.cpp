#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "population.h"
#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)



/***** PUBLIC POPULATION FUNCTIONS *****/

Population::Population (int N, int size, 
                        double mutation_probability, int alpha) {
    if (N < 1) {
        throw PopulationException ("Population.N has to be greater than 0");
    }
    if (size < 1) {
        throw PopulationException ("Population.size has to be greater than 0");
    }
    if (mutation_probability < 0 || mutation_probability > 1) {
        throw PopulationException ("Population.size has to be greater than 0"
                                   "and less than 1");
    }
    if (alpha < 1) {
        throw PopulationException ("Population.alpha has to be greater than 0");
    }
    
    this->N = N;
    Individual::N = N;

    this->mutation_probability = mutation_probability;
    this->alpha = alpha;
    
    if (size % 4) {
        this->size = size + (4 - size % 4);
    } else {
        this->size = size;
    }
    
    this->individuals = new Individual [this->size];
    
    calcFittest ();
    
    // saving stats
    pop_stats.N = N;
    pop_stats.pop_size = size;
    pop_stats.mutation_probability = mutation_probability;
    pop_stats.alpha = alpha;
//    pop_stats.average_fitness.push_back (calcAvFitness (individuals, size));
//    pop_stats.highest_fitness.push_back (fittest->getFitness ());

}

Population::~Population () {
    for (int i = 0; i < size; i++) {
//        delete & individuals [i];
    }
    
    delete [] individuals;
}

Individual * Population::iterate (int iterations) {
    Individual * fittest = getFittest ();
    for (int i = 0; i < iterations; i++)
    {
        if (fittest->getFitness () == 0) {
            return fittest;
        } else {
            pop_stats.average_fitness.push_back (calcAvFitness (individuals, size));
            pop_stats.highest_fitness.push_back (fittest->getFitness ());
            
            Individual * winners = tournament ();
            mutation (winners);
            Individual * children = crossOver (winners);
            merge (winners, children);
            
            delete [] winners;
            delete [] children;
            
            calcFittest ();
            fittest = getFittest ();
        }
    }
    
    return fittest;
}

// GETTER AND SETTER METHODS

Individual * Population::getFittest () {
    return this->fittest;
}

double Population::getAvFitness () {
    return calcAvFitness (individuals, size);
}

// OUTPUT METHODS

void Population::saveStats () {
    pop_stats.write_to_file ("nqueens");
}

void Population::print () {
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

        for (int j = k; j < k_up; j++) {
            std::cout << " " << individuals [j].getFitness () <<"  ";
        }
                
        std::cout << std::endl << std::endl;
    }
}

void Population::print (Individual * individuals, int size) {
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

        for (int j = k; j < k_up; j++) {
            std::cout << " " << individuals [j].getFitness () <<"  ";
        }
                
        std::cout << std::endl << std::endl;
    }
}



/***** PRIVATE POPULATION FUNCTIONS *****/

// BEHAVIOUR METHODS

Individual * Population::tournament () {
    std::uniform_real_distribution<double> distribution (0, 1);
    
    Individual * winners = new Individual [size/2];
    
    for (int i = 0; i < size; i+=2) {
        int fitness_1 = individuals [i].getFitness();
        int fitness_2 = individuals [i + 1].getFitness();
        
        double probability = pow (fitness_2, alpha) / 
                             double (pow (fitness_1, alpha) 
                                   + pow (fitness_2, alpha));
        
        double pivot = distribution (generator);
        if (pivot <= probability) {
            winners [i/2] = individuals [i];
        } else {
            winners [i/2] = individuals [i + 1];
        }
    }
    
    // saving stats
    pop_stats.average_winner_fitness.push_back (calcAvFitness (winners, 
                                                               size/2));
    pop_stats.highest_winner_fitness.push_back (getFittest (winners, 
                                      size/2)->getFitness ());
    
    return winners;
}

void Population::mutation (Individual * individuals) {
    std::uniform_real_distribution<double> 
        distribution_1 (0, mutation_probability * (size/2));
    
    int mutations = distribution_1 (generator);

    std::uniform_real_distribution<double> 
        distribution_2 (0, (size/2)-1);

    for (int i = 0; i < mutations; i++) {
        int index = distribution_2 (generator);
        individuals [index].mutate();
    }
    
    // saving stats
    pop_stats.average_postmutation_fitness.push_back 
        (calcAvFitness (individuals, size/2));
    pop_stats.highest_postmutation_fitness.push_back 
        (getFittest (individuals, size/2)->getFitness ());
    pop_stats.mutations.push_back (mutations);
}

Individual * Population::crossOver (Individual * individuals) {
    Individual * children = new Individual [size/2];
    
    for (int i = 0; i < size/2; i+=2) {
        Individual children_1;
        Individual children_2;
        
        crossOver (& individuals [i], & individuals [i + 1], 
                   children_1, children_2);
        
        children [i] = children_1;
        children [i + 1] = children_2;
    }
    
    pop_stats.average_children_fitness.push_back 
        (calcAvFitness (children, size/2));
    pop_stats.highest_children_fitness.push_back 
        (getFittest (children, size/2)->getFitness ());
    
    return children;
}

void Population::crossOver (const Individual * parent_1, 
                            const Individual * parent_2,
                            Individual  & children_1, 
                            Individual  & children_2) {
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

void Population::merge (Individual* pool_1, Individual * pool_2) {
    for (int i = 0; i < size/2; i++) {
        individuals [i] = pool_1 [i];
    }
    for (int i = 0; i < size/2; i++) {
        individuals [i + size/2] = pool_2 [i];
    }
                
//    dice (5);
}


// CALCULATION METHODS

void Population::calcFittest () {
    int fitness = individuals [0].getFitness ();
    int index = 0;
    
    for (int i = 1; i < size; i++) {
        if (individuals [i].getFitness () > fitness) {
            fitness = individuals [i].getFitness ();
            index = i;
        }
    }
    
    this->fittest = & individuals [index];
}

double Population::calcAvFitness (Individual * individuals, int size) {
    double sum = 0;
    
    for (int i = 0; i < size; i++) {
        sum += individuals [i].getFitness ();
    }
    
    return sum / size;
}

Individual * Population::getFittest (Individual * individuals, int size) {
    int fitness = individuals [0].getFitness ();
    int index = 0;
    
    for (int i = 1; i < size; i++) {
        if (individuals [i].getFitness () > fitness) {
            fitness = individuals [i].getFitness ();
            index = i;
        }
    }
    
    return & individuals [index];
}

