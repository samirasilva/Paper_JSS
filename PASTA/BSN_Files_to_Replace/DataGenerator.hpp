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
                void remove_useless_States();
                void print_Markov();
                void print_arcs();
                int sum_row(int row_number);
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
