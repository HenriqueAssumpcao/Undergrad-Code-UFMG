import numpy as np
import pandas as pd
import time

from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score,rand_score

from functions import approx_k_clustering,pairwise_minkowski_distance,compute_cluster_radius




if __name__ == "__main__":
	datasets = []


	# ## Mammographic Mass
	# Link:https://archive.ics.uci.edu/ml/datasets/Mammographic+Mass
	# 
	# Tarefa: Prever se o tumor mamário é maligno ou benigno (Variável categórica binária)

	# In[4]:


	df = pd.read_csv("./datasets/mammographic_masses.data",delimiter=',',header=None)

	idx_to_keep = []
	for idx,i in enumerate(df.to_numpy()):
	    if len(np.where(i == '?')[0]) == 0:
	        idx_to_keep.append(idx)

	df_numpy = df.iloc[idx_to_keep].to_numpy(dtype=int)

	true_classes = df_numpy[:,-1]
	points = df_numpy[:,:5]
	num_classes = 2

	datasets.append(("MM",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)
	df.head()


	# ## South German Credit
	# Link: https://archive.ics.uci.edu/ml/datasets/South+German+Credit+%28UPDATE%29
	# 
	# Tarefa: Classificar o indivíduo de acordo com seu histórico de crédito (Variável categórica "moral" com 5 valores possíveis)

	# In[5]:


	df = pd.read_csv("./datasets/SouthGermanCredit.asc",delimiter = ' ')

	true_classes = df['moral'].to_numpy()
	df_cluster = df.drop(columns=['moral'])

	points = df_cluster.to_numpy()
	num_classes = 5

	datasets.append(("SGC",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Blood Transfusion Service Center
	# Link: https://archive.ics.uci.edu/ml/datasets/Blood+Transfusion+Service+Center
	# 
	# Tarefa: Prever a variável categórica binária que indica se a pessoa doou sangue em março de 2007

	# In[6]:


	df = pd.read_csv("./datasets/transfusion.data",delimiter=',')

	df_numpy = df.to_numpy()
	true_classes = df_numpy[:,-1]
	points = df_numpy[:,:-1]
	num_classes = 2

	datasets.append(("BT",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Audit Data
	# Link: https://archive.ics.uci.edu/ml/datasets/Audit+Data
	# 
	# Tarefa: Prever a variável categórica binária "risk"

	# In[7]:


	df = pd.read_csv("./datasets/audit_risk.csv")
	df = df.drop(columns=['LOCATION_ID']) # Identificador que não será utilizado na clusterização
	df = df.dropna()

	true_classes = df.iloc[:,-1].to_numpy(int)
	points = df.iloc[:,:-1].to_numpy(float)
	num_classes = 2

	datasets.append(("AD",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Drug Consumption
	# Link: https://archive.ics.uci.edu/ml/datasets/Drug+consumption+%28quantified%29
	# 
	# Tarefa: Prever uma classe que identifica o número de drogas já utilizadas pelo indivíduo (18 possíveis)

	# In[8]:


	df = pd.read_csv("./datasets/drug_consumption.data",header=None).iloc[:,1:]

	for category in ["CL" + str(i) for i in range(7)]:
	    if category == "CL0":
	        df = df.replace(category,0)
	    else:
	        df = df.replace(category,1)

	points = df.iloc[:,:12].to_numpy(dtype=float)

	true_classes = df.iloc[:,12:].sum(axis=1).to_numpy(dtype=int)
	num_classes = 18

	datasets.append(("DC",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Myocardial infarction complications
	# Link: https://archive.ics.uci.edu/ml/datasets/Myocardial+infarction+complications
	# 
	# Tarefa: Prever a variável categórica "Lethal Outcome", que pode assumir 8 valores que representam a causa da morte do paciente.

	# In[9]:


	df = pd.read_csv("./datasets/MI.data",header=None).iloc[:,1:]
	df = df.replace('?',0)


	points = df.iloc[:,:110].to_numpy(float)
	      
	true_classes = df.iloc[:,-1].to_numpy(int)
	num_classes = 8

	datasets.append(("MI",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Cervical cancer (Risk Factors)
	# Link: https://archive.ics.uci.edu/ml/datasets/Cervical+cancer+%28Risk+Factors%29
	# 
	# Tarefa: Prever o resultado binário da biopsia do paciente

	# In[10]:


	df = pd.read_csv("./datasets/risk_factors_cervical_cancer.csv")
	df = df.drop(columns = ['STDs: Time since first diagnosis','STDs: Time since last diagnosis']) # Muitos valores faltantes


	# Identifica quais linhas não possuem valores faltantes
	non_missing_idx = []
	for idx,i in enumerate(df.to_numpy()):
	    if len(np.where(i == '?')[0]) == 0:
	        non_missing_idx.append(idx)

	# Substitui os valores faltantes de cada coluna w.r.t. média daquela coluna
	for col_idx,col in enumerate(df.columns):
	    df[col] = df[col].replace("?",df.iloc[non_missing_idx,col_idx].astype(float).mean())


	points = df.iloc[:,:32].to_numpy(float)
	true_classes = df['Biopsy'].to_numpy(int)
	num_classes = 2
	datasets.append(("CC",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)

	df.head()


	# ## Room Occupancy Estimation
	# Link: https://archive.ics.uci.edu/ml/datasets/Room+Occupancy+Estimation
	# 
	# Tarefa: Classificar a sala de acordo com o número de moradores

	# In[11]:


	df = pd.read_csv("./datasets/Occupancy_Estimation.csv").iloc[:,2:].sample(3000) # Amostra de tamanho 3000 pois amostras maiores estouram a memória (pairwise distance é uma matriz grande)

	true_classes = df.iloc[:,-1].to_numpy(int)
	points = df.iloc[:,:-1].to_numpy(float)
	num_classes = 4

	datasets.append(("ROE",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)
	df.head()


	# ## Wine Quality (Red)
	# Link: https://archive.ics.uci.edu/ml/datasets/Wine+Quality
	# 
	# Tarefa: Prever a qualidade do vinho tinto (Variável categórica de 0 a 10)

	# In[12]:


	df = pd.read_csv("./datasets/winequality-red.csv",delimiter=';')

	true_classes = df['quality'].to_numpy(int)
	points = df.iloc[:,:-1].to_numpy(float)

	num_classes = 11

	datasets.append(("WQ",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)
	df.head()


	# ## Spambase
	# Link: https://archive.ics.uci.edu/ml/datasets/Spambase
	# 
	# Tarefa: Prever se o email é ou não spam.

	# In[13]:


	df = pd.read_csv("./datasets/spambase.data",header=None).sample(3000)

	true_classes = df.iloc[:,-1].to_numpy(int)
	points = df.iloc[:,:-1].to_numpy(float)

	num_classes = 2

	datasets.append(("SB",points,true_classes,num_classes))
	print(points.shape,true_classes.shape)
	df.head()


	# # Tests

	# In[14]:


	ilocs = []
	for (dataset_name,points,true_classes,num_classes) in datasets:
	    print(f"Dataset: {dataset_name}")
	    print(f"Points shape:{points.shape}")
	    print(f"Number of clusters(classes):{num_classes}")
	    
	    model_rand_scores = []
	    sklearn_rand_scores = []
	    
	    model_silhouette_scores = []
	    sklearn_silhouette_scores = []
	    
	    model_cluster_radiuses = []
	    sklearn_cluster_radiuses = []
	    
	    model_runtimes = []
	    sklearn_runtimes = []
	    
	    for p in [1,2]:
	        pairwise_dist = pairwise_minkowski_distance(points,p)
	        for seed in [*range(1,31)]:
	            start = time.time()
	            model_clusters,point_classes = approx_k_clustering(points,num_classes,p,seed)
	            model_runtimes.append(time.time() - start)
	            
	            model_rand_scores.append(rand_score(true_classes,point_classes))
	            model_silhouette_scores.append(silhouette_score(pairwise_dist,point_classes))
	            model_cluster_radiuses.append(compute_cluster_radius(points,model_clusters,p))
	            
	            kmeans = KMeans(num_classes,random_state=seed)
	            start = time.time()
	            kmeans.fit(points)
	            sklearn_runtimes.append(time.time() - start)

	            sklearn_rand_scores.append(rand_score(true_classes,kmeans.labels_))
	            sklearn_silhouette_scores.append(silhouette_score(pairwise_dist,kmeans.labels_))
	            sklearn_cluster_radiuses.append(compute_cluster_radius(points,[center for center in kmeans.cluster_centers_],p))

	        curr_iloc = [dataset_name,num_classes,len(true_classes),points.shape[1],p,np.mean(model_rand_scores),np.std(model_rand_scores),np.mean(model_silhouette_scores),np.std(model_silhouette_scores),
	                    np.mean(model_cluster_radiuses),np.std(model_cluster_radiuses),np.mean(model_runtimes),np.std(model_runtimes),
	                     np.mean(sklearn_rand_scores),np.std(sklearn_rand_scores),np.mean(sklearn_silhouette_scores),
	                     np.std(sklearn_silhouette_scores),np.mean(sklearn_cluster_radiuses),np.std(sklearn_cluster_radiuses),np.mean(sklearn_runtimes),np.std(sklearn_runtimes)]
	        ilocs.append(curr_iloc)
	    

	    




	# In[15]:


	table_cols = ['Dataset','K','No. Instances','No. Dimensions','Distance Factor (P)',
	              'Model: Rand Score Mean','Model: Rand Score Std','Model: Silhouette Mean','Model: Silhouette Std',
	              'Model: Cluster Radius Mean','Model: Cluster Radius Std','Model: Runtime Mean','Model: Runtime Std',
	              'KMeans: Rand Score Mean','KMeans: Rand Score Std','KMeans: Silhouette Mean','KMeans: Silhouette Std',
	              'KMeans: Cluster Radius Mean','KMeans: Cluster Radius Std','KMeans: Runtime Mean','KMeans: Runtime Std']


	final_table = pd.DataFrame(ilocs,columns=table_cols)


	# In[16]:


	final_table.to_csv("Resultados_Experimentos.csv")