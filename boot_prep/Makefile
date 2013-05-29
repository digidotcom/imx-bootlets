CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)

INCLUDEDIR	= $(SRCDIR)/../mach-$(ARCH)/includes/registers

# Linker script
BASE_ADDR	?= 0x00000000
LAYOUT  	= $(SRCDIR)/link.lds


CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

MEM_TYPE ?= MEM_DDR1

CFLAGS 	= -g -Wall -I$(INCLUDEDIR) -I./ -O -D$(MEM_TYPE) -Wa,-ahlsn=$(basename $@).lst
LDFLAGS = -static -nostdlib -T $(LAYOUT) -M -Map=$(basename $@).map
# Do not let gcc 'optimize' nor complain about our hacked stdio functions
CFLAGS 	+= -fno-builtin-printf -fno-builtin-putc

CFLAGS += $(MYCFLAGS)

# Default goal
all: boot_prep

%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

#
# Make targets
#

.PHONY: all clean

ifneq ($(findstring $(BOARD), stmp37xx_dev stmp378x_dev iMX28_EVK),)

# Use original Freescale bootlet for Freescale boards
BP_OBJS = init-$(ARCH).o debug.o

else

# Use new style for new boards
BP_OBJS = boot_prep_$(BOARD).o debug.o

endif

boot_prep: $(LAYOUT) $(BP_OBJS)
	$(LD) -o $@ $(BP_OBJS) $(LDFLAGS)

clean:
	@echo Cleaning...
	rm -rf *.o *.lst
	rm -f boot_prep boot_prep.map
