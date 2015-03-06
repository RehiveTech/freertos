include ../Makefile.inc

ZYNQ_SRC  := $(FREERTOS_ROOT)/zynq
ZYNQ_BSP  := $(FREERTOS_ROOT)/zynq/bsp
BSP_INC   := -I$(ZYNQ_BSP)/ps7_cortexa9_0/include

CFLAGS_EXTRA := $(BSP_INC) -I$(ZYNQ_SRC)
ASFLAGS_EXTRA := $(BSP_INC) -I$(ZYNQ_SRC)
LDFLAGS_EXTRA += -Xlinker -Tlscript.ld
LDFLAGS_EXTRA += -Xlinker --gc-sections

all: test
test: test.o printf-stdarg.o platform.o serial.o asm_vectors.o tick_config.o libfreertos.a libhw.a libzynq.a
clean-test:
	$(RM) test.o printf-stdarg.o platform.o serial.o asm_vectors.o tick_config.o
clean: clean-test

ZYNQ_OBJ  := _exit.o _open.o _sbrk.o abort.o close.o errno.o fcntl.o \
             fstat.o getpid.o inbyte.o isatty.o kill.o lseek.o open.o \
             outbyte.o print.o putnum.o read.o sbrk.o sleep.o smc.o uart.o \
             unlink.o usleep.o vectors.o write.o xil_assert.o xil_cache.o \
             xil_exception.o xil_io.o xil_misc_psreset_api.o xil_mmu.o \
             xil_printf.o xl2cc_counter.o xpm_counter.o xtime_l.o \
	     cpu_init.o asm_vectors.o boot.o translation_table.o
SCU_OBJ := xscugic.o xscugic_g.o xscugic_hw.o xscugic_intr.o xscugic_sinit.o
SCUWDT_OBJ := xscuwdt.o xscuwdt_g.o xscuwdt_sinit.o
UARTPS_OBJ := xuartps_g.o xuartps_hw.o xuartps_intr.o xuartps.o xuartps_options.o  xuartps_sinit.o
SCUTMR_OBJ := xscutimer.o xscutimer_g.o xscutimer_sinit.o
USBPS_OBJ  := xusbps_endpoint.o xusbps_g.o xusbps_hw.o xusbps_intr.o xusbps.o xusbps_sinit.o
GPIOPS_OBJ := xgpiops.o xgpiops_g.o xgpiops_hw.o xgpiops_intr.o xgpiops_sinit.o
DMAPS_OBJ  := xdmaps.o xdmaps_g.o xdmaps_hw.o xdmaps_sinit.o
DEVCFG_OBJ := xdevcfg.o xdevcfg_g.o xdevcfg_hw.o xdevcfg_intr.o xdevcfg_sinit.o
SDPS_OBJ   := xsdps.o xsdps_g.o xsdps_options.o xsdps_sinit.o
EMACPS_OBJ := xemacps.o xemacps_bdring.o xemacps_control.o xemacps_g.o xemacps_hw.o xemacps_intr.o xemacps_sinit.o
QSPIPS_OBJ := xqspips.o xqspips_g.o xqspips_hw.o xqspips_options.o xqspips_sinit.o
IICPS_OBJ  := xiicps_g.o xiicps_hw.o xiicps_intr.o xiicps_options.o xiicps_sinit.o \
              xiicps.o xiicps_master.o xiicps_slave.o
TTCPS_OBJ  := xttcps.o xttcps_g.o xttcps_options.o xttcps_sinit.o
XADCPS_OBJ := xadcps.o xadcps_g.o xadcps_intr.o xadcps_sinit.o

libzynq.a: $(ZYNQ_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/standalone_v4_1/src/%)
	$(AR) rcs $@ $^
clean-libzynq:
	$(RM) $(ZYNQ_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/standalone_v4_1/src/%)
distclean-libzynq:
	$(RM) libzynq.a

clean: clean-libzynq
distclean: distclean-libzynq

libhw.a: $(SCU_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scugic_v2_1/src/%) \
         $(SCUWDT_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scuwdt_v2_0/src/%) \
         $(UARTPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/uartps_v2_1/src/%) \
         $(SCUTMR_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scutimer_v2_0/src/%) \
         $(USBPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/usbps_v2_1/src/%) \
         $(GPIOPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/%) \
         $(DMAPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/dmaps_v2_0/src/%) \
         $(DEVCFG_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/devcfg_v3_1/src/%) \
         $(SDPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/sdps_v2_1/src/%) \
         $(EMACPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/emacps_v2_0/src/%) \
         $(QSPIPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/qspips_v3_0/src/%) \
         $(TTCPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/ttcps_v2_0/src/%) \
         $(XADCPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/xadcps_v2_0/src/%) \
         $(IICPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/iicps_v2_1/src/%)
	$(AR) rcs $@ $^
clean-libhw:
	$(RM) $(SCU_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scugic_v2_1/src/%)
	$(RM) $(SCUWDT_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scuwdt_v2_0/src/%)
	$(RM) $(UARTPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/uartps_v2_1/src/%)
	$(RM) $(SCUTMR_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/scutimer_v2_0/src/%)
	$(RM) $(USBPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/usbps_v2_1/src/%)
	$(RM) $(GPIOPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/%)
	$(RM) $(DMAPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/dmaps_v2_0/src/%)
	$(RM) $(DEVCFG_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/devcfg_v3_1/src/%)
	$(RM) $(SDPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/sdps_v2_1/src/%)
	$(RM) $(EMACPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/emacps_v2_0/src/%)
	$(RM) $(QSPIPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/qspips_v3_0/src/%)
	$(RM) $(TTCPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/ttcps_v2_0/src/%)
	$(RM) $(XADCPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/xadcps_v2_0/src/%)
	$(RM) $(IICPS_OBJ:%=$(ZYNQ_BSP)/ps7_cortexa9_0/libsrc/iicps_v2_1/src/%)
distclean-libhw:
	$(RM) libhw.a

clean: clean-libhw
distclean: distclean-libhw
