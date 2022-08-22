/*
 * File:   FairwavesXTRX.h
 * Author: Julia Computing
 */

#ifndef FAIRWAVESXTRX_H
#define	FAIRWAVESXTRX_H
#include "LmsGeneric.h"

namespace lime
{

class LMS7_FairwavesXTRX : public LMS7_Device
{
public:
    LMS7_FairwavesXTRX(lime::IConnection* conn, LMS7_Device *obj = nullptr);
    int Init() override;
};

}

#endif	/* FAIRWAVESXTRX_H */

