import sys
import foo.draw as dr
import foo.graphs as gr
import foo.ns3_connector as ns
from foo.class_node import *  

from foo.omnet_connector  import *  



def main():

 simt=5
 n_sta=100
 n_ap=26
 G=gr.open_graph("900")
 ns.ns3(G)
 #ns.runns3(num_aps=26,num_stas=n_sta,simtime=simt)
 OmNet(G,"/home/h/h/ometpp/sim/simulations/",simtime=simt,num_sta_override=n_sta)
 RunOmNet()
 #read_results()


if __name__ == "__main__":
     main()
