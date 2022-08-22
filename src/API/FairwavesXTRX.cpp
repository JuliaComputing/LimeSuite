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

void EnableLDO(lime::LMS7002M* lms, const bool enable) {
    const int val = enable?1:0;

    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIG), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIGGN), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIGSXR), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIGSXT), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIVGN), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIVSXR), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_DIVSXT), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_LNA12), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_LNA14), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_MXRFE), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_RBB), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_RXBUF), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TBB), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TIA12), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TIA14), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_G_LDO), val);

    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_AFE), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_CPGN), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_CPSXR), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TLOB), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TPAD), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_TXBUF), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_VCOGN), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_VCOSXR), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_VCOSXT), val);
    lms->Modify_SPI_Reg_bits(LMS7param(EN_LDO_CPSXT), val);

    lms->Modify_SPI_Reg_bits(LMS7param(EN_LOADIMP_LDO_TLOB), 1);
    lms->Modify_SPI_Reg_bits(LMS7param(PD_LDO_DIGIp1), val);
    lms->Modify_SPI_Reg_bits(LMS7param(PD_LDO_DIGIp2), val);
    lms->Modify_SPI_Reg_bits(LMS7param(PD_LDO_SPIBUF), val);

    return;
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

        EnableLDO(lms, true);

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
