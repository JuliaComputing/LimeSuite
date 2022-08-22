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



int LMS7_FairwavesXTRX::Init()
{
    for (unsigned i = 0; i < lms_list.size(); i++)
    {
        lime::LMS7002M* lms = lms_list[i];
        if (lms->ResetChip() != 0)
            return -1;

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);

        // XTRX Specific Config
        lms->Modify_SPI_Reg_bits(LMS7param(EN_OUT2_XBUF_TX),1);
        lms->Modify_SPI_Reg_bits(LMS7param(EN_TBUFIN_XBUF_RX),1);

        lms->SPI_write(0x0092, 0xFFFF, true); // default 0x0001
        lms->SPI_write(0x0093, 0x03FF, true); // default 0x0000

        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        EnableChannel(true, 2*i, false);
        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 2);

        if(lms->CalibrateTxGain(0,nullptr) != 0)
            return -1;

        EnableChannel(false, 2*i+1, false);
        EnableChannel(true, 2*i+1, false);

        lms->Modify_SPI_Reg_bits(LMS7param(MAC), 1);

        if(SetFrequency(true,2*i,GetFrequency(true,2*i))!=0)
            return -1;
        if(SetFrequency(false,2*i,GetFrequency(false,2*i))!=0)
            return -1;
    }

    if (SetRate(10e6,2)!=0)
        return -1;
    return 0;
}


}
