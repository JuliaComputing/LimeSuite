/*
 * File:   FairwavesXTRX.cpp
 * Author: Julia Computing
 */
#include "FairwavesXTRX.h"
#include "FPGA_common.h"

namespace lime
{

LMS7_FairwavesXTRX::LMS7_FairwavesXTRX(lime::IConnection* conn, LMS7_Device *obj) : LMS7_Device(obj)
{
    tx_channels.resize(GetNumChannels());
    rx_channels.resize(GetNumChannels());

    while (obj && lms_list.size() > 1)
    {
        delete lms_list.back();
        lms_list.pop_back();
    }
    // TODO: do we want to keep the LimeSDR's FPGA base (FPGA_Common)
    //       or should we rewrite that functionality from scratch?
    //fpga = new lime::FPGA();
    //fpga->SetConnection(conn);
    // TODO: implement this for the XTRX's LiteX gateware
    //double refClk = fpga->DetectRefClk(100.6e6);
    double refClk = 26e6;
    this->lms_list[0]->SetConnection(conn);
    //mStreamers.push_back(new lime::Streamer(fpga,lms_list[0],0));
    lms_list[0]->SetReferenceClk_SX(false, refClk);
    connection = conn;
}

}
