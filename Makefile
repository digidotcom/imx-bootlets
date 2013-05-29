export CROSS_COMPILE
MEM_TYPE ?= MEM_DDR1
export MEM_TYPE

BOARD ?= stmp378x_dev

ifeq ($(BOARD), stmp37xx_dev)
ARCH = 37xx
endif
ifeq ($(BOARD), stmp378x_dev)
ARCH = mx23
endif
ifeq ($(BOARD), iMX28_EVK)
ARCH = mx28
endif
ifeq ($(BOARD), CPX2)
ARCH = mx28
endif
ifeq ($(BOARD), CCARDIMX28JS)
ARCH = mx28
endif
ifeq ($(BOARD), WR21)
ARCH = mx28
endif

all: build_prep

build_prep: linux_prep boot_prep power_prep linux.bd uboot.bd linux.bd uboot.bd updater.bd

power_prep:
	@echo "build power_prep"
	$(MAKE) -C power_prep ARCH=$(ARCH) BOARD=$(BOARD) MYCFLAGS="$(MYCFLAGS)"

boot_prep:
	@echo "build boot_prep"
	$(MAKE) -C boot_prep  ARCH=$(ARCH) BOARD=$(BOARD) MYCFLAGS="$(MYCFLAGS)"

linux_prep:
ifneq "$(CMDLINE1)" ""
	@echo "Using environment command line"
	@echo -e "$(CMDLINE1)\n$(CMDLINE2)\n$(CMDLINE3)\n$(CMDLINE4)" \
		> linux_prep/cmdlines/$(BOARD).txt
else
	@echo "Using the pre-built command line"
endif
	# force building linux_prep
	$(MAKE) clean -C linux_prep
	@echo "cross-compiling linux_prep"
	$(MAKE) -C linux_prep ARCH=$(ARCH) BOARD=$(BOARD) MYCFLAGS="$(MYCFLAGS)"

install:
	cp -f boot_prep/boot_prep  ${DESTDIR}
	cp -f power_prep/power_prep  ${DESTDIR}
	cp -f linux_prep/output-target/linux_prep ${DESTDIR}
	cp -f ./*.bd ${DESTDIR}
	cp -f ./create_updater.sh  ${DESTDIR}

distclean: clean
clean:
	-rm -rf *.sb
	rm -f sd_mmc_bootstream.raw
	$(MAKE) -C linux_prep clean ARCH=$(ARCH)
	$(MAKE) -C boot_prep clean ARCH=$(ARCH)
	$(MAKE) -C power_prep clean ARCH=$(ARCH)

.PHONY: all build_prep linux_prep boot_prep power_prep distclean clean

