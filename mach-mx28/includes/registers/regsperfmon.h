/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc. All Rights Reserved.
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

 * WARNING!  THIS FILE IS AUTOMATICALLY GENERATED FROM XML.
 *                DO NOT MODIFY THIS FILE DIRECTLY.
 *
 * Xml Revision: 1.36
 *
 * Template revision: 26195
 *
 *
 * The following naming conventions are followed in this file.
 *      XX_<module>_<regname>_<field>
 *
 * XX specifies the define / macro class
 *      HW pertains to a register
 *      BM indicates a Bit Mask
 *      BF indicates a Bit Field macro
 *
 * <module> is the hardware module name which can be any of the following...
 *      USB20 (Note when there is more than one copy of a given module, the
 *      module name includes a number starting from 0 for the first instance
 *      of that module)
 *
 * <regname> is the specific register within that module
 *
 * <field> is the specific bitfield within that <module>_<register>
 *
 * We also define the following...
 *      hw_<module>_<regname>_t is typedef of anonymous union
 *
 */

#ifndef _PERFMON_H
#define _PERFMON_H  1

#include "regs.h"

#ifndef REGS_PERFMON_BASE
#define REGS_PERFMON_BASE (REGS_BASE + 0x6000)
#endif

/*
 * HW_PERFMON_CTRL - PerfMon Control Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned RUN             :  1;
        unsigned SNAP            :  1;
        unsigned CLR             :  1;
        unsigned READ_EN         :  1;
        unsigned TRAP_ENABLE     :  1;
        unsigned TRAP_IN_RANGE   :  1;
        unsigned LATENCY_ENABLE  :  1;
        unsigned TRAP_IRQ_EN     :  1;
        unsigned LATENCY_IRQ_EN  :  1;
        unsigned BUS_ERR_IRQ_EN  :  1;
        unsigned TRAP_IRQ        :  1;
        unsigned LATENCY_IRQ     :  1;
        unsigned BUS_ERR_IRQ     :  1;
        unsigned RSVD1           :  3;
        unsigned IRQ_MID         :  8;
        unsigned RSVD2           :  6;
        unsigned CLKGATE         :  1;
        unsigned SFTRST          :  1;
    } B;
} hw_perfmon_ctrl_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_CTRL register
 */
#define HW_PERFMON_CTRL_ADDR         (REGS_PERFMON_BASE + 0x0)
#define HW_PERFMON_CTRL_SET_ADDR     (HW_PERFMON_CTRL_ADDR + 4)
#define HW_PERFMON_CTRL_CLR_ADDR     (HW_PERFMON_CTRL_ADDR + 8)
#define HW_PERFMON_CTRL_TOG_ADDR     (HW_PERFMON_CTRL_ADDR + 12)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_CTRL           (*(volatile hw_perfmon_ctrl_t *) HW_PERFMON_CTRL_ADDR)
#define HW_PERFMON_CTRL_RD()      (HW_PERFMON_CTRL.U)
#define HW_PERFMON_CTRL_WR(v)     (HW_PERFMON_CTRL.U = (v))
#define HW_PERFMON_CTRL_SET(v)    ((*(volatile reg32_t *) HW_PERFMON_CTRL_SET_ADDR) = (v))
#define HW_PERFMON_CTRL_CLR(v)    ((*(volatile reg32_t *) HW_PERFMON_CTRL_CLR_ADDR) = (v))
#define HW_PERFMON_CTRL_TOG(v)    ((*(volatile reg32_t *) HW_PERFMON_CTRL_TOG_ADDR) = (v))
#endif


/*
 * constants & macros for individual HW_PERFMON_CTRL bitfields
 */
/* --- Register HW_PERFMON_CTRL, field SFTRST */

#define BP_PERFMON_CTRL_SFTRST      31
#define BM_PERFMON_CTRL_SFTRST      0x80000000

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_CTRL_SFTRST(v)   ((((reg32_t) v) << 31) & BM_PERFMON_CTRL_SFTRST)
#else
#define BF_PERFMON_CTRL_SFTRST(v)   (((v) << 31) & BM_PERFMON_CTRL_SFTRST)
#endif
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_SFTRST(v)   BF_CS1(PERFMON_CTRL, SFTRST, v)
#endif

#define BV_PERFMON_CTRL_SFTRST__RUN    0x0
#define BV_PERFMON_CTRL_SFTRST__RESET  0x1

/* --- Register HW_PERFMON_CTRL, field CLKGATE */

#define BP_PERFMON_CTRL_CLKGATE      30
#define BM_PERFMON_CTRL_CLKGATE      0x40000000

#define BF_PERFMON_CTRL_CLKGATE(v)   (((v) << 30) & BM_PERFMON_CTRL_CLKGATE)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_CLKGATE(v)   BF_CS1(PERFMON_CTRL, CLKGATE, v)
#endif

#define BV_PERFMON_CTRL_CLKGATE__RUN      0x0
#define BV_PERFMON_CTRL_CLKGATE__NO_CLKS  0x1

/* --- Register HW_PERFMON_CTRL, field RSVD2 */

#define BP_PERFMON_CTRL_RSVD2      24
#define BM_PERFMON_CTRL_RSVD2      0x3F000000

#define BF_PERFMON_CTRL_RSVD2(v)   (((v) << 24) & BM_PERFMON_CTRL_RSVD2)
/* --- Register HW_PERFMON_CTRL, field IRQ_MID */

#define BP_PERFMON_CTRL_IRQ_MID      16
#define BM_PERFMON_CTRL_IRQ_MID      0x00FF0000

#define BF_PERFMON_CTRL_IRQ_MID(v)   (((v) << 16) & BM_PERFMON_CTRL_IRQ_MID)
/* --- Register HW_PERFMON_CTRL, field RSVD1 */

#define BP_PERFMON_CTRL_RSVD1      13
#define BM_PERFMON_CTRL_RSVD1      0x0000E000

#define BF_PERFMON_CTRL_RSVD1(v)   (((v) << 13) & BM_PERFMON_CTRL_RSVD1)
/* --- Register HW_PERFMON_CTRL, field BUS_ERR_IRQ */

#define BP_PERFMON_CTRL_BUS_ERR_IRQ      12
#define BM_PERFMON_CTRL_BUS_ERR_IRQ      0x00001000

#define BF_PERFMON_CTRL_BUS_ERR_IRQ(v)   (((v) << 12) & BM_PERFMON_CTRL_BUS_ERR_IRQ)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_BUS_ERR_IRQ(v)   BF_CS1(PERFMON_CTRL, BUS_ERR_IRQ, v)
#endif

/* --- Register HW_PERFMON_CTRL, field LATENCY_IRQ */

#define BP_PERFMON_CTRL_LATENCY_IRQ      11
#define BM_PERFMON_CTRL_LATENCY_IRQ      0x00000800

#define BF_PERFMON_CTRL_LATENCY_IRQ(v)   (((v) << 11) & BM_PERFMON_CTRL_LATENCY_IRQ)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_LATENCY_IRQ(v)   BF_CS1(PERFMON_CTRL, LATENCY_IRQ, v)
#endif

/* --- Register HW_PERFMON_CTRL, field TRAP_IRQ */

#define BP_PERFMON_CTRL_TRAP_IRQ      10
#define BM_PERFMON_CTRL_TRAP_IRQ      0x00000400

#define BF_PERFMON_CTRL_TRAP_IRQ(v)   (((v) << 10) & BM_PERFMON_CTRL_TRAP_IRQ)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_TRAP_IRQ(v)   BF_CS1(PERFMON_CTRL, TRAP_IRQ, v)
#endif

/* --- Register HW_PERFMON_CTRL, field BUS_ERR_IRQ_EN */

#define BP_PERFMON_CTRL_BUS_ERR_IRQ_EN      9
#define BM_PERFMON_CTRL_BUS_ERR_IRQ_EN      0x00000200

#define BF_PERFMON_CTRL_BUS_ERR_IRQ_EN(v)   (((v) << 9) & BM_PERFMON_CTRL_BUS_ERR_IRQ_EN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_BUS_ERR_IRQ_EN(v)   BF_CS1(PERFMON_CTRL, BUS_ERR_IRQ_EN, v)
#endif

/* --- Register HW_PERFMON_CTRL, field LATENCY_IRQ_EN */

#define BP_PERFMON_CTRL_LATENCY_IRQ_EN      8
#define BM_PERFMON_CTRL_LATENCY_IRQ_EN      0x00000100

#define BF_PERFMON_CTRL_LATENCY_IRQ_EN(v)   (((v) << 8) & BM_PERFMON_CTRL_LATENCY_IRQ_EN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_LATENCY_IRQ_EN(v)   BF_CS1(PERFMON_CTRL, LATENCY_IRQ_EN, v)
#endif

/* --- Register HW_PERFMON_CTRL, field TRAP_IRQ_EN */

#define BP_PERFMON_CTRL_TRAP_IRQ_EN      7
#define BM_PERFMON_CTRL_TRAP_IRQ_EN      0x00000080

#define BF_PERFMON_CTRL_TRAP_IRQ_EN(v)   (((v) << 7) & BM_PERFMON_CTRL_TRAP_IRQ_EN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_TRAP_IRQ_EN(v)   BF_CS1(PERFMON_CTRL, TRAP_IRQ_EN, v)
#endif

/* --- Register HW_PERFMON_CTRL, field LATENCY_ENABLE */

#define BP_PERFMON_CTRL_LATENCY_ENABLE      6
#define BM_PERFMON_CTRL_LATENCY_ENABLE      0x00000040

#define BF_PERFMON_CTRL_LATENCY_ENABLE(v)   (((v) << 6) & BM_PERFMON_CTRL_LATENCY_ENABLE)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_LATENCY_ENABLE(v)   BF_CS1(PERFMON_CTRL, LATENCY_ENABLE, v)
#endif

/* --- Register HW_PERFMON_CTRL, field TRAP_IN_RANGE */

#define BP_PERFMON_CTRL_TRAP_IN_RANGE      5
#define BM_PERFMON_CTRL_TRAP_IN_RANGE      0x00000020

#define BF_PERFMON_CTRL_TRAP_IN_RANGE(v)   (((v) << 5) & BM_PERFMON_CTRL_TRAP_IN_RANGE)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_TRAP_IN_RANGE(v)   BF_CS1(PERFMON_CTRL, TRAP_IN_RANGE, v)
#endif

/* --- Register HW_PERFMON_CTRL, field TRAP_ENABLE */

#define BP_PERFMON_CTRL_TRAP_ENABLE      4
#define BM_PERFMON_CTRL_TRAP_ENABLE      0x00000010

#define BF_PERFMON_CTRL_TRAP_ENABLE(v)   (((v) << 4) & BM_PERFMON_CTRL_TRAP_ENABLE)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_TRAP_ENABLE(v)   BF_CS1(PERFMON_CTRL, TRAP_ENABLE, v)
#endif

/* --- Register HW_PERFMON_CTRL, field READ_EN */

#define BP_PERFMON_CTRL_READ_EN      3
#define BM_PERFMON_CTRL_READ_EN      0x00000008

#define BF_PERFMON_CTRL_READ_EN(v)   (((v) << 3) & BM_PERFMON_CTRL_READ_EN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_READ_EN(v)   BF_CS1(PERFMON_CTRL, READ_EN, v)
#endif

/* --- Register HW_PERFMON_CTRL, field CLR */

#define BP_PERFMON_CTRL_CLR      2
#define BM_PERFMON_CTRL_CLR      0x00000004

#define BF_PERFMON_CTRL_CLR(v)   (((v) << 2) & BM_PERFMON_CTRL_CLR)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_CLR(v)   BF_CS1(PERFMON_CTRL, CLR, v)
#endif

/* --- Register HW_PERFMON_CTRL, field SNAP */

#define BP_PERFMON_CTRL_SNAP      1
#define BM_PERFMON_CTRL_SNAP      0x00000002

#define BF_PERFMON_CTRL_SNAP(v)   (((v) << 1) & BM_PERFMON_CTRL_SNAP)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_SNAP(v)   BF_CS1(PERFMON_CTRL, SNAP, v)
#endif

/* --- Register HW_PERFMON_CTRL, field RUN */

#define BP_PERFMON_CTRL_RUN      0
#define BM_PERFMON_CTRL_RUN      0x00000001

#define BF_PERFMON_CTRL_RUN(v)   (((v) << 0) & BM_PERFMON_CTRL_RUN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_CTRL_RUN(v)   BF_CS1(PERFMON_CTRL, RUN, v)
#endif

#define BV_PERFMON_CTRL_RUN__HALT  0x0
#define BV_PERFMON_CTRL_RUN__RUN   0x1



/*
 * HW_PERFMON_MASTER_EN - PerfMon Master Enable Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned MID0   :  1;
        unsigned MID1   :  1;
        unsigned MID2   :  1;
        unsigned MID3   :  1;
        unsigned MID4   :  1;
        unsigned MID5   :  1;
        unsigned MID6   :  1;
        unsigned MID7   :  1;
        unsigned MID8   :  1;
        unsigned MID9   :  1;
        unsigned MID10  :  1;
        unsigned MID11  :  1;
        unsigned MID12  :  1;
        unsigned MID13  :  1;
        unsigned MID14  :  1;
        unsigned MID15  :  1;
        unsigned RSVD0  : 16;
    } B;
} hw_perfmon_master_en_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_MASTER_EN register
 */
#define HW_PERFMON_MASTER_EN_ADDR         (REGS_PERFMON_BASE + 0x10)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_MASTER_EN           (*(volatile hw_perfmon_master_en_t *) HW_PERFMON_MASTER_EN_ADDR)
#define HW_PERFMON_MASTER_EN_RD()      (HW_PERFMON_MASTER_EN.U)
#define HW_PERFMON_MASTER_EN_WR(v)     (HW_PERFMON_MASTER_EN.U = (v))
#define HW_PERFMON_MASTER_EN_SET(v)    (HW_PERFMON_MASTER_EN_WR(HW_PERFMON_MASTER_EN_RD() |  (v)))
#define HW_PERFMON_MASTER_EN_CLR(v)    (HW_PERFMON_MASTER_EN_WR(HW_PERFMON_MASTER_EN_RD() & ~(v)))
#define HW_PERFMON_MASTER_EN_TOG(v)    (HW_PERFMON_MASTER_EN_WR(HW_PERFMON_MASTER_EN_RD() ^  (v)))
#endif


/*
 * constants & macros for individual HW_PERFMON_MASTER_EN bitfields
 */
/* --- Register HW_PERFMON_MASTER_EN, field RSVD0 */

#define BP_PERFMON_MASTER_EN_RSVD0      16
#define BM_PERFMON_MASTER_EN_RSVD0      0xFFFF0000

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_MASTER_EN_RSVD0(v)   ((((reg32_t) v) << 16) & BM_PERFMON_MASTER_EN_RSVD0)
#else
#define BF_PERFMON_MASTER_EN_RSVD0(v)   (((v) << 16) & BM_PERFMON_MASTER_EN_RSVD0)
#endif
/* --- Register HW_PERFMON_MASTER_EN, field MID15 */

#define BP_PERFMON_MASTER_EN_MID15      15
#define BM_PERFMON_MASTER_EN_MID15      0x00008000

#define BF_PERFMON_MASTER_EN_MID15(v)   (((v) << 15) & BM_PERFMON_MASTER_EN_MID15)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID15(v)   BF_CS1(PERFMON_MASTER_EN, MID15, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID14 */

#define BP_PERFMON_MASTER_EN_MID14      14
#define BM_PERFMON_MASTER_EN_MID14      0x00004000

#define BF_PERFMON_MASTER_EN_MID14(v)   (((v) << 14) & BM_PERFMON_MASTER_EN_MID14)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID14(v)   BF_CS1(PERFMON_MASTER_EN, MID14, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID13 */

#define BP_PERFMON_MASTER_EN_MID13      13
#define BM_PERFMON_MASTER_EN_MID13      0x00002000

#define BF_PERFMON_MASTER_EN_MID13(v)   (((v) << 13) & BM_PERFMON_MASTER_EN_MID13)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID13(v)   BF_CS1(PERFMON_MASTER_EN, MID13, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID12 */

#define BP_PERFMON_MASTER_EN_MID12      12
#define BM_PERFMON_MASTER_EN_MID12      0x00001000

#define BF_PERFMON_MASTER_EN_MID12(v)   (((v) << 12) & BM_PERFMON_MASTER_EN_MID12)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID12(v)   BF_CS1(PERFMON_MASTER_EN, MID12, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID11 */

#define BP_PERFMON_MASTER_EN_MID11      11
#define BM_PERFMON_MASTER_EN_MID11      0x00000800

#define BF_PERFMON_MASTER_EN_MID11(v)   (((v) << 11) & BM_PERFMON_MASTER_EN_MID11)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID11(v)   BF_CS1(PERFMON_MASTER_EN, MID11, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID10 */

#define BP_PERFMON_MASTER_EN_MID10      10
#define BM_PERFMON_MASTER_EN_MID10      0x00000400

#define BF_PERFMON_MASTER_EN_MID10(v)   (((v) << 10) & BM_PERFMON_MASTER_EN_MID10)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID10(v)   BF_CS1(PERFMON_MASTER_EN, MID10, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID9 */

#define BP_PERFMON_MASTER_EN_MID9      9
#define BM_PERFMON_MASTER_EN_MID9      0x00000200

#define BF_PERFMON_MASTER_EN_MID9(v)   (((v) << 9) & BM_PERFMON_MASTER_EN_MID9)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID9(v)   BF_CS1(PERFMON_MASTER_EN, MID9, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID8 */

#define BP_PERFMON_MASTER_EN_MID8      8
#define BM_PERFMON_MASTER_EN_MID8      0x00000100

#define BF_PERFMON_MASTER_EN_MID8(v)   (((v) << 8) & BM_PERFMON_MASTER_EN_MID8)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID8(v)   BF_CS1(PERFMON_MASTER_EN, MID8, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID7 */

#define BP_PERFMON_MASTER_EN_MID7      7
#define BM_PERFMON_MASTER_EN_MID7      0x00000080

#define BF_PERFMON_MASTER_EN_MID7(v)   (((v) << 7) & BM_PERFMON_MASTER_EN_MID7)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID7(v)   BF_CS1(PERFMON_MASTER_EN, MID7, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID6 */

#define BP_PERFMON_MASTER_EN_MID6      6
#define BM_PERFMON_MASTER_EN_MID6      0x00000040

#define BF_PERFMON_MASTER_EN_MID6(v)   (((v) << 6) & BM_PERFMON_MASTER_EN_MID6)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID6(v)   BF_CS1(PERFMON_MASTER_EN, MID6, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID5 */

#define BP_PERFMON_MASTER_EN_MID5      5
#define BM_PERFMON_MASTER_EN_MID5      0x00000020

#define BF_PERFMON_MASTER_EN_MID5(v)   (((v) << 5) & BM_PERFMON_MASTER_EN_MID5)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID5(v)   BF_CS1(PERFMON_MASTER_EN, MID5, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID4 */

#define BP_PERFMON_MASTER_EN_MID4      4
#define BM_PERFMON_MASTER_EN_MID4      0x00000010

#define BF_PERFMON_MASTER_EN_MID4(v)   (((v) << 4) & BM_PERFMON_MASTER_EN_MID4)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID4(v)   BF_CS1(PERFMON_MASTER_EN, MID4, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID3 */

#define BP_PERFMON_MASTER_EN_MID3      3
#define BM_PERFMON_MASTER_EN_MID3      0x00000008

#define BF_PERFMON_MASTER_EN_MID3(v)   (((v) << 3) & BM_PERFMON_MASTER_EN_MID3)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID3(v)   BF_CS1(PERFMON_MASTER_EN, MID3, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID2 */

#define BP_PERFMON_MASTER_EN_MID2      2
#define BM_PERFMON_MASTER_EN_MID2      0x00000004

#define BF_PERFMON_MASTER_EN_MID2(v)   (((v) << 2) & BM_PERFMON_MASTER_EN_MID2)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID2(v)   BF_CS1(PERFMON_MASTER_EN, MID2, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID1 */

#define BP_PERFMON_MASTER_EN_MID1      1
#define BM_PERFMON_MASTER_EN_MID1      0x00000002

#define BF_PERFMON_MASTER_EN_MID1(v)   (((v) << 1) & BM_PERFMON_MASTER_EN_MID1)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID1(v)   BF_CS1(PERFMON_MASTER_EN, MID1, v)
#endif

/* --- Register HW_PERFMON_MASTER_EN, field MID0 */

#define BP_PERFMON_MASTER_EN_MID0      0
#define BM_PERFMON_MASTER_EN_MID0      0x00000001

#define BF_PERFMON_MASTER_EN_MID0(v)   (((v) << 0) & BM_PERFMON_MASTER_EN_MID0)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_MASTER_EN_MID0(v)   BF_CS1(PERFMON_MASTER_EN, MID0, v)
#endif



/*
 * HW_PERFMON_TRAP_ADDR_LOW - PerfMon Trap Range Low Address Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned ADDR  : 32;
    } B;
} hw_perfmon_trap_addr_low_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_TRAP_ADDR_LOW register
 */
#define HW_PERFMON_TRAP_ADDR_LOW_ADDR         (REGS_PERFMON_BASE + 0x20)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_TRAP_ADDR_LOW           (*(volatile hw_perfmon_trap_addr_low_t *) HW_PERFMON_TRAP_ADDR_LOW_ADDR)
#define HW_PERFMON_TRAP_ADDR_LOW_RD()      (HW_PERFMON_TRAP_ADDR_LOW.U)
#define HW_PERFMON_TRAP_ADDR_LOW_WR(v)     (HW_PERFMON_TRAP_ADDR_LOW.U = (v))
#define HW_PERFMON_TRAP_ADDR_LOW_SET(v)    (HW_PERFMON_TRAP_ADDR_LOW_WR(HW_PERFMON_TRAP_ADDR_LOW_RD() |  (v)))
#define HW_PERFMON_TRAP_ADDR_LOW_CLR(v)    (HW_PERFMON_TRAP_ADDR_LOW_WR(HW_PERFMON_TRAP_ADDR_LOW_RD() & ~(v)))
#define HW_PERFMON_TRAP_ADDR_LOW_TOG(v)    (HW_PERFMON_TRAP_ADDR_LOW_WR(HW_PERFMON_TRAP_ADDR_LOW_RD() ^  (v)))
#endif


/*
 * constants & macros for individual HW_PERFMON_TRAP_ADDR_LOW bitfields
 */
/* --- Register HW_PERFMON_TRAP_ADDR_LOW, field ADDR */

#define BP_PERFMON_TRAP_ADDR_LOW_ADDR      0
#define BM_PERFMON_TRAP_ADDR_LOW_ADDR      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_TRAP_ADDR_LOW_ADDR(v)   ((reg32_t) v)
#else
#define BF_PERFMON_TRAP_ADDR_LOW_ADDR(v)   (v)
#endif
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_TRAP_ADDR_LOW_ADDR(v)   (HW_PERFMON_TRAP_ADDR_LOW.B.ADDR = (v))
#endif



/*
 * HW_PERFMON_TRAP_ADDR_HIGH - PerfMon Trap Range High Address Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned ADDR  : 32;
    } B;
} hw_perfmon_trap_addr_high_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_TRAP_ADDR_HIGH register
 */
#define HW_PERFMON_TRAP_ADDR_HIGH_ADDR         (REGS_PERFMON_BASE + 0x30)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_TRAP_ADDR_HIGH           (*(volatile hw_perfmon_trap_addr_high_t *) HW_PERFMON_TRAP_ADDR_HIGH_ADDR)
#define HW_PERFMON_TRAP_ADDR_HIGH_RD()      (HW_PERFMON_TRAP_ADDR_HIGH.U)
#define HW_PERFMON_TRAP_ADDR_HIGH_WR(v)     (HW_PERFMON_TRAP_ADDR_HIGH.U = (v))
#define HW_PERFMON_TRAP_ADDR_HIGH_SET(v)    (HW_PERFMON_TRAP_ADDR_HIGH_WR(HW_PERFMON_TRAP_ADDR_HIGH_RD() |  (v)))
#define HW_PERFMON_TRAP_ADDR_HIGH_CLR(v)    (HW_PERFMON_TRAP_ADDR_HIGH_WR(HW_PERFMON_TRAP_ADDR_HIGH_RD() & ~(v)))
#define HW_PERFMON_TRAP_ADDR_HIGH_TOG(v)    (HW_PERFMON_TRAP_ADDR_HIGH_WR(HW_PERFMON_TRAP_ADDR_HIGH_RD() ^  (v)))
#endif


/*
 * constants & macros for individual HW_PERFMON_TRAP_ADDR_HIGH bitfields
 */
/* --- Register HW_PERFMON_TRAP_ADDR_HIGH, field ADDR */

#define BP_PERFMON_TRAP_ADDR_HIGH_ADDR      0
#define BM_PERFMON_TRAP_ADDR_HIGH_ADDR      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_TRAP_ADDR_HIGH_ADDR(v)   ((reg32_t) v)
#else
#define BF_PERFMON_TRAP_ADDR_HIGH_ADDR(v)   (v)
#endif
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_TRAP_ADDR_HIGH_ADDR(v)   (HW_PERFMON_TRAP_ADDR_HIGH.B.ADDR = (v))
#endif



/*
 * HW_PERFMON_LAT_THRESHOLD - PerfMon Latency Threshold Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned VALUE  : 12;
        unsigned RSVD0  : 20;
    } B;
} hw_perfmon_lat_threshold_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_LAT_THRESHOLD register
 */
#define HW_PERFMON_LAT_THRESHOLD_ADDR         (REGS_PERFMON_BASE + 0x40)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_LAT_THRESHOLD           (*(volatile hw_perfmon_lat_threshold_t *) HW_PERFMON_LAT_THRESHOLD_ADDR)
#define HW_PERFMON_LAT_THRESHOLD_RD()      (HW_PERFMON_LAT_THRESHOLD.U)
#define HW_PERFMON_LAT_THRESHOLD_WR(v)     (HW_PERFMON_LAT_THRESHOLD.U = (v))
#define HW_PERFMON_LAT_THRESHOLD_SET(v)    (HW_PERFMON_LAT_THRESHOLD_WR(HW_PERFMON_LAT_THRESHOLD_RD() |  (v)))
#define HW_PERFMON_LAT_THRESHOLD_CLR(v)    (HW_PERFMON_LAT_THRESHOLD_WR(HW_PERFMON_LAT_THRESHOLD_RD() & ~(v)))
#define HW_PERFMON_LAT_THRESHOLD_TOG(v)    (HW_PERFMON_LAT_THRESHOLD_WR(HW_PERFMON_LAT_THRESHOLD_RD() ^  (v)))
#endif


/*
 * constants & macros for individual HW_PERFMON_LAT_THRESHOLD bitfields
 */
/* --- Register HW_PERFMON_LAT_THRESHOLD, field RSVD0 */

#define BP_PERFMON_LAT_THRESHOLD_RSVD0      12
#define BM_PERFMON_LAT_THRESHOLD_RSVD0      0xFFFFF000

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_LAT_THRESHOLD_RSVD0(v)   ((((reg32_t) v) << 12) & BM_PERFMON_LAT_THRESHOLD_RSVD0)
#else
#define BF_PERFMON_LAT_THRESHOLD_RSVD0(v)   (((v) << 12) & BM_PERFMON_LAT_THRESHOLD_RSVD0)
#endif
/* --- Register HW_PERFMON_LAT_THRESHOLD, field VALUE */

#define BP_PERFMON_LAT_THRESHOLD_VALUE      0
#define BM_PERFMON_LAT_THRESHOLD_VALUE      0x00000FFF

#define BF_PERFMON_LAT_THRESHOLD_VALUE(v)   (((v) << 0) & BM_PERFMON_LAT_THRESHOLD_VALUE)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_LAT_THRESHOLD_VALUE(v)   BF_CS1(PERFMON_LAT_THRESHOLD, VALUE, v)
#endif



/*
 * HW_PERFMON_ACTIVE_CYCLE - PerfMon AXI Active Cycle Count Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned COUNT  : 32;
    } B;
} hw_perfmon_active_cycle_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_ACTIVE_CYCLE register
 */
#define HW_PERFMON_ACTIVE_CYCLE_ADDR         (REGS_PERFMON_BASE + 0x50)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_ACTIVE_CYCLE           (*(volatile hw_perfmon_active_cycle_t *) HW_PERFMON_ACTIVE_CYCLE_ADDR)
#define HW_PERFMON_ACTIVE_CYCLE_RD()      (HW_PERFMON_ACTIVE_CYCLE.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_ACTIVE_CYCLE bitfields
 */
/* --- Register HW_PERFMON_ACTIVE_CYCLE, field COUNT */

#define BP_PERFMON_ACTIVE_CYCLE_COUNT      0
#define BM_PERFMON_ACTIVE_CYCLE_COUNT      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_ACTIVE_CYCLE_COUNT(v)   ((reg32_t) v)
#else
#define BF_PERFMON_ACTIVE_CYCLE_COUNT(v)   (v)
#endif


/*
 * HW_PERFMON_TRANSFER_COUNT - PerfMon Transfer Count Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned VALUE  : 32;
    } B;
} hw_perfmon_transfer_count_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_TRANSFER_COUNT register
 */
#define HW_PERFMON_TRANSFER_COUNT_ADDR         (REGS_PERFMON_BASE + 0x60)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_TRANSFER_COUNT           (*(volatile hw_perfmon_transfer_count_t *) HW_PERFMON_TRANSFER_COUNT_ADDR)
#define HW_PERFMON_TRANSFER_COUNT_RD()      (HW_PERFMON_TRANSFER_COUNT.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_TRANSFER_COUNT bitfields
 */
/* --- Register HW_PERFMON_TRANSFER_COUNT, field VALUE */

#define BP_PERFMON_TRANSFER_COUNT_VALUE      0
#define BM_PERFMON_TRANSFER_COUNT_VALUE      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_TRANSFER_COUNT_VALUE(v)   ((reg32_t) v)
#else
#define BF_PERFMON_TRANSFER_COUNT_VALUE(v)   (v)
#endif


/*
 * HW_PERFMON_TOTAL_LATENCY - PerfMon Total Latency Count Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned COUNT  : 32;
    } B;
} hw_perfmon_total_latency_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_TOTAL_LATENCY register
 */
#define HW_PERFMON_TOTAL_LATENCY_ADDR         (REGS_PERFMON_BASE + 0x70)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_TOTAL_LATENCY           (*(volatile hw_perfmon_total_latency_t *) HW_PERFMON_TOTAL_LATENCY_ADDR)
#define HW_PERFMON_TOTAL_LATENCY_RD()      (HW_PERFMON_TOTAL_LATENCY.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_TOTAL_LATENCY bitfields
 */
/* --- Register HW_PERFMON_TOTAL_LATENCY, field COUNT */

#define BP_PERFMON_TOTAL_LATENCY_COUNT      0
#define BM_PERFMON_TOTAL_LATENCY_COUNT      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_TOTAL_LATENCY_COUNT(v)   ((reg32_t) v)
#else
#define BF_PERFMON_TOTAL_LATENCY_COUNT(v)   (v)
#endif


/*
 * HW_PERFMON_DATA_COUNT - PerfMon Total Data Count Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned COUNT  : 32;
    } B;
} hw_perfmon_data_count_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_DATA_COUNT register
 */
#define HW_PERFMON_DATA_COUNT_ADDR         (REGS_PERFMON_BASE + 0x80)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_DATA_COUNT           (*(volatile hw_perfmon_data_count_t *) HW_PERFMON_DATA_COUNT_ADDR)
#define HW_PERFMON_DATA_COUNT_RD()      (HW_PERFMON_DATA_COUNT.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_DATA_COUNT bitfields
 */
/* --- Register HW_PERFMON_DATA_COUNT, field COUNT */

#define BP_PERFMON_DATA_COUNT_COUNT      0
#define BM_PERFMON_DATA_COUNT_COUNT      0xFFFFFFFF

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_DATA_COUNT_COUNT(v)   ((reg32_t) v)
#else
#define BF_PERFMON_DATA_COUNT_COUNT(v)   (v)
#endif


/*
 * HW_PERFMON_MAX_LATENCY - PerfMon Maximum Latency Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned COUNT   : 12;
        unsigned RSVD0   :  3;
        unsigned TAGID   :  8;
        unsigned ASIZE   :  3;
        unsigned ALEN    :  4;
        unsigned ABURST  :  2;
    } B;
} hw_perfmon_max_latency_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_MAX_LATENCY register
 */
#define HW_PERFMON_MAX_LATENCY_ADDR         (REGS_PERFMON_BASE + 0x90)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_MAX_LATENCY           (*(volatile hw_perfmon_max_latency_t *) HW_PERFMON_MAX_LATENCY_ADDR)
#define HW_PERFMON_MAX_LATENCY_RD()      (HW_PERFMON_MAX_LATENCY.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_MAX_LATENCY bitfields
 */
/* --- Register HW_PERFMON_MAX_LATENCY, field ABURST */

#define BP_PERFMON_MAX_LATENCY_ABURST      30
#define BM_PERFMON_MAX_LATENCY_ABURST      0xC0000000

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_MAX_LATENCY_ABURST(v)   ((((reg32_t) v) << 30) & BM_PERFMON_MAX_LATENCY_ABURST)
#else
#define BF_PERFMON_MAX_LATENCY_ABURST(v)   (((v) << 30) & BM_PERFMON_MAX_LATENCY_ABURST)
#endif
/* --- Register HW_PERFMON_MAX_LATENCY, field ALEN */

#define BP_PERFMON_MAX_LATENCY_ALEN      26
#define BM_PERFMON_MAX_LATENCY_ALEN      0x3C000000

#define BF_PERFMON_MAX_LATENCY_ALEN(v)   (((v) << 26) & BM_PERFMON_MAX_LATENCY_ALEN)
/* --- Register HW_PERFMON_MAX_LATENCY, field ASIZE */

#define BP_PERFMON_MAX_LATENCY_ASIZE      23
#define BM_PERFMON_MAX_LATENCY_ASIZE      0x03800000

#define BF_PERFMON_MAX_LATENCY_ASIZE(v)   (((v) << 23) & BM_PERFMON_MAX_LATENCY_ASIZE)
/* --- Register HW_PERFMON_MAX_LATENCY, field TAGID */

#define BP_PERFMON_MAX_LATENCY_TAGID      15
#define BM_PERFMON_MAX_LATENCY_TAGID      0x007F8000

#define BF_PERFMON_MAX_LATENCY_TAGID(v)   (((v) << 15) & BM_PERFMON_MAX_LATENCY_TAGID)
/* --- Register HW_PERFMON_MAX_LATENCY, field RSVD0 */

#define BP_PERFMON_MAX_LATENCY_RSVD0      12
#define BM_PERFMON_MAX_LATENCY_RSVD0      0x00007000

#define BF_PERFMON_MAX_LATENCY_RSVD0(v)   (((v) << 12) & BM_PERFMON_MAX_LATENCY_RSVD0)
/* --- Register HW_PERFMON_MAX_LATENCY, field COUNT */

#define BP_PERFMON_MAX_LATENCY_COUNT      0
#define BM_PERFMON_MAX_LATENCY_COUNT      0x00000FFF

#define BF_PERFMON_MAX_LATENCY_COUNT(v)   (((v) << 0) & BM_PERFMON_MAX_LATENCY_COUNT)


/*
 * HW_PERFMON_DEBUG - PerfMon Debug Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned ERR_MID             :  1;
        unsigned TOTAL_CYCLE_CLR_EN  :  1;
        unsigned RSVD                : 30;
    } B;
} hw_perfmon_debug_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_DEBUG register
 */
#define HW_PERFMON_DEBUG_ADDR         (REGS_PERFMON_BASE + 0xa0)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_DEBUG           (*(volatile hw_perfmon_debug_t *) HW_PERFMON_DEBUG_ADDR)
#define HW_PERFMON_DEBUG_RD()      (HW_PERFMON_DEBUG.U)
#define HW_PERFMON_DEBUG_WR(v)     (HW_PERFMON_DEBUG.U = (v))
#define HW_PERFMON_DEBUG_SET(v)    (HW_PERFMON_DEBUG_WR(HW_PERFMON_DEBUG_RD() |  (v)))
#define HW_PERFMON_DEBUG_CLR(v)    (HW_PERFMON_DEBUG_WR(HW_PERFMON_DEBUG_RD() & ~(v)))
#define HW_PERFMON_DEBUG_TOG(v)    (HW_PERFMON_DEBUG_WR(HW_PERFMON_DEBUG_RD() ^  (v)))
#endif


/*
 * constants & macros for individual HW_PERFMON_DEBUG bitfields
 */
/* --- Register HW_PERFMON_DEBUG, field RSVD */

#define BP_PERFMON_DEBUG_RSVD      2
#define BM_PERFMON_DEBUG_RSVD      0xFFFFFFFC

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_DEBUG_RSVD(v)   ((((reg32_t) v) << 2) & BM_PERFMON_DEBUG_RSVD)
#else
#define BF_PERFMON_DEBUG_RSVD(v)   (((v) << 2) & BM_PERFMON_DEBUG_RSVD)
#endif
/* --- Register HW_PERFMON_DEBUG, field TOTAL_CYCLE_CLR_EN */

#define BP_PERFMON_DEBUG_TOTAL_CYCLE_CLR_EN      1
#define BM_PERFMON_DEBUG_TOTAL_CYCLE_CLR_EN      0x00000002

#define BF_PERFMON_DEBUG_TOTAL_CYCLE_CLR_EN(v)   (((v) << 1) & BM_PERFMON_DEBUG_TOTAL_CYCLE_CLR_EN)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_DEBUG_TOTAL_CYCLE_CLR_EN(v)   BF_CS1(PERFMON_DEBUG, TOTAL_CYCLE_CLR_EN, v)
#endif

/* --- Register HW_PERFMON_DEBUG, field ERR_MID */

#define BP_PERFMON_DEBUG_ERR_MID      0
#define BM_PERFMON_DEBUG_ERR_MID      0x00000001

#define BF_PERFMON_DEBUG_ERR_MID(v)   (((v) << 0) & BM_PERFMON_DEBUG_ERR_MID)
#ifndef __LANGUAGE_ASM__
#define BW_PERFMON_DEBUG_ERR_MID(v)   BF_CS1(PERFMON_DEBUG, ERR_MID, v)
#endif



/*
 * HW_PERFMON_VERSION - PerfMon Version Register
 */
#ifndef __LANGUAGE_ASM__
typedef union
{
    reg32_t  U;
    struct
    {
        unsigned STEP   : 16;
        unsigned MINOR  :  8;
        unsigned MAJOR  :  8;
    } B;
} hw_perfmon_version_t;
#endif

/*
 * constants & macros for entire HW_PERFMON_VERSION register
 */
#define HW_PERFMON_VERSION_ADDR         (REGS_PERFMON_BASE + 0xb0)

#ifndef __LANGUAGE_ASM__
#define HW_PERFMON_VERSION           (*(volatile hw_perfmon_version_t *) HW_PERFMON_VERSION_ADDR)
#define HW_PERFMON_VERSION_RD()      (HW_PERFMON_VERSION.U)
#endif


/*
 * constants & macros for individual HW_PERFMON_VERSION bitfields
 */
/* --- Register HW_PERFMON_VERSION, field MAJOR */

#define BP_PERFMON_VERSION_MAJOR      24
#define BM_PERFMON_VERSION_MAJOR      0xFF000000

#ifndef __LANGUAGE_ASM__
#define BF_PERFMON_VERSION_MAJOR(v)   ((((reg32_t) v) << 24) & BM_PERFMON_VERSION_MAJOR)
#else
#define BF_PERFMON_VERSION_MAJOR(v)   (((v) << 24) & BM_PERFMON_VERSION_MAJOR)
#endif
/* --- Register HW_PERFMON_VERSION, field MINOR */

#define BP_PERFMON_VERSION_MINOR      16
#define BM_PERFMON_VERSION_MINOR      0x00FF0000

#define BF_PERFMON_VERSION_MINOR(v)   (((v) << 16) & BM_PERFMON_VERSION_MINOR)
/* --- Register HW_PERFMON_VERSION, field STEP */

#define BP_PERFMON_VERSION_STEP      0
#define BM_PERFMON_VERSION_STEP      0x0000FFFF

#define BF_PERFMON_VERSION_STEP(v)   (((v) << 0) & BM_PERFMON_VERSION_STEP)

#endif /* _PERFMON_H */

////////////////////////////////////////////////////////////////////////////////