import pickle
import random
from foo.class_node import *  
import os
import networkx as nx
import sys

def open_graph(file):
 """
 Opens a file and unpicles it

Args:
       param2 (str): Name of the file to open.

Returns:
        Any: returns the unpickled objet
      
"""


 archivo = open("../Graphs/Graphs/%s_0.pk" % file,"rb")
 print("Archivo %s_0.pk, abierto" %file)
 grafo = pickle.load(archivo,encoding='latin1')
 archivo.close()
 return grafo

def Generar_Grafo( num_hosts=2000,
 num_aps=25,
 lowRange=0,
 HighRange=2000,
 floor=4,floor_sep=1.5):
 
 """
 Generates a Random NetworkX Graph based on input arguments os sys inputs

Args:
       num_hosts (int): number of Hosts to be generated
       num_aps (int): number of APs to be generated
       lowRange, HignhRange (float) = Range for valid APs and Hosts placements
       floor (int) = Max of the building
       floor_sep (float) = separation between floors

Returns:
        Graph: returns the NetworkX generated graph
      
"""

     
 G = nx.Graph()
 if len(sys.argv)>1:
  num_hosts=int(sys.argv[1])
 if(len(sys.argv)>2): 
       num_aps=int(sys.argv[2])
 if(len(sys.argv)>4): 
       lowRange=int(sys.argv[3])
       HighRange=int(sys.argv[4])
 if(len(sys.argv)>5):
       floor = int(sys.arg[5]) 

 aps=list(range(num_aps))
 hosts=list(range(num_aps,num_aps + num_hosts))
 APS=list(range(num_aps))

 for i in aps:
  """ Generates num_aps APS in random position and floor""" 

  num_planta=random.randint(0,floor) 
  APS[i]=Ap(random.uniform(lowRange, HighRange),random.uniform(lowRange, HighRange),floor_sep*(num_planta*1),G,i,num_planta)




 for i in hosts:
  """ Generates num_hosts Hosts connected to random routers and located in random position and floor
      and connects the selected router to the Host
  """
   
  R=random.choice(aps)  
  APS[R].Union(G,i,1)
  num_planta=random.randint(0,floor)     
  Host(G,random.uniform(lowRange, HighRange),random.uniform(lowRange, HighRange),floor_sep*(num_planta*1),i,R,num_planta)
 


 return G


def Save_Graph(G,name="output"):
 
 """Function that pickles a selected Graph and saves it

 Args:
       G (Graph) : NetworkX Graph to store
       name (str) : Name of the file to write

Returns:
        None

"""

 archivo = open("../Graphs/Graphs/%s" % name,"wb")
 print("Archivo %s, creado" %name)
 pickle.dump(G,archivo)
 archivo.close()
