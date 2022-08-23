/*--------------------------------------------------------------------------
LMS7002_REGx51.h (Ver 1.0)

Header file for the LIME MCU chip.
Copyright (c) 2013 LIME, Inc.  All rights reserved.
--------------------------------------------------------------------------*/

/*------------------------------------------------
Make sure that this file gets included only once.
------------------------------------------------*/
#ifndef __LMS7002_H__
#define __LMS7002_H__

#include <compiler.h>

/*------------------------------------------------
Byte Registers
------------------------------------------------*/
SFR(P0, 0x80);
SFR(SP    , 0x81);
SFR(DPL0  , 0x82); 
SFR(DPH0  , 0x83); 
SFR(DPL1  , 0x84); 
SFR(DPH1  , 0x85);
SFR(DPS   , 0x86); 
SFR(PCON  , 0x87); 
SFR(TCON  , 0x88); 
SFR(TMOD  , 0x89);
SFR(TL0   , 0x8A);
SFR(TL1   , 0x8B);
SFR(TH0   , 0x8C);
SFR(TH1   , 0x8D);
SFR(PMSR  , 0x8E); 
SFR(P1    , 0x90); 
SFR(DIR1	, 0x91); 
SFR(SCON  , 0x98);
SFR(SBUF  , 0x99);
SFR(P2    , 0xA0);
SFR(DIR2	, 0xA1); 
SFR(DIR0	, 0xA2); 
SFR(IEN0    , 0xA8); 
SFR(IEN1	, 0xA9); 
SFR(EECTRL	, 0xB0); 
SFR(EEDATA	, 0xB1); 
SFR(IP0     , 0xB8); 
SFR(IP1   , 0xB9); 
SFR(USR2	, 0xBF); 
SFR(IRCON	, 0xC0); 
SFR(T2CON	, 0xC8); 
SFR(RCAP2L	, 0xCA); 
SFR(RCAP2H	, 0xCB); 
SFR(TL2 , 0xCC); 
SFR(TH2 , 0xCD); 
SFR(PSW   , 0xD0);
SFR(ACC   , 0xE0);
SFR(REG0	, 0xEC); 
SFR(REG1	, 0XED); 
SFR(REG2	, 0xEE); 
SFR(REG3	, 0xEF); 
SFR(B     , 0xF0); 
SFR(REG4	, 0xF4); 
SFR(REG5	, 0xF5); 
SFR(REG6	, 0xF6); 
SFR(REG7	, 0xF7); 
SFR(REG8	, 0xFC); 
SFR(REG9	, 0xFD); 

/*------------------------------------------------
P0 bits 
------------------------------------------------*/
SBIT(P0_0, 0x80, 0x0);
SBIT(P0_1, 0x80, 0x1);
SBIT(P0_2, 0x80, 0x2);
SBIT(P0_3, 0x80, 0x3);
SBIT(P0_4, 0x80, 0x4);
SBIT(P0_5, 0x80, 0x5);
SBIT(P0_6, 0x80, 0x6);
SBIT(P0_7, 0x80, 0x7);

/*------------------------------------------------
TCON bits
------------------------------------------------*/
SBIT(IT0, 0x80, 0x8);
SBIT(IE0, 0x80, 0x9);
SBIT(IT1, 0x80, 0xA);
SBIT(IE1, 0x80, 0xB);
SBIT(TR0, 0x80, 0xC);
SBIT(TF0, 0x80, 0xD);
SBIT(TR1, 0x80, 0xE);
SBIT(TF1, 0x80, 0xF);

/*------------------------------------------------
P1 bits
------------------------------------------------*/
SBIT(P1_0, 0x90, 0x0);
SBIT(P1_1, 0x90, 0x1);
SBIT(P1_2, 0x90, 0x2);
SBIT(P1_3, 0x90, 0x3);
SBIT(P1_4, 0x90, 0x4);
SBIT(P1_5, 0x90, 0x5);
SBIT(P1_6, 0x90, 0x6);
SBIT(P1_7, 0x90, 0x7);

/*------------------------------------------------
SCON bits
------------------------------------------------*/
SBIT(RI , 0x90, 0x8);
SBIT(TI , 0x90, 0x9);
SBIT(RB8, 0x90, 0xA);
SBIT(TB8, 0x90, 0xB);
SBIT(REN, 0x90, 0xC);
SBIT(SM2, 0x90, 0xD);
SBIT(SM1, 0x90, 0xE);
SBIT(SM0, 0x90, 0xF);

/*------------------------------------------------
P2 bits
------------------------------------------------*/
SBIT(P2_0, 0xA0, 0x0);
SBIT(P2_1, 0xA0, 0x1);
SBIT(P2_2, 0xA0, 0x2);
SBIT(P2_3, 0xA0, 0x3);
/// reserved for SPI
SBIT(ucSCLK , 0xA0 , 0x0);
SBIT(ucSEN  , 0xA0 , 0x1);
SBIT(ucSDIN , 0xA0 , 0x2);
SBIT(ucSDOUT, 0xA0 , 0x3);


/*------------------------------------------------
IEN0 bits
------------------------------------------------*/
SBIT(EX0, 0xA0 ,0x8);       /* 1=Enable External interrupt 0 */
SBIT(ET0, 0xA0 ,0x9);       /* 1=Enable Timer 0 interrupt */
SBIT(EX1, 0xA0 ,0xA);       /* 1=Enable External interrupt 1 */
SBIT(ET1, 0xA0 ,0xB);       /* 1=Enable Timer 1 interrupt */
SBIT(ES0, 0xA0 ,0xC);       /* 1=Enable Serial port 0 interrupt */
SBIT(ET2, 0xA0 ,0xD);		/* 1=Enable Timer 2 interrupt */
//sbit XX   = 0xAE; /* Don't Care */
SBIT(EA, 0xA0, 0xF);       /* 0=Disable all interrupts */

/*------------------------------------------------
EECTRL bits
------------------------------------------------*/
SBIT(CMD0  , 0xB0, 0x0);       /* Command 0 bit */
SBIT(CMD1  , 0xB0, 0x1);       /* Command 1 bit */
SBIT(CMD2  , 0xB0, 0x2);       /* Command 2 bit */
SBIT(CMD3  , 0xB0, 0x3);       /* Command 3 bit */
SBIT(TX_ACK, 0xB0, 0x4);       /* 1= ACK Transmited to EEPROM */
SBIT(RX_ACK, 0xB0, 0x5);	  /* 1= ACK Received from EEPROM */
SBIT(BUSY  , 0xB0, 0x6);	  /* 1= Serial Data BUS is busy */
SBIT(ERROR , 0xB0, 0x7);       /* 1= Illegal Command Received */

/*------------------------------------------------
IP0 bits
------------------------------------------------*/
SBIT(PX0, 0xB0, 0x8); /* External iterrupt 0 priority bit */
SBIT(PT0, 0xB0, 0x9); /* Timer 0 iterrupt priority bit */
SBIT(PX1, 0xB0, 0xA); /* External iterrupt 1 priority bit */
SBIT(PT1, 0xB0, 0xB); /* Timer 1 iterrupt priority bit */
SBIT(PS0, 0xB0, 0xC); /* Serial port 0 iterrupt priority bit */
SBIT(PT2, 0xB0, 0xD); /* Timer 2 iterrupt priority bit */
//sbit XX   = 0xBE; /* Don't Care */
//sbit XX   = 0xBF; /* Don't Care */

/*------------------------------------------------
IRCON bits
------------------------------------------------*/
//sbit XX   = 0xCO; /* Don't Care */
//sbit XX   = 0xC1; /* Don't Care */
SBIT(IE2, 0xC0, 0x2); /* External iterrupt 2 flag bit */
SBIT(IE3, 0xC0, 0x3); /* External iterrupt 3 flag bit */
SBIT(IE4, 0xC0, 0x4); /* External iterrupt 4 flag bit */
SBIT(IE5, 0xC0, 0x5); /* External iterrupt 5 flag bit */
//sbit XX   = 0xC6; /* Don't Care */
//sbit XX   = 0xC7; /* Don't Care */

/*------------------------------------------------
T2CON bits
------------------------------------------------*/
SBIT(CP_RL2, 0xC0, 0x8); /* Capture/Reload Flag */
SBIT(C_T2  , 0xC0, 0x9); /* 0/1 - Timer/Counter select */
SBIT(TR2   , 0xC0, 0xA); /* 0/1 - Stop/Strat Timer */
SBIT(EXEN2 , 0xC0, 0xB); /* Timer 2 enable Flag */
SBIT(TCLK  , 0xC0, 0xC); /* Transmit Clock Flag */
SBIT(RCLK  , 0xC0, 0xD); /* Receive Clock Flag */
SBIT(EXF2  , 0xC0, 0xE); /* Timer 2 External Flag */
SBIT(TF2   , 0xC0, 0xF); /* Timer 2 Overflow Flag */ 

/*------------------------------------------------
PSW bits
------------------------------------------------*/
SBIT(P  , 0xD0, 0x0); /* Parity Flag */
//SBIT(XX	, 0xD1; /* Don't Care */
SBIT(OV , 0xD0, 0x2); /* Overflow Flag */
SBIT(RS0, 0xD0, 0x3); /* Register Bank Select 0 */
SBIT(RS1, 0xD0, 0x4); /* Register Bank Select 0 */
//SBIT(XX , 0xD5; /* Don't Care */
SBIT(AC , 0xD0, 0x6); /* Auxiliary Carry Flag */
SBIT(CY , 0xD0, 0x7); /* Carry Flag */


/*------------------------------------------------
TMOD Bit Values - Defines the bit position in the reg!!!
------------------------------------------------*/
#define T0_M0_   0x01 /* 0000 0001 = T0_M0 */
#define T0_M1_   0x02 /* 0000 0010 = T0_M1 */
#define T0_CT_   0x04 /* 0000 0100 = T0_CT */
#define T0_GATE_ 0x08 /* 0000 1000 = T0_GATE */
#define T1_M0_   0x10 /* 0001 0000 = T1_M0 */
#define T1_M1_   0x20 /* 0010 0000 = T1_M1 */
#define T1_CT_   0x40 /* 0100 0000 = T1_CT */
#define T1_GATE_ 0x80 /* 1000 0000 = T1_GATE */

#define T1_MASK_ 0xF0
#define T0_MASK_ 0x0F

/*------------------------------------------------
PCON Bit Values - Defines the bit position in the reg!!!
------------------------------------------------*/
#define IDL_    0x01
#define STOP_   0x02
#define SMOD_   0x80

/*------------------------------------------------
PMSR Bit Values - Defines the bit position in the reg!!!
------------------------------------------------*/
#define SEL_DIV_0_ 0x01
#define SEL_DIV_1_ 0x02
#define SEL_DIV_2_ 0x04

/*------------------------------------------------
Interrupt Vectors:
Interrupt Address = (Number * 8) + 3 
------------------------------------------------*/

#define IE0_VECTOR      0  /* 0x03 External Interrupt 0 */
#define TF0_VECTOR      1  /* 0x0B Timer 0 */
#define IE1_VECTOR      2  /* 0x13 External Interrupt 1 */
#define TF1_VECTOR      3  /* 0x1B Timer 1 */
#define SIO_VECTOR	4  /* 0x23 Serial channel 0 interrupt! (RI0/TI0) */
#define TF2_VECTOR      5  /* 0x2B Timer 2 */
//  SKIPPED #define X	6  /* RESERVED*/
#define IEX2_VECTOR	7    /* 0x3B External interrupt 2 */
#define IEX3_VECTOR	8    /* 0x43 External interrupt 3 */
#define IEX4_VECTOR	9    /* 0x4B External interrupt 4 */
#define IEX5_VECTOR	10   /* 0x53 External interrupt 5 */

/*------------------------------------------------
------------------------------------------------*/
#endif