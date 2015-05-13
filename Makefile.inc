CROSS ?= arm-none-eabi-
CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld
AR = $(CROSS)ar
SIZE = $(CROSS)size
OBJCOPY = $(CROSS)objcopy
Q ?= @
PLAT ?= zynq

FREERTOS_ROOT ?= $(PWD)/..
FREERTOS_SRC  ?= $(FREERTOS_ROOT)/Source

ASFLAGS_port   := -march=armv7-a -mcpu=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=softfp -g
ASFLAGS         = $(ASFLAGS_port) $(ASFLAGS_EXTRA)
CFLAGS_include := -I$(FREERTOS_SRC)/include -I$(FREERTOS_SRC)/portable/GCC/ARM_CA9 -I$(FREERTOS_ROOT)/Common/include
CFLAGS_port    := -march=armv7-a -mtune=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=softfp -O2 -g
CFLAGS          = -std=gnu99 -fno-short-enums $(CFLAGS_include) $(CFLAGS_port) $(CFLAGS_EXTRA)
LDFLAGS_port   := -march=armv7-a -mtune=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=softfp -O2 -g
LDFLAGS         = -std=gnu99 $(LDFLAGS_port) $(LDFLAGS_EXTRA)

FREERTOS_OBJ   := croutine.o event_groups.o list.o queue.o tasks.o timers.o

all:
libfreertos.a: $(FREERTOS_OBJ) port.o portASM.o
	$(Q) $(AR) rcs $@ $^

define build_obj
$(1): $(2)
	$(Q) $$(CC) $$(CFLAGS) -c -o $$@ $$^
endef

$(foreach obj,$(FREERTOS_OBJ),$(eval $(call build_obj,$(obj),$(FREERTOS_SRC)/$(obj:%.o=%.c))))

heap_%.o: $(FREERTOS_SRC)/portable/MemMang/heap_%.c
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
port.o: $(FREERTOS_SRC)/portable/GCC/ARM_CA9/port.c
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
portASM.o: $(FREERTOS_SRC)/portable/GCC/ARM_CA9/portASM.S
	$(Q) $(AS) $(ASFLAGS) -c -o $@ $<

%.bin: %
	$(Q) $(OBJCOPY) -O binary $< $@
%.bin: %.elf
	$(Q) $(OBJCOPY) -O binary $< $@

clean: clean-libfreertos
clean-libfreertos:
	$(Q) $(RM) $(FREERTOS_OBJ)
	$(Q) $(RM) port.o portASM.o
distclean: clean distclean-libfreertos
distclean-libfreertos:
	$(Q) $(RM) libfreertos.a

include $(FREERTOS_ROOT)/$(PLAT)/Makefile.platform
