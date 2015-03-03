CROSS ?= arm-none-eabi-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AR = $(CROSS)ar

FREERTOS_ROOT ?= $(PWD)/..
FREERTOS_SRC  ?= $(FREERTOS_ROOT)/Source

HEAP ?= heap_1

CFLAGS_include := -I$(FREERTOS_SRC)/include -I$(FREERTOS_SRC)/portable/GCC/ARM_CA9
CFLAGS_port    := -march=armv7-a -mtune=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=softfp -O2 -g
CFLAGS          = -std=gnu99 $(CFLAGS_include) $(CFLAGS_port) $(CFLAGS_EXTRA)
LDFLAGS_port   := -march=armv7-a -mtune=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=softfp -O2 -g
LDFLAGS         = -std=gnu99 $(LDFLAGS_port) $(LDFLAGS_EXTRA)

FREERTOS_OBJ   := croutine.o event_groups.o list.o queue.o tasks.o timers.o

all:
libfreertos.a: $(FREERTOS_OBJ) heap.o port.o
	$(AR) rcs $@ $^

define build_obj
$(1): $(2)
	$$(CC) $$(CFLAGS) -c -o $$@ $$^
endef

$(foreach obj,$(FREERTOS_OBJ),$(eval $(call build_obj,$(obj),$(FREERTOS_SRC)/$(obj:%.o=%.c))))

heap.o: $(FREERTOS_SRC)/portable/MemMang/$(HEAP).c
	$(CC) $(CFLAGS) -c -o $@ $<
port.o: $(FREERTOS_SRC)/portable/GCC/ARM_CA9/port.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean: clean-libfreertos
clean-libfreertos:
	$(RM) $(FREERTOS_OBJ)
	$(RM) heap.o
distclean: clean distclean-libfreertos
distclean-libfreertos:
	$(RM) libfreertos.a
