

package sim.simulations;

import sim.UdpBasicApp2;

/////////////

import inet.common.MessageDispatcher;
import inet.common.lifecycle.NodeStatus;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.contract.IMacForwardingTable;
import inet.linklayer.contract.IMacRelayUnit;
import inet.linklayer.contract.IWirelessInterface;
import inet.linklayer.ethernet.contract.IEthernetLayer;
import inet.mobility.contract.IMobility;
import inet.networklayer.common.InterfaceTable;

////////////



import inet.physicallayer.wireless.common.pathloss.TwoRayGroundReflection;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.ManetRouter;
import inet.physicallayer.wireless.apsk.bitlevel.ApskLayeredScalarRadioMedium;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.physicallayer.wireless.noise.NoiseSource;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.visualizer.common.IntegratedVisualizer;
import inet.node.inet.WirelessHost;
import inet.node.inet.Router;
import inet.applications.udpapp.UdpBasicApp;
import inet.environment.common.PhysicalEnvironment;
import inet.environment.common.PhysicalEnvironment;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.node.wireless.AccessPoint;
import inet.node.aodv.AodvRouter;
import inet.linklayer.ieee80211.mac.Ieee80211Mac;
import inet.visualizer.common.IntegratedVisualizer;
//module AccessPoint extends AdhocHost
module AccessPoint2 extends ManetRouter
{
    
    parameters:
        numWlanInterfaces=default(1);
    	wlan[*].mgmt.typename = default("Ieee80211MgmtAp");
    	numApps=default(1);
    	hasUdp=default(true);
    	app[*].typename = default("UdpBasicApp2");
    	app[*].localPort = default(1000);
		app[*].destPort = default(1000);
		app[*].messageLength = default(1000B);
		app[*].sendInterval = default(1s);
 	
}


module AccessPoint3 extends WirelessHost
{
        parameters:
        @networkNode();
        @labels(node,ethernet-node,wireless-node);
        @display("i=device/accesspoint");
        numApps=1;
    	hasUdp=true;
    	app[*].typename = "UdpBasicApp2";
    	app[*].localPort = 1000;
		app[*].destPort = 1000;
		app[*].messageLength = 1000B;
		app[*].sendInterval = 1s;
        numWlanInterfaces = default(1);               // the number of radios in the access point
        hasStatus = default(false);
        fcsMode @enum("declared","computed") = default("declared");
        wlan[*].protocol = default("ethernetmac");
        wlan[*].mgmt.typename = default("Ieee80211MgmtAp");
        
        wlan[*].llc.typename = default("Ieee80211Portal");
        wlan[*].agent.typename = default("");
        wlan[*].radio.antenna.mobilityModule = default("^.^.^.mobility");
        eth[*].encap.typename = "";
        *.macTableModule = default(absPath(".macTable"));
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        bridging.hasStp = false;
        *.fcsMode = this.fcsMode;
        ethernet.registerProtocol = default(true);
    gates:
         radioIn[numWlanInterfaces] @directIn;
         ethg[] @labels(EtherFrame-conn);
    submodules:
       
        macTable: <default("MacForwardingTable")> like IMacForwardingTable {
            @display("p=100,300;is=s");
        }

      
    
 	
}

module WirelessHost2 extends WirelessHost{
wlan[*].mgmt.typename = default("Ieee80211MgmtSta");


}


network SimNetwork
{
    parameters:
        @display("bgb=901.5187,376.65");
      
      //  @statistic[receptionPower](source="receptionMinSignalPower(radioMedium.signalArrivalStarted)"; record=vector);
        
    submodules:
        
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=64.256,180.72");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=62.247997,287.14398");
        }
      
       
    AP[26] :AccessPoint2 {@display("i=device/accesspoint;is=s");}
    STA[200] :WirelessHost2 {}


}