import sys
import os


import openpyxl
import pandas as pd
import math

def truncate(f, n):
    return math.floor(f * 10 ** n) / 10 ** n
    
	
#COMPUTATION OF THE EXPECTED OUTCOME
def compute_oracle(spo2, hr, temp, sys_bp, dias_bp, glu):

    output=""

    all_sensors=[]
    all_sensors.append(spo2)
    all_sensors.append(hr)
    all_sensors.append(temp)
    all_sensors.append(sys_bp)
    all_sensors.append(dias_bp)
    all_sensors.append(glu)

    amount_high=0
    amount_med=0
    amount_low=0

    for item in all_sensors:
         if(item=="low risk"):
             amount_low=amount_low+1
         elif(item=="moderate risk"):
             amount_med=amount_med+1
         elif(item=="high risk"):
             amount_high=amount_high+1

    total= amount_high+amount_med+amount_low

    if (total!=6):
    	return "Error"

    elif (amount_low==6):
        return "VERY LOW RISK"

    elif (amount_med==1 and amount_high==0):
        return "LOW RISK"

    elif (amount_med>=2 and amount_high==0):
        return "MODERATE RISK"

    elif (amount_high==1):
        return "CRITICAL RISK"

    elif (amount_high>1):
        return "VERY CRITICAL RISK"

    else: 
        return "ERROR!"




#FORMAT OUTPUT OF THE BSN AND COMPUTE THE ORACLE
#Example of call: 
#python read_output_prob_t2.py './output' 'output_sensor_readings.xlsx' 278
#or
#python read_output_prob_t2.py

def main(argv):
    #data = OrderedDict()
    content=[]
    
    # Create a new Excel workbook
    workbook = openpyxl.Workbook()
    # Select the default sheet (usually named 'Sheet')
    sheet = workbook.active

    result=[]
    sum_diff=[0,0,0,0,0]
    


    if(len(sys.argv)==4):
        BSN_output_folder= sys.argv[1]
        resulting_file_name = sys.argv[2]
        number_of_patients= int(sys.argv[3])   
    elif(len(sys.argv)==1):
        BSN_output_folder= './output_6'
        resulting_file_name = 'output_sensor_readings.xlsx'
        number_of_patients=278
    else:
        print("Error!")
        exit(0)

    #file1 = open(BSN_output_folder+"/"+resulting_file_name, "w") 
    content.append(["Id","Patient","Oxi","Ecg","Term","Abps","Abpd","Glc","BSN Outcome","Expected Outcome","Difference","Oxi-Risk","Ecg-Risk","Term-Risk","Abps-Risk","Abpd-Risk","Glc-Risk","Oxi-Sens","Ecg-Sens","Term-Sens","Abps-Sens","Abpd-Sens","Glc-Sens","Timestamp"])
      
    #file1.write("Id\t"+"Patient"+"\t"+"Oxi"+"\t"+"Ecg"+"\t"+"Term"+"\t"+"Abps"+"\t"+"Abpd"+"\t"+"Glc\t"+"BSN Outcome\t"+"Expected Outcome\t"+"Difference\t"+"Oxi-Risk"+"\t"+"Ecg-Risk"+"\t"+"Term-Risk"+"\t"+"Abps-Risk"+"\t"+"Abpd-Risk"+"\t"+"Glc-Risk\t"+"Oxi-Sens"+"\t"+"Ecg-Sens"+"\t"+"Term-Sens"+"\t"+"Abps-Sens"+"\t"+"Abpd-Sens"+"\t"+"Glc-Sens\t"+"Timestamp\n")

    term_sensor="0"
    ecg_sensor="0"
    oxi_sensor="0"
    abps_sensor="0"
    abpd_sensor="0"
    glc_sensor="0"
    sensor_values=False
    id=0
    
    term="unknown"
    ecg="unknown"
    oxi="unknown"
    abps="unknown"
    abpd="unknown"
    glc="unknown"

    id_outcome=0
    id_oracle=0
    id_difference=0
    for i in range(0,number_of_patients):
      if(os.path.isfile(BSN_output_folder+'/g4t1_'+str(i)+'-1-stdout.log')):
        with open(BSN_output_folder+'/g4t1_'+str(i)+'-1-stdout.log') as f:
            term_sensor="0"
            ecg_sensor="0"
            oxi_sensor="0"
            abps_sensor="0"
            abpd_sensor="0"
            glc_sensor="0"
            sensor_values=False
    	    id_outcome=0
            id_oracle=0
            id_difference=0

            x = f.readline() 
            for line in f: 

                if(sensor_values==False):
            	    if(line.startswith("Term:")):
                	    term=line[6:len(line)-1]
            	    if(line.startswith("Ecg:")):
                	    ecg=line[5:len(line)-1]
            	    if(line.startswith("Oxi:")):
                	    oxi=line[5:len(line)-1]
            	    if(line.startswith("Abps:")):
                	    abps=line[6:len(line)-1]
            	    if(line.startswith("Abpd:")):
                	    abpd=line[6:len(line)-1]
            	    if(line.startswith("Glc:")):
                	    glc=line[5:len(line)-1]

           	    if(line.startswith("| THERM_RISK: ")):
                	    term_risk=line[14:len(line)-1]
            	    if(line.startswith("| ECG_RISK: ")):
                	    ecg_risk=line[12:len(line)-1]
            	    if(line.startswith("| OXIM_RISK: ")):
               	 	    oxi_risk=line[13:len(line)-1]
           	    if(line.startswith("| ABPS_RISK: ")):
                	    abps_risk=line[13:len(line)-1]
            	    if(line.startswith("| ABPD_RISK: ")):
                	    abpd_risk=line[13:len(line)-1]
            	    if(line.startswith("| GLC_RISK: ")):
                	    glc_risk=line[12:len(line)-1]
                    if(line.startswith("++++++++++++++++++++")):
                            sensor_values=True
                else:
	            if(line.startswith("Trm:")):
                	    term_sensor=line[5:len(line)-1]
            	    if(line.startswith("Ecg:")):
	                    ecg_sensor=line[5:len(line)-1]
            	    if(line.startswith("Oxi:")):
               		    oxi_sensor=line[5:len(line)-1]
            	    if(line.startswith("Abps:")):
               		    abps_sensor=line[6:len(line)-1]
            	    if(line.startswith("Abpd:")):
                	    abpd_sensor=line[6:len(line)-1]
            	    if(line.startswith("Glc:")):
               		    glc_sensor=line[5:len(line)-1]
                    if(line.startswith("++++++++++++++++++++")):
                            sensor_values=False           

                if(line.startswith("MilliSeconds Since Epoch:")):
                    time=line[25:(len(line)-1)]

                if(line.startswith("| PATIENT_STATE:")):
                    resultado=line[16:(len(line)-1)]
                    if(term!="unknown" and ecg!="unknown" and oxi!="unknown" and abps!="unknown" and abpd!="unknown" and glc!="unknown"):
                        oracle=compute_oracle(oxi,ecg,term,abps, abpd, glc)

                        if (resultado=="VERY LOW RISK"):
                           id_outcome=0
                        elif (resultado=="LOW RISK"):
                           id_outcome=1
                        elif (resultado=="MODERATE RISK"):
                           id_outcome=2
                        elif (resultado=="CRITICAL RISK"):
                           id_outcome=3
                        elif (resultado=="VERY CRITICAL RISK"):
                           id_outcome=4
                        else:
                           id_outcome=9999999999999

                        if (oracle=="VERY LOW RISK"):
                           id_oracle=0
                        elif (oracle=="LOW RISK"):
                           id_oracle=1
                        elif (oracle=="MODERATE RISK"):
                           id_oracle=2
                        elif (oracle=="CRITICAL RISK"):
                           id_oracle=3
                        elif (oracle=="VERY CRITICAL RISK"):
                           id_oracle=4
                        else:
                           id_oracle=9999999999999
                        id_difference=abs(id_outcome-id_oracle)
                        content.append([str(id),str(i),oxi,ecg,term,abps,abpd,glc,resultado,oracle,str(id_difference), oxi_risk,ecg_risk,term_risk,abps_risk,abpd_risk, glc_risk,oxi_sensor, ecg_sensor,term_sensor, abps_sensor, abpd_sensor,glc_sensor,time])
                        sum_diff[id_difference]=sum_diff[id_difference]+1
                        
                        #testcase = str(id)+"\t"+str(i)+"\t"+oxi+"\t"+ecg+"\t"+term+"\t"+abps+"\t"+abpd+"\t"+glc+"\t"+resultado+"\t"+oracle+"\t"+str(id_difference)+"\t"+oxi_risk+"\t"+ecg_risk+"\t"+term_risk+"\t"+abps_risk+"\t"+abpd_risk+"\t"+glc_risk+"\t"+oxi_sensor+"\t"+ecg_sensor+"\t"+term_sensor+"\t"+abps_sensor+"\t"+abpd_sensor+"\t"+glc_sensor+"\t"+time+"\n"
                        #testcase = str(id)+"\t"+str(i)+"\t"+oxi+"\t"+ecg+"\t"+term+"\t"+abps+"\t"+abpd+"\t"+glc+"\t"+resultado+"\t"+oracle+"\t"+str(id_difference)+"\t"+oxi_risk+"\t"+ecg_risk+"\t"+term_risk+"\t"+abps_risk+"\t"+abpd_risk+"\t"+glc_risk+"\t"+oxi_sensor+"\t"+ecg_sensor+"\t"+term_sensor+"\t"+abps_sensor+"\t"+abpd_sensor+"\t"+glc_sensor+"\n"
                        
		        id=id+1
                        
                        #file1.write(testcase)
                       
    
    tot_diff=sum_diff[0]+sum_diff[1]+sum_diff[2]+sum_diff[3]+sum_diff[4]
    tot_diff_perc=[]

    tot_diff_perc.append(sum_diff[0]*100/float(tot_diff))
    tot_diff_perc.append(sum_diff[1]*100/float(tot_diff))
    tot_diff_perc.append(sum_diff[2]*100/float(tot_diff))
    tot_diff_perc.append(sum_diff[3]*100/float(tot_diff))
    tot_diff_perc.append(sum_diff[4]*100/float(tot_diff))
  
    content.append([])
    content.append(["","Absolute Amount","Percentage Amount"])
    content.append(["Difference 0", sum_diff[0],str(tot_diff_perc[0])+"%"])
    content.append(["Difference 1", sum_diff[1],str(tot_diff_perc[1])+"%","","Passing TC Rate:"])
    content.append(["Difference 2", sum_diff[2],str(tot_diff_perc[2])+"%","",str(float("{:.2f}".format(tot_diff_perc[0]+tot_diff_perc[1])))+"%"])
    content.append(["Difference 3", sum_diff[3],str(tot_diff_perc[3])+"%"])
    content.append(["Difference 4", sum_diff[4],str(tot_diff_perc[4])+"%"])
    content.append([])
    content.append([])
    content.append(["Sum",tot_diff,str(tot_diff_perc[0]+tot_diff_perc[1]+tot_diff_perc[2]+tot_diff_perc[3]+tot_diff_perc[4])+"%"])
        
    for row in content:
       sheet.append(row)
    
    # Save the workbook to a file
    workbook.save(BSN_output_folder+"/"+resulting_file_name)
    
 
if __name__ == "__main__":
    main(sys.argv)

