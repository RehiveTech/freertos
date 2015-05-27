/**
 * platform_dependent.h
 * Copyright (C) 2014 Jan Viktorin
 */

#ifndef PLATFORM_DEPENDENT_H
#define PLATFORM_DEPENDENT_H

#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_mmu.h"

#define L1_CACHE_LINE_SIZE 32

void platSetupHardware(void);

#define __PLAT_COHERENT_GRANULARITY_SIZE (0x100000)
#define __PLAT_COHERENT_GRANULARITY_MASK (__PLAT_COHERENT_GRANULARITY_SIZE - 1)
#define __plat_coherent_align(size) \
	(((size) & ~__PLAT_COHERENT_GRANULARITY_MASK)? \
		(((size) + __PLAT_COHERENT_GRANULARITY_SIZE) & ~__PLAT_COHERENT_GRANULARITY_MASK) : \
		(size))

#define PLAT_DECLARE_COHERENT_MEM(name, size) \
	uint8_t name[__plat_coherent_align(size)]

int platInitCoherentMemory(uint8_t *m, size_t size);

#endif
