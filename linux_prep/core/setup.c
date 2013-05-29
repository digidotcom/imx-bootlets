/*
 * Linux parameters setting code
 *
 * - Parse a command line linux_prep section
 *
 * - Detect a magic key combination to choice a proper
 *   command line
 *
 * - Generate a list of ARM tags handled by Linux kernel
 *
 * Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 SigmaTel, Inc
 * Copyright 2008 Embedded Alley Solutions, Inc
 * Copyright 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program  is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include <setup.h>
#include <keys.h>
#include <stmp3xxx.h>
#include <arch/platform.h>
#include "../../mach-mx28/includes/registers/regsocotp.h"

static struct tag *params;
static char *cmdlines[MAGIC_KEY_NR];

struct ccardimx28_ident {
        const int       sdram;
        const int       flash;
        const char      *id_string;
};

/**
 * To add new valid variant ID, append new lines in this array with its configuration
 */
struct ccardimx28_ident ccardxmx28_id[] = {
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

/* minimal implementation of string functions */
static char *strstr(const char *s1, const char *s2)
{
	int i;

	if (*s1 == '\0')
		return *s2 ? 0 : (char *)s1;

	while (*s1) {
		for (i = 0; ; i++) {
			if (s2[i] == '\0')
				return (char *)s1;
			if (s2[i] != s1[i])
				break;
		}
		s1++;
	}
	return 0;
}

static int strlen(const char *s)
{
	const char *start = s;

	while (*s)
		s++;

	return s - start;
}

static char *strcpy(char *s1, const char *s2)
{
	char *s = s1;

	while ((*s1++ = *s2++) != '\0')
		;

	return s;
}

static void *memcpy(void *s1, const void *s2, int n)
{
	char *dst = s1;
	const char *src = s2;

	while (n-- > 0)
		*dst++ = *src++;

	return s1;
}

#ifdef MX28
#include "../../mach-mx28/includes/registers/regsrtc.h"
#elif defined(STMP378X)
#include "../../mach-mx23/includes/registers/regsrtc.h"
#endif

#define NAND_SECONDARY_BOOT          0x00000002

/***********************************************************************/

/*
 * Magic key detection
 */
static enum magic_key get_magic_key (void)
{
	u32 keys;
	int magic;

	/* Detect pressed keys */
	keys = get_pressed_keys();

	/*
	 * Check if detected keys combination corresponds to
	 * any magic key code
	 */
	for (magic = MAGIC_KEY1; magic < MAGIC_KEY_NR; magic++)
		if (magic_keys[magic] == keys)
			return magic;

	return NO_MAGIC_KEY;
}

/*
 * Command lines handling
 *
 * Go over a .cmdlines section. It must contatin \0 terminated
 * command lines storead one by one:
 *
 * 	command_line1\0command_line2\0command_line3\0
 *	|					     |
 * cmdlines_start				cmdlines_end
 */
static void find_command_lines (void)
{
	char *start = &cmdlines_start[0];
	char *end = &cmdlines_end[0];
	char *head, *cur;
	int l;

	head = cur = start;

	for (l = 0; l < MAGIC_KEY_NR && cur < end; l++) {

		/* Find a command line end */
		while (cur < end && *cur++ != '\0') ;

		/*
		 * If there is a garbage or unterminated command
		 * line at the end of the section then skip it.
		 */
		if (cur == end && *(cur-1) != '\0')
			break;

		cmdlines[l] = head;
		head = cur;
	}

	/*
	 * Bind default command line to magic keys not presented
	 * in .commandlines
	 */
	while (l < MAGIC_KEY_NR)
		cmdlines[l++] = cmdline_def;
}

/*
 * Calculate SDRAM size:
 *
 * size = #cs * 4 banks * #columns * #rows * 2 bytes/word
 */

#define MAX_ROW		13
#define MAX_COLUMN	12

static u32 get_sdram_size (void)
{
	u32 row, column, csmap, size;
	u32 cs = 0;

#if defined(BOARD_CCARDIMX28)
	unsigned long retries = 100000;
	unsigned char variant = 0;

	/* Open OTP banks */
	HW_OCOTP_CTRL_SET(BM_OCOTP_CTRL_RD_BANK_OPEN);
	while((BM_OCOTP_CTRL_BUSY & HW_OCOTP_CTRL_RD()) &&
	      retries--);

	/* Read variant from CUST1[15..8] */
	if (retries)
		variant = (HW_OCOTP_CUSTn_RD(1) >> 8) & 0xff;

	/* Close OTP banks */
	HW_OCOTP_CTRL_CLR(BM_OCOTP_CTRL_RD_BANK_OPEN);

	/* return sdram size of variant */
	if (0 != ccardxmx28_id[variant].sdram)
		return (ccardxmx28_id[variant].sdram);
#endif

#ifdef SDRAM_SIZE
	return SDRAM_SIZE;
#endif

	row = MAX_ROW -
	      ((HW_DRAM_CTL10_RD() & BM_DRAM_CTL10_ADDR_PINS) >> 16);

	column = MAX_COLUMN -
		 ((HW_DRAM_CTL11_RD() & BM_DRAM_CTL11_COLUMN_SIZE) >> 8);

	csmap = HW_DRAM_CTL14_RD() & BM_DRAM_CTL14_CS_MAP;

/*
 * Calculate the number of 1 bits in csmap
 * x & (x - 1) 	- clears the least significant 1 bit.
 */
	while (csmap) {
		csmap &= (csmap - 1);
		cs++;
	}

	size = cs * 4 * (1 << row) * (1 << column) * 2;

	return size;
}

/*
 * ARM atags
 */
static void setup_start_tag (void)
{
	params = (struct tag*)ATAGS_BASE_ADDRESS;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next(params);
}

static void setup_mem_tag (void)
{
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size(tag_mem32);

	params->u.mem.start = SDRAM_BASE;
	params->u.mem.size = get_sdram_size();

	params = tag_next(params);
}

static void setup_initrd_tag(void)
{
	params->hdr.tag = ATAG_INITRD2 ;
	params->hdr.size = tag_size(tag_initrd);

	params->u.initrd.start = 0x40800000;
	params->u.initrd.size =  0x00400000;

	params = tag_next(params);
}

static void setup_cmdline_tag (char *cmdline)
{

	u32 len;
	const char *mtdprefix[] = { "root=/dev/mtdblock", "root=ubi0:rootfs"};
	char *o, *m, *s, *rest;
	int nr, i, j, p, digits;
	unsigned int val;

	/* is a nand root present in the command line? */
	for (p = 0; p < sizeof(mtdprefix)/sizeof(mtdprefix[0]); p++) {
		m = strstr(cmdline, mtdprefix[p]);
		if (m)
			break;
	}

	if (!m)
		goto normal_copy;

	/* yes, we have to read the persistent bit now */

	/* start the RTC block */
	HW_RTC_CTRL_CLR(BM_RTC_CTRL_SFTRST | BM_RTC_CTRL_CLKGATE);

	while (HW_RTC_STAT_RD() & BF_RTC_STAT_STALE_REGS(0x1 << 1))
		;
	val = HW_RTC_PERSISTENT1_RD();

	/* normal boot, no need to modify cmdline */
	if ((val & NAND_SECONDARY_BOOT) == 0)
		goto normal_copy;

	/* add one to the rootfs partition number
	 * we assume that the secondary rootfs partition
	 * is the next one
	 */
	o = params->u.cmdline.cmdline;
	m += strlen(mtdprefix[p]);
	memcpy(o, cmdline, m - cmdline);
	o += m - cmdline;

	s = m;
	while (*s && *s >= '0' && *s <= '9')
		s++;

	rest = s;

	s--;
	nr = 0;
	while (s >= m) {
		nr = nr * 10 + (*s - '0');
		s--;
	}

	nr++;
	digits = 0;
	i = nr;
	while (i > 0) {
		digits++;
		i /= 10;
	}

	i = nr;
	j = digits;
	while (j > 0) {
		o[j - 1] = '0' + (i % 10);
		i /= 10;
		j--;
	}
	o += digits;
	strcpy(o, rest);

	goto done_copy;

normal_copy:
	/* Copy unchanged command line to SDRAM */
	strcpy(params->u.cmdline.cmdline, cmdline);

done_copy:
	/* Command line length including '\0' */
	len = strlen(params->u.cmdline.cmdline) + 1;

	params->hdr.tag = ATAG_CMDLINE;

	/*
	 * Tag size should be multiple of 4, so it is needed to round
	 * up a command line size.
	 */
	params->hdr.size = (sizeof(struct tag_header) + len + 4) >> 2;

	params = tag_next(params);
}

static void setup_end_tag (void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

/*
 * External interface to set up tags in SDRAM
 * Returns a pointer to tag list in SDRAM
 *
 * The following tags are passed to kernel in addition to start and end
 * tags which must always exist:
 *
 * - Memory tag
 * - Command line tag
 */
u32 setup_tags (void)
{
	enum magic_key magic_key;

	magic_key = get_magic_key();
	find_command_lines();

	setup_start_tag();
	setup_mem_tag();
#if !defined(BOARD_CPX2) && !defined(BOARD_CCARDIMX28) && !defined(BOARD_WR21)
	setup_initrd_tag();
#endif
	setup_cmdline_tag(cmdlines[magic_key]);
	setup_end_tag();

	return (u32)ATAGS_BASE_ADDRESS;
}
