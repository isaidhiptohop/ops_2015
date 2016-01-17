#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdlib>

#include "individual.h"

extern std::default_random_engine generator;

struct PopulationException {
    std::string message;
    
    PopulationException (std::string message) {
        this->message = message;
    }
    
    std::string getMessage () {
        return this->message;
    }
};

struct PopulationStats {
    int N;
    int pop_size;
    double mutation_probability;
    int alpha;

    std::vector<double> average_fitness;
    std::vector<int> highest_fitness;
    std::vector<double> average_winner_fitness;
    std::vector<int> highest_winner_fitness;
    std::vector<double> average_postmutation_fitness;
    std::vector<int> highest_postmutation_fitness;
    std::vector<double> average_children_fitness;
    std::vector<int> highest_children_fitness;
    std::vector<int> mutations;
    
    void write_to_file (std::string file) {
        std::ostringstream strs;
        strs << 0 << int (100 * mutation_probability);
        std::string m_p_str = strs.str ();
        
        std::string filename = file + "_" +
                               std::to_string (N) + "_" +
                               std::to_string (pop_size) + "_" +
                               m_p_str + "_" +
                               std::to_string (alpha);
        
        std::fstream fop (filename, std::ios::out);
        
        long unsigned int size = average_winner_fitness.size ();
        
        fop << N << " " << pop_size << " ";
        fop << mutation_probability << " " << alpha << " ";
        fop << size << " " << 0 << " " << 0 << " ";
        fop << 0 << " " << 0 << std::endl;
        
        for (long unsigned int i = 0; i < size; i++) {
            try {
                fop << average_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << highest_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << average_winner_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << highest_winner_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << average_postmutation_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << highest_postmutation_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << average_children_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << highest_children_fitness.at (i) << " ";
            } catch (std::out_of_range oof) {
                fop << 0 << " ";
            }
            try {
                fop << mutations.at (i) << std::endl;
            } catch (std::out_of_range oof) {
                fop << 0 << std::endl;
            }
        }
    }
};



/***** POPULATION CLASS *****/

class Population {
        int N;
        
        double mutation_probability;
        int alpha;

        int size;
        Individual * individuals;
        
        Individual * fittest;
        
        PopulationStats pop_stats;
        
    public:
        Population (int N, int size, 
                    double mutation_probability = 0, int alpha = 1);
        ~Population ();

        Individual * iterate (int iterations = 1);
        Individual * getFittest ();
        double getAvFitness ();
        
        void saveStats ();
        void print ();
        void print (Individual * individuals, int size);
        
//    private:
        Individual * tournament ();
        void mutation (Individual * individuals);
        Individual * crossOver (Individual * individuals);
        void merge (Individual * pool_1, Individual * pool_2);
        void crossOver (const Individual * parent_1, 
                        const Individual * parent_2,
                        Individual & children_1, 
                        Individual & children_2);

        void calcFittest ();
        double calcAvFitness (Individual * individuals, int size);
        Individual * getFittest (Individual * individuals, int size);

        void dice (int iterations = 1);
        void swap (int index_1, int index_2);
};

#endif // POPULATION_H

