from random import random
import matplotlib.pyplot as plt
import numpy as np
import os
import random
import networkx as nx



def network_2D(G,planta=0,dist=0,alpha=0.3):

 
 """
   Draws a 2D graph of a networkX graph G floor

   Draws all edges and nodes with diferents styles, based on inputs

Args:
       G (graph): NetworkX Graph to be drawn
       planta(int) : can choose floor to be drawn
       dist (bool) : if "1" distance is drawn in edges
       alpha (float) : edges and nodes lines 
       

Returns:
        None
   
   """
   

 nodes_floor = {k: G.nodes[k]["floor"] for k in G.nodes}


 Planta=[] # subgrafo de todos los elementos de una planta
 APS=[]
 STAS=[]


 for k in nodes_floor.items():
  if(k[1])==planta:
     Planta.append(k[0])

 if not Planta:
  print("El grafo no contiene la planta %d",planta)   
  return     
 PG = G.subgraph(Planta)


 nodes = {k: PG.nodes[k]["type"] for k in PG.nodes}
 for k in nodes.items():
  if(k[1])=="AP":
     APS.append(k[0])  
  if(k[1])=="STA":
     STAS.append(k[0])  
 
 AG = G.subgraph(APS)
 SG = G.subgraph(STAS)
 pos={}
 for i in PG.nodes:
  
  x=nx.get_node_attributes(G,'pos')[i][0]
  y=nx.get_node_attributes(G,'pos')[i][1]
  pos[i]=  (x,y)

 

 nx.draw_networkx(AG,pos=pos,edgelist=list(AG.edges()),node_shape="^",node_color="r",with_labels=True,alpha=alpha)
 
 nx.draw_networkx(SG,pos=pos,edgelist=list(AG.edges()),node_shape="o",with_labels=True,node_size=30,alpha=alpha)
 if(dist):
  edge_labels = nx.get_edge_attributes(G,'dist')
  for i in edge_labels:
   edge_labels[i]=round(edge_labels[i])
  
 nx.draw_networkx_edges(SG, pos,style="dashed",alpha=alpha/2 )
 nx.draw_networkx_edges(G, pos,edgelist=G.edges()-SG.edges(),alpha=alpha/2 )
 if dist:
  nx.draw_networkx_edge_labels(G, pos,edge_labels = edge_labels,font_size=2)
 
 plt.title('Planta %d' % planta)
 plt.show()
 
 

def network_3D(grafo,video):

    """
     Draws a 3D graph and optionally makes a video of the graph 
     
     rotating based on flag
     "video", if active the video will be located in same folder and named "output.mp4"
     temporary folder "Video" is used for storing the images that make up the video.

    
   
Args:
      grafo (Graph) : NetworkX graph to be drawn
      video (bool) : If "1" a video is make of the graph from diferent angles, temporary folder "Videos" is used

Returns:
        None
   
   """
  

    pos = nx.get_node_attributes(grafo, 'pos') 
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
       
    
        # Loop on the pos dictionary to extract the x,y,z coordinates of each node
  
    
    #Plot de los edges
     
    
    x1=[]
    y2=[]
    z3=[]
    for key, value in pos.items():
     x1.append(value[0])
     y2.append(value[1])
     z3.append(value[2])
    ax.scatter(x1, y2, z3,  edgecolors='k', alpha=0.4)  
    x1=[]
    y2=[]
    z3=[]
    for i,j in enumerate(grafo.edges()):

            x = np.array((pos[j[0]][0], pos[j[1]][0]))
            y = np.array((pos[j[0]][1], pos[j[1]][1]))
            z = np.array((pos[j[0]][2], pos[j[1]][2]))
        
        # Plot the connecting lines
            ax.plot(x, y, z, c='black', alpha=0.1)
    os.system('mkdir ../Video/')     
    os.system('mkdir ../tmp/')    
    os.system('rm ../Video/image-*')

    if video:
     j=30   
     sube=1
     
     """
     Iterates for 360 degrees and saves images to temporary folder
     uses ffmpeg to make the video and deletes al images
     """
     for i in range(360):  
          if (j==30) :  
             sube=-1
          if (j==0) :
             sube=1  
          j=j+sube/2   
          ax.view_init(j,i)
          
          plt.savefig("../tmp/image-%d.png"% i)
     file=  ("../Video/output%d.mp4" % (random.randint(0,1024)))
     os.system('ffmpeg -framerate 25 -i ../tmp/image-%d.png ' + file)
     os.system('rm ../tmp/image-*')
    else:
          plt.show()     
     
    
    return
