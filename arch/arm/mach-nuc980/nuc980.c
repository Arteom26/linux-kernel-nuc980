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

/* Initial IO mappings */
static struct map_desc nuc980_iodesc[] __initdata = {
	IODESC_ENT(IRQ),
	IODESC_ENT(GCR_CLK),
	IODESC_ENT(SDIC),
	IODESC_ENT(EBI),
	IODESC_ENT(GPIO),
	IODESC_ENT(EMAC0),
	IODESC_ENT(EMAC1),
	IODESC_ENT(PDMA0),
	IODESC_ENT(PDMA1),
	IODESC_ENT(EHCI),
	IODESC_ENT(OHCI),
	IODESC_ENT(USBDEV),
	IODESC_ENT(I2S),
	IODESC_ENT(SDH),
	IODESC_ENT(FMI),
	IODESC_ENT(CCAP0),
	IODESC_ENT(CCAP1),
	IODESC_ENT(CRYPTO),
	IODESC_ENT(WDT_WWDT),
	IODESC_ENT(RTC),
	IODESC_ENT(SC0),
	IODESC_ENT(SC1),
	IODESC_ENT(I2C0),
	IODESC_ENT(I2C1),
	IODESC_ENT(I2C2),
	IODESC_ENT(I2C3),
	IODESC_ENT(UART0),
	IODESC_ENT(UART1),
	IODESC_ENT(UART2),
	IODESC_ENT(UART3),
	IODESC_ENT(UART4),
	IODESC_ENT(UART5),
	IODESC_ENT(UART6),
	IODESC_ENT(UART7),
	IODESC_ENT(UART8),
	IODESC_ENT(UART9),
	IODESC_ENT(SPI0),
	IODESC_ENT(SPI1),
	IODESC_ENT(SPI2),
	IODESC_ENT(TIMER01),
	IODESC_ENT(TIMER23),
	IODESC_ENT(TIMER45),
	IODESC_ENT(PWM0),
	IODESC_ENT(PWM1),
	IODESC_ENT(ADC),
	IODESC_ENT(CAN0),
	IODESC_ENT(CAN1),
	IODESC_ENT(CAN2),
	IODESC_ENT(CAN3),
	IODESC_ENT(SRAM),
};
extern void nuc980_restart(enum reboot_mode mode, const char *cmd);

static void __init nuc980_map_io(void)
{
	iotable_init(nuc980_iodesc, ARRAY_SIZE(nuc980_iodesc));
}

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
	// .map_io			= nuc980_map_io,
	.init_machine 	= nuc980_dt_device_init,
	.dt_compat		= nuc980_dt_board_compat,
	// .init_late		= nuc980_init_late,
	// .restart		= nuc980_restart,
MACHINE_END
