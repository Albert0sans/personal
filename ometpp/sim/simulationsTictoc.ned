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
import inet.applications.udpapp.UdpBasicApp;
import inet.environment.common.PhysicalEnvironment;
import inet.environment.common.PhysicalEnvironment;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;

import inet.visualizer.common.IntegratedVisualizer;
module AccessPoint extends ManetRouter
{
    
    parameters:
        numWlanInterfaces=1;
    	wlan[*].mgmt.typename = default("Ieee80211MgmtAp");
    	numApps=1;
    	hasUdp=true;
    	app[*].typename = "UdpSink";
    	app[*].localPort = 1000;

 
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
      
       
    AP[26] :AccessPoint {@display("i=device/accesspoint;is=s");}
    STA[200] :WirelessHost {}


}