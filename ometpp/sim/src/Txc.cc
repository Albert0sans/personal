//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Txc.h"
#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "inet/physicallayer/wireless/common/contract/packetlevel/SignalTag_m.h"
//#include "inet/common/ProtocolTag_m.h"
//#include "inet/physicallayer/wireless/common/base/packetlevel/NarrowbandNoiseBase.h"
//#include "inet/physicallayer/wireless/common/contract/packetlevel/IRadio.h"
//#include "inet/physicallayer/wireless/common/contract/packetlevel/IRadioMedium.h"
//
//#include "inet/physicallayer/wireless/common/radio/packetlevel/ReceptionDecision.h"
//#include "inet/physicallayer/wireless/common/radio/packetlevel/ReceptionResult.h"
//#include "inet/physicallayer/wireless/common/signal/Interference.h"





void gnuplot_text(FILE * newfile,const char* title,bool hist,bool start)
{
char buffer[1024];

                   sprintf(buffer, "./plot_%s", title);
                   newfile=fopen(buffer,"w");
                   fputs("set terminal png \n", newfile);
                   sprintf(buffer, "set output '%s'\n", title);
                   fputs(buffer, newfile);
                   sprintf(buffer, "set title '%s'\n", title);
                   fputs(buffer, newfile);
                   fputs("set xlabel 'X Values' \n" , newfile);
                   fputs("set ylabel 'Y Values' \n", newfile);
                   fputs("set xrange [0:*] \n", newfile);
                   fputs("set boxwidth 0.5 \n", newfile);
                   fputs("set style fill solid \n", newfile);
                   if(hist){

                       sprintf(buffer, "  plot '-'  title 's%' with histeps \n", title);
                   }
                   else
                       sprintf(buffer, "  plot '-'  title 's%' with linespoints \n", title);
                   fputs(buffer, newfile);



               fclose(newfile);


}
void call_gnuplot(string file)
{
    string cmd= "gnuplot ";
    cmd=cmd+file;

   system(cmd.c_str());

}
int flag_once=1;

namespace sim {

Define_Module(UdpBasicApp2);



UdpBasicApp2::UdpBasicApp2(){
FILE * snrfile,*snrhistfile;

      gnuplot_text(snrfile,"SNR");
      gnuplot_text(snrhistfile,"SNR_histogram",true);


}

UdpBasicApp2::~ UdpBasicApp2(){

    FILE * snrfile2,*snrhistfile2;
    cout<<"destructor"<<flag_once<<endl;
    if(flag_once){
    snrfile2=fopen("./plot_SNR","a");

    snrhistfile2=fopen("./plot_SNR_histogram","a");

    char buffer[1024];
    for(auto i : dataset_snr){
    //    cout<<"i"<<i.value<<endl;
        sprintf(buffer, "%lf %lf \n",i.time,i.value);

        fputs(buffer, snrfile2);
    }
    fputs("e \n",snrfile2);
    //


    for(int i=0;i<hist_snr.size();i++){

         sprintf(buffer,"%lf %d \n",hist_snr[i].x,hist_snr[i].times);

         fputs(buffer, snrhistfile2);
     }
    fputs("e \n",snrhistfile2);
     //

    fclose(snrfile2);
     fclose(snrhistfile2);
     //
     call_gnuplot("plot_SNR");
     call_gnuplot("plot_SNR_histogram");

     //
    flag_once=0;
    }
}

void UdpBasicApp2::sendPacket()
{

    //randomly decides wether or not to send msq
    //srand( (unsigned)time( NULL ) );
    float r = ((double) rand() / (RAND_MAX)) ;

    if(r<0.2)
    {
        std::ostringstream str;
        str << packetName << "-foohjhkjbar-" << numSent;
        Packet *packet = new Packet(str.str().c_str());
        if (dontFragment)
            packet->addTag<FragmentationReq>()->setDontFragment(true);
        const auto& payload = makeShared<ApplicationPacket>();
        payload->setChunkLength(B(par("messageLength")));
        payload->setSequenceNumber(numSent);
        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        packet->insertAtBack(payload);
        L3Address destAddr = chooseDestAddr();
        emit(packetSentSignal, packet);
        socket.sendTo(packet, destAddr, destPort);
        //cout<<"numSent()"<<numSent<<" "<<simTime()<<endl;
        numSent++;
    }
}

void UdpBasicApp2::socketDataArrived(UdpSocket *socket, Packet *packet)
{




    double snir_log = 10*log10(packet->getTag<SnirInd>()->getMaximumSnir());
    double snir_norm = packet->getTag<SnirInd>()->getMaximumSnir();
    static int recv=0;

        static double snr_meas=0;
         static auto this_time=simTime();

         static auto last_time=simTime();
         histogram h;
         static double t=0; // frequency divisor

         auto i=std::find(hist_snr.begin(), (hist_snr.end()),snir_log);
         const bool found = (i != hist_snr.end()) ;
         auto idx = i - hist_snr.begin();
       //  cout<<"recv "<<snir_log<<"   "<<this_time<<endl;
         this_time=simTime();



             if(!(found))
               {

                   h.x+=snir_log;
                   h.times=1;

                   hist_snr.push_back(h);

               }
               else{
                   hist_snr[idx].times+=1;


                 }


             snr_meas+=snir_norm;
                  // cout<<"this"<<this_time<<"last"<<recv<<endl;
                 //   if(this_time>(last_time+t)){

                        ValueInTime aux;
                       // snr_meas=(snr_meas)/(this_time-last_time);
                        aux.value=10*log10(snr_meas);

                        aux.time=1/(1/this_time);
                        dataset_snr.push_back(aux);
                           snr_meas=0;


                          last_time=this_time;

               // }
              //


                    recv=recv+1;


    processPacket(packet);
}

}; // namespace
