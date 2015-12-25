#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

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

#endif // INDIVIDUAL_H

