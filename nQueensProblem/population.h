#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

#include "individual.h"

#define FILE_LINE_OUT() (std::cout << __FILE__ ":" << __LINE__ << std::endl)

extern std::default_random_engine generator;

template <int N>
class Population {
        int size;
        double mutation_probability;
        Individual<N> * individuals;
        
    public:
        Population (int size, double mutation_probability) {
            if (size % 4 != 0) {
                this->size = size + (4 - size % 4);
                individuals = new Individual<N> [this->size];
            }
        }
        
        bool iterate (int iterations = 1) {
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
        
        void print (Individual<N> * gene_pool) {
            for (int k = 10; k < size/2 + 10; k+=10) {
                if (k > size/2) k = size/2;
                std::cout << k << std::endl;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < k; j++) {
                        std::cout << "[" << gene_pool [j].getChromosom (i) 
                                  << "]  ";
                    }
                    
                    std::cout << std::endl;
                }
                
                std::cout << std::endl;
            }
        }
        
        void print () {
            for (int k = 10; k < size + 10; k+=10) {
                if (k > size) k = size;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < k; j++) {
                        std::cout << "[" << individuals [j].getChromosom (i) 
                                  << "]  ";
                    }
                    
                    std::cout << std::endl;
                }
                
                std::cout << std::endl;
            }
        }
        
//    private:
        Individual<N> * getFittest () {
            for (int i = 0; i < size; i++) {
                if (individuals [i].getFitness() == 0) {
                    return & individuals [i];
                }
            }
            
            return nullptr;
        }

        Individual<N> * tournament () {
            FILE_LINE_OUT ();
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
        
        void mutation (Individual<N> * gene_pool) {
            std::uniform_real_distribution<double> 
                distribution_1 (0, mutation_probability * size);
                
            int pivot = distribution_1 (generator);

            std::uniform_real_distribution<double> 
                distribution_2 (0, size-1);
            
            for (int i = 0; i < pivot; i++) {
                int index = distribution_2 (generator);
                gene_pool [index].mutate();
            }
        }
        
        Individual<N> * crossOver (Individual<N> * gene_pool) {
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
        
        void merge (Individual<N> * pool_1, Individual<N> * pool_2) {
            for (int i = 0; i < size/2; i++) {
                individuals [i] = pool_1 [i];
            }
            for (int i = 0; i < size/2; i++) {
                individuals [i + size/2] = pool_2 [i];
            }
                        
            dice(5);
        }
        
        
        void dice (int iterations = 1) {
            for (int i = 0; i < iterations; i++) {
                std::uniform_real_distribution<double> distribution (0, size-1);

                for (int i = 0; i < size/2; i++) {
                    int random_index_1 = distribution (generator);
                    int random_index_2 = distribution (generator);
                    swap (random_index_1, random_index_2);
                }
            }
        }

        void swap (int index_1, int index_2) {
            Individual<N> * help = & individuals [index_1];
            individuals [index_1] = individuals [index_2];
            individuals [index_2] = * help;
        }
};

#endif // POPULATION_H

