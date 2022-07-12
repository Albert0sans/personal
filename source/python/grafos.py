import sys
import foo.draw as dr
import foo.graphs as gr
import foo.ns3_connector as ns
from foo.class_node import *  

from foo.omnet_connector  import *  



def main():


 G=gr.open_graph("900")
 #ns.ns3(G)
 #ns.runns3()
 OmNet(G,"/home/h/h/ometpp/sim/simulations/",simtime=5,num_sta_override=26)
 RunOmNet()
 #read_results()


if __name__ == "__main__":
     main()
