#include "libbsn/generator/DataGenerator.hpp"
#include "libbsn/range/Range.hpp"

namespace bsn {
    namespace generator {
        DataGenerator::DataGenerator() : markovChain(), seed() {
             for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    arcs[i][j]=0;
                }
            } 
        }

        DataGenerator::DataGenerator(const Markov& markov):markovChain(markov), seed() {
              for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    arcs[i][j]=0;
                }
              } 
        }

        DataGenerator::DataGenerator(const DataGenerator& obj) : markovChain(obj.markovChain),seed() {
              for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    arcs[i][j]=0;               
                }
            } 
        }

        DataGenerator::~DataGenerator() {}

        DataGenerator& DataGenerator::operator=(const DataGenerator& obj) {
            markovChain = obj.markovChain;
            return (*this);
        }

        std::uniform_int_distribution<int> probabilityGenerator(1,100);


        void DataGenerator::setSeed() {
            std::random_device rd;
            std::mt19937 aux(rd());
            seed = aux;
        }

        void DataGenerator::nextStatePureCoverage() {

            //Randomly choose a new row if current row has already been covered
            while(sum_row(markovChain.currentState)==5){
                // Seed the random number generator with the current time
                std::srand(static_cast<unsigned int>(std::time(0)));
                // Generate a random number between 0 and 4
                markovChain.currentState = std::rand() % 5;
            }
                
            // Calcula o offset do vetor baseado no estado
            int32_t offset = markovChain.currentState * 5;
    
            // Probabilities of current state's row 
            int32_t old_currentState=markovChain.currentState;
            double prob_0 =markovChain.transitions[offset+0];
            double prob_1 = markovChain.transitions[offset+1];
            double prob_2 = markovChain.transitions[offset+2];
            double prob_3 = markovChain.transitions[offset+3];
            double prob_4 = markovChain.transitions[offset+4];

            // Define the probabilities
            std::vector<double> probabilities = {prob_0,prob_1, prob_2, prob_3,prob_4};
            // Define a discrete distribution based on the probabilities
            std::discrete_distribution<> distribution(probabilities.begin(), probabilities.end());
            // Sample a number based on the given probabilities
            int sampled_number = distribution(seed);
                
            //Mark the arc
            arcs[markovChain.currentState][sampled_number]=1;

            //Count the number of arcs still to be covered
            int count=0;
            for(int i=0;i<5;i++){
                if(arcs[markovChain.currentState][i]==0){
                    count++;
                }
            }

            //If there are still arcs to be covered
            if(count!=0){
                //If the current probability is different from 100
                if(markovChain.transitions[offset+sampled_number]!=100.0){

                    //Set probability as 0
                    markovChain.transitions[offset+sampled_number]=0;
                    //Divide probability by the number of arcs uncovered
                    double part = probabilities[sampled_number]/count;
                    for(int i=0;i<5;i++){
                        if(arcs[markovChain.currentState][i]==0){
                            markovChain.transitions[offset+i]=markovChain.transitions[offset+i]+part;
                        }
                    }

                }
            }
            markovChain.currentState = sampled_number;

            //Check if it is finished because it covered all the arcs
            sensor_finished=true;
            for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    if(arcs[i][j]==0){
                        sensor_finished=false;
                    }
                }
            }

        }

        double DataGenerator::calculateValue() {
            if (markovChain.currentState > 4 || markovChain.currentState < 0){
                throw std::out_of_range("current state is out of bounds");
            }

            bsn::range::Range range = markovChain.states[markovChain.currentState];
            // Cria um número aleatório baseado no range
            std::uniform_real_distribution<double> value_generator(range.getLowerBound(), range.getUpperBound());
            double val = value_generator(seed);
            return val;
        }

        double DataGenerator::getValue() {
            return calculateValue();
        }

        void DataGenerator::remove_useless_States(){
               arcs[0][0]=1;
               arcs[0][1]=1;
               arcs[0][2]=1;
               arcs[0][3]=1;
               arcs[0][4]=1;
               arcs[1][0]=1;
               arcs[1][1]=1;
               arcs[1][2]=1;
               arcs[1][3]=1;
               arcs[1][4]=1;
               arcs[2][0]=1;
               arcs[3][0]=1;
               arcs[4][0]=1;
               arcs[2][1]=1;
               arcs[3][1]=1;
               arcs[4][1]=1;
        }
        void DataGenerator::print_Markov(){
            std::cout<<markovChain.toString()<<std::endl;
        }
        void DataGenerator::print_arcs(){
            std::cout<<"Arcs:"<<std::endl;
            for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    std::cout<<std::to_string(arcs[i][j])+ " ";                   
                }
                std::cout<<std::endl;
            } 
        }
        int DataGenerator::sum_row(int row_number){
            int sum=0;
            for(int j=0;j<5;j++){
                    sum=sum+arcs[row_number][j];                   
            }
            return sum;
        }
        
    }
    
} // namespace bs 


