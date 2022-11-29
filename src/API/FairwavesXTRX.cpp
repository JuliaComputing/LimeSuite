/*
 * File:   FairwavesXTRX.cpp
 * Author: Julia Computing
 */
#include "FairwavesXTRX.h"
#include "FPGA_common.h"
#include <chrono>
#include <thread>
#include "Logger.h"

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

int LMS7_FairwavesXTRX::Reset(lime::LMS7002M* lms)
{
    lms->SPI_write(0x0020, 0x0);
    lms->SPI_write(0x0020, 0xFFFF); // Channel AB
    lms->SPI_write(0x002E, 0x0); //must write
    return 0;
}

int LMS7_FairwavesXTRX::Init()
{

    struct regVal
    {
        uint16_t adr;
        uint16_t val;
    };

    // Settings copied from
    // https://github.com/xtrx-sdr/liblms7002m/blob/master/liblms7002m.c#L98
    const std::vector<regVal> initVals = {
        {0x0092, 0x0D15}, {0x0093, 0x01B1}, {0x00A6, 0x000F}, {0x0085, 0x0019},
        {0x0081, 0x000F},

        {0x0086, 0x4101}, {0x010F, 0x0042}, {0x011C, 0x8D41}, {0x011F, 0x3780},
        {0x0120, 0xCCC0}, {0x0122, 0x2514}, // 0x011F might not be necessary
    };

    for (unsigned i = 0; i < lms_list.size(); i++)
    {
        lime::LMS7002M* lms = lms_list[i];
        if (Reset(lms) != 0)
            return -1;

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);
        for (auto j : initVals) {
            int val = lms->SPI_write(j.adr, j.val, true);
        }

        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        EnableChannel(true, 2*i, false);

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 2);

        for (auto j : initVals)
            if (j.adr >= 0x100)
                lms->SPI_write(j.adr, j.val, true);

        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        EnableChannel(false, 2*i+1, false);
        EnableChannel(true, 2*i+1, false);

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);

        if(SetFrequency(true, 2 * i, GetFrequency(true, 2 * i)) != 0)
            return -1;
        if(SetFrequency(false, 2 * i, GetFrequency(false, 2 * i)) != 0)
            return -1;

    }
    if (SetRate(10e6, 1) != 0)
        return -1;
    return 0;
}


}
