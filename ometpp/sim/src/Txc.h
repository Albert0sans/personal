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

#ifndef __SIM_TCX_H
#define __SIM_TCX_H

#include <omnetpp.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <vector>
#include <chrono>
#include <stdio.h>
#include "inet/applications/udpapp/UdpBasicApp.h"



#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/physicallayer/wireless/common/base/packetlevel/ReceiverBase.h"

using namespace inet;

using namespace std;
using namespace omnetpp;
using namespace physicallayer;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
int starttime=0;
namespace sim {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
void call_gnuplot(string file="file");
void gnuplot_text(FILE * newfile,const char* title=" ",bool hist = false,bool start= true);
class  ReceiverBase2 : public ReceiverBase
{
public:
   ~ ReceiverBase2();

};




class  UdpBasicApp2 : public UdpBasicApp {

void sendPacket();
void socketDataArrived(UdpSocket *socket, Packet *packet);
};








}; // namespace

#endif








