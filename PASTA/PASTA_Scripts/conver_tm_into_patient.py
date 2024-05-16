import sys
import os 

#FORMAT OUTPUT OF THE BSN AND COMPUTE THE ORACLE
#Example of call: 
#python conver_tm_into_patient.py './patient/' '1_ACTS_BSN_Test_Set_t_2.txt'
#or
#python conver_tm_into_patient.py


def main(argv):

   if(len(sys.argv)==3):
       patients_folder= sys.argv[1]
       input_file = sys.argv[2]      
   elif(len(sys.argv)==1):
       patients_folder="./patient/"
       input_file="1_ACTS_BSN_Test_Set_t_2.txt"
   else:
       print("Error!")
       exit(0)

   if not os.path.isdir(patients_folder): 
       os.makedirs(patients_folder) 


   tm_ox = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]
   tm_hr = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]
   tm_temp = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]
   tm_abps = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]
   tm_abpd = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]
   tm_glc = [[0, 0, 0, 0,0], [0, 0, 0, 0,0],  [0, 0, 0, 0,0], [0, 0, 0, 0,0], [0, 0, 0, 0,0]]

   patient_id=0
   start=False
   with open(input_file) as f:
      while True:
         line = f.readline()
         if not line:
            break
         row = line.strip()
         if(line.startswith("[Test Set]")):
            line = f.readline()
            line = f.readline()
            line = f.readline()
            line = f.readline()
            row = line.strip()
            start=True
         if(start==True):
            l = [int(num) for num in line.split(',')]
            tm_ox[2][0] = l[0]
            tm_ox[2][1] = l[1]
            tm_ox[2][2] = l[2]
            tm_ox[2][3] = l[3]
            tm_ox[2][4] = l[4]

            tm_ox[3][0] = l[0]
            tm_ox[3][1] = l[1]
            tm_ox[3][2] = l[2]
            tm_ox[3][3] = l[3]
            tm_ox[3][4] = l[4]

            tm_ox[4][0] = l[0]
            tm_ox[4][1] = l[1]
            tm_ox[4][2] = l[2]
            tm_ox[4][3] = l[3]
            tm_ox[4][4] = l[4]
#########################################################
            tm_hr[0][0] = l[5]
            tm_hr[0][1] = l[6]
            tm_hr[0][2] = l[7]
            tm_hr[0][3] = l[8]
            tm_hr[0][4] = l[9]

            tm_hr[1][0] = l[5]
            tm_hr[1][1] = l[6]
            tm_hr[1][2] = l[7]
            tm_hr[1][3] = l[8]
            tm_hr[1][4] = l[9]

            tm_hr[2][0] = l[5]
            tm_hr[2][1] = l[6]
            tm_hr[2][2] = l[7]
            tm_hr[2][3] = l[8]
            tm_hr[2][4] = l[9]

            tm_hr[3][0] = l[5]
            tm_hr[3][1] = l[6]
            tm_hr[3][2] = l[7]
            tm_hr[3][3] = l[8]
            tm_hr[3][4] = l[9]

            tm_hr[4][0] = l[5]
            tm_hr[4][1] = l[6]
            tm_hr[4][2] = l[7]
            tm_hr[4][3] = l[8]
            tm_hr[4][4] = l[9]
#########################################################
            tm_temp[0][0] = l[10]
            tm_temp[0][1] = l[11]
            tm_temp[0][2] = l[12]
            tm_temp[0][3] = l[13]
            tm_temp[0][4] = l[14]

            tm_temp[1][0] = l[10]
            tm_temp[1][1] = l[11]
            tm_temp[1][2] = l[12]
            tm_temp[1][3] = l[13]
            tm_temp[1][4] = l[14]

            tm_temp[2][0] = l[10]
            tm_temp[2][1] = l[11]
            tm_temp[2][2] = l[12]
            tm_temp[2][3] = l[13]
            tm_temp[2][4] = l[14]

            tm_temp[3][0] = l[10]
            tm_temp[3][1] = l[11]
            tm_temp[3][2] = l[12]
            tm_temp[3][3] = l[13]
            tm_temp[3][4] = l[14]

            tm_temp[4][0] = l[10]
            tm_temp[4][1] = l[11]
            tm_temp[4][2] = l[12]
            tm_temp[4][3] = l[13]
            tm_temp[4][4] = l[14]
#########################################################
            tm_abps[2][0] = l[15]
            tm_abps[2][1] = l[16]
            tm_abps[2][2] = l[17]
            tm_abps[2][3] = l[18]
            tm_abps[2][4] = l[19]

            tm_abps[3][0] = l[15]
            tm_abps[3][1] = l[16]
            tm_abps[3][2] = l[17]
            tm_abps[3][3] = l[18]
            tm_abps[3][4] = l[19]

            tm_abps[4][0] = l[15]
            tm_abps[4][1] = l[16]
            tm_abps[4][2] = l[17]
            tm_abps[4][3] = l[18]
            tm_abps[4][4] = l[19]
#########################################################
            tm_abpd[2][0] = l[20]
            tm_abpd[2][1] = l[21]
            tm_abpd[2][2] = l[22]
            tm_abpd[2][3] = l[23]
            tm_abpd[2][4] = l[24]

            tm_abpd[3][0] = l[20]
            tm_abpd[3][1] = l[21]
            tm_abpd[3][2] = l[22]
            tm_abpd[3][3] = l[23]
            tm_abpd[3][4] = l[24]

            tm_abpd[4][0] = l[20]
            tm_abpd[4][1] = l[21]
            tm_abpd[4][2] = l[22]
            tm_abpd[4][3] = l[23]
            tm_abpd[4][4] = l[24]
#########################################################
            tm_glc[0][0] = l[25]
            tm_glc[0][1] = l[26]
            tm_glc[0][2] = l[27]
            tm_glc[0][3] = l[28]
            tm_glc[0][4] = l[29]

            tm_glc[1][0] = l[25]
            tm_glc[1][1] = l[26]
            tm_glc[1][2] = l[27]
            tm_glc[1][3] = l[28]
            tm_glc[1][4] = l[29]

            tm_glc[2][0] = l[25]
            tm_glc[2][1] = l[26]
            tm_glc[2][2] = l[27]
            tm_glc[2][3] = l[28]
            tm_glc[2][4] = l[29]

            tm_glc[3][0] = l[25]
            tm_glc[3][1] = l[26]
            tm_glc[3][2] = l[27]
            tm_glc[3][3] = l[28]
            tm_glc[3][4] = l[29]

            tm_glc[4][0] = l[25]
            tm_glc[4][1] = l[26]
            tm_glc[4][2] = l[27]
            tm_glc[4][3] = l[28]
            tm_glc[4][4] = l[29]

            header = "<launch>\n\t <!-- Blood Oxigenation Measurement Sensor -->\n\t<node name=\"patient\" pkg=\"patient\" type=\"patient\" output=\"screen\" />\n\n\t<param name=\"frequency\" value=\"10\" />\n\n\t<param name=\"vitalSigns\" value=\"oxigenation, heart_rate, temperature, abps, abpd, glucose\" />\n\n\t <!-- Frequency for changes in states of each markov in Hertz -->\n\t<param name=\"oxigenation_Change\" value=\"0.4\"/>\n\t<param name=\"heart_rate_Change\" value=\"0.4\"/>\n\t<param name=\"temperature_Change\" value=\"0.4\"/>\n\t<param name=\"abps_Change\" value=\"0.4\"/>\n\t<param name=\"abpd_Change\" value=\"0.4\"/>\n\t<param name=\"glucose_Change\" value=\"0.4\"/>\n\n\t<!-- Offsets for each changes, in seconds -->\n\t<param name=\"oxigenation_Offset\" value=\"4\"/>\n\t<param name=\"heart_rate_Offset\" value=\"4\"/>\n\t<param name=\"temperature_Offset\" value=\"4\"/>\n\t<param name=\"abps_Offset\" value=\"4\"/>\n\t<param name=\"abpd_Offset\" value=\"4\"/>\n\t<param name=\"glucose_Offset\" value=\"4\"/>\n\n"
   
            final="\t<!-- Markov chain for oxigenation -->\n\t<param name=\"oxigenation_State0\" value=\"0,0,0,0,0\" />\n\t<param name=\"oxigenation_State1\" value=\"0,0,0,0,0\" />\n\t<param name=\"oxigenation_State2\" value=\""+str(tm_ox[2][0])+","+str(tm_ox[2][1])+","+str(tm_ox[2][2])+","+str(tm_ox[2][3])+","+str(tm_ox[2][4])+"\" />\n\t<param name=\"oxigenation_State3\" value=\""+str(tm_ox[3][0])+","+str(tm_ox[3][1])+","+str(tm_ox[3][2])+","+str(tm_ox[3][3])+","+str(tm_ox[3][4])+"\" />\n\t<param name=\"oxigenation_State4\" value=\""+str(tm_ox[4][0])+","+str(tm_ox[4][1])+","+str(tm_ox[4][2])+","+str(tm_ox[4][3])+","+str(tm_ox[4][4])+"\" />\n\t\n\t<!-- Risk values for oximeter -->\n\t<param name=\"oxigenation_HighRisk0\" value=\"-1,-1\" />\n\t<param name=\"oxigenation_MidRisk0\" value=\"-1,-1\" />\n\t<param name=\"oxigenation_LowRisk\" value=\"65,100\" />\n\t<param name=\"oxigenation_MidRisk1\" value=\"55,65\" />\n\t<param name=\"oxigenation_HighRisk1\" value=\"0,55\" />\n\n\t<!-- Markov chain for heart frequency -->\n\t<param name=\"heart_rate_State0\" value=\""+str(tm_hr[0][0])+","+str(tm_hr[0][1])+","+str(tm_hr[0][2])+","+str(tm_hr[0][3])+","+str(tm_hr[0][4])+"\" />\n\t<param name=\"heart_rate_State1\" value=\""+str(tm_hr[1][0])+","+str(tm_hr[1][1])+","+str(tm_hr[1][2])+","+str(tm_hr[1][3])+","+str(tm_hr[1][4])+"\" />\n\t<param name=\"heart_rate_State2\" value=\""+str(tm_hr[2][0])+","+str(tm_hr[2][1])+","+str(tm_hr[2][2])+","+str(tm_hr[2][3])+","+str(tm_hr[2][4])+"\" />\n\t<param name=\"heart_rate_State3\" value=\""+str(tm_hr[3][0])+","+str(tm_hr[3][1])+","+str(tm_hr[3][2])+","+str(tm_hr[3][3])+","+str(tm_hr[3][4])+"\" />\n\t<param name=\"heart_rate_State4\" value=\""+str(tm_hr[4][0])+","+str(tm_hr[4][1])+","+str(tm_hr[4][2])+","+str(tm_hr[4][3])+","+str(tm_hr[4][4])+"\" />\n\n\t<!-- Risk values for heart frequency -->\n\t<param name=\"heart_rate_HighRisk0\" value=\"0,70\" />\n\t<param name=\"heart_rate_MidRisk0\" value=\"70,85\" />\n\t<param name=\"heart_rate_LowRisk\" value=\"85,97\" />\n\t<param name=\"heart_rate_MidRisk1\" value=\"97,115\" />\n\t<param name=\"heart_rate_HighRisk1\" value=\"115,300\" />\n\n\t<!-- Markov chain for temperature -->\n\t<param name=\"temperature_State0\" value=\""+str(tm_temp[0][0])+","+str(tm_temp[0][1])+","+str(tm_temp[0][2])+","+str(tm_temp[0][3])+","+str(tm_temp[0][4])+"\" />\n\t<param name=\"temperature_State1\" value=\""+str(tm_temp[1][0])+","+str(tm_temp[1][1])+","+str(tm_temp[1][2])+","+str(tm_temp[1][3])+","+str(tm_temp[1][4])+"\" />\n\t<param name=\"temperature_State2\" value=\""+str(tm_temp[2][0])+","+str(tm_temp[2][1])+","+str(tm_temp[2][2])+","+str(tm_temp[2][3])+","+str(tm_temp[2][4])+"\" />\n\t<param name=\"temperature_State3\" value=\""+str(tm_temp[3][0])+","+str(tm_temp[3][1])+","+str(tm_temp[3][2])+","+str(tm_temp[3][3])+","+str(tm_temp[3][4])+"\" />\n\t<param name=\"temperature_State4\" value=\""+str(tm_temp[4][0])+","+str(tm_temp[4][1])+","+str(tm_temp[4][2])+","+str(tm_temp[4][3])+","+str(tm_temp[4][4])+"\" />\n\n\t<!-- Risk values for temperature -->\n\t<param name=\"temperature_HighRisk0\" value=\"0,31.99\" />\n\t<param name=\"temperature_MidRisk0\" value=\"32,35.99\" />\n\t<param name=\"temperature_LowRisk\" value=\"36,37.99\" />\n\t<param name=\"temperature_MidRisk1\" value=\"38,40.99\" />\n\t<param name=\"temperature_HighRisk1\" value=\"41,50\" />\n\n\t<!-- Markov chain for diastolic pressure -->\n\t<param name=\"abpd_State0\" value=\"0,0,0,0,0\" />\n\t<param name=\"abpd_State1\" value=\"0,0,0,0,0\" />\n\t<param name=\"abpd_State2\" value=\""+str(tm_abpd[2][0])+","+str(tm_abpd[2][1])+","+str(tm_abpd[2][2])+","+str(tm_abpd[2][3])+","+str(tm_abpd[2][4])+"\" />\n\t<param name=\"abpd_State3\" value=\""+str(tm_abpd[3][0])+","+str(tm_abpd[3][1])+","+str(tm_abpd[3][2])+","+str(tm_abpd[3][3])+","+str(tm_abpd[3][4])+"\" />\n\t<param name=\"abpd_State4\" value=\""+str(tm_abpd[4][0])+","+str(tm_abpd[4][1])+","+str(tm_abpd[4][2])+","+str(tm_abpd[4][3])+","+str(tm_abpd[4][4])+"\" />\n\n\t<!-- Risk values for diastolic pressure -->\n\t<param name=\"abpd_HighRisk0\" value=\"-1,-1\" />\n\t<param name=\"abpd_MidRisk0\" value=\"-1,-1\" />\n\t<param name=\"abpd_LowRisk\" value=\"0,80\" />\n\t<param name=\"abpd_MidRisk1\" value=\"80,90\" />\n\t<param name=\"abpd_HighRisk1\" value=\"90,300\" />\n\n\t<!-- Markov chain for systolic pressure -->\n\t<param name=\"abps_State0\" value=\"0,0,0,0,0\" />\n\t<param name=\"abps_State1\" value=\"0,0,0,0,0\" />\n\t<param name=\"abps_State2\" value=\""+str(tm_abps[2][0])+","+str(tm_abps[2][1])+","+str(tm_abps[2][2])+","+str(tm_abps[2][3])+","+str(tm_abps[2][4])+"\" />\n\t<param name=\"abps_State3\" value=\""+str(tm_abps[3][0])+","+str(tm_abps[3][1])+","+str(tm_abps[3][2])+","+str(tm_abps[3][3])+","+str(tm_abps[3][4])+"\" />\n\t<param name=\"abps_State4\" value=\""+str(tm_abps[4][0])+","+str(tm_abps[4][1])+","+str(tm_abps[4][2])+","+str(tm_abps[4][3])+","+str(tm_abps[4][4])+"\" />\n\n\t<!-- Risk values for systolic pressure -->\n\t<param name=\"abps_MidRisk0\" value=\"-1,-1\" />\n\t<param name=\"abps_HighRisk0\" value=\"-1,-1\" />\n\t<param name=\"abps_LowRisk\" value=\"0,120\" />\n\t<param name=\"abps_MidRisk1\" value=\"120,140\" />\n\t<param name=\"abps_HighRisk1\" value=\"140,300\" />\n\n\t<!-- Markov chain for glucose -->\n\t<param name=\"glucose_State0\" value=\""+str(tm_glc[0][0])+","+str(tm_glc[0][1])+","+str(tm_glc[0][2])+","+str(tm_glc[0][3])+","+str(tm_glc[0][4])+"\" />\n\t<param name=\"glucose_State1\" value=\""+str(tm_glc[1][0])+","+str(tm_glc[1][1])+","+str(tm_glc[1][2])+","+str(tm_glc[1][3])+","+str(tm_glc[1][4])+"\" />\n\t<param name=\"glucose_State2\" value=\""+str(tm_glc[2][0])+","+str(tm_glc[2][1])+","+str(tm_glc[2][2])+","+str(tm_glc[2][3])+","+str(tm_glc[2][4])+"\" />\n\t<param name=\"glucose_State3\" value=\""+str(tm_glc[3][0])+","+str(tm_glc[3][1])+","+str(tm_glc[3][2])+","+str(tm_glc[3][3])+","+str(tm_glc[3][4])+"\" />\n\t<param name=\"glucose_State4\" value=\""+str(tm_glc[4][0])+","+str(tm_glc[4][1])+","+str(tm_glc[4][2])+","+str(tm_glc[4][3])+","+str(tm_glc[4][4])+"\" />\n\n\t<!-- Risk values for glucose -->\n\t<param name=\"glucose_HighRisk0\" value=\"20,39.99\" />\n\t<param name=\"glucose_MidRisk0\" value=\"40,54.99\" />\n\t<param name=\"glucose_LowRisk\" value=\"55,95.99\" />\n\t<param name=\"glucose_MidRisk1\" value=\"96,119.99\" />\n\t<param name=\"glucose_HighRisk1\" value=\"120,200\" />\n</launch>"

            file1 = open(patients_folder+"patient_"+str(patient_id)+".launch", "a") 
            file1.write(header+final)
            file1.close()
            patient_id=patient_id+1


if __name__ == "__main__":
    main(sys.argv)

