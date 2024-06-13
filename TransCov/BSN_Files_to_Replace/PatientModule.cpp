#include <PatientModule.hpp>
#include <numeric>
#include <iostream>
#include <fstream>

PatientModule::PatientModule(int  &argc, char **argv, std::string name) : ROSComponent(argc, argv, name) {}

PatientModule::~PatientModule() {}

void PatientModule::setUp() {
    srand(time(NULL));

    // TODO change Operation to static
    std::string vitalSigns;
    service = nh.advertiseService("getPatientData", &PatientModule::getPatientData, this);
    double aux;

    frequency = 1000;

    // Get what vital signs this module will simulate
    nh.getParam("vitalSigns", vitalSigns);

    // Removes white spaces from vitalSigns
    vitalSigns.erase(std::remove(vitalSigns.begin(), vitalSigns.end(),' '), vitalSigns.end());

    std::vector<std::string> splittedVitalSigns = bsn::utils::split(vitalSigns, ',');

    for (std::string s : splittedVitalSigns) {
        vitalSignsFrequencies[s] = 0;
        nh.getParam(s + "_Change", aux);
        vitalSignsChanges[s] = 1/aux;
        nh.getParam(s + "_Offset", vitalSignsOffsets[s]);
    }

    for (const std::string& s : splittedVitalSigns) {
        patientData[s] = configureDataGenerator(s);
        
        //CHECK AS COVERED THE ARCS THAT DOESNT EXIST for these sensors
        if(s=="oxigenation" || s=="abps"|| s=="abpd"){
            patientData[s].remove_useless_States();
        }
    }

    system("sleep 10s");

    rosComponentDescriptor.setFreq(frequency); 
    
    period = 1/frequency;
}

bsn::generator::DataGenerator PatientModule::configureDataGenerator(const std::string& vitalSign) {
    srand(time(NULL));
    
    std::vector<std::string> t_probs;
    std::array<float, 25> transitions;
    std::array<bsn::range::Range,5> ranges;
    std::string s;
    ros::NodeHandle handle;

    for(uint32_t i = 0; i < transitions.size(); i++){
        for(uint32_t j = 0; j < 5; j++){
            handle.getParam(vitalSign + "_State" + std::to_string(j), s);
            t_probs = bsn::utils::split(s, ',');
            for(uint32_t k = 0; k < 5; k++){
                transitions[i++] = std::stod(t_probs[k]);
            }
        }
    }
    
    std::vector<std::string> lrs,mrs0,hrs0,mrs1,hrs1;

    handle.getParam(vitalSign + "_LowRisk", s);
    lrs = bsn::utils::split(s, ',');
    handle.getParam(vitalSign + "_MidRisk0", s);
    mrs0 = bsn::utils::split(s, ',');
    handle.getParam(vitalSign + "_HighRisk0", s);
    hrs0 = bsn::utils::split(s, ',');
    handle.getParam(vitalSign + "_MidRisk1", s);
    mrs1 = bsn::utils::split(s, ',');
    handle.getParam(vitalSign + "_HighRisk1", s);
    hrs1 = bsn::utils::split(s, ',');

    ranges[0] = bsn::range::Range(std::stod(hrs0[0]), std::stod(hrs0[1]));
    ranges[1] = bsn::range::Range(std::stod(mrs0[0]), std::stod(mrs0[1]));
    ranges[2] = bsn::range::Range(std::stod(lrs[0]), std::stod(lrs[1]));
    ranges[3] = bsn::range::Range(std::stod(mrs1[0]), std::stod(mrs1[1]));
    ranges[4] = bsn::range::Range(std::stod(hrs1[0]), std::stod(hrs1[1]));

    bsn::generator::Markov markov(transitions, ranges, 2);
    bsn::generator::DataGenerator dataGenerator(markov);
    dataGenerator.setSeed();

    return dataGenerator;
}

void PatientModule::tearDown() {}

bool PatientModule::getPatientData(services::PatientData::Request &request, 
                                services::PatientData::Response &response) {

    response.data = patientData[request.vitalSign].getValue();
    return true;
}

 double PatientModule::computeCoverage(std::string sensor){
    double sum=0;
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            sum=sum+patientData[sensor].arcs[i][j];
        }
    }
    sum=sum/25;
    return sum;

 }


void PatientModule::body() {
    for (auto &p : vitalSignsFrequencies) {
        
        if (p.second >= (vitalSignsChanges[p.first] + vitalSignsOffsets[p.first])) {

            //if sensor is not finished, go to next state
            if(!patientData[p.first].sensor_finished){
                patientData[p.first].nextStatePureCoverage();
            }
             
            //If a sensor is finished, it assigns 1 to its corresponding position in the "finished" vector
            if(patientData[p.first].sensor_finished){
                if (p.first == "oxigenation")
                     finished[0]=1;
                else if (p.first == "heart_rate")
                     finished[1]=1;
                else if (p.first== "temperature")
                     finished[2]=1;
                else if (p.first== "abps")
                     finished[3]=1;
                else if (p.first== "abpd")		
                     finished[4]=1;
                else if (p.first== "glucose")        
                     finished[5]=1;
                else {
                    std::cout << "UNKNOWN SENSOR " + p.first  + '\n';
                }

                //If for all the sensors, all the arcs have been covered, it prints the coverage and the markov matrices
                int sum=0;
                for(int i=0;i<6;i++){
                    sum=sum+finished[i];
                }

                if(sum==6){
                    //std::cout<<"oxigenation"<<std::endl;
                    //patientData["oxigenation"].print_arcs();
                    //patientData["oxigenation"].print_Markov();

                    //std::cout<<"heart_rate"<<std::endl;
                    //patientData["heart_rate"].print_arcs();
                    //patientData["heart_rate"].print_Markov();

                    //std::cout<<"temperature"<<std::endl;
                    //patientData["temperature"].print_arcs();
                    //patientData["temperature"].print_Markov();

                    //std::cout<<"abps"<<std::endl;
                    //patientData["abps"].print_arcs();
                    //patientData["abps"].print_Markov();

                    //std::cout<<"abpd"<<std::endl;
                    //patientData["abpd"].print_arcs();
                    //patientData["abpd"].print_Markov();

                    //std::cout<<"glucose"<<std::endl;
                    //patientData["glucose"].print_arcs();
                    //patientData["glucose"].print_Markov();

                    /*double cov_oxi = computeCoverage("oxigenation");
                    double cov_hr = computeCoverage("heart_rate");
                    double cov_temp = computeCoverage("temperature");
                    double cov_abps = computeCoverage("abps");
                    double cov_abpd = computeCoverage("abpd");
                    double cov_gluc = computeCoverage("glucose");

                    std::ofstream outfile;
                    outfile.open("/home/samira/Downloads/Cov_all_patients.txt", std::ios_base::app);
                    outfile<<cov_oxi<<std::endl;
                    outfile<<cov_hr<<std::endl;
                    outfile<<cov_temp<<std::endl;
                    outfile<<cov_abps<<std::endl;
                    outfile<<cov_abpd<<std::endl;
                    outfile<<cov_gluc<<std::endl<<std::endl;
                    outfile.close(); 
*/
                    
                    system("kill $(cat /var/tmp/data_access.pid && rm /var/tmp/data_access.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/strategy_enactor.pid && rm /var/tmp/strategy_enactor.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/logger.pid && rm /var/tmp/logger.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/probe.pid && rm /var/tmp/probe.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/effector.pid && rm /var/tmp/effector.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g4t1.pid && rm /var/tmp/g4t1.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_1.pid && rm /var/tmp/g3t1_1.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_2.pid && rm /var/tmp/g3t1_2.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_3.pid && rm /var/tmp/g3t1_3.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_4.pid && rm /var/tmp/g3t1_4.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_5.pid && rm /var/tmp/g3t1_5.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_6.pid && rm /var/tmp/g3t1_6.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/patient.pid && rm /var/tmp/patient.pid) & sleep 1s");

                    system("kill $(cat /var/tmp/injector.pid && rm /var/tmp/injector.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/strategy_manager.pid && rm /var/tmp/strategy_manager.pid) & sleep 1s");
                    system("kill $(pgrep roscore)");
                    system ("killall sleep");

                }
            }


            p.second = vitalSignsOffsets[p.first];
        
        } else {
            p.second += period;
        }
        
    }
}
