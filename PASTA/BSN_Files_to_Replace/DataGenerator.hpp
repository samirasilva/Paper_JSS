#ifndef DATAGENERATOR_HPP
#define DATAGENERATOR_HPP

#include "libbsn/generator/Markov.hpp"

#include <random>
#include <stdint.h>
#include <ctime>

namespace bsn {
    namespace generator {
        class  DataGenerator {
            public:
                DataGenerator();
                DataGenerator(const Markov& markov);
                DataGenerator(const DataGenerator& obj);
                ~DataGenerator();

                DataGenerator& operator=(const DataGenerator& obj);
                
                double getValue();
                void setSeed();
                void nextState();
<<<<<<< HEAD
                void print_Markov();
                void print_arcs();
                int sum_row(int row_number);
                void remove_zero_arcs();
=======
                void remove_useless_States();
                void print_Markov();
                void print_arcs();
                int sum_row(int row_number);
>>>>>>> eb994484a77247e7a82df0b9a48b823b42c0e1a8
                int arcs[5][5];
                
            private:
                double calculateValue();
                Markov markovChain;
                double value;
                std::mt19937 seed;
        };
    }
}

#endif
