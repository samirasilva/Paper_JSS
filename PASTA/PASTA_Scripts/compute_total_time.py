import pandas as pd
import numpy as np


number_of_patients=278
number_of_ex=10

print("Total time for each execution:")
for s in range(0,number_of_ex):
	nome_do_arquivo = './output_'+str(s)+'/tempo.txt'
	df = pd.read_csv(nome_do_arquivo, header=None, sep=' ')
	tempo=df[0].values
	total=np.sum(tempo)
	print(total)
	
	
