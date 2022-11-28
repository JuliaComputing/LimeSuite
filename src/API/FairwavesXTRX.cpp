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
    const std::vector<regVal> initVals = {
        {0x0092, 0x0D15}, {0x0093, 0x01B1}, {0x00A6, 0x000F}, {0x0085, 0x0019},
        {0x0081, 0x000F},

        {0x0086, 0x4101}, {0x010F, 0x0042}, {0x011C, 0x8D41}, {0x011F, 0x3780},
        {0x0120, 0xCCC0}, {0x0122, 0x2514}, // 0x011F might not be necessary
    };

/*
    const std::vector<regVal> initVals = {
        {0x0022, 0x0FFF}, {0x0023, 0x5550}, {0x002B, 0x0038}, {0x002C, 0x0000},
        {0x002D, 0x0641}, {0x0086, 0x4101}, {0x0087, 0x5555}, {0x0088, 0x03F0},
        {0x0089, 0x1078}, {0x008B, 0x2100}, {0x008C, 0x267B}, {0x00A1, 0x656A},
        {0x00A6, 0x0009}, {0x00A7, 0x8A8A}, {0x00A9, 0x8000}, {0x00AC, 0x2000},
        {0x0105, 0x0011}, {0x0108, 0x218C}, {0x0109, 0x6100}, {0x010A, 0x1F4C},
        {0x010B, 0x0001}, {0x010C, 0x8865}, {0x010E, 0x0000}, {0x010F, 0x3142},
        {0x0110, 0x2B14}, {0x0111, 0x0000}, {0x0112, 0x942E}, {0x0113, 0x03C2},
        {0x0114, 0x00D0}, {0x0117, 0x1230}, {0x0119, 0x18D2}, {0x011C, 0x8941},
        {0x011D, 0x0000}, {0x011E, 0x0740}, {0x0120, 0xC5C0}, {0x0121, 0x8650},
        {0x0123, 0x000F}, {0x0200, 0x00E1}, {0x0208, 0x017B}, {0x020B, 0x4000},
        {0x020C, 0x8000}, {0x0400, 0x8081}, {0x0404, 0x0006}, {0x040B, 0x1020},
        {0x040C, 0x00FB}
    };
*/

    for (unsigned i = 0; i < lms_list.size(); i++)
    {
        lime::LMS7002M* lms = lms_list[i];
/*
        if (lms->ResetChip() != 0)
            return -1;
*/
        if (Reset(lms) != 0)
            return -1;

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);
        for (auto i : initVals) {
            int val = lms->SPI_write(i.adr, i.val, true);
        }

//        std::this_thread::sleep_for(std::chrono::microseconds(500));

        // Settings copied from
        // https://github.com/xtrx-sdr/liblms7002m/blob/master/liblms7002m.c#L98
//        EnableLDO(lms);

//        lms->Modify_SPI_Reg_bits(LMS7param(EN_OUT2_XBUF_TX), 1);
//        lms->Modify_SPI_Reg_bits(LMS7param(EN_TBUFIN_XBUF_RX), 1);


        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        EnableChannel(true, 2*i, false);

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 2);

        for (auto i : initVals)
            if (i.adr >= 0x100)
                lms->SPI_write(i.adr, i.val, true);

        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        //lms->SPI_write(0x0123, 0x000F);  //SXT
        //lms->SPI_write(0x0120, 0xE6C0);  //SXT
        //lms->SPI_write(0x011C, 0x8941);  //SXT

        EnableChannel(false, 2*i+1, false);
        EnableChannel(true, 2*i+1, false);

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);

        if(SetFrequency(true, 2 * i, GetFrequency(true, 2 * i)) != 0)
            return -1;
        if(SetFrequency(false, 2 * i, GetFrequency(false, 2 * i)) != 0)
            return -1;

    }
    if (SetRate(10e6, 2) != 0)
        return -1;
    return 0;
}


}
