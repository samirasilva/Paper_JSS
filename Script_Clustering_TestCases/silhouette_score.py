from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.metrics import davies_bouldin_score
from sklearn.preprocessing import StandardScaler
import pandas as pd
import csv

cols = ["Oxi-Risk", "Ecg-Risk","Term-Risk","Abps-Risk","Abpd-Risk","Glc-Risk","Label"]
data = pd.read_csv("sensors_pasta_x_comb.csv",names=cols)
y = data['Label']
X = data.ix[:,:'Glc-Risk']

scaler = StandardScaler()
data_scaled = scaler.fit_transform(X)

kmeans = KMeans(360).fit(data_scaled)
labels = kmeans.labels_
silhouette_avg = silhouette_score(data_scaled, labels)
print("Silhouette Score:", silhouette_avg)
