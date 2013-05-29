CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)
OUTPUTDIR	= $(CURRENTDIR)/output-target
COMMONSRCDIR	= $(CURRENTDIR)/../mach-$(ARCH)

INCLUDEDIRS	= -I$(COMMONSRCDIR) -I$(COMMONSRCDIR)/includes

# Linker script
BASE_ADDR	?= 0x00000000
LAYOUT	        = $(SRCDIR)/link.lds


CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

CFLAGS 	+= -Wall $(INCLUDEDIRS) -I./ -O -D$(ARCH) -g -Wa,-ahlsn=$(basename $@).lst
LDFLAGS = -static -gc-sections -nostdlib -T $(LAYOUT) -M -Map=$(basename $@).map
# Do not let gcc 'optimize' nor complain about our hacked stdio functions
CFLAGS 	+= -fno-builtin-printf -fno-builtin-putc

CFLAGS += $(MYCFLAGS)

# Default goal
all: power_prep

%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.arm
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

#
# Make targets
#

.PHONY: all clean

ifneq ($(findstring $(BOARD), stmp37xx_dev stmp378x_dev iMX28_EVK),)

# Use original Freescale bootlet for Freescale boards

# Freescale makefile sets this for all boards
CFLAGS += -DSTMP378x

PP_OBJS = power_prep.o debug.o eabi.o

PP_LIBS += $(COMMONSRCDIR)/hw/power/libhw_power.a
PP_LIBS += $(COMMONSRCDIR)/hw/lradc/libhw_lradc.a
PP_LIBS += $(COMMONSRCDIR)/hw/icoll/libhw_icoll.a
PP_LIBS += $(COMMONSRCDIR)/hw/digctl/libhw_digctl.a
PP_LIBS += $(COMMONSRCDIR)/hw/core/libhw_core.a
PP_LIBS += $(COMMONSRCDIR)/drivers/power/libddi_power.a

power_prep: $(LAYOUT) $(PP_LIBS) $(PP_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(PP_OBJS) $(PP_LIBS)

$(PP_LIBS):
	$(MAKE) -C $(COMMONSRCDIR)/hw
	$(MAKE) -C $(COMMONSRCDIR)/drivers

clean:
	@echo Cleaning...
	rm -f power_prep power_prep.map
	$(MAKE) clean -C $(COMMONSRCDIR)/hw
	$(MAKE) clean -C $(COMMONSRCDIR)/drivers
	rm -f $(PP_OBJS)

else

# Use simple bootlet for new boards

# ccardimx28 platforms use the libraries for the battery code
# cpx2 platform's bootlet are based on the FSL bootlet, so it is using the libraries
ifneq ($(findstring $(BOARD), CCARDIMX28JS CPX2),)
PP_LIBS += $(COMMONSRCDIR)/hw/power/libhw_power.a
PP_LIBS += $(COMMONSRCDIR)/hw/lradc/libhw_lradc.a
PP_LIBS += $(COMMONSRCDIR)/hw/icoll/libhw_icoll.a
PP_LIBS += $(COMMONSRCDIR)/hw/digctl/libhw_digctl.a
PP_LIBS += $(COMMONSRCDIR)/hw/core/libhw_core.a
PP_LIBS += $(COMMONSRCDIR)/drivers/power/libddi_power.a

$(PP_LIBS):
	$(MAKE) -C $(COMMONSRCDIR)/hw
	$(MAKE) -C $(COMMONSRCDIR)/drivers
endif

PP_OBJS = power_prep_$(BOARD).o debug.o eabi.o

power_prep: $(LAYOUT) $(PP_LIBS) $(PP_OBJS) $(COMMON_OBJS)
	$(LD) -o $@ $(LDFLAGS) $(PP_OBJS) $(PP_LIBS) $(COMMON_OBJS)

clean:
	@echo Cleaning...
	rm -f power_prep power_prep.map
	rm -f $(PP_OBJS) $(addsuffix .lst,$(basename $(PP_OBJS)))
endif
