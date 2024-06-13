#include <PatientModule.hpp>
#include <numeric>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include <filesystem>
#include <iostream>


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
<<<<<<< HEAD
        patientData[s].remove_zero_arcs();
=======
        if(s=="oxigenation" || s=="abps"|| s=="abpd"){
            patientData[s].remove_useless_States();
            //std::cout<<"entrou"<<::std::endl;
        }
>>>>>>> eb994484a77247e7a82df0b9a48b823b42c0e1a8

    }

    system("sleep 10s");

<<<<<<< HEAD
    rosComponentDescriptor.setFreq(frequency); 
=======
    rosComponentDescriptor.setFreq(frequency); //Será essa a frequência que buscamos?
>>>>>>> eb994484a77247e7a82df0b9a48b823b42c0e1a8
    
    period = 1/frequency;
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

void PatientModule::body() {
    for (auto &p : vitalSignsFrequencies) {
        
        if (p.second >= (vitalSignsChanges[p.first] + vitalSignsOffsets[p.first])) {

            patientData[p.first].nextState();

            p.second = vitalSignsOffsets[p.first];

<<<<<<< HEAD
            double cov_oxi = computeCoverage("oxigenation");
            double cov_hr = computeCoverage("heart_rate");
            double cov_temp = computeCoverage("temperature");
            double cov_abps = computeCoverage("abps");
            double cov_abpd = computeCoverage("abpd");
            double cov_gluc = computeCoverage("glucose");

            const char *homedir;
            std::string home;

            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;   
            }
            home=homedir;

            std::string command= "cp "+home+"/sa-bsn/Cov_all_patients.txt"+" "+home+"/sa-bsn/Cov_all_patients_old.txt";
            system (command.c_str());

            std::string cov_str = std::to_string(cov_oxi)+"\n"+std::to_string(cov_hr)+"\n"+std::to_string(cov_temp)+"\n"+std::to_string(cov_abps)+"\n"+std::to_string(cov_abpd)+"\n"+std::to_string(cov_gluc)+"\n\n";
            std::ofstream outfile;
            outfile.open(home+"/sa-bsn/Cov_all_patients.txt");
            outfile<<cov_str;
            outfile.close();

=======
>>>>>>> eb994484a77247e7a82df0b9a48b823b42c0e1a8
        } else {
            p.second += period;
        }
        
    }

<<<<<<< HEAD
=======
    double cov_oxi = computeCoverage("oxigenation");
    double cov_hr = computeCoverage("heart_rate");
    double cov_temp = computeCoverage("temperature");
    double cov_abps = computeCoverage("abps");
    double cov_abpd = computeCoverage("abpd");
    double cov_gluc = computeCoverage("glucose");

    const char *homedir;
    std::string home;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;   
    }
    home=homedir;
    
    //std::string searchfilename="rm "+home+"/sa-bsn/Cov_all_patients.txt";
    //system(searchfilename.c_str());
   
    std::string cov_str = std::to_string(cov_oxi)+"\n"+std::to_string(cov_hr)+"\n"+std::to_string(cov_temp)+"\n"+std::to_string(cov_abps)+"\n"+std::to_string(cov_abpd)+"\n"+std::to_string(cov_gluc)+"\n\n";
    std::ofstream outfile;
    outfile.open(home+"/sa-bsn/Cov_all_patients.txt");
    outfile<<cov_str;
    outfile.close();
>>>>>>> eb994484a77247e7a82df0b9a48b823b42c0e1a8
}
