#include "component/g4t1/G4T1.hpp"
#include <chrono>

#define W(x) std::cerr << #x << " = " << x << std::endl;

#define BATT_UNIT 0.001

using namespace bsn::processor;
using namespace std::chrono;

G4T1::G4T1(int &argc, char **argv, const std::string &name) : lost_packt(false),
    CentralHub(argc, argv, name, true, bsn::resource::Battery("ch_batt", 100, 100, 1) ),
    patient_status(0.0) {}
	
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

    //APAGUEI A IMPRESSÃO AQUI
/*  std::vector<std::string> v = {std::to_string(trm_raw), std::to_string(ecg_raw), std::to_string(oxi_raw), std::to_string(abps_raw), std::to_string(abpd_raw), std::to_string(glc_raw)};
    std::cout<<"++++++++++++++++++++"<<std::endl;
    std::cout<<"Trm: "+v[0]<<std::endl;
    std::cout<<"Ecg: "+v[1]<<std::endl;
    std::cout<<"Oxi: "+v[2]<<std::endl;
    std::cout<<"Abps: "+v[3]<<std::endl;
    std::cout<<"Abpd: "+v[4]<<std::endl;
    std::cout<<"Glc: "+v[5]<<std::endl;
    std::cout<<"++++++++++++++++++++"<<std::endl; */
    
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

void G4T1::process(){
    battery.consume(BATT_UNIT * data_buffer.size());
    std::vector<double> current_data;




    for(std::vector<std::list<double>>::iterator it = data_buffer.begin(); it != data_buffer.end(); it++) {
        double el = it->front();
        current_data.push_back(el);
        if(it->size() > 1) it->pop_front();
    }

    //std::vector<double> vet;
    //vet.push_back(packetsReceived.at(0));
    //vet.push_back(85.6011);
    //vet.push_back(14.9541);
    //vet.push_back(92.414);
    //vet.push_back(72.3443);
    //vet.push_back(18.3003);
    patient_status = data_fuse(current_data);

    //patient_status = data_fuse(current_data); // consumes 1 packt per sensor (in the buffers that have packages to data_bufferbe processed)
    for (int i = 0; i < buffer_size.size(); ++i){ // update buffer sizes
        buffer_size[i] = data_buffer[i].size();
    }
    total_buffer_size = std::accumulate(std::begin(buffer_size), std::end(buffer_size), 0, std::plus<int>()); //update total buffer size 



    //MUDEI A IMPRESSÃO DOS DADOS RAW DO SENSOR PARA AQUI:
    std::cout<<"++++++++++++++++++++"<<std::endl;
    std::cout<<"Trm: "<<std::to_string(trm_raw)<<std::endl;
    std::cout<<"Ecg: "<<std::to_string(ecg_raw)<<std::endl;
    std::cout<<"Oxi: "<<std::to_string(oxi_raw)<<std::endl;
    std::cout<<"Abps: "<<std::to_string(abps_raw)<<std::endl;
    std::cout<<"Abpd: "<<std::to_string(abpd_raw)<<std::endl;
    std::cout<<"Glc: "<<std::to_string(glc_raw)<<std::endl;
    std::cout<<"++++++++++++++++++++"<<std::endl;

    std::vector<std::string> v = getPatientStatus();

    std::string label_term,label_ecg,label_oxi, label_abps, label_abpd,label_glc;
    if (current_data.at(0) > 0 && current_data.at(0) <= 20) {
        label_term= "low risk";
    } else if (current_data.at(0) > 20 && current_data.at(0) <= 65) {
        label_term = "moderate risk";
    } else if (current_data.at(0) > 65 && current_data.at(0) <= 100) {
        label_term = "high risk";
    } else {
        label_term = "unknown";
    }

    if (current_data.at(1) > 0 && current_data.at(1) <= 20) {
        label_ecg= "low risk";
    } else if (current_data.at(1) > 20 && current_data.at(1) <= 65) {
        label_ecg = "moderate risk";
    } else if (current_data.at(1) > 65 && current_data.at(1) <= 100) {
        label_ecg = "high risk";
    } else {
        label_ecg = "unknown";
    }

    if (current_data.at(2) > 0 && current_data.at(2) <= 20) {
        label_oxi= "low risk";
    } else if (current_data.at(2) > 20 && current_data.at(2) <= 65) {
        label_oxi = "moderate risk";
    } else if (current_data.at(2) > 65 && current_data.at(2) <= 100) {
        label_oxi = "high risk";
    } else {
        label_oxi = "unknown";
    }

    if (current_data.at(3) > 0 && current_data.at(3) <= 20) {
        label_abps= "low risk";
    } else if (current_data.at(3) > 20 && current_data.at(3) <= 65) {
        label_abps = "moderate risk";
    } else if (current_data.at(3) > 65 && current_data.at(3) <= 100) {
        label_abps = "high risk";
    } else {
        label_abps = "unknown";
    }

    if (current_data.at(4) > 0 && current_data.at(4) <= 20) {
        label_abpd= "low risk";
    } else if (current_data.at(4) > 20 && current_data.at(4) <= 65) {
        label_abpd = "moderate risk";
    } else if (current_data.at(4) > 65 && current_data.at(4) <= 100) {
        label_abpd = "high risk";
    } else {
        label_abpd = "unknown";
    }

    if (current_data.at(5) > 0 && current_data.at(5) <= 20) {
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
