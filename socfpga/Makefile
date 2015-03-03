include ../Makefile.inc

SOCFPGA_SRC  := $(FREERTOS_ROOT)/socfpga
SOCFPGA_BSP  := $(FREERTOS_ROOT)/socfpga/bsp
BSP_INC      := -I$(SOCFPGA_BSP)/SoCSupport/include -I$(SOCFPGA_BSP)/HardwareLibrary/include

CFLAGS_EXTRA := $(BSP_INC) -I$(SOCFPGA_SRC)
LDFLAGS_EXTRA := -Tlscript.ld

all: test
test: test.o printf-stdarg.o libfreertos.a libsocfpga.a libhw.a

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

$(foreach obj,$(SOCFPGA_OBJ),\
	$(eval $(call build_obj,\
		$(obj:%=$(SOCFPGA_BSP)/SoCSupport/%),\
		$(obj:%.o=$(SOCFPGA_BSP)/SoCSupport/%.c))))
clean: clean-libsocfpga
distclean: distclean-libsocfpga

libhw.a: $(HWLIB_OBJ:%=$(SOCFPGA_BSP)/HardwareLibrary/%)
	$(AR) rcs $@ $^
clean-libhw:
	$(RM) $(HWLIB_OBJ:%=$(SOCFPGA_BSP)/HardwareLibrary/%)
distclean-libhw:
	$(RM) libhw.a

$(foreach obj,$(HWLIB_OBJ),\
	$(eval $(call build_obj,\
		$(obj:%=$(SOCFPGA_BSP)/HardwareLibrary/%),\
		$(obj:%.o=$(SOCFPGA_BSP)/HardwareLibrary/%.c))))
clean: clean-libhw
distclean: distclean-libhw
