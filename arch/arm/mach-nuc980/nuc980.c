// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2018 Nuvoton Technology corporation.
// Copyright 2018 Google, Inc.

#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <asm/mach/map.h>
#include <asm/hardware/cache-l2x0.h>

#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>

#include "include/mach/regs-clock.h"
#include "include/mach/regs-gcr.h"
#include "include/mach/map.h"

#include "cpu.h"
#include "pm.h"

static void __init nuc980_init_late(void)
{
	nuc980_init_suspend();
}

static void __init nuc980_dt_device_init(void)
{
	printk(KERN_INFO "<DT> %s +\n", __func__);
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
	printk(KERN_INFO "<DT> %s -\n", __func__);
}

static const char *const nuc980_dt_board_compat[] = {
	"nuvoton,nuc980",
	"nuvoton,nuc980-dev-v1.0",
	NULL
};

DT_MACHINE_START(NUC980_DT, "Nuvoton NUC980 Board")
	.atag_offset	= 0x100,
	.init_machine 	= nuc980_dt_device_init,
	.dt_compat		= nuc980_dt_board_compat,
	.nr_irqs 		= 64,
	.restart		= nuc980_restart,
	.init_late		= nuc980_init_late
MACHINE_END
