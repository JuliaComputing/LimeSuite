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
    /*
    const std::vector<regVal> initVals = {
        {0x0092, 0x0D15}, {0x0093, 0x01B1}, {0x00A6, 0x000F}, {0x0085, 0x0019},
        {0x0081, 0x000F},

        {0x0086, 0x4101}, {0x010F, 0x0042}, {0x011C, 0x8D41}, {0x011F, 0x3780},
        {0x0120, 0xCCC0}, {0x0122, 0x2514}, // 0x011F might not be necessary
    };
    */

    const std::vector<regVal> initVals = {
        {0x0022, 0x0FFF},
        {0x0023, 0x5550},
        {0x002B, 0x0038},
        {0x002C, 0x0000},
        {0x002D, 0x0641},
        {0x0081, 0x000F}, // from xtrx repo
        {0x0085, 0x0019}, // from xtrx repo
        {0x0086, 0x4101},
        {0x0087, 0x5555},
        {0x0088, 0x0780}, // lms_device: 0x0525, mini: 0x03F0, xtrx and default: 0x0780
        {0x0089, 0x0020}, // lms_device and mini: 0x1078, xtrx repo and default: 0x0020
        {0x008B, 0x1f00}, // lms_device: 0x218C, mini: 0x2100, xtrx and default: 0x1f00
        {0x008C, 0x267B}, // Comparator! Why does it need to be set? 067B is default upper 4 bits are readonly
        {0x0092, 0x0D15}, // from xtrx repo
        {0x0093, 0x01B1}, // from xtrx repo
        {0x00A1, 0x6565}, // This is default but also equal xtrx repo
        {0x00A4, 0x6565}, // This is default but also equal xtrx repo
        {0x00A6, 0x000F}, // Equal to XTRX repo =)
        {0x00A7, 0x6565}, // This is default but also equal xtrx repo
        {0x00A9, 0x8000}, // Signature?
        {0x00AC, 0x2000}, // Signature?
        {0x0108, 0x218C}, // Some default gain
        {0x0109, 0x57C1}, // Some default LPF bandwidth
        {0x010A, 0x104C}, // Adjusted according to xtrx repo, different from lms device
        {0x010B, 0x0001}, // Not in xtrx repo, default 0x0000, 0x0001 equal in lms_device and mini
        {0x010C, 0x8865}, // default 0x88FD, 0x8865 equal in lms_device and mini, xtrx repo: 0x88FF
        {0x010D, 0x011A}, // Only set in lms_device, xtrx repo ?
        {0x010E, 0x0000}, // DC offset set to zero, equal in lms_device and mini
        {0x010F, 0x0042}, // lms_device and mini : 0x3142, xtrx repo: 0x0042
        {0x0110, 0x2B14}, // Not set in xtrx repo, default: 0x0994, lms_device and mini : 0x2B14
        {0x0111, 0x0000}, // Not set in xtrx repo, default: 0x0083, lms_device and mini : 0x0000
        {0x0112, 0x000C}, // lms_device: 0x000C, mini: 0x942E, xtrx repo: is set manually
        {0x0113, 0x03C2}, // lms_device and mini : 0x03C2, xtrx repo unclear
        {0x0114, 0x00D0}, // lms_device: 0x01F0, mini: 0x00D0, xtrx repo only lower 5 bits fixed to 16
        {0x0115, 0x000D}, // lms_device: 0x000D, mini and default: 0x0009, xtrx repo: set manually
        {0x0118, 0x418C}, // lms_device: 0x418C, mini and default: 0x018c, xtrx repo: lower 8c are equal, rest is set manually
        {0x0119, 0x528b}, // lms_device: 0x5292, mini: 0x18D2, default: 0x18cb, xtrx repo: 0x528b
        {0x011A, 0x3001}, // lms_device: 0x3001 mini and default: 0x2e02, xtrx repo: set manually
        {0x011C, 0x8D41}, // lms_device and mini: 0x8941, xtrx repo: 0x8D41
        {0x011D, 0x0000}, // lms_device and mini: 0x0000, xtrx repo and default: 0x0400; is set manually
        {0x011E, 0x0984}, // lms_device: 0x0984, mini: 0x0740, will be set during tuning, why set it here?
        {0x011F, 0x3780}, // from xtrx repo
        {0x0120, 0xCCC0}, // lms_device: 0xE6C0, mini: 0xC5C0, xtrx repo: 0xCCC0
        {0x0121, 0x8404}, // lms_device: 0x3638, mini: 0x8650, xtrx repo and default: 0x8404
        {0x0122, 0x2514}, // lms_device: 0x0514, mini and default: 0x0514, xtrx repo: 0x2514
        {0x0123, 0x067b}, // Comperator, lms_device: 0x200F, mini: 0x000F, xtrx repo and default: 0x067b
        {0x0200, 0x00E1}, // lms_device and mini: 0x00E1, xtrx repo: ?
        {0x0208, 0x017B}, // lms_device and mini: 0x017B
        {0x020B, 0x4000}, // lms_device and mini: 0x4000
        {0x020C, 0x8000}, // lms_device and mini: 0x8000
        {0x0400, 0x8081}, // lms_device and mini: 0x8081
        {0x0404, 0x0006}, // lms_device and mini: 0x0006 number of samples for dc correction
        {0x040B, 0x1020}, // lms_device and mini: 0x1020
        {0x040C, 0x00FB}, // lms_device and mini: 0x00FB
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
