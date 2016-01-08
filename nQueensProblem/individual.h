#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

extern std::default_random_engine generator;

class Individual {
        static unsigned int N;
        int * chromosom = nullptr;
    
    public:
        Individual (unsigned int N, int * chromosom = nullptr);
        Individual (const Individual & individual);
        Individual & operator= (const Individual & individual);
        ~Individual ();
        
        void setChromosom (int * array);
        void getChromosom (int * array);
        int getChromosom (int i) const;
        int getFitness ();
        void mutate ();
        void print ();

        static void crossOver (const Individual * parent_1, 
                               const Individual * parent_2,
                               Individual & children_1, 
                               Individual & children_2);
};

#endif // INDIVIDUAL_H

