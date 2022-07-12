import math
import random
import networkx
class Node:
     def __init__(self,xpos,ypos,zpos,id,floor) :

          """
          
          init instance of base class "Node", inputs are position (x,y,z), and identifier
          of the object, and the floor in wich the node is located.
          the position must be positive, if any parameter is necative a random position
          is asigned to the node based on the non negative parameters
          
          Args:
              xpos, ypos, xpos (float) : Position of object Node
              id(int): identifier of node object
              floor(int): floor of the building in wich the node is located
          """
         
          if (xpos<0) or (ypos<0) or (zpos<0) :
              pos=[xpos,ypos,zpos]
              pos = list(filter( lambda x: x>=0, pos))
              lowRange=min(pos)
              HighRange=max(pos)
              xpos=round(random.uniform(lowRange, HighRange),4)
              ypos=round(random.uniform(lowRange, HighRange),4)
              zpos=round(random.uniform(lowRange, HighRange),4)         
          
        
          self.pos=(xpos,ypos,zpos)
          self.floor=floor
          self.id=id

     def distance(self, node):

        """
             Calculates the distance between 2 nodes in 
             if pos is not found KetError exception y catched and distance 0 is returned
             Args:         
                    node(Node): NetworkX Node object to wich to calculate the distances

             Returns:

                  Calculated distance       
        
        """ 
          #gets the distance between the self node and other.
        try:
          return(math.sqrt(pow(self.pos[0]-node["pos"][0],2)+pow(self.pos[1]-node["pos"][1],2)+pow(self.pos[2]-node["pos"][2],2)))
        except KeyError:
          return 0     
     def changePos(self,xpos,ypos,zpos):
          #changes the position of the node
          self.pos=(xpos,ypos,zpos)
     def RandPos(self,lowRange,HighRange):
          #Places the node in a random position based on low and high ranges
           xpos=round(random.uniform(lowRange, HighRange),4)
           ypos=round(random.uniform(lowRange, HighRange),4)
           zpos=round(random.uniform(lowRange, HighRange),4)
           self.pos=(xpos,ypos,zpos)
         
           
     def interference(self,G):

         """
          Calculares all interferences with NetworkX graph G

          Args:
               G(Graph) : NetworkX Graph
          
          
          """
               
         try: 
         
            for j in G:
              
              if self.id != j:
               if(G.has_node(j)):
                 if 'pos' in G.nodes[j]:
                  if not(G.has_edge(self.id,j)):
                   G.add_edge(self.id,j,type='interference')
                  d=G[self.id][j]['dist']=self.distance(G.nodes[j])
               
                 
              
         except TypeError:
           pass      
                

class Host(Node):

     #class of Host objects, interited from Node base class, init method calls init of Node
     #sets the type o the Host as STA, adds it to the NetworkX graph G and calculates all the interfeneces
     # of the rest of comoponents in G and joins with signal links to attached APs.
     def __init__(self,G,xpos,ypos,zpos,id,attachedAPs,floor=0) :

          """Init method of Host class

          Calls init method of Node Class, initiates all atributes and calculates
          all interferences with the other Nodes

         Args: 
                  xpos, ypos, zpos (float) : Position of the Node
                  G (Graph) : NetworkX Graph
                  attachedAPs (Any) : Identifier of the APs to wich the node is connected 
                  Id of the object (Any) : Identifier of the node
                  floor(int): floor in wich the node is located
     """

          Node.__init__(self,xpos,ypos,zpos,id,floor)
          self.attachedAP=[]
          self.type="STA"
          G.add_node(id,pos=self.pos,type="STA",attachedAP=attachedAPs,floor=self.floor)
          self.interference(G)
         
    
     def Union(self, G, join: bool, Ap: int):
          """joins or removes an AP from list based on join flag

          joins an AP to the list of connected Access Points of the Node, giving the the atrribute
          signal and distance  an exception is given if there is only one AP in the list a cactched
          as TypeError. If join flag is set to 0 the AP is remove from G and the node.

         Args: 
                  G (Graph) : NetworkX graph
                  join (bool) : "1" indicates if the nodes is joined or removed
                  Ap (int) : identifier of Access point to wich be connected or removed, based on join flag
          """
          if(join):
           self.attachedAP.append(Ap)
           
           try:
            for i in Ap:
             if(i in G):
              
               G[self.id][i]['dist']=self.distance(G.nodes[i])
               G[self.id][i]['type']="signal"
             else:
               
               G.add_edge(self.id,i)
               G[self.id][i]['dist']=self.distance(G.nodes[i])
               G[self.id][i]['type']="signal"
           except TypeError:
               
                
                   G.add_edge(self.id,Ap) 
                   G[self.id][Ap]['dist']=self.distance(G.nodes[Ap])
                   G[self.id][Ap]['type']="signal"

               
          else: 
           try:
            for i in Ap:
             if(i in G):   
              self.attachedAP.remove(Ap)
              G.remove_edge(self.id, Ap)
           except ValueError:
            print("Ap %d is no longer connected to Host" % (Ap))

    
class Ap(Node):


    
     def __init__(self,xpos,ypos,zpos,G,id,floor=0) :

          """Init method of Ap class

          Calls init method of Node Class, initiates all atributes and calculates
          all interferences with the other Nodes

         Args: 
                  xpos, ypos, zpos (float) : Position of the Node
                  G (Graph) : NetworkX Graph
                  Id of the object (Any) : Identifier of the node
                  floor(int): floor in wich the node is located
     """

          Node.__init__(self,xpos,ypos,zpos,id,floor)
          self.id=id
          self.type="AP"
          self.listSTA=[]
          G.add_node(id,listSTA=self.listSTA,type="AP",pos=self.pos,floor=floor)
          self.interference(G)
          
          
     def Union(self,G,STA,join): 


           """joins or removes a Host from  STAlist of the AP based on join flag

          joins an Host to the list of connected Node of the  Access Points, giving the the atrribute
          signal and distance. If join flag is set to 0 the AP is remove from G and the node.

         Args: 
                  G (Graph) : NetworkX graph
                  join (bool) : "1" indicates if the nodes is joined or removed
                  STA (int) : identifier of Access point to wich be connected or removed, based on join flag
          """

           if(join):
            self.listSTA.append(STA)
            
             
            if(STA in G):
              if 'pos' in G.nodes[STA]:    
               G[self.id][STA]['dist']=self.distance(G.nodes[STA])
               G[self.id][STA]['type']="signal"
             
            
            else:
              G.add_edge(self.id,STA)
              G[self.id][STA]['dist']=self.distance(G.nodes[STA])
              G[self.id][STA]['type']="signal"
           else: 
            try:
             self.listSTA.remove(STA)
             G.remove_edge(self.id, STA)
            except ValueError:
             print("Ap %d is no longer connected to Host" % (STA))

