#include <ros/ros.h>
#include "libbsn/generator/DataGenerator.hpp"
#include "libbsn/utils/utils.hpp"
#include "libbsn/range/Range.hpp"
#include <string>
#include "services/PatientData.h"
#include <ros/console.h>

#include "archlib/ROSComponent.hpp"

#include <ctime>


class PatientModule : public arch::ROSComponent {
    public:
        PatientModule(int &argc, char **argv, std::string name);
        ~PatientModule();

        void setUp();
        void tearDown();
        void body();

        double computeCoverage(std::string sensor);

    private:
        bool getPatientData(services::PatientData::Request &request, services::PatientData::Response &response);
        bsn::generator::DataGenerator configureDataGenerator(const std::string& vitalSign);

        std::map<std::string, bsn::generator::DataGenerator> patientData;
        std::map<std::string, double> vitalSignsFrequencies;
        std::map<std::string, double> vitalSignsChanges;
        std::map<std::string, double> vitalSignsOffsets;

        int finished[6]={0,0,0,0,0,0};
        double frequency;
        double period;
        ros::NodeHandle nh;
        ros::ServiceServer service;
};
