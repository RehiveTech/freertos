/**
 * platform_dependent.h
 * Copyright (C) 2014 Jan Viktorin
 */

#ifndef PLATFORM_DEPENDENT_H
#define PLATFORM_DEPENDENT_H

/* Altera library includes. */
#include "alt_timers.h"
#include "alt_clock_manager.h"
#include "alt_interrupt.h"
#include "alt_globaltmr.h"
#include "alt_address_space.h"
#include "mmu_support.h"
#include "cache_support.h"

void platSetupHardware(void);

#define PLAT_DECLARE_COHERENT_MEM(name, size) \
	uint8_t name[(size)]

static inline
int platInitCoherentMemory(uint8_t *m, size_t size)
{
	return -1;
}

#endif
