 
import jinja2
import subprocess
import sys
import pandas as pd
import numpy as np
import re
import bisect
import pickle
import matplotlib.pyplot as plt


def OmNet(G,path="/home/h/h/ometpp/sim/simulations/",simtime=10,num_sta_override=40,num_ap=26): 

 """
 Generates a templates of omnetpp network

 Uses jinja2 to populate the templates located in templates folder based on
 graph G network, and saves them in path

 Args:
    G(Graph): NetworkX graph
    num_sta(int) : override parameter for number of STA, must be greater
                    than number of Acces Points
    path(string) : string of path for the files
 
 
 """
 
 num_ap=0

 for i in G.nodes:
    if G.nodes[i]['type'] == 'AP':
        num_ap+=1
      
 for i in G.nodes:
    num_sta=i
 
 if (num_sta_override>0):
    num_sta=num_sta_override



 archivo_ned=open(path+"Tictoc.ned","w")
 archivo_ned.write(jinja2.Template(open("../templates/plantilla_red.txt").read()).render(num_ap=num_ap,num_sta=num_sta) )
 archivo_ned.close()


 archivo_ned=open(path+"omnetpp.ini","w")
 archivo_ned.write(jinja2.Template(open("../templates/plantilla_ini.txt").read()).render(simtime=simtime,num_ap=num_ap,num_sta=num_sta,nodes = {k: G.nodes[k] for k in G.nodes})) 
 archivo_ned.close()
 
def RunOmNet(omnet_path="/home/h/Downloads/omnetpp-6.0-linux-x86_64/omnetpp-6.0/bin",
 simulation_path="/home/h/h/ometpp/sim",inet_path="/home/h/h/ometpp/inet4.4"): 
    
  
  print("Exporting PATH...") 
  subprocess.run("export PATH=$PATH:%s" %omnet_path,shell=True)
  
  print("Compiling Sources...")
  subprocess.run("make -C %s/src MODE=release -j16 all" % (simulation_path),shell=True)
  print("Running Simulations...")
  
  r= ("%s/src/sim -r 0 -m -u Cmdenv -c General -n" % (simulation_path)+
      " %s/simulations:%s/src:%s/examples" %(simulation_path,simulation_path,inet_path) +
      ":%s/showcases:%s/src" %(inet_path,inet_path)+
      ":%s/tests/validation:%s/tests/networks"  %(inet_path,inet_path) +
      ":%s/tutorials" %(inet_path)+
      " -x inet.common.selfdoc,inet.linklayer.configurator.gatescheduling.z3,"+
      "inet.emulation,inet.showcases.visualizer.osg,"+
      "inet.examples.emulation,inet.showcases.emulation,"+
      "inet.transportlayer.tcp_lwip,inet.applications.voipstream,"+
      "inet.visualizer.osg,"+
      "inet.examples.voipstream"+
      " --image-path=%s/images" %(inet_path)+
      "  -l %s/src/INET %s/simulations/omnetpp.ini" % (inet_path,simulation_path))
  print(r)
  subprocess.run(r, shell=True)   



def parse_if_number(s):
    try: return float(s)
    except: return True if s=="true" else False if s=="false" else s if s else None

def parse_ndarray(s):
    return np.fromstring(s, sep=' ') if s else None


def export_results(simulation_path="/home/h/h/ometpp/prueba",destination_path="../results/",omnet_path="$HOME/Downloads/omnetpp-6.0-linux-x86_64/omnetpp-6.0"):

    print("Converting to CSV vector ")
    subprocess.run("%s/bin/opp_scavetool x %s/src/results/*.vec -o %s/resultados_vec.csv" %(omnet_path,simulation_path,destination_path) ,shell=True, executable='/bin/bash')
    print("Converting to CSV attr ")
    subprocess.run("%s/bin/opp_scavetool x %s/src/results/*.sca -o %s/resultados_sca.csv" %(omnet_path,simulation_path,destination_path) ,shell=True, executable='/bin/bash')

    print("Reading CSV")
    data_csv_vec=pd.read_csv("%s/resultados_vec.csv" % (destination_path),
    converters = {
    'attrvalue': parse_if_number,
    'binedges': parse_ndarray,
    'binvalues': parse_ndarray,
    'vectime': parse_ndarray,
    'vecvalue': parse_ndarray}
    
    )



    data_csv_sca=pd.read_csv("%s/resultados_sca.csv" % (destination_path),
    converters = {
    'attrvalue': parse_if_number,
    'binedges': parse_ndarray,
    'binvalues': parse_ndarray,
    'vectime': parse_ndarray,
    'vecvalue': parse_ndarray}
    
    )



    archivo_sca = open("%s/resultados_sca.pk" % destination_path,"wb")
    archivo_vec = open("%s/resultados_vec.pk" % destination_path,"wb")    
    pickle.dump(data_csv_sca,archivo_sca)
    pickle.dump(data_csv_vec,archivo_vec)
    archivo_sca.close()
    archivo_vec.close()


def insertOrderedTuple(ordered_list,tuple_list,value_to_cmp):
     for i in tuple_list:
          index = bisect.bisect_left(ordered_list, i)
          if index >= len(ordered_list): # value must be inserted
               ordered_list.append(i)
          else:
               index=index-1
               
               if ordered_list[index][value_to_cmp]==i[value_to_cmp]: #media de los valores

                    mean_val=(ordered_list[index][value_to_cmp],(i[1-value_to_cmp]+ordered_list[index][1-value_to_cmp])/2)
                    if value_to_cmp == 1:
                         mean_val=mean_val[::-1]
                   
                    ordered_list[index]=mean_val
                 

               else:
                    bisect.insort(ordered_list,i)
     return ordered_list

def read_results(simulation_path="/home/h/h/ometpp/sim/simulations/",destination_path="../results/",omnet_path="$HOME/Downloads/omnetpp-6.0-linux-x86_64/omnetpp-6.0"):
    """
    https://docs.omnetpp.org/tutorials/pandas/index-src/#3-getting-your-simulation-results-into-python
    Funcion that reads results from simulation
    
    
    
    
    """
    
    archivo_sca = open("%s/resultados_sca.pk" % destination_path,"rb")
    archivo_vec = open("%s/resultados_vec.pk" % destination_path,"rb")    
    data_csv_sca=pickle.load(archivo_sca)
    data_csv_vec=pickle.load(archivo_vec)
    archivo_sca.close()
    archivo_vec.close()

    histograms = data_csv_sca[data_csv_sca.type=='histogram']
    vectors = data_csv_vec[data_csv_vec.type=='vector']
    print("--Vectores--")
    k=[]
    d={}
    for i in vectors.iloc:
        print(i["name"],i["module"])
    estadisticos=["packetReceived:vector(packetBytes)","endToEndDelay:vector","packetReceivedFromPeerWithRetry:vector(packetBytes)","packetSentToPeerWithRetry:vector(packetBytes)","packetSentToUpper:vector(packetBytes)","throughput:vector"]
    for i in vectors.iloc:
       # if i["name"] in estadisticos:  
            try:
                d[i["name"]]=insertOrderedTuple(d[i["name"]],(list(zip(i["vectime"],i["vecvalue"]))),1)
            except:
                d[i["name"]]=list(zip(i["vectime"],i["vecvalue"]))
        

    for i in d:

        
        val=list(map(list, zip(*d[i])))
        plt.plot(val[0],val[1])
        plt.title(i)
        plt.show()
    k=list(set(k))
    print(k)
    print("--fin Vectores--")
  #  print("escalars",scalars.columns)
  #  for name in scalars.name:
   #  print(name)
   
  #  print("histogramas",histograms.columns)
  #  for name in histograms.name:
   #  print(name)


    aux={}

    key2=0

    num_aps=0
    num_sta=0

    ap_array=[]
    sta_array=[]
    ap_attr=[]
    sta_attr=[]
    for i in histograms.iloc:
        
        x = re.search("[a-zA-z]*.(\w*)\[+([0-9]*)",i.head()["module"])
        id=x.groups()[0]
 
        if id=="STA":    
            sta_attr.append(i["name"][:-10])   
            sta_array=np.append(sta_array,i["mean"])
        elif id=="AP":
            ap_attr.append(i["name"][:-10])
           # print(i["name"])
           # print(ap_attr)
            ap_array=np.append(ap_array,i["mean"])
       
        key=(id+"["+ x.groups()[1]+"]")
       
        try:     
            aux[key]=1+aux[key]
        except:
             aux[key]=0
       
        if id == "STA":
                num_sta=num_sta+1
               
        elif id=="AP":
               
                num_aps=num_aps+1
            

 
    ap_attr=ap_attr[0:len(list(set(ap_attr)))+1]

    sta_attr=sta_attr[0:len(list(set(sta_attr)))+1]

  
    ap_array=np.reshape(ap_array,(int(num_aps/(aux[list(aux.keys())[0]]+1)),aux[list(aux.keys())[0]]+1))
    
    sta_array=np.reshape(sta_array,(int(num_sta/(aux[list(aux.keys())[-1]]+1)),aux[list(aux.keys())[-1]]+1))

    j=0
    l=[]

    print("Atributos de los AP")
    for i in ap_attr:
        print(i," ",j)
        j=j+1
    j=0
    print("Atributos de los STA")
    for i in sta_attr:
        print(i," ",j)
        j=j+1
    j=0           
    
    print("\n\n ---- Statistics of Access Points \n\n")
    for i in ap_array.T:
        plt.close()
        plt.plot(i)
        l.append(i.mean())
        try:
            print(j," ",(ap_attr)[j],np.nanmean(i),np.nanstd(i))
            plt.title((ap_attr)[j])
            plt.savefig("../images/aps/"+(ap_attr)[j])
        except:
            break
        j=j+1   

    j=0
    m=[]

    print("\n\n ---- Statistics of Hosts \n\n")
    for i in sta_array.T:
        plt.close()
        plt.plot(i)
        m.append(i.mean())
        try:
            print(j," ",(sta_attr)[j],np.nanmean(i),np.nanstd(i))
            plt.title((sta_attr)[j])
            plt.savefig("../images/hosts/"+(sta_attr)[j])
        except:
            break

        j=j+1   
        
