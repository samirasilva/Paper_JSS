#include "component/g4t1/G4T1.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include <random>
#include <stdint.h>

#include "libbsn/range/Range.hpp"
#include "libbsn/resource/Battery.hpp"
#include "libbsn/generator/Markov.hpp"
#include "libbsn/generator/DataGenerator.hpp"
#include "libbsn/filters/MovingAverage.hpp"
#include "libbsn/utils/utils.hpp"
#include "libbsn/configuration/SensorConfiguration.hpp"

#define W(x) std::cerr << #x << " = " << x << std::endl;

#define BATT_UNIT 0.001

using namespace bsn::processor;
using namespace std::chrono;
using namespace bsn::range;
using namespace bsn::resource;
using namespace bsn::generator;
using namespace bsn::configuration;

G4T1::G4T1(int &argc, char **argv, const std::string &name) : lost_packt(false),
    CentralHub(argc, argv, name, true, bsn::resource::Battery("ch_batt", 100, 100, 1) ),
    patient_status(0.0) {

        
        const char *homedir;
        std::string home;

        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;   
        }
        home=homedir;

        std::ifstream myfile;
        myfile.open(home+"/sa-bsn/combinations_100.txt",std::ios_base::in);

        int number;
        for (int i = 0; i < 3375; i++) {
            for (int j = 0; j < 30; j++) {
                myfile >> number;
                mat[i][j]=number;
            }
        }
        current_row_comb=0;
    }
	
G4T1::~G4T1() {}

std::vector<std::string> G4T1::getPatientStatus() {
    std::string sensor_risk_str;
    std::string abps;
    std::string abpd;
    std::string oxi;
    std::string ecg;
    std::string trm;
    std::string glc;

    for (int i = 0; i < 6; i++) {
        double sensor_risk = data_buffer[i].back();


        if (sensor_risk > 0 && sensor_risk <= 20) {
            sensor_risk_str = "low risk";
        } else if (sensor_risk > 20 && sensor_risk <= 65) {
            sensor_risk_str = "moderate risk";
        } else if (sensor_risk > 65 && sensor_risk <= 100) {
            sensor_risk_str = "high risk";
        } else {
            sensor_risk_str = "unknown";
        }

        if (i == 0) {
            trm = sensor_risk_str;
            trm_risk = sensor_risk;
        } else if (i == 1) {
            ecg = sensor_risk_str;
            ecg_risk = sensor_risk;
        } else if (i == 2) {
            oxi = sensor_risk_str;
            oxi_risk = sensor_risk;
        } else if (i == 3) {
            abps = sensor_risk_str;
            abps_risk = sensor_risk;
        } else if (i == 4) {
            abpd = sensor_risk_str;
            abpd_risk = sensor_risk;
        } else if (i == 5) {
            glc = sensor_risk_str;
            glc_risk = sensor_risk;
        }
    }

    std::vector<std::string> v = {trm, ecg, oxi, abps, abpd, glc};  
    return v;
}

void G4T1::setUp() {
    Component::setUp();

    ros::NodeHandle config;

    double freq;
    config.getParam("frequency", freq);
    rosComponentDescriptor.setFreq(freq);

    for (std::vector<std::list<double>>::iterator it = data_buffer.begin();
        it != data_buffer.end(); ++it) {
            (*it) = {0.0};
    }

    pub = config.advertise<messages::TargetSystemData>("TargetSystemData", 10);
}

void G4T1::tearDown() {}

void G4T1::collect(const messages::SensorData::ConstPtr& msg) {
    int type = getSensorId(msg->type);
    double risk = msg->risk;
    double batt = msg->batt;
    
    
    battery.consume(BATT_UNIT);
    if (msg->type == "null" || int32_t(risk) == -1)  throw std::domain_error("risk data out of boundaries");

    /*update battery status for received sensor info*/
    if (msg->type == "thermometer") {
        trm_batt = batt;
        trm_raw = msg->data;
    } else if (msg->type == "ecg") {
        ecg_batt = batt;
        ecg_raw = msg->data;
    } else if (msg->type == "oximeter") {
        oxi_batt = batt;
        oxi_raw = msg->data;
    } else if (msg->type == "abps") {
        abps_batt = batt;
        abps_raw = msg->data;
    } else if (msg->type == "abpd") {
        abpd_batt = batt;
        abpd_raw = msg->data;
    } else if (msg->type == "glucosemeter") {
        glc_batt = batt;
        glc_raw = msg->data;
    }
    
    if (buffer_size[type] < max_size) {
        data_buffer[type].push_back(risk);
        buffer_size[type] = data_buffer[type].size();
        total_buffer_size = std::accumulate(std::begin(buffer_size), std::end(buffer_size), 0, std::plus<int>());
    } else {
        data_buffer[type].push_back(risk);
        data_buffer[type].erase(data_buffer[type].begin());//erase the first element to avoid overflow
        lost_packt = true;
    }
}
double G4T1::compute_risk(std::string sensor, double raw_number){

    bsn::configuration::SensorConfiguration sensorConfig;
    std::array<bsn::range::Range,5> ranges;
    if(sensor=="oxi"){
        ranges[0] = Range(-1,-1);
        ranges[1] = Range(-1,-1);
        ranges[2] = Range(65,100);
        ranges[3] = Range(55,65);
        ranges[4] = Range(0,55);

    }else if (sensor=="hr")
    {
        ranges[0] = Range(0,70);
        ranges[1] = Range(70,85);
        ranges[2] = Range(85,97);
        ranges[3] = Range(97,115);
        ranges[4] = Range(115,300);
        /* code */
    }else if (sensor=="trm")
    {
        ranges[0] = Range(0,31.99);
        ranges[1] = Range(32,35.99);
        ranges[2] = Range(36,37.99);
        ranges[3] = Range(38,40.99);
        ranges[4] = Range(41,50);
        /* code */
    }else if (sensor=="abps")
    {
        ranges[0] = Range(-1,-1);
        ranges[1] = Range(-1,-1);
        ranges[2] = Range(0,120);
        ranges[3] = Range(120,140);
        ranges[4] = Range(140,300);
        /* code */
    }else if (sensor=="abpd")
    {
        ranges[0] = Range(-1,-1);
        ranges[1] = Range(-1,-1);
        ranges[2] = Range(0,80);
        ranges[3] = Range(80,90);
        ranges[4] = Range(90,300);
        /* code */
    }else if (sensor=="glc")
    {
        ranges[0] = Range(20,39.99);
        ranges[1] = Range(40,54.99);
        ranges[2] = Range(55,95.99);
        ranges[3] = Range(96,119.99);
        ranges[4] = Range(120,200);
        /* code */
    }

    Range low_range = ranges[2];
        
    std::array<Range,2> midRanges;
    midRanges[0] = ranges[1];
    midRanges[1] = ranges[3];
     
    std::array<Range,2> highRanges;
    highRanges[0] = ranges[0];
    highRanges[1] = ranges[4];
    std::array<Range,3> percentages;

    percentages[0] = Range(0,20);
    percentages[1] = Range(21,65);
    percentages[2] = Range(66,100);
    sensorConfig = SensorConfiguration(0, low_range, midRanges, highRanges, percentages);
    double a=sensorConfig.evaluateNumber(raw_number);
    return a;
}
void G4T1::process(){
    battery.consume(BATT_UNIT * data_buffer.size());
    std::vector<double> current_data;




    for(std::vector<std::list<double>>::iterator it = data_buffer.begin(); it != data_buffer.end(); it++) {
        double el = it->front();
        current_data.push_back(el);
        if(it->size() > 1) it->pop_front();
    }

  
    current_data.clear();
    double oxi,trm,hr,abps,abpd,glc;
    double oxi_conv,trm_conv,hr_conv,abps_conv,abpd_conv,glc_conv;

    //oxi:
    if(mat[current_row_comb][2]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(65,100);
            oxi = probabilityGenerator(seed);
            oxi_conv=compute_risk("oxi", oxi);

    }else if(mat[current_row_comb][3]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(55,65);
            oxi = probabilityGenerator(seed);
            oxi_conv=compute_risk("oxi", oxi);

    }else if(mat[current_row_comb][4]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(0,55);
            oxi = probabilityGenerator(seed);
            oxi_conv=compute_risk("oxi", oxi);
    }

    //hr:
    if(mat[current_row_comb][5]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(0,70);
            hr = probabilityGenerator(seed);
            hr_conv=compute_risk("hr", hr);

    }else if(mat[current_row_comb][6]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(70,85);
            hr = probabilityGenerator(seed);
            hr_conv=compute_risk("hr", hr);

    }else if(mat[current_row_comb][7]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(85,97);
            hr = probabilityGenerator(seed);
            hr_conv=compute_risk("hr", hr);
    }else if(mat[current_row_comb][8]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(97,115);
            hr = probabilityGenerator(seed);
            hr_conv=compute_risk("hr", hr);
    }else if(mat[current_row_comb][9]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(115,300);
            hr = probabilityGenerator(seed);
            hr_conv=compute_risk("hr", hr);
    }

    //trm:
    if(mat[current_row_comb][10]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(0,31.99);
            trm = probabilityGenerator(seed);
            trm_conv=compute_risk("trm", trm);

    }else if(mat[current_row_comb][11]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(32,35.99);
            trm = probabilityGenerator(seed);
            trm_conv=compute_risk("trm", trm);

    }else if(mat[current_row_comb][12]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(36,37.99);
            trm = probabilityGenerator(seed);
            trm_conv=compute_risk("trm", trm);
    }else if(mat[current_row_comb][13]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(38,40.99);
            trm = probabilityGenerator(seed);
            trm_conv=compute_risk("trm", trm);
    }else if(mat[current_row_comb][14]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(41,50);
            trm = probabilityGenerator(seed);
            trm_conv=compute_risk("trm", trm);
    }

    //abps:
    if(mat[current_row_comb][17]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(0,120);
            abps = probabilityGenerator(seed);
            abps_conv=compute_risk("abps", abps);

    }else if(mat[current_row_comb][18]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(120,140);
            abps = probabilityGenerator(seed);
            abps_conv=compute_risk("abps", abps);

    }else if(mat[current_row_comb][19]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(140,300);
            abps = probabilityGenerator(seed);
            abps_conv=compute_risk("abps", abps);
    }

    //abpd:
    if(mat[current_row_comb][22]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(0,80);
            abpd = probabilityGenerator(seed);
            abpd_conv=compute_risk("abpd", abpd);

    }else if(mat[current_row_comb][23]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(80,90);
            abpd = probabilityGenerator(seed);
            abpd_conv=compute_risk("abpd", abpd);

    }else if(mat[current_row_comb][24]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(90,300);
            abpd = probabilityGenerator(seed);
            abpd_conv=compute_risk("abpd", abpd);
    }

    //glc:
    if(mat[current_row_comb][25]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(20,39.99);
            glc = probabilityGenerator(seed);
            glc_conv=compute_risk("glc", glc);

    }else if(mat[current_row_comb][26]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(40,54.99);
            glc = probabilityGenerator(seed);
            glc_conv=compute_risk("glc", glc);

    }else if(mat[current_row_comb][27]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(55,95.99);
            glc = probabilityGenerator(seed);
            glc_conv=compute_risk("glc", glc);
    }else if(mat[current_row_comb][28]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(96,119.99);
            glc = probabilityGenerator(seed);
            glc_conv=compute_risk("glc", glc);
    }else if(mat[current_row_comb][29]==100){
            std::random_device rd;
            std::mt19937 seed(rd());
            std::uniform_int_distribution<int> probabilityGenerator(120,200);
            glc = probabilityGenerator(seed);
            glc_conv=compute_risk("glc", glc);
    }


    current_data.push_back(trm_conv); 
    current_data.push_back(hr_conv); 
    current_data.push_back(oxi_conv);
    current_data.push_back(abps_conv); 
    current_data.push_back(abpd_conv); 
    current_data.push_back(glc_conv); 
    current_row_comb++;
    std::cout<<std::endl<<current_row_comb<<std::endl;
    if(current_row_comb==3375){
                    system("kill $(cat /var/tmp/g4t1.pid && rm /var/tmp/g4t1.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/data_access.pid && rm /var/tmp/data_access.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/strategy_enactor.pid && rm /var/tmp/strategy_enactor.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/logger.pid && rm /var/tmp/logger.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/probe.pid && rm /var/tmp/probe.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/effector.pid && rm /var/tmp/effector.pid) & sleep 1s");
                    //system("kill $(cat /var/tmp/g4t1.pid && rm /var/tmp/g4t1.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_1.pid && rm /var/tmp/g3t1_1.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_2.pid && rm /var/tmp/g3t1_2.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_3.pid && rm /var/tmp/g3t1_3.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_4.pid && rm /var/tmp/g3t1_4.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_5.pid && rm /var/tmp/g3t1_5.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/g3t1_6.pid && rm /var/tmp/g3t1_6.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/patient.pid && rm /var/tmp/patient.pid) & sleep 1s");
                    //system("kill $(pgrep -f \"patient\""); 
                    //system("sleep 1s");
                    system("kill $(cat /var/tmp/injector.pid && rm /var/tmp/injector.pid) & sleep 1s");
                    system("kill $(cat /var/tmp/strategy_manager.pid && rm /var/tmp/strategy_manager.pid) & sleep 1s");
                    system("kill $(pgrep roscore)");
                    system ("killall sleep");
    }


    patient_status = data_fuse(current_data);
    

    //patient_status = data_fuse(current_data); // consumes 1 packt per sensor (in the buffers that have packages to data_bufferbe processed)
    for (int i = 0; i < buffer_size.size(); ++i){ // update buffer sizes
        buffer_size[i] = data_buffer[i].size();
    }
    total_buffer_size = std::accumulate(std::begin(buffer_size), std::end(buffer_size), 0, std::plus<int>()); //update total buffer size 



/*     //MUDEI A IMPRESSÃO DOS DADOS RAW DO SENSOR PARA AQUI:
    std::cout<<"++++++++++++++++++++"<<std::endl;
    std::cout<<"Trm: "<<std::to_string(trm_raw)<<std::endl;
    std::cout<<"Ecg: "<<std::to_string(ecg_raw)<<std::endl;
    std::cout<<"Oxi: "<<std::to_string(oxi_raw)<<std::endl;
    std::cout<<"Abps: "<<std::to_string(abps_raw)<<std::endl;
    std::cout<<"Abpd: "<<std::to_string(abpd_raw)<<std::endl;
    std::cout<<"Glc: "<<std::to_string(glc_raw)<<std::endl;
    std::cout<<"++++++++++++++++++++"<<std::endl; */

    std::cout<<"++++++++++++++++++++"<<std::endl;
    std::cout<<"Trm: "<<std::to_string(trm)<<std::endl;
    std::cout<<"Ecg: "<<std::to_string(hr)<<std::endl;
    std::cout<<"Oxi: "<<std::to_string(oxi)<<std::endl;
    std::cout<<"Abps: "<<std::to_string(abps)<<std::endl;
    std::cout<<"Abpd: "<<std::to_string(abpd)<<std::endl;
    std::cout<<"Glc: "<<std::to_string(glc)<<std::endl;
    std::cout<<"++++++++++++++++++++"<<std::endl; 

    std::vector<std::string> v = getPatientStatus();

    std::string label_term,label_ecg,label_oxi, label_abps, label_abpd,label_glc;
    if (current_data.at(0) >= 0 && current_data.at(0) <= 20) {
        label_term= "low risk";
    } else if (current_data.at(0) > 20 && current_data.at(0) <= 65) {
        label_term = "moderate risk";
    } else if (current_data.at(0) > 65 && current_data.at(0) <= 100) {
        label_term = "high risk";
    } else {
        label_term = "unknown";
    }

    if (current_data.at(1) >= 0 && current_data.at(1) <= 20) {
        label_ecg= "low risk";
    } else if (current_data.at(1) > 20 && current_data.at(1) <= 65) {
        label_ecg = "moderate risk";
    } else if (current_data.at(1) > 65 && current_data.at(1) <= 100) {
        label_ecg = "high risk";
    } else {
        label_ecg = "unknown";
    }

    if (current_data.at(2) >= 0 && current_data.at(2) <= 20) {
        label_oxi= "low risk";
    } else if (current_data.at(2) > 20 && current_data.at(2) <= 65) {
        label_oxi = "moderate risk";
    } else if (current_data.at(2) > 65 && current_data.at(2) <= 100) {
        label_oxi = "high risk";
    } else {
        label_oxi = "unknown";
    }

    if (current_data.at(3) >= 0 && current_data.at(3) <= 20) {
        label_abps= "low risk";
    } else if (current_data.at(3) > 20 && current_data.at(3) <= 65) {
        label_abps = "moderate risk";
    } else if (current_data.at(3) > 65 && current_data.at(3) <= 100) {
        label_abps = "high risk";
    } else {
        label_abps = "unknown";
    }

    if (current_data.at(4) >= 0 && current_data.at(4) <= 20) {
        label_abpd= "low risk";
    } else if (current_data.at(4) > 20 && current_data.at(4) <= 65) {
        label_abpd = "moderate risk";
    } else if (current_data.at(4) > 65 && current_data.at(4) <= 100) {
        label_abpd = "high risk";
    } else {
        label_abpd = "unknown";
    }

    if (current_data.at(5) >= 0 && current_data.at(5) <= 20) {
        label_glc= "low risk";
    } else if (current_data.at(5) > 20 && current_data.at(5) <= 65) {
        label_glc = "moderate risk";
    } else if (current_data.at(5) > 65 && current_data.at(5) <= 100) {
        label_glc = "high risk";
    } else {
        label_glc = "unknown";
    }

    std::cout<<"Term: "+label_term<<std::endl;
    std::cout<<"Ecg: "+label_ecg<<std::endl;
    std::cout<<"Oxi: "+label_oxi<<std::endl;
    std::cout<<"Abps: "+label_abps<<std::endl;
    std::cout<<"Abpd: "+label_abpd<<std::endl;
    std::cout<<"Glc: "+label_glc<<std::endl;
    std::cout<<"Patient_Status: "+std::to_string(patient_status)<<std::endl; 


/*     std::cout<<"Term: "+v[0]<<std::endl;
    std::cout<<"Ecg: "+v[1]<<std::endl;
    std::cout<<"Oxi: "+v[2]<<std::endl;
    std::cout<<"Abps: "+v[3]<<std::endl;
    std::cout<<"Abpd: "+v[4]<<std::endl;
    std::cout<<"Glc: "+v[5]<<std::endl;
    std::cout<<"Patient_Status: "+std::to_string(patient_status)<<std::endl; */

    std::string patient_risk;

    if(patient_status <= 20) {
        patient_risk = "VERY LOW RISK";
    } else if(patient_status > 20 && patient_status <= 40) {
        patient_risk = "LOW RISK";
    } else if(patient_status > 40 && patient_status <= 60) {
        patient_risk = "MODERATE RISK";
    } else if(patient_status > 60 && patient_status <= 80) {
        patient_risk = "CRITICAL RISK";
    } else if(patient_status > 80 && patient_status <= 100) {
        patient_risk = "VERY CRITICAL RISK";
    }
/*     std::cout << std::endl << "*****************************************" << std::endl;
    std::cout << "PatientStatusInfo#" << std::endl;
    std::cout << "| THERM_RISK: " << trm_risk << std::endl;
    std::cout << "| ECG_RISK: " << ecg_risk << std::endl;
    std::cout << "| OXIM_RISK: " << oxi_risk << std::endl;
    std::cout << "| ABPS_RISK: " << abps_risk << std::endl;
    std::cout << "| ABPD_RISK: " << abpd_risk << std::endl;
    std::cout << "| GLC_RISK: " << glc_risk << std::endl;
    std::cout << "| PATIENT_STATE:" << patient_risk << std::endl; */

    std::cout << std::endl << "*****************************************" << std::endl;
    std::cout << "PatientStatusInfo#" << std::endl;
    std::cout << "| THERM_RISK: " << current_data.at(0) << std::endl;
    std::cout << "| ECG_RISK: " << current_data.at(1) << std::endl;
    std::cout << "| OXIM_RISK: " << current_data.at(2) << std::endl;
    std::cout << "| ABPS_RISK: " << current_data.at(3) << std::endl;
    std::cout << "| ABPD_RISK: " << current_data.at(4) << std::endl;
    std::cout << "| GLC_RISK: " << current_data.at(5) << std::endl;
    std::cout << "| PATIENT_STATE:" << patient_risk << std::endl; 

    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    //const auto p1 = std::chrono::system_clock::now();
    std::cout << "MilliSeconds Since Epoch: "
              << ms << '\n';

    std::cout << "*****************************************" << std::endl;
}

int32_t G4T1::getSensorId(std::string type) {
    if (type == "thermometer")
        return 0;
    else if (type == "ecg")
        return 1;
    else if (type == "oximeter")
        return 2;
    else if (type == "abps")
        return 3;
    else if (type == "abpd")		
        return 4;
    else if (type == "glucosemeter")        
        return 5;
    else {
        std::cout << "UNKNOWN TYPE " + type << std::endl;
        return -1;
    }
}

void G4T1::transfer() {
    messages::TargetSystemData msg;

    msg.trm_batt = trm_batt;
    msg.ecg_batt = ecg_batt;
    msg.oxi_batt = oxi_batt;
    msg.abps_batt = abps_batt;
    msg.abpd_batt = abpd_batt;
    msg.glc_batt = glc_batt;

    msg.trm_risk = trm_risk;
    msg.ecg_risk = ecg_risk;
    msg.oxi_risk = oxi_risk;
    msg.abps_risk = abps_risk;
    msg.abpd_risk = abpd_risk;
    msg.glc_risk = glc_risk;
    
    msg.trm_data = trm_raw;
    msg.ecg_data = ecg_raw;
    msg.oxi_data = oxi_raw;
    msg.abps_data = abps_raw;
    msg.abpd_data = abpd_raw;
    msg.glc_data = glc_raw;

    msg.patient_status = patient_status;

    pub.publish(msg);

    if (lost_packt) {
        lost_packt = false;
        throw std::domain_error("lost data due to package overflow");
    }
}
