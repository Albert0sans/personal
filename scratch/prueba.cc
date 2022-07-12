
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <algorithm>
#include "ns3/simulator.h"
#include <tuple>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/position-allocator.h"
#include "ns3/rng-seed-manager.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <vector>
#include "ns3/stats-module.h"
#include "ns3/snr-tag.h"
#include <chrono>

#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"

#include <fstream>

#include "ns3/gnuplot.h"

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
using namespace ns3;
using namespace std;

struct ValueInTime
{
    float time;//time in s
    int value; // value
    int value2; // value
};

struct histogram
{
    double x;
    int times;
    
   
   
   bool operator==(const histogram& h) const
	{
		return (x == h.x);
	} 
    bool operator==(const double h) const
	{
		return (x == h);
	} 


};
 Gnuplot2dDataset dataset_snr;
 vector<histogram> hist_snr;
 vector<ValueInTime> Lost_vec;
 Gnuplot2dDataset dataset_num;

uint32_t RecvPkts=0,TotalPkts=0;
 uint32_t simuTime =36000; //segundos
 auto t_ini = high_resolution_clock::now();

void Create2DPlotFile ( Gnuplot2dDataset dataset,std::string name="SNR",bool h=0,std::string folder = "resultados/plots/")
{
  
  std::string graphicsFileName        = folder + name + ".png";
  std::string plotFileName            = folder + name + ".plt";


  // Instantiate the plot and set its title.
  Gnuplot plot (graphicsFileName);
  plot.SetTitle (name);

  // Make the graphics file, which the plot file will create when it
  // is used with Gnuplot, be a PNG file.
 // plot.SetTerminal ("png");

  // Set the labels for each axis.
  plot.SetLegend ("X Values", "Y Values");

  // Set the range for the x axis.
  plot.AppendExtra ("set xrange [0:*]");

  // Instantiate the dataset, set its title, and make the points be
  // plotted along with connecting lines.

  dataset.SetTitle (name);
  if (h)
  {

     dataset.SetStyle (Gnuplot2dDataset::HISTEPS);

  }
  else
     dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

  plot.AppendExtra ("set boxwidth 0.5");
  plot.AppendExtra ("set style fill solid");
  // Create the 2-D dataset.


  // Add the dataset to the plot.
  plot.AddDataset (dataset);

  // Open the plot file.
  std::ofstream plotFile (plotFileName.c_str());

  // Write the plot file.
  plot.GenerateOutput (plotFile);
  string cmd= "gnuplot ";
  cmd=cmd+plotFileName;

 system(cmd.c_str());
  // Close the plot file.
  plotFile.close ();
  char rm_cmd[40];
  sprintf(rm_cmd,"rm %s*.plt",folder.c_str());
  system(rm_cmd);

}

static const uint32_t packetSize = 1420;

vector<float> snr;
static int received_pks=1;
void PhyCallback (std::string path, Ptr<const Packet> packet)
{
   
     
    static int rcv=0,snr_meas=0;
    static auto this_time=Simulator::Now(). GetSeconds() ;

    static int last_time;
    histogram h;
    int t=10; // frequency divisor

    this_time=Simulator::Now(). GetSeconds() ;
   
    SnrTag tag;
    packet->PeekPacketTag(tag);
    auto i=std::find(hist_snr.begin(), (hist_snr.end()), tag.Get());
    const bool found = (i != hist_snr.end()) ;
    auto idx = i - hist_snr.begin();

    if(!(found))
    {
        h.x=tag.Get();
        h.times=1;
        hist_snr.push_back(h);
        
    }
    else{
hist_snr[idx].times+=1;


    }
         // count received pkts each second //
        snr_meas+=tag.Get();

        if(last_time!=int(this_time*t)){
        // estimated end time

        auto t2 = high_resolution_clock::now();
        auto tiempo = duration_cast<milliseconds>(t2-t_ini);
        int total=0;

        total= (tiempo.count()/1000)/this_time*simuTime;
        int days,seconds, hours, minutes;

        minutes = total / 60;
        seconds = total % 60;
        hours = (minutes / 60);
        days= hours /24;
        hours = (minutes / 60) % 24;
        minutes = minutes % 60;
        cout << "Estimated Simulation Time =  " <<days <<" d " <<hours << " h " << minutes << " m " << seconds << " s.\n" ;
        dataset_num.Add(this_time,rcv);
        dataset_snr.Add(this_time,snr_meas/this_time);
        rcv=0;

    }
  //  
    RecvPkts+=1;
    received_pks+=1;
    rcv+=1;

    last_time=int(this_time*t);
}

 

void GetDropPer( )
// Get droped paquets each t miliseconds
{
    uint32_t t=100;
    ValueInTime aux;
    aux.time= Simulator::Now().GetSeconds();
    aux.value= RecvPkts;
    aux.value2= TotalPkts;
    Lost_vec.push_back(aux);
  Simulator::Schedule(MilliSeconds (t), &GetDropPer);

  RecvPkts=0;
  TotalPkts=0;
}


void
Recv_Trace(std::string context,Ptr<const Packet> p)
{
RecvPkts+=1;
}

void
Send_Trace(std::string context,Ptr<const Packet> p)
{
TotalPkts+=1;
}

MobilityHelper SetMobility(const char* file)
{

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> lp =CreateObject<ListPositionAllocator>();


   float x,y,z;
   
   
    FILE * newfile;
    newfile=fopen(file,"r");
 
    
      while((fscanf (newfile, "%f,%f,%f", &x,&y,&z))!=EOF){

           lp->Add(Vector (x,y,z));

      }
     fclose(newfile);
   
  //node1
    mobility.SetPositionAllocator(lp);

    return mobility;


}
YansWifiPhyHelper CreatePhy()
{


    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();

    //asignamos a nuestra interfaz física el canal, de esta forma todos los dispositivos que usen esta interfaz podrán comunicarse e interferirse

    phy.SetChannel (channel.Create ());

    return phy;

}

std::tuple <NodeContainer,NetDeviceContainer >CreateSTA(vector<int> listaAPs ,YansWifiPhyHelper phy)
{
    NodeContainer STA;

    uint32_t n_stas=listaAPs.size();
    STA.Create (n_stas);
    WifiMacHelper mac;
    NetDeviceContainer staDevices;
    char ap_conn[7];
    for(uint32_t i=0;i<n_stas;i++)
    {
        sprintf(ap_conn, "AP%d", listaAPs[i]);

   
        Ssid ssid = Ssid (ap_conn);
   
        
        //definimos el estándar a utilizar, que por defecto es 802.11ax

        WifiHelper wifi;

        //instalamos wifi en los dispositivos STA, desactivando las tramas beacon, para que estos dispositivos esperen recibirlas

        wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
        wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate54Mbps"));
        mac.SetType ("ns3::StaWifiMac",
        "Ssid", SsidValue (ssid),
        "ActiveProbing", BooleanValue (false));

        staDevices.Add (wifi.Install (phy, mac, STA.Get(i)));
        
    }

    return  std::make_tuple(STA,staDevices);
}

std::tuple <NodeContainer,NetDeviceContainer > CreateAP(uint32_t n_aps,YansWifiPhyHelper phy)
{

    WifiMacHelper mac;
    NodeContainer AP;
    AP.Create (n_aps);
    NetDeviceContainer apDevices;
    char ap_conn[7];

    for(uint32_t i=0;i<n_aps;i++)
    {
        sprintf(ap_conn, "AP%d", i);
        Ssid ssid = Ssid (ap_conn);

        //definimos el estándar a utilizar, que por defecto es 802.11ax

        WifiHelper wifi;

        wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
        wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate54Mbps"));
        mac.SetType ("ns3::ApWifiMac",
        "Ssid", SsidValue (ssid));

        apDevices.Add (wifi.Install (phy, mac, AP.Get(i)));
     
    }
    return  std::make_tuple(AP,apDevices);
}

int main (int argc, char *argv[])
{
//uint32_t port=9;
auto t1 = high_resolution_clock::now();

ns3::RngSeedManager::SetSeed(2);


vector<int> listaAPs;
 
uint32_t n_stas=200;
uint32_t n_aps=26;

///

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> lp =CreateObject<ListPositionAllocator>();


 FILE * newfile;
    newfile=fopen("./scratch/conexiones_ns3.txt","rb");
    if (newfile == NULL) {
    perror("Failed: ");
    return 1;
}

 
     int x;
     uint32_t i=0;

     while((fscanf (newfile, "%d\n", &x))!=EOF)
     {
      
            if(i>=n_stas)
              break;
            listaAPs.push_back(x);
              i++;
      }



YansWifiPhyHelper phy = CreatePhy();

NodeContainer STA;
NetDeviceContainer staDevices;
NodeContainer AP;
NetDeviceContainer apDevices;

std::tie(STA,staDevices) = CreateSTA(listaAPs,phy);
std::tie(AP,apDevices) = CreateAP(n_aps,phy);

GetDropPer();
// definir posiciones

MobilityHelper mobility_aps=SetMobility("./scratch/positions_aps.txt");
MobilityHelper mobility_sta=SetMobility("./scratch/positions_stas.txt");

mobility_sta.Install (STA);
mobility_aps.Install (AP);



//INSTALAMOS LOS PROTOCOLOS

InternetStackHelper stack;
stack.Install (STA);
stack.Install (AP);



// DEFINIMOS ips
Ipv4AddressHelper address;
address.SetBase ("10.0.0.0", "255.0.0.0");



Ipv4InterfaceContainer staNodeInterface;
Ipv4InterfaceContainer apNodeInterface;

staNodeInterface = address.Assign (staDevices);
apNodeInterface = address.Assign (apDevices);
//INSTALAMOS APLICACION DE TRAFICO en APS


uint32_t nNodes;
/// SINK application in APs
ApplicationContainer apps_sink;
 nNodes = apDevices.GetN ();
for (uint32_t i = 0; i < nNodes; ++i)

   {
    PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
    apps_sink.Add (sink.Install (AP.Get (i)));
    

   }   


 
// source application in all stas

ApplicationContainer apps_source;
 nNodes = staDevices.GetN ();
  
for (uint32_t i = 0; i < nNodes; ++i)

   {
   
     BulkSendHelper source  ("ns3::TcpSocketFactory", InetSocketAddress (apNodeInterface.GetAddress (listaAPs[i]), 9));
     source.SetAttribute ("MaxBytes", UintegerValue (0));
     apps_source.Add(source.Install (STA.Get (i)));


   }   
cout<<"Simulation Running"<<endl;
    apps_sink.Start (Seconds (0));
    apps_sink.Stop (Seconds (simuTime*0.9));
    apps_source.Start (Seconds (0.5));
    apps_source.Stop (Seconds (simuTime*0.8));

   



phy.EnablePcap ("resultados/capturas/STA", staDevices.Get (0));

Config::Connect("/NodeList/*/DeviceList/*/Mac/MacTx",
MakeCallback(&Send_Trace));
Config::Connect ("/NodeList/*/$ns3::PointToPointNetDevice/Mac/MacRx",
MakeCallback (&Recv_Trace));

 Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxEnd",
                   MakeCallback (&PhyCallback));

Simulator::Stop (Seconds (simuTime));






FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();




  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {

           Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
     
           cout<< "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
           cout<< "  Tx Bytes:   " << i->second.txBytes << "\n";
           cout<< "  Rx Bytes:   " << i->second.rxBytes << "\n";
           cout<< "  Throughput to  i->first: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds ()) / 1024 / 1024  << " Mbps\n";
           cout<< "  Mean delay:   " << i->second.delaySum.GetSeconds () / i->second.rxPackets << "\n";
           cout<< "  Mean jitter:   " << i->second.jitterSum.GetSeconds () / (i->second.rxPackets - 1) << "\n";
           cout<< "  Mean jitter:   " << i->second.txPackets << i->second.lostPackets  << "\n";

    }



Create2DPlotFile(dataset_snr,"SNR");

// HIST //
 Gnuplot2dDataset dataset_hist;
 
for (uint32_t i = 0; i < hist_snr.size(); ++i)
{
      dataset_hist.Add(hist_snr[i].x,hist_snr[i].times);
     
}
 Gnuplot2dDataset dataset_lost_count,dataset_lost_per,dataset_recv_count,dataset_recv_per;

for (uint32_t i = 0; i < Lost_vec.size(); ++i)
{
       dataset_lost_count.Add(Lost_vec[i].time,Lost_vec[i].value-Lost_vec[i].value2);
       double per=(Lost_vec[i].value2);
       per=per/(Lost_vec[i].value+1);
      
       dataset_recv_per.Add(Lost_vec[i].time,(per*100));
       dataset_lost_per.Add(Lost_vec[i].time,1-(per));
       dataset_recv_count.Add(Lost_vec[i].time,Lost_vec[i].value2);
}
Create2DPlotFile(dataset_recv_count,"Number-of-recv-pkts");
Create2DPlotFile(dataset_lost_count,"Number-of-lost-pkt");
Create2DPlotFile(dataset_lost_per,"Percentaje-of-lost-pkt");
Create2DPlotFile(dataset_recv_per,"Percentaje-of-received-pkt");
Create2DPlotFile(dataset_hist,"SNR_Histogram", 1);
Create2DPlotFile(dataset_num,"Total-Number-of-Packets");

 Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();
Simulator::Destroy ();

auto t2 = high_resolution_clock::now();

auto tiempo = duration_cast<milliseconds>(t2 - t1);

cout<< "Simulation ended in "<<(float(tiempo.count())/1000)<<" seconds"<<endl;

return 0;


}
