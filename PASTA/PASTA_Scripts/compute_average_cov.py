# -*- coding: utf-8 -*-


def calcular_media_vetores(vetores):
 
    # Verificar se os vetores foram lidos corretamente
    if vetores:
        # Calcular a média de cada posição entre todos os vetores
        media_vetores = [round(sum(coluna) / len(vetores),2) for coluna in zip(*vetores)]

        #print("Vetor de medias:", media_vetores)
        return media_vetores
        

def ler_vetores(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        linhas = arquivo.readlines()
        
    vetor_atual=[]
    for line in linhas:
        if line.strip():
    		vetor_atual.append(float(line))
    #print("Vetor:", vetor_atual)
    return vetor_atual
    

medias=[]
number_of_patients=278
number_of_ex=10

for s in range(0,number_of_ex):
	vetores = []
	for j in range(0,number_of_patients):
		nome_do_arquivo = './output_'+str(s)+'/cov_'+str(j)+'.txt'
		vetores.append(ler_vetores(nome_do_arquivo))
	vetor_medio=calcular_media_vetores(vetores)
	medias.append(vetor_medio)

m=calcular_media_vetores(medias)
print("Average Coverage Vector "+str(number_of_ex)+" executions -  Oxi: "+str(float("{:.2f}".format(m[0])))+" Ecg: "+str(float("{:.2f}".format(m[1])))+ " Temp: "+str(float("{:.2f}".format(m[2])))+" Abps:"+str(float("{:.2f}".format(m[3])))+ " Abpd: "+str(float("{:.2f}".format(m[4])))+" Glc: "+str(float("{:.2f}".format(m[5]))))
