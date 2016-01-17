#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

extern std::default_random_engine generator;

class Individual {
        int * chromosom = nullptr;
        int fitness;
    
    public:
        static int N;

        Individual (int * chromosom = nullptr);
        Individual (const Individual & individual);
        Individual & operator= (const Individual & individual);
        ~Individual ();
        
        void setChromosom (int * array);
        int getChromosom (int * array);
        int * getChromosom ();
        int getChromosom (int i) const;

        int getFitness ();
        void mutate ();

        void print ();
        void printBoard ();

//        static void setN (int N);
    private:
        void calcFitness ();
};

#endif // INDIVIDUAL_H

