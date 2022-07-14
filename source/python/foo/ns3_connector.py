 
import jinja2
import subprocess
import pathlib
import os
def ns3(G):
    """
    Function that makes a .txt of the position of the nodes, for using with ns3
    
    
    """
    archivo_aps=open("../ns3/positions_aps.txt","w")
    archivo_aps.write(jinja2.Template(open("../templates/plantilla_aps_pos.txt").read()).render(nodes = {k: G.nodes[k] for k in G.nodes})) 
    archivo_aps.close()


    archivo_stas=open("../ns3/positions_stas.txt","w")
    archivo_stas.write(jinja2.Template(open("../templates/plantilla_stas_pos.txt").read()).render(nodes = {k: G.nodes[k] for k in G.nodes})) 
    archivo_stas.close()


    archivo_stas=open("../ns3/conexiones_ns3.txt","w")

    archivo_stas.write(jinja2.Template(open("../templates/plantilla_conex_ns3.txt").read()).render(nodes = {k: G.nodes[k] for k in G.nodes})) 
    archivo_stas.close()

def runns3(waf_path="/home/h/ns3/ns-allinone-3.30/ns-3.30/",sim_file="scratch/prueba",num_aps=1,num_stas=1,simtime=5):

  subprocess.Popen("./waf --run '%s' %d %d %d "%(sim_file,num_aps,num_stas,simtime),shell=True, cwd=waf_path)

