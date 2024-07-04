/*
 * linux/arch/arm/mach-nuc980/time.c
 *
 *
 * Copyright (c) 2017 Nuvoton technology corporation
 * All rights reserved.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/leds.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/of_address.h>

#include <linux/module.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clkdev.h>
#include <linux/sched_clock.h>
#include <linux/irq.h>

#include <asm/mach-types.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

#include "timer-of.h"

#define NUC980_VA_TIMER45 0
#define TIMER5			(NUC980_VA_TIMER45 + 0x100)

#define REG_TIMER_CTL(x)	(void __iomem *)((x) + 0x00)
#define REG_TIMER_PRECNT(x)	(void __iomem *)((x) + 0x04)
#define REG_TIMER_CMPR(x)	(void __iomem *)((x) + 0x08)
#define REG_TIMER_IER(x)	(void __iomem *)((x) + 0x0C)
#define REG_TIMER_ISR(x)	(void __iomem *)((x) + 0x10)
#define REG_TIMER_DR(x)		(void __iomem *)((x) + 0x14)
#define REG_TIMER_TCAP(x)	(void __iomem *)((x) + 0x18)
#define REG_TIMER_ECTL(x)	(void __iomem *)((x) + 0x20)

/* Advance Interrupt Controller (AIC) Registers */
#define		REG_AIC_SRCCTL0		(0x00)	/* Source control register 0 */
#define		REG_AIC_SRCCTL1		(0x04)	/* Source control register 1 */
#define		REG_AIC_SRCCTL2		(0x08)	/* Source control register 2 */
#define		REG_AIC_SRCCTL3		(0x0C)	/* Source control register 3 */
#define		REG_AIC_SRCCTL4		(0x10)	/* Source control register 4 */
#define		REG_AIC_SRCCTL5		(0x14)	/* Source control register 5 */
#define		REG_AIC_SRCCTL6		(0x18)	/* Source control register 6 */
#define		REG_AIC_SRCCTL7		(0x1C)	/* Source control register 7 */
#define		REG_AIC_SRCCTL8		(0x20)	/* Source control register 8 */
#define		REG_AIC_SRCCTL9		(0x24)	/* Source control register 9 */
#define		REG_AIC_SRCCTL10	(0x28)	/* Source control register 10 */
#define		REG_AIC_SRCCTL11	(0x2C)	/* Source control register 11 */
#define		REG_AIC_SRCCTL12	(0x30)	/* Source control register 12 */
#define		REG_AIC_SRCCTL13	(0x34)	/* Source control register 13 */
#define		REG_AIC_SRCCTL14	(0x38)	/* Source control register 14 */
#define		REG_AIC_SRCCTL15	(0x3C)	/* Source control register 15 */
#define		REG_AIC_IRQNUM		(0x120)	/* IRQ source number register */
#define		REG_AIC_FIQNUM		(0x124)	/* FIQ source number register */
#define		REG_AIC_INTMSK0		(0x128)	/* Interrupt mask register 0 */
#define		REG_AIC_INTMSK1		(0x12C)	/* Interrupt mask register 1 */
#define		REG_AIC_INTEN0		(0x130)	/* Mask enable command register 0 */
#define		REG_AIC_INTEN1		(0x134)	/* Mask enable command register 1 */
#define		REG_AIC_INTDIS0		(0x138)	/* Mask disable command register 0 */
#define		REG_AIC_INTDIS1		(0x13C)	/* Mask disable command register 1 */
#define		REG_AIC_EOIS		(0x150)	/* End of IRQ service command register */

#define NUC980_IRQ(x)	(x)
#define IRQ_TIMER4	NUC980_IRQ(32)

#define PERIOD		(0x1 << 4)
#define ONESHOT		(0x0)
#define COUNTEN		(0x1)

#define TICKS_PER_SEC	100
#define PRESCALE	0x63	/* Divider = prescale + 1 */

#define	TDR_SHIFT	24
#define	TDR_MASK	((1 << TDR_SHIFT) - 1)

static unsigned int timer_load;
static void __iomem *base_clocksourse_regs;

static inline void nuc980_timer_shutdown(struct clock_event_device *evt)
{
	struct timer_of *to = to_timer_of(evt);

	/* disable timer */
	__raw_writel(0x00, REG_TIMER_CTL(timer_of_base(to)));
}

static int nuc980_shutdown(struct clock_event_device *evt)
{
	nuc980_timer_shutdown(evt);

	return 0;
}

static int nuc980_set_periodic(struct clock_event_device *evt)
{
	struct timer_of *to = to_timer_of(evt);
	unsigned int val;

	val = __raw_readl(REG_TIMER_CTL(timer_of_base(to))) & ~(0x03 << 4);
	__raw_writel(timer_load, REG_TIMER_CMPR(timer_of_base(to)));
	val |= (PERIOD | COUNTEN);
	__raw_writel(val, REG_TIMER_CTL(timer_of_base(to)));
	return 0;
}

static int nuc980_set_oneshot(struct clock_event_device *evt)
{
	struct timer_of *to = to_timer_of(evt);
	unsigned int val;

	val = __raw_readl(REG_TIMER_CTL(timer_of_base(to))) & ~(0x03 << 4);
	val |= (ONESHOT | COUNTEN);
	__raw_writel(val, REG_TIMER_CTL(timer_of_base(to)));
	return 0;
}

static int nuc980_clockevent_setnextevent(unsigned long evt, struct clock_event_device *clk)
{
	struct timer_of *to = to_timer_of(clk);

	__raw_writel(0, REG_TIMER_CTL(timer_of_base(to)));
	__raw_writel(evt, REG_TIMER_CMPR(timer_of_base(to)));
	while(__raw_readl(REG_TIMER_DR(timer_of_base(to))) != 0);
	__raw_writel(__raw_readl(REG_TIMER_CTL(timer_of_base(to))) | COUNTEN,
		     REG_TIMER_CTL(timer_of_base(to)));

	return 0;
}

#ifdef CONFIG_PM
static int tmr4_msk;
static void nuc980_clockevent_suspend(struct clock_event_device *evt)
{
	unsigned long flags;

	local_irq_save(flags);
	if (__raw_readl(REG_AIC_INTMSK1) & (1 << 0)) {
		tmr4_msk = 1;
		__raw_writel(0x1, REG_AIC_INTDIS1);
	} else
		tmr4_msk = 0;

	local_irq_restore(flags);

}

static void nuc980_clockevent_resume(struct clock_event_device *clk)
{
	unsigned long flags;

	local_irq_save(flags);
	if (tmr4_msk == 1)
		__raw_writel(0x1, REG_AIC_INTEN1);
	local_irq_restore(flags);
}
#endif

/*IRQ handler for the timer*/
static irqreturn_t nuc980_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = (struct clock_event_device *)dev_id;
	struct timer_of *to = to_timer_of(evt);

	__raw_writel(1, REG_TIMER_ISR(timer_of_base(to)));	/* clear interrupt */
	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct timer_of nuc980_to = {
    .flags = TIMER_OF_IRQ | TIMER_OF_BASE | TIMER_OF_CLOCK,
    .clkevt = {
        .name = "nuc980-clkevt",
        .shift = 24,
        .features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
        .set_state_shutdown = nuc980_shutdown,
        .set_state_oneshot = nuc980_set_oneshot,
        .set_state_periodic = nuc980_set_periodic,
        .tick_resume = nuc980_shutdown,
        .set_next_event = nuc980_clockevent_setnextevent,
    #ifdef CONFIG_PM
        .suspend = nuc980_clockevent_suspend,
        .resume = nuc980_clockevent_resume,
    #endif
        .rating = 300,
    },
    .of_irq = {
        .handler = nuc980_timer_interrupt,
        .flags = IRQF_TIMER | IRQF_IRQPOLL
    }
};

static void __init nuc980_clockevents_init(void)
{
	unsigned int rate;

	__raw_writel(0, REG_TIMER_CTL(timer_of_base(&nuc980_to)));
	rate = nuc980_to.of_clk.rate;

	timer_load = (rate / TICKS_PER_SEC);

	__raw_writel(1, REG_TIMER_ISR(timer_of_base(&nuc980_to)));
	__raw_writel(0, REG_TIMER_PRECNT(timer_of_base(&nuc980_to)));
	__raw_writel(1, REG_TIMER_IER(timer_of_base(&nuc980_to)));

	nuc980_to.clkevt.cpumask = cpumask_of(0);
	clockevents_config_and_register(&nuc980_to.clkevt, rate, 12, 0xffffff);
}

static u64 read_sched_clock(void)
{
	return __raw_readl(REG_TIMER_DR(base_clocksourse_regs));
}

static void __init nuc980_clocksource_init(struct device_node *np)
{
	struct resource res;
	unsigned int rate = 0;

	// Retrieve memory resource from device tree
    if (of_address_to_resource(np, 0, &res)){
        pr_err("Failed to get memory resource\n");
	}

	// Remap register memory
	base_clocksourse_regs = ioremap(res.start, resource_size(&res));
	if (!base_clocksourse_regs)
		pr_err("Failed to get memory resource\n");

	// Enable timer clock
	struct clk *clk = of_clk_get(np, 0);
	if(clk_prepare_enable(clk))
		pr_warn("Unable to enable clock %pe", clk);

	__raw_writel(0x00, REG_TIMER_CTL(base_clocksourse_regs));

	rate = clk_get_rate(clk) / (PRESCALE + 1);

	__raw_writel(0xffffffff, REG_TIMER_CMPR(base_clocksourse_regs));
	__raw_writel(PRESCALE, REG_TIMER_PRECNT(base_clocksourse_regs));

	__raw_writel(COUNTEN | PERIOD, REG_TIMER_CTL(base_clocksourse_regs));

	clocksource_mmio_init(REG_TIMER_DR(base_clocksourse_regs), "nuc980-clksource", rate, 200, 24, clocksource_mmio_readl_up);
	sched_clock_register(read_sched_clock, 24, rate);
}

static int __init nuc980_timer_init(struct device_node *np)
{
	static int init_count = 0;
    struct clk *clk;
	int ret;

	if(init_count >= 2)
		goto err;

	/* Enable the clock for timer */
	clk = of_clk_get(np, 0);
	if (clk) {
		if (!IS_ERR(clk))
			clk_prepare_enable(clk);
		else
			pr_warn("%pOF: Failed to get clock for timer: %pe", np, clk);
	}

	if(!init_count)
	{
		ret = timer_of_init(np, &nuc980_to);
		if (ret)
			goto err;
		nuc980_clockevents_init();

		pr_info("Enabling NUC980 clockevents timer base: %px, IRQ: %d ",
			timer_of_base(&nuc980_to), timer_of_irq(&nuc980_to));
	}
	else
	{
		nuc980_clocksource_init(np);

		pr_info("Enabling NUC980 clocksource timer base: %px", base_clocksourse_regs);
	}

	init_count++;
    return 0;

err:
	pr_err("Unable to initialize NUC980 Timer, Code: %d", ret);
	return ret;
}

TIMER_OF_DECLARE(nuc980, "nuvoton,nuc980-timer", nuc980_timer_init);
