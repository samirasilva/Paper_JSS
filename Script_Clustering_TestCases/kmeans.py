from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
import numpy as np
import pandas as pd
import pandas
import math
import csv
from itertools import izip_longest



#https://www.w3schools.com/python/python_ml_k-means.asp
def my_kmeans (data,n,y,i):
	#kmeans = KMeans(n_clusters=n,init='k-means++')
	kmeans = KMeans(n_clusters=n)
	kmeans.fit(data)
	labels = kmeans.labels_

	dt1=[]
	dt2=[]
	aux=0
	for line in labels:
		if(y[aux]==1):
			dt1.append(line)
		elif(y[aux]==2):
			dt2.append(line)
		aux=aux+1

	data = []
	data.append(dt1)
	data.append(dt2)
	columns_data = izip_longest(*data)

	with open("sensors_"+str(i)+"_output_"+str(n)+'.csv', 'w') as out_file:
	        writer = csv.writer(out_file)
	        writer.writerow(('Dataset1', 'Dataset2'))
	        writer.writerows(columns_data)
	return labels

def plot_2_Dataset1es(j,n_cluster):
	cols = ["Dataset1", "Dataset2"]
	data = pd.read_csv("sensors_"+str(j)+"_output_"+str(n_cluster)+".csv",names=cols)

	dataset1 = data['Dataset1']
	dataset2 = data['Dataset2']
	dataset1=dataset1.fillna(-1)

	
	dt1=[]
	dt2=[]

	for line in dataset1:
		if(line!=-1):
			dt1.append(line)
	dt1=dt1[1:]

	for line in dataset2:
	 	dt2.append(line)
	dt2=dt2[1:]

	x_multi=[]
	x_multi.append(dt1)
	x_multi.append(dt2)

	n, bins, patches = plt.hist(x_multi, n_cluster, histtype='bar', align='mid',density=False,orientation="vertical")
	plt.legend(['Combinatorial', 'PASTA'])

	ax = plt.gca()

	max_value = n_cluster-1
	
	min_value = 0
	number_of_steps = 10
	l = np.arange(min_value, max_value+1, number_of_steps)

	ax.set(xticks=l, xticklabels=l)
	plt.ylabel('Frequency')
	plt.xlabel('Cluster')
	plt.title('Histogram - Combinatorial x PASTA')

	#plt.savefig(str(j)+'pasta_x_comb'+str(n_cluster)+'.png')
	#plt.clf()
	plt.show()




cols = ["Oxi-Risk", "Ecg-Risk","Term-Risk","Abps-Risk","Abpd-Risk","Glc-Risk","Label"]
data = pd.read_csv("sensors_pasta_x_comb.csv",names=cols)
y = data['Label']
X = data.ix[:,:'Glc-Risk']

scaler = StandardScaler()
data_scaled = scaler.fit_transform(X)


my_kmeans (data_scaled,400,y,0)
plot_2_Dataset1es(0,400)



