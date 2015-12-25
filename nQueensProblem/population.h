#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

#include "individual.h"

extern std::default_random_engine generator;

template <int N>
class Population {
        int size;
        double mutation_probability;
        Individual<N> * individuals;
        
    public:
        Population (int size, double mutation_probability);
        bool iterate (int iterations = 1);
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

#endif // POPULATION_H

