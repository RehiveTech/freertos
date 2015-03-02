include ../Makefile.inc

SOCFPGA_SRC  := $(FREERTOS_ROOT)/socfpga
BSP_INC      := -I$(SOCFPGA_SRC)/bsp/SoCSupport/include -I$(SOCFPGA_SRC)/bsp/HardwareLibrary/include

CFLAGS_EXTRA := $(BSP_INC) -I$(SOCFPGA_SRC)

all: test
test: test.o libfreertos.a
