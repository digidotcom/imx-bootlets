/*
 * iMX28 Boot Prep
 *
 * Copyright 2008-2010 Freescale Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <debug.h>
#include "regsclkctrl.h"
#include "regsemi.h"
#include "regsdram.h"
#include "regspower.h"
#include "regsuartdbg.h"
#include "regspinctrl.h"
#include "regsdigctl.h"
#include "regsocotp.h"
#include <stdarg.h>

/*
 *	Overview of MX28 clock generation
 *	(See RefMan Rev 1 Figure 10.2.2 "Logical Diagram of Clock Domains")
 *
 *	PLL0 generates 480Mhz base reference clock (ref_pll)
 *	=>	which is divided by a "fractional divider" to
 *		produce the subsystem reference clock (ref_<sub>)
 *		=>	which is then divided by the subsystem divider
 *			to produce the subsystem clock (CLK_<SUB>):
 *
 *	subsys		ref										subsys		subsys
 *	clock		pll			'fractional divider'		ref clk		divider
 *	========	===		  =========================		========	======================
 *	CLK_P	 := 480 * (18/HW_CLKCTRL_FRAC0.CPUFRAC ) [= ref_cpu ] / HW_CLKCTRL_CPU.DIV_CPU
 *	CLK_EMI	 := 480 * (18/HW_CLKCTRL_FRAC0.EMIFRAC ) [= ref_emi ] / HW_CLKCTRL_EMI.DIV_EMI
 *	CLK_SSPx := 480 * (18/HW_CLKCTRL_FRAC0.IOxFRAC ) [= ref_ioX ] / HW_CLKCTRL_SSPx.DIV
 *	CLK_GPMI := 480 * (18/HW_CLKCTRL_FRAC0.GPMIFRAC) [= ref_gpmi] / HW_CLKCTRL_GPMI.DIV
 *
 *	CLK_H	 := CLK_P / HW_CLKCTRL_HBUS.DIV
 *
 *	Additionally, each subsystem has a 'bypass' mode where the PLL is bypassed
 *	and the 24MHz ref_xtal is used as the subsystem clock source and then
 *	further divided by the subsystem DIV_XTAL field.
 *
 *	These defines select from the small subset of possible combinations
 *	which have been validated by Freescale (see IMX28CEC data sheet,
 *	table 15 "Recommended Operating States"). There are many other
 *	possible voltage/clocking combinations which we don't want to use
 *	since Freescale may not have tested them.
 *
 *						BO
 *	State	VDDD		VDDD 	CLK_P (div,frac)	CLK_H (div)		CLK_EMI (div,frac)
 *	-----	-----		-----	----------------	-----------		------------------
 *	 "64"	 tbd	 	tbd	 64    (5,27)		 64    (1)		130.91 (2,33)
 *	 "261"	1.350 (22)	1.250	261.81 (1,33)		130.91 (2)		130.91 (2,33)
 *	 "360"	1.350 (22)	1.250	360    (1,24)		120    (3)		130.91 (2,33)
 *	 "400"	1.450 (26)	1.350
 *	 "454"	1.550 (30)	1.450	454.73 (1,19)		151.57 (3)		205.71 (2,21)
 */

#define STATE_64_VDDD		tbd
#define STATE_64_CPU_FRAC	27
#define STATE_64_CPU_DIV	5
#define STATE_64_HBUS_DIV	1
#define STATE_64_EMI_FRAC	33
#define STATE_64_EMI_DIV	2

#define STATE_261_VDDD		22
#define STATE_261_CPU_FRAC	33
#define STATE_261_CPU_DIV	1
#define STATE_261_HBUS_DIV	2
#define STATE_261_EMI_FRAC	33
#define STATE_261_EMI_DIV	2

#define STATE_360_VDDD		22
#define STATE_360_CPU_FRAC	24
#define STATE_360_CPU_DIV	1
#define STATE_360_HBUS_DIV	3
#define STATE_360_EMI_FRAC	33
#define STATE_360_EMI_DIV	2

#define STATE_400_VDDD		26
#define STATE_400_CPU_FRAC	tbd
#define STATE_400_CPU_DIV	tbd
#define STATE_400_HBUS_DIV	tbd
#define STATE_400_EMI_FRAC	tbd
#define STATE_400_EMI_DIV	tbd

#define STATE_454_VDDD		30
#define STATE_454_CPU_FRAC	19
#define STATE_454_CPU_DIV	1
#define STATE_454_HBUS_DIV	3
#define STATE_454_EMI_FRAC	21
#define STATE_454_EMI_DIV	2

#define CONCAT(a,b,c)		a ## _ ## b ## _ ## c
#define VDDD(x)				CONCAT( STATE, x, VDDD )
#define CPU_FRAC(x)			CONCAT( STATE, x, CPU_FRAC )
#define CPU_DIV(x)			CONCAT( STATE, x, CPU_DIV )
#define HBUS_DIV(x)			CONCAT( STATE, x, HBUS_DIV )
#define EMI_FRAC(x)			CONCAT( STATE, x, EMI_FRAC )
#define EMI_DIV(x)			CONCAT( STATE, x, EMI_DIV )

// Use 454 Mhz mode normally
#ifndef CPUFREQ
#define CPUFREQ				454
#endif

#define PIN_DRIVE_12mA     2
#define EMI_PIN_DRIVE_20mA 2
#define EMI_PIN_DRIVE_ADDRESS          EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_CONTROL          EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_DUALPAD          EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_DATA_SLICE_3     EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_DATA_SLICE_2     EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_DATA_SLICE_1     EMI_PIN_DRIVE_20mA
#define EMI_PIN_DRIVE_DATA_SLICE_0     EMI_PIN_DRIVE_20mA

struct ccardimx28_ident {
        const int       sdram;
        const int       flash;
        const char      *id_string;
};

/**
 * To add new valid variant ID, append new lines in this array with its configuration
 */
struct ccardimx28_ident ccardimx28_id[] = {
/* 0x00 */	        	{         0,     0, "Unknown"},
/* 0x01 */	        	{         0,     0, "Not supported"},
/* 0x02 - 55001667-01 */	{0x10000000,   256, "i.MX287, 2 Eth, 1 USB, Wireless, BT, LCD, JTAG, 1-wire"},
/* 0x03 - 55001668-01 */	{0x10000000,   256, "i.MX287, 2 Eth, 1 USB, Wireless, BT, LCD, JTAG"},
/* 0x04 - 55001669-01 */	{ 0x8000000,   128, "i.MX287, 1 Eth, 2 USB, Wireless, BT, LCD, JTAG"},
/* 0x05 - 55001674-01 */	{ 0x8000000,   128, "i.MX287, 1 Eth, 2 USB, LCD, JTAG"},
/* 0x06 - 55001670-01 */	{ 0x8000000,   128, "i.MX280, 2 USB, Wireless"},
/* 0x07 - 55001671-01 */	{ 0x8000000,   128, "i.MX280, 1 Eth, 2 USB, Wireless, JTAG"},
/* 0x08 - 55001672-01 */	{ 0x8000000,   128, "i.MX280, 1 Eth, 2 USB, Wireless"},
/* 0x09 - 55001673-01 */	{ 0x8000000,   128, "i.MX280, 1 Eth, 2 USB"},
/* SPR variants */
/* 0x0a - 55001671-02 */	{ 0x8000000,   128, "i.MX283, 1 Eth, 2 USB, Wireless, LCD, JTAG"},
/* 0x0b - 55001671-03 */	{ 0x8000000,   128, "i.MX283, 1 Eth, 2 USB, LCD, JTAG"},
/* 0x0c - 55001674-02 */	{ 0x8000000,   128, "i.MX287, 2 Eth, 1 USB, LCD, JTAG"},
/* 0x0d - 55001674-03 */	{ 0x8000000,   128, "i.MX287, 1 Eth, 2 USB, LCD, JTAG"},
/* 0x0a */			{         0,     0, "Reserved for future use"},
/* 0x0b */			{         0,     0, "Reserved for future use"},
};

//#define EMI_96M
 /* Debug uart have been init by boot rom. */
void putc(char ch)
{
	int loop = 0;
	while (HW_UARTDBGFR_RD()&BM_UARTDBGFR_TXFF) {
		loop++;
		if (loop > 10000)
			break;
	};

	/* if(!(HW_UARTDBGFR_RD() &BM_UARTDBGFR_TXFF)) */
	HW_UARTDBGDR_WR(ch);
}
void delay(unsigned int us)
{
	unsigned int start , cur;
	start = cur = HW_DIGCTL_MICROSECONDS_RD();

	while (cur < start+us) {

		cur = HW_DIGCTL_MICROSECONDS_RD();
		/*printf("0x%x\r\n",cur);*/
	}

}

/* Obtain the SDRAM size from the variant code
 * which is read from OTP bits at CUST1[15..8]
 */
unsigned int get_sdram_size(void)
{
        unsigned long retries = 100000;
        unsigned char variant;

        /* Open OTP banks */
        HW_OCOTP_CTRL_SET(BM_OCOTP_CTRL_RD_BANK_OPEN);
        while((BM_OCOTP_CTRL_BUSY & HW_OCOTP_CTRL_RD()) &&
              retries--);

        /* Read variant from CUST1[15..8] */
        variant = (HW_OCOTP_CUSTn_RD(1) >> 8) & 0xff;

        /* Close OTP banks */
        HW_OCOTP_CTRL_CLR(BM_OCOTP_CTRL_RD_BANK_OPEN);

        /* return sdram size of variant */
        return (ccardimx28_id[variant].sdram);
}

/*
 *  Init steps for memories:
 *  	1 Gbit (128MiB)
 *  	  - Micron MT47H64M16NF-25E
 *  	  - ISSI IS43DR16640B-25DBLI
 *  	  - Nanya NT5TU64M16HG-ACI
 *	  - Winbond W971GG6KB-25I
 *	2 Gbit (256MiB)
 *	  - Micron MT47H128M16RT-25E
 *	  - ISSI IS43DR16128B
 *	  - Winbond W972GG6JB-25I
 *  running at variable EMI clock speed (205.71 or 130.91 MHz) depending
 *  on CPU freq selection
 *
 *      NOTES:
 *
 *  When computing number of clocks, we must round up if the
 *  timing parameter is a 'min' number, and round down if the parameter
 *  is a 'max' number; for round-down these are commented as floor(x).
 *
 *      Although we are not using the IT (Industrial Temp) part,
 *      MarkT found in testing that refreshing faster (decreasing refresh
 *      period) bought us substantial additional margin on max ambient
 *  temperature, so we are performing normal and self-refresh at 2X
 *  their normal rates. This causes a ~1% hit in memory bandwidth but
 *  the temp margin is worth it.
 *
 *      Using more significant digits (say, 4.861111) actually does give
 *      a different result for some microsecond-resolution registers,
 *      (ie, TINIT and TRAS_max), but since these are 'min' params,
 *      we leave them at the higher number given by 4.86ns.
 */

void InitEMI_MT47HxxxM16_25E(void)
{
	volatile unsigned int * DRAM_REG = (volatile unsigned int*) HW_DRAM_CTL00_ADDR;

	DRAM_REG[0]  = 0x00000000;  // reset value
	DRAM_REG[1]  = 0x00000000;  // reset value
	DRAM_REG[2]  = 0x00000000;  // reset value
	DRAM_REG[3]  = 0x00000000;  // reset value
	DRAM_REG[4]  = 0x00000000;  // reset value
	DRAM_REG[5]  = 0x00000000;  // reset value
	DRAM_REG[6]  = 0x00000000;  // reset value
	DRAM_REG[7]  = 0x00000000;  // reset value

	DRAM_REG[16] = 0x00000000;  // reset value
	DRAM_REG[17] = 0x00000100;      // EVK   value
	DRAM_REG[18] = 0x00000000;  // EVK   value (undoc in RefMan)
	DRAM_REG[19] = 0x00000000;  // EVK   value (undoc in RefMan)
	DRAM_REG[20] = 0x00000000;  // EVK   value (undoc in RefMan)
	DRAM_REG[21] = 0x00000000;  // reset value
	DRAM_REG[22] = 0x00000000;  // reset value
	DRAM_REG[23] = 0x00000000;  // reset value
	DRAM_REG[24] = 0x00000000;  // reset value
	DRAM_REG[25] = 0x00000000;  // reset value
	DRAM_REG[26] = 0x00010101;
	DRAM_REG[27] = 0x01010101;
	DRAM_REG[28] = 0x000f0f01;

	/* Detect variant from OTP bits and init SDRAM
	 * controller for the appropriate SDRAM chip.
	 */
	if (get_sdram_size() == 0x8000000) {
		/* 128 SDRAM */
		DRAM_REG[29] = 0x0102020a;  // Enable CS0; 10 bit col addr, 13 addr pins, auto precharge=A10
		if (CPUFREQ == 454) {
			/* EMI freq = 205.71 MHz, cycle=4.861ns */
			DRAM_REG[38] = 0x06005303;  // tDAL=tWR+tRP=15ns+12.5ns=27.5ns/4.86ns=6, CPD=400ns/4.86ns=83 (0x53), TCKE=3
			DRAM_REG[41] = 0x0002030c;  // TPDEX=tXP=2, tRCD=12.5ns/4.86ns=3, tRC=55/4.86ns=12
			DRAM_REG[43] = 0x031b0322;  // tRP=12.5ns/4.86ns=3, tRFC=127.5ns/4.86ns=27=0x1b, tREFIit=floor(3900ns/4.86ns)=802=0x322 (32ms refresh)
			DRAM_REG[45] = 0x00c8001d;  // TSXR=tXSRDmin=200, TXSNR=tXSNR=tRFC+10ns=137.5ns/4.86ns=29=0x1d

		} else {
			/* EMI freq = 130.91 MHz, cycle=7.639ns */
			DRAM_REG[38] = 0x04003503;  // tDAL=tWR+tRP=15ns+12.5ns=27.5ns/7.639ns=4, CPD=400ns/7.639ns=53 (0x35), TCKE=3
			DRAM_REG[41] = 0x00020208;  // TPDEX=tXP=2, tRCD=12.5ns/7.639ns=2, tRC=55/7.639ns=8
			DRAM_REG[43] = 0x020e01fe;  // tRP=12.5ns/7.639ns=2, tRFC=127.5ns/7.639ns=0x11, tREFIit=floor(3900ns/7.639ns)=510=0x1fe (32ms refresh)
			DRAM_REG[45] = 0x00c80012;  // TSXR=tXSRDmin=200, TXSNR=tXSNR=tRFC+10ns=137.5ns/7.639ns=18=0x12
		}
	} else {
		/* 256 SDRAM (default) */
		DRAM_REG[29] = 0x0102010a;  // Enable CS0; 10 bit col addr, 14 addr pins, auto precharge=A10
		if (CPUFREQ == 454) {
			/* EMI freq = 205.71 MHz, cycle=4.861ns */
			DRAM_REG[38] = 0x07005303;  // tDAL=tWR+tRP=15ns+15ns=30ns/4.86ns=7, CPD=400ns/4.86ns=83 (0x53), TCKE=3
			DRAM_REG[41] = 0x0002040c;  // TPDEX=tXP=2, tRCD=15ns/4.86ns=4, tRC=57.5/4.86ns=12
			DRAM_REG[43] = 0x04290322;  // tRP=15ns/4.86ns=4, tRFC=195ns/4.86ns=41=0x29, tREFIit=floor(3900ns/4.86ns)=802=0x322 (32ms refresh)
			DRAM_REG[45] = 0x00c8002b;  // TSXR=tXSRDmin=200, TXSNR=tXSNR=tRFC+10ns=205ns/4.86ns=43=0x2b

		} else {
			/* EMI freq = 130.91 MHz, cycle=7.639ns */
			DRAM_REG[38] = 0x04003503;  // tDAL=tWR+tRP=15ns+15ns=30ns/7.639ns=4, CPD=400ns/7.639ns=53 (0x35), TCKE=3
			DRAM_REG[41] = 0x00020208;  // TPDEX=tXP=2, tRCD=15ns/7.639ns=2, tRC=57.5/7.639ns=8
			DRAM_REG[43] = 0x021a01fe;  // tRP=15ns/7.639ns=2, tRFC=195ns/7.639ns=26=0x1a, tREFIit=floor(3900ns/7.639ns)=510=0x1fe (32ms refresh)
			DRAM_REG[45] = 0x00c8001b;  // TSXR=tXSRDmin=200, TXSNR=tXSNR=tRFC+10ns=205ns/7.639ns=27=0x1b
		}
	}

	DRAM_REG[31] = 0x00010101;  // 8 bank mode
	DRAM_REG[32] = 0x00000100;
	DRAM_REG[33] = 0x00000100;
	DRAM_REG[34] = 0x00000000;  // reset value
	DRAM_REG[35] = 0x00000002;
	DRAM_REG[36] = 0x01010000;      // TREF_ENABLE=1, TRAS_LOCKOUT=1, FAST_WRITE=0
	DRAM_REG[37] = 0x07080403;      // CASLAT_LIN_GATE=7 CASLAT_LIN=8 CASLAT=4 WRLAT=3 (could potentially use: CASLAT_LIN_GATE=6, 6, 3, 2)

	if (CPUFREQ == 454) {
		/* EMI freq = 205.71 MHz, cycle=4.861ns */
		DRAM_REG[39] = 0x0b0000c8;  // tFAW=50ns/4.86ns=11, DLL reset recovery (lock) time = 200 cycles
		DRAM_REG[40] = 0x0200a0c1;  // TMRD=2, TINIT=200us/4.86ns=41153=0xa0c1 - see init timing diagram (note 3)
		DRAM_REG[42] = 0x0038430a;  // TRAS_max=floor(70000ns/4.86ns)=14403=0x3843, TRAS_min=45ns/4.86ns=10
		DRAM_REG[44] = 0x02040203;  // tWTR=7.5ns/4.86ns=2, tWR=15ns/4.86ns=4 tRTP=7.5ns/4.86ns=2 tRRD(x16)=10ns/4.86ns=3
	}
	else {
		/* EMI freq = 130.91 MHz, cycle=7.639ns */
		DRAM_REG[39] = 0x070000c8;  // tFAW=50ns/7.639ns=7, DLL reset recovery (lock) time = 200 cycles
		DRAM_REG[40] = 0x02006646;  // TMRD=2, TINIT=200us/7.639ns=26182=0x6646 - see init timing diagram (note 3)
		DRAM_REG[42] = 0x0023cb06;  // TRAS_max=floor(70000ns/7.639ns)=9163=0x23cb, TRAS_min=45ns/7.639ns=6
		DRAM_REG[44] = 0x02020202;  // tWTR=7.5ns/7.639ns=1*, tWR=15ns/7.639ns=2 tRTP=7.5ns/7.639ns=1* tRRD(x16)=10ns/7.639ns=2
					    // *NOTE: note 37 on DDR manual says force any value to a minimum of 2 clocks.
	}

	DRAM_REG[48] = 0x00012100;      // EVK   value
	DRAM_REG[49] = 0xffff0303;      // EVK   value
	DRAM_REG[50] = 0x00012100;      // EVK   value
	DRAM_REG[51] = 0xffff0303;      // EVK   value
	DRAM_REG[52] = 0x00012100;      // EVK   value
	DRAM_REG[53] = 0xffff0303;      // EVK   value
	DRAM_REG[54] = 0x00012100;      // EVK   value
	DRAM_REG[55] = 0xffff0303;      // EVK   value
	DRAM_REG[56] = 0x00000003;      // EVK   value

	DRAM_REG[58] = 0x00000000;  // reset value

	DRAM_REG[66] = 0x00000612;      // EVK   value
	DRAM_REG[67] = 0x01000102;      // Enable CS0 clock only
	DRAM_REG[68] = 0x06120612;      // EVK   value (though RefMan says read-only?)
	DRAM_REG[69] = 0x00000200;      // EVK   value
	DRAM_REG[70] = 0x00020007;      // EVK   value
	DRAM_REG[71] = 0xf4004a27;      // EVK   value - disable termination (among other things)
	DRAM_REG[72] = 0xf4004a27;      // EVK   value - disable termination (among other things)

	DRAM_REG[75] = 0x07400300;  // EVK   value - bit 22 is usually set by FSL, but not in
	DRAM_REG[76] = 0x07400300;  //                               200Mhz case; assume a typo and correct it

	DRAM_REG[79] = 0x00000005;      // EVK   value
	DRAM_REG[80] = 0x00000000;  // reset value
	DRAM_REG[81] = 0x00000000;  // reset value
	DRAM_REG[82] = 0x01000000;
	DRAM_REG[83] = 0x00000000;  // Disable CS0 ODT during reads
	DRAM_REG[84] = 0x00000001;  // Enable  CS0 ODT during writes to CS0
	DRAM_REG[85] = 0x000f1133;      // EVK value - F=undoc, 150 ohm, DDR2, feed I/O into IDDQ

	DRAM_REG[87] = 0x00001f04;      // EVK   value
	DRAM_REG[88] = 0x00001f04;      // EVK   value

	DRAM_REG[91] = 0x00001f04;      // EVK   value
	DRAM_REG[92] = 0x00001f04;      // EVK   value

	DRAM_REG[127] = 0x00000000;     // reset value
	DRAM_REG[132] = 0x00000000;     // reset value
	DRAM_REG[137] = 0x00000000;     // reset value
	DRAM_REG[142] = 0x00000000;     // reset value
	DRAM_REG[147] = 0x00000000;     // reset value
	DRAM_REG[152] = 0x00000000;     // reset value
	DRAM_REG[157] = 0x00000000;     // reset value

	DRAM_REG[162] = 0x00010000;     // EVK   value
	DRAM_REG[163] = 0x00030404;
	DRAM_REG[164] = 0x00000002; // TMOD=tMRD=2 cycles

	DRAM_REG[171] = 0x01010000;     // EVK   value
	DRAM_REG[172] = 0x01000000;     // EVK   value
	DRAM_REG[173] = 0x03030000;     // EVK   value
	DRAM_REG[174] = 0x00010303;     // EVK   value
	DRAM_REG[175] = 0x01020202;     // EVK   value
	DRAM_REG[176] = 0x00000000;     // EVK   value

	if (CPUFREQ == 454)
		/* TCCD=2,
		 * TRPA=
		 * 	Nanya: tRPA = tRP + 1tCK = 12.5 + 1 = 13.5/4.86 = 3
		 * 	Winbond: tRPA = tnRP + 1nCK = (tRP / tCK) + 1 = (12.5 / 4.86) + 1 = 4
		 * 	Micron: tRPA = 15 / 4.86 = 4
		 * CKSRX/CKSRE=1 (see pg 115, note 1)
		 */
		DRAM_REG[177] = 0x02040101;
	else
		/* TCCD=2,
		 * TRPA=
		 * 	Nanya: tRPA = tRP + 1tCK = 12.5 + 1 = 13.5/7.639 = 2
		 * 	Winbond: tRPA = tnRP + 1nCK = (tRP / tCK) + 1 = (12.5 / 7.639) + 1 = 3
		 * 	Micron: tRPA = 15 / 7.639 = 2
		 * CKSRX/CKSRE=1 (see pg 115, note 1)
		 */
		DRAM_REG[177] = 0x02030101;

	DRAM_REG[178] = 0x21002103;     // EVK   value
	DRAM_REG[179] = 0x00061200;     // EVK   value (may be read-only?)
	DRAM_REG[180] = 0x06120612;     // EVK   value (may be read-only?)
	if (CPUFREQ == 454)
		DRAM_REG[181] = 0x00000642;     // MR0 settings for CS0: WR=tWR/tCK=4, CASLat=4, Sequential, BurstLength=4
	else
		DRAM_REG[181] = 0x00000242;     // MR0 settings for CS0: WR=tWR/tCK=2, CASLat=4, Sequential, BurstLength=4
	DRAM_REG[183] = 0x00000004;     // MR1 settings for CS0: 75ohm ODT nominal, Full drive strength
	DRAM_REG[185] = 0x00000080;     // MR2 settings for CS0: 2x self-refresh timing (Tcase > 85C) (to give us more temp margin)
	DRAM_REG[187] = 0x00000000;     // MR3 settings for CS0:
	DRAM_REG[189] = 0xffffffff;     // EVK   value
}

void poweron_pll()
{
	HW_CLKCTRL_PLL0CTRL0_SET(BM_CLKCTRL_PLL0CTRL0_POWER);
}
void turnon_mem_rail(int mv)
{
	unsigned int value;
//	HW_POWER_CTRL_CLR(BM_POWER_CTRL_CLKGATE);

	value = BM_POWER_VDDMEMCTRL_ENABLE_ILIMIT|
		BM_POWER_VDDMEMCTRL_ENABLE_LINREG|
		BM_POWER_VDDMEMCTRL_PULLDOWN_ACTIVE|
		(mv-1700)/50;

	HW_POWER_VDDMEMCTRL_WR(value);
	delay(20000);
	value &= ~(BM_POWER_VDDMEMCTRL_ENABLE_ILIMIT|
		 BM_POWER_VDDMEMCTRL_PULLDOWN_ACTIVE);
	HW_POWER_VDDMEMCTRL_WR(value);
}
void set_emi_frac(unsigned int div)
{
	HW_CLKCTRL_FRAC0_SET(BM_CLKCTRL_FRAC0_EMIFRAC);
	div = (~div);
	HW_CLKCTRL_FRAC0_CLR(BF_CLKCTRL_FRAC0_EMIFRAC(div));
}
void init_clock()
{
	HW_CLKCTRL_FRAC0_SET(BM_CLKCTRL_FRAC0_CLKGATEEMI);

	set_emi_frac(EMI_FRAC(CPUFREQ));

	HW_CLKCTRL_FRAC0_CLR(BM_CLKCTRL_FRAC0_CLKGATEEMI);
	delay(11000);

	HW_CLKCTRL_EMI_WR(BF_CLKCTRL_EMI_DIV_XTAL(1)|
			  BF_CLKCTRL_EMI_DIV_EMI(EMI_DIV(CPUFREQ))
			 );

	/*choose ref_emi*/
	HW_CLKCTRL_CLKSEQ_CLR(BM_CLKCTRL_CLKSEQ_BYPASS_EMI);

	//printf("FRAC 0x%x\r\n" , HW_CLKCTRL_FRAC0_RD());

}

void disable_emi_padkeepers(void)
{
#if 0
    HW_PINCTRL_CTRL_CLR(BM_PINCTRL_CTRL_SFTRST | BM_PINCTRL_CTRL_CLKGATE);

    HW_PINCTRL_PULL3_SET(
    BM_PINCTRL_PULL3_BANK3_PIN17 |
    BM_PINCTRL_PULL3_BANK3_PIN16 |
    BM_PINCTRL_PULL3_BANK3_PIN15 |
    BM_PINCTRL_PULL3_BANK3_PIN14 |
    BM_PINCTRL_PULL3_BANK3_PIN13 |
    BM_PINCTRL_PULL3_BANK3_PIN12 |
    BM_PINCTRL_PULL3_BANK3_PIN11 |
    BM_PINCTRL_PULL3_BANK3_PIN10 |
    BM_PINCTRL_PULL3_BANK3_PIN09 |
    BM_PINCTRL_PULL3_BANK3_PIN08 |
    BM_PINCTRL_PULL3_BANK3_PIN07 |
    BM_PINCTRL_PULL3_BANK3_PIN06 |
    BM_PINCTRL_PULL3_BANK3_PIN05 |
    BM_PINCTRL_PULL3_BANK3_PIN04 |
    BM_PINCTRL_PULL3_BANK3_PIN03 |
    BM_PINCTRL_PULL3_BANK3_PIN02 |
    BM_PINCTRL_PULL3_BANK3_PIN01 |
    BM_PINCTRL_PULL3_BANK3_PIN00);
#endif
}

#define PIN_VOL(pin , v) ((v) ? (pin) : 0)
void init_emi_pin(int pin_voltage,
		  int pin_drive
		  )
{
#ifdef MEM_MDDR
	disable_emi_padkeepers();
	HW_PINCTRL_EMI_DS_CTRL_WR(
		BF_PINCTRL_EMI_DS_CTRL_ADDRESS_MA(EMI_PIN_DRIVE_ADDRESS)     |
		BF_PINCTRL_EMI_DS_CTRL_CONTROL_MA(EMI_PIN_DRIVE_CONTROL)     |
		BF_PINCTRL_EMI_DS_CTRL_DUALPAD_MA(EMI_PIN_DRIVE_DUALPAD)     |
		BF_PINCTRL_EMI_DS_CTRL_SLICE3_MA(EMI_PIN_DRIVE_DATA_SLICE_3) |
		BF_PINCTRL_EMI_DS_CTRL_SLICE2_MA(EMI_PIN_DRIVE_DATA_SLICE_2) |
		BF_PINCTRL_EMI_DS_CTRL_SLICE1_MA(EMI_PIN_DRIVE_DATA_SLICE_1) |
		BF_PINCTRL_EMI_DS_CTRL_SLICE0_MA(EMI_PIN_DRIVE_DATA_SLICE_0));

	/* Configure Bank-3 Pins 0-15 as EMI pins*/
	HW_PINCTRL_MUXSEL10_CLR(
		BM_PINCTRL_MUXSEL10_BANK5_PIN00 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN01 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN02 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN03 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN04 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN05 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN06 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN07 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN08 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN09 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN10 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN11 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN12 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN13 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN14 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN15);

	HW_PINCTRL_MUXSEL11_CLR(
		BM_PINCTRL_MUXSEL11_BANK5_PIN16 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN17 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN18 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN19 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN20 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN21 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN22 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN23 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN26 );

	HW_PINCTRL_MUXSEL12_CLR(
		BM_PINCTRL_MUXSEL12_BANK6_PIN00 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN01 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN02 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN03 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN04 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN05 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN06 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN07 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN08 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN09 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN10 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN11 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN12 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN13 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN14 );

	HW_PINCTRL_MUXSEL13_CLR(
		BM_PINCTRL_MUXSEL13_BANK6_PIN16 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN17 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN18 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN19 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN20 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN21 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN22 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN23 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN24 );
#else
	/* Configure Bank-3 Pins 0-15 as EMI pins*/
	HW_PINCTRL_MUXSEL10_CLR(
		BM_PINCTRL_MUXSEL10_BANK5_PIN00 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN01 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN02 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN03 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN04 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN05 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN06 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN07 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN08 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN09 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN10 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN11 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN12 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN13 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN14 |
		BM_PINCTRL_MUXSEL10_BANK5_PIN15);

	HW_PINCTRL_MUXSEL11_CLR(
		BM_PINCTRL_MUXSEL11_BANK5_PIN16 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN17 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN18 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN19 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN20 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN21 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN22 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN23 |
		BM_PINCTRL_MUXSEL11_BANK5_PIN26 );

	HW_PINCTRL_MUXSEL12_CLR(
		BM_PINCTRL_MUXSEL12_BANK6_PIN00 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN01 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN02 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN03 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN04 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN05 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN06 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN07 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN08 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN09 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN10 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN11 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN12 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN13 |
		BM_PINCTRL_MUXSEL12_BANK6_PIN14 );

	HW_PINCTRL_MUXSEL13_CLR(
		BM_PINCTRL_MUXSEL13_BANK6_PIN16 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN17 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN18 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN19 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN20 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN21 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN22 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN23 |
		BM_PINCTRL_MUXSEL13_BANK6_PIN24 );
#endif
}
void exit_selfrefresh()
{
#if 0
	unsigned int start;
	unsigned int value;
	value = HW_DRAM_CTL16_RD();
	value &= ~(1<<17);
	HW_DRAM_CTL16_WR(value);

	start = HW_DIGCTL_MICROSECONDS_RD();

	while ((HW_EMI_STAT_RD()&BM_EMI_STAT_DRAM_HALTED)) {

		if (HW_DIGCTL_MICROSECONDS_RD() > (start + 1000000)) {

			printf("exit self refresh timeout\r\n");
			return;
		}
	}
#endif
}

void set_port_priority()
{
	//unsigned int value;
#if 0
	HW_EMI_CTRL_CLR(BM_EMI_CTRL_PORT_PRIORITY_ORDER);
	HW_EMI_CTRL_SET(BF_EMI_CTRL_PORT_PRIORITY_ORDER(
				BV_EMI_CTRL_PORT_PRIORITY_ORDER__PORT1230)
		       );

	HW_EMI_CTRL_CLR(BM_EMI_CTRL_PORT_PRIORITY_ORDER);
	HW_EMI_CTRL_SET(BF_EMI_CTRL_PORT_PRIORITY_ORDER(0x2));
#endif

}
void entry_auto_clock_gate()
{
	unsigned int value;
	value =  HW_DRAM_CTL16_RD();
	value |= 1<<19;
	HW_DRAM_CTL16_WR(value);

	value =  HW_DRAM_CTL16_RD();
	value |= 1<<11;
	HW_DRAM_CTL16_WR(value);
}
void change_cpu_freq()
{
	int value = HW_POWER_VDDDCTRL_RD();

	//printf("power 0x%x\r\n" , value);
	value &= ~BM_POWER_VDDDCTRL_TRG;
	value |= BF_POWER_VDDDCTRL_TRG(VDDD(CPUFREQ));
	value &= ~BM_POWER_VDDDCTRL_BO_OFFSET;
	value |= BF_POWER_VDDDCTRL_BO_OFFSET(4);	/* 100mV below TRG */
	value &= ~BM_POWER_VDDDCTRL_LINREG_OFFSET;
	value |= BF_POWER_VDDDCTRL_LINREG_OFFSET(2);
	//value |= BM_POWER_VDDDCTRL_ENABLE_LINREG;
	//value |= BM_POWER_VDDDCTRL_DISABLE_STEPPING;
	HW_POWER_VDDDCTRL_WR(value);

	delay(10000);

	//printf("Frac 0x%x\r\n", HW_CLKCTRL_FRAC0_RD());

	value = HW_CLKCTRL_FRAC0_RD();
	value &= ~BM_CLKCTRL_FRAC0_CPUFRAC;
	value |= BF_CLKCTRL_FRAC0_CPUFRAC(CPU_FRAC(CPUFREQ));
	value &= ~BM_CLKCTRL_FRAC0_CLKGATECPU;

	HW_CLKCTRL_FRAC0_WR(value); /*Change cpu freq */

	HW_CLKCTRL_CLKSEQ_SET(BM_CLKCTRL_CLKSEQ_BYPASS_CPU);

	value = HW_CLKCTRL_HBUS_RD();
	value &= ~BM_CLKCTRL_HBUS_DIV;
	value |= BF_CLKCTRL_HBUS_DIV(HBUS_DIV(CPUFREQ));
	HW_CLKCTRL_HBUS_WR(value);

	delay(10000);
	//printf("start change cpu freq\r\n");

	value = HW_CLKCTRL_CPU_RD();
	value &= ~BM_CLKCTRL_CPU_DIV_CPU;
	value |= BF_CLKCTRL_HBUS_DIV(CPU_DIV(CPUFREQ));
	HW_CLKCTRL_CPU_WR(value);

	HW_CLKCTRL_CLKSEQ_CLR(BM_CLKCTRL_CLKSEQ_BYPASS_CPU);

	//printf("hbus 0x%x\r\n" , HW_CLKCTRL_HBUS_RD());
	//printf("cpu 0x%x\r\n" , HW_CLKCTRL_CPU_RD());

}
void poweron_vdda()
{
	int value = HW_POWER_VDDACTRL_RD();
	value &= ~BM_POWER_VDDACTRL_TRG;
	value |= BF_POWER_VDDACTRL_TRG(0xC);
	value &= ~BM_POWER_VDDACTRL_BO_OFFSET;
	value |= BF_POWER_VDDACTRL_BO_OFFSET(6);
	value &= ~BM_POWER_VDDACTRL_LINREG_OFFSET;
	value |= BF_POWER_VDDACTRL_LINREG_OFFSET(2);

	HW_POWER_VDDACTRL_WR(value);
}

int _start(int arg)
{
        unsigned int value;
        volatile int *pTest = 0x40000000;
        int i;

#ifdef MEM_MDDR
	/* set to mddr mode*/
	HW_PINCTRL_EMI_DS_CTRL_CLR(BW_PINCTRL_EMI_DS_CTRL_DDR_MODE(0x3));
#else
	/* set to ddr2 mode*/
	HW_PINCTRL_EMI_DS_CTRL_SET(BW_PINCTRL_EMI_DS_CTRL_DDR_MODE(0x3));
#endif
	//printf(__DATE__ __TIME__);
	//printf("\r\n");
	/*printf("Fuse 0x%x\r\n",HW_OCOTP_CUSTCAP_RD());*/

	poweron_pll();
	delay(11000);

	init_emi_pin(
		0,
		PIN_DRIVE_12mA
		);

	init_clock();

	delay(10000);

	poweron_vdda();

	value = HW_DRAM_CTL16_RD();
	value &= ~BM_DRAM_CTL16_START;
	HW_DRAM_CTL16_WR(value);

        /* Init SDRAM controller */
        InitEMI_MT47HxxxM16_25E();

	value = HW_DRAM_CTL17_RD();
	value &= ~BM_DRAM_CTL17_SREFRESH;
	HW_DRAM_CTL17_WR(value);

	value = HW_DRAM_CTL16_RD();
	value |= BM_DRAM_CTL16_START;
	HW_DRAM_CTL16_WR(value);

	//Wait for DDR ready
	//printf("Wait for ddr ready 1");
	while(!(HW_DRAM_CTL58_RD()&0x100000));

#if 0
	exit_selfrefresh();

	set_port_priority();

	entry_auto_clock_gate();
#endif

	change_cpu_freq();

#if 0
	for (i = 0; i <= 40; i++) {
		printf("mem %x - 0x%x\r\n",
			i, *(volatile int*)(0x800E0000 + i * 4));
	}
#endif

	/*Test Memory;*/
#if 0
	printf("start test memory accress\r\n");
	printf("ddr2 0x%x\r\n", pTest);
	for (i = 0; i < 1000; i++)
		*pTest++ = i;

	pTest = (volatile int *)0x40000000;

	for (i = 0; i < 1000; i++) {
		if (*pTest != (i)) {
			printf("0x%x error value 0x%x\r\n", i, *pTest);
		}
		pTest++;
	}
	printf("finish simple test\r\n");
#endif
	return 0;
}

/* kiss gcc's ass to make it happy */
void __aeabi_unwind_cpp_pr0() {}
void __aeabi_unwind_cpp_pr1() {}
