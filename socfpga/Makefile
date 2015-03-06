include ../Makefile.inc

SOCFPGA_SRC  := $(FREERTOS_ROOT)/socfpga
SOCFPGA_BSP  := $(FREERTOS_ROOT)/socfpga/bsp
BSP_INC      := -I$(SOCFPGA_BSP)/SoCSupport/include -I$(SOCFPGA_BSP)/HardwareLibrary/include

CFLAGS_EXTRA := $(BSP_INC) -I$(SOCFPGA_SRC)
CFLAGS_EXTRA += -DALT_INT_PROVISION_VECTOR_SUPPORT=0
LDFLAGS_EXTRA := -Xlinker -Tlscript.ld
LDFLAGS_EXTRA += -Xlinker --gc-sections
LDFLAGS_EXTRA += -Xlinker --defsym=__cs3_isr_irq=FreeRTOS_IRQ_Handler
LDFLAGS_EXTRA += -Xlinker --defsym=__cs3_isr_swi=FreeRTOS_SWI_Handler

all: test
test: test.o printf-stdarg.o serial.o libfreertos.a libsocfpga.a libhw.a
test-clean:
	$(RM) test.o printf-stdarg.o serial.o
clean: test-clean

SOCFPGA_OBJ  := cache_support.o fpga_support.o mmu_support.o uart0_support.o
HWLIB_OBJ    := alt_16550_uart.o alt_address_space.o alt_bridge_manager.o \
                alt_cache.o alt_clock_manager.o alt_dma.o alt_dma_program.o \
                alt_ecc.o alt_fpga_manager.o alt_generalpurpose_io.o alt_globaltmr.o \
                alt_i2c.o alt_interrupt.o alt_mmu.o alt_nand.o alt_qspi.o \
                alt_reset_manager.o alt_sdmmc.o alt_spi.o alt_system_manager.o \
                alt_timers.o alt_watchdog.o

libsocfpga.a: $(SOCFPGA_OBJ:%=$(SOCFPGA_BSP)/SoCSupport/%)
	$(AR) rcs $@ $^
clean-libsocfpga:
	$(RM) $(SOCFPGA_OBJ:%=$(SOCFPGA_BSP)/SoCSupport/%)
distclean-libsocfpga:
	$(RM) libsocfpga.a

clean: clean-libsocfpga
distclean: distclean-libsocfpga

libhw.a: $(HWLIB_OBJ:%=$(SOCFPGA_BSP)/HardwareLibrary/%)
	$(AR) rcs $@ $^
clean-libhw:
	$(RM) $(HWLIB_OBJ:%=$(SOCFPGA_BSP)/HardwareLibrary/%)
distclean-libhw:
	$(RM) libhw.a

clean: clean-libhw
distclean: distclean-libhw
