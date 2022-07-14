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

using namespace inet;

using namespace std;
using namespace omnetpp;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
int starttime=1;
struct ValueInTime
{
    double time=0;//time in s
    double value=0; // value

};

struct histogram
{
    double x=0;
    int times=0;



   bool operator==(const histogram& h) const
    {
        return (x == h.x);cout<<"recv"<<endl;
    }
    bool operator==(const double h) const
    {
        return (x == h);
    }
};
vector<ValueInTime> dataset_snr;
vector<histogram> hist_snr;
void call_gnuplot(string file="file");

void gnuplot_text(FILE * newfile,const char* title=" ",bool hist = false,bool start= true);
namespace sim {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */

//class  ReceiverBase2 : public ReceiverBase
//{
//public:
//   ~ ReceiverBase2();
//    const inet::physicallayer::IReceptionResult *computeReceptionResult(const IListening *listening, const inet::physicallayer::IReception *reception, const IInterference *interference, const ISnir *snir, const std::vector<const inet::physicallayer::IReceptionDecision *> *decisions) const;
//
//};




class  UdpBasicApp2 : public UdpBasicApp {
public:
    UdpBasicApp2();
    ~ UdpBasicApp2();
void sendPacket();
void socketDataArrived(UdpSocket *socket, Packet *packet);
};

}; // namespace

#endif








