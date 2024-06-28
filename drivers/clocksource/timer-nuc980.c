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
#define TIMER4			NUC980_VA_TIMER45
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
#define		AIC_BA		0	/* Interrupt Controller */
#define		REG_AIC_SRCCTL0		(AIC_BA+0x00)	/* Source control register 0 */
#define		REG_AIC_SRCCTL1		(AIC_BA+0x04)	/* Source control register 1 */
#define		REG_AIC_SRCCTL2		(AIC_BA+0x08)	/* Source control register 2 */
#define		REG_AIC_SRCCTL3		(AIC_BA+0x0C)	/* Source control register 3 */
#define		REG_AIC_SRCCTL4		(AIC_BA+0x10)	/* Source control register 4 */
#define		REG_AIC_SRCCTL5		(AIC_BA+0x14)	/* Source control register 5 */
#define		REG_AIC_SRCCTL6		(AIC_BA+0x18)	/* Source control register 6 */
#define		REG_AIC_SRCCTL7		(AIC_BA+0x1C)	/* Source control register 7 */
#define		REG_AIC_SRCCTL8		(AIC_BA+0x20)	/* Source control register 8 */
#define		REG_AIC_SRCCTL9		(AIC_BA+0x24)	/* Source control register 9 */
#define		REG_AIC_SRCCTL10	(AIC_BA+0x28)	/* Source control register 10 */
#define		REG_AIC_SRCCTL11	(AIC_BA+0x2C)	/* Source control register 11 */
#define		REG_AIC_SRCCTL12	(AIC_BA+0x30)	/* Source control register 12 */
#define		REG_AIC_SRCCTL13	(AIC_BA+0x34)	/* Source control register 13 */
#define		REG_AIC_SRCCTL14	(AIC_BA+0x38)	/* Source control register 14 */
#define		REG_AIC_SRCCTL15	(AIC_BA+0x3C)	/* Source control register 15 */
#define		REG_AIC_IRQNUM		(AIC_BA+0x120)	/* IRQ source number register */
#define		REG_AIC_FIQNUM		(AIC_BA+0x124)	/* FIQ source number register */
#define		REG_AIC_INTMSK0		(AIC_BA+0x128)	/* Interrupt mask register 0 */
#define		REG_AIC_INTMSK1		(AIC_BA+0x12C)	/* Interrupt mask register 1 */
#define		REG_AIC_INTEN0		(AIC_BA+0x130)	/* Mask enable command register 0 */
#define		REG_AIC_INTEN1		(AIC_BA+0x134)	/* Mask enable command register 1 */
#define		REG_AIC_INTDIS0		(AIC_BA+0x138)	/* Mask disable command register 0 */
#define		REG_AIC_INTDIS1		(AIC_BA+0x13C)	/* Mask disable command register 1 */
#define		REG_AIC_EOIS		(AIC_BA+0x150)	/* End of IRQ service command register */

#define NUC980_IRQ(x)	(x)
#define IRQ_TIMER4	NUC980_IRQ(32)

#define PERIOD		(0x1 << 4)
#define ONESHOT		(0x0)
#define COUNTEN		(0x1)

#define TICKS_PER_SEC	100
#define PRESCALE	0x63	/* Divider = prescale + 1 */

#define	TDR_SHIFT	24
#define	TDR_MASK	((1 << TDR_SHIFT) - 1)

static unsigned int timer4_load;

static inline void nuc980_timer_shutdown(struct clock_event_device *evt)
{
	/* disable timer */
	__raw_writel(0x00, REG_TIMER_CTL(TIMER4));
}

static int nuc980_shutdown(struct clock_event_device *evt)
{
	nuc980_timer_shutdown(evt);

	return 0;
}

int nuc980_set_periodic(struct clock_event_device *clk)
{
	unsigned int val;

	val = __raw_readl(REG_TIMER_CTL(TIMER4)) & ~(0x03 << 4);
	__raw_writel(timer4_load, REG_TIMER_CMPR(TIMER4));
	val |= (PERIOD | COUNTEN);
	__raw_writel(val, REG_TIMER_CTL(TIMER4));
	return 0;
}

int nuc980_set_oneshot(struct clock_event_device *clk)
{
	unsigned int val;

	val = __raw_readl(REG_TIMER_CTL(TIMER4)) & ~(0x03 << 4);
	val |= (ONESHOT | COUNTEN);
	__raw_writel(val, REG_TIMER_CTL(TIMER4));
	return 0;
}

static int nuc980_clockevent_setnextevent(unsigned long evt,
					  struct clock_event_device *clk)
{
	__raw_writel(0, REG_TIMER_CTL(TIMER4));
	__raw_writel(evt, REG_TIMER_CMPR(TIMER4));
	while(__raw_readl(REG_TIMER_DR(TIMER4)) != 0);
	__raw_writel(__raw_readl(REG_TIMER_CTL(TIMER4)) | COUNTEN,
		     REG_TIMER_CTL(TIMER4));

	return 0;
}

#ifdef CONFIG_PM
static int tmr4_msk;
static void nuc980_clockevent_suspend(struct clock_event_device *clk)
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
static struct clock_event_device nuc980_clockevent_device = {
	.name = "nuc980-timer4",
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
};

/*IRQ handler for the timer*/
static irqreturn_t nuc980_timer4_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &nuc980_clockevent_device;

	__raw_writel(1, REG_TIMER_ISR(TIMER4));	/* clear interrupt */
	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct timer_of nuc980_to = {
    .flags = TIMER_OF_IRQ | TIMER_OF_BASE | TIMER_OF_CLOCK,
    .clkevt = {
        .name = "nuc980-timer4",
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
        .handler = nuc980_timer4_interrupt,
        .flags = IRQF_TIMER | IRQF_IRQPOLL
    }
};

static void __init nuc980_clockevents_init(void)
{
    printk("Entered nuc980_clockevents_init\n");
	unsigned int rate;
	struct clk *clk = clk_get(NULL, "timer4");
	struct clk *eclk = clk_get(NULL, "timer4_eclk");

	BUG_ON(IS_ERR(clk));
	BUG_ON(IS_ERR(eclk));

	clk_prepare(clk);
	clk_enable(clk);
	clk_prepare(eclk);
	clk_enable(eclk);

	__raw_writel(0, REG_TIMER_CTL(TIMER4));
	rate = clk_get_rate(eclk);

	timer4_load = (rate / TICKS_PER_SEC);

	__raw_writel(1, REG_TIMER_ISR(TIMER4));
	__raw_writel(0, REG_TIMER_PRECNT(TIMER4));
	__raw_writel(1, REG_TIMER_IER(TIMER4));
	//setup_irq(IRQ_TIMER4, &nuc980_timer4_irq); //schung
	if (request_irq(IRQ_TIMER4, nuc980_timer4_interrupt, IRQF_TIMER|IRQF_IRQPOLL, "timer", NULL))
                pr_err("Failed to request irq %d (timer)\n", IRQ_TIMER4);


	nuc980_clockevent_device.cpumask = cpumask_of(0);

	clockevents_config_and_register(&nuc980_clockevent_device, rate, 12, 0xffffff);
}

static u64 read_sched_clock(void)
{
	return __raw_readl(REG_TIMER_DR(TIMER5));
}

static void __init nuc980_clocksource_init(void)
{
    printk("Entered nuc980_clocksource_init\n");
	unsigned int rate = 0;
	struct clk *clk = clk_get(NULL, "timer5");
	struct clk *eclk = clk_get(NULL, "timer5_eclk");

	BUG_ON(IS_ERR(clk));
	BUG_ON(IS_ERR(eclk));

	clk_prepare(clk);
	clk_enable(clk);
	clk_prepare(eclk);
	clk_enable(eclk);

	__raw_writel(0x00, REG_TIMER_CTL(TIMER5));

	rate = clk_get_rate(eclk) / (PRESCALE + 1);

	__raw_writel(0xffffffff, REG_TIMER_CMPR(TIMER5));
	__raw_writel(PRESCALE, REG_TIMER_PRECNT(TIMER5));

	__raw_writel(COUNTEN | PERIOD, REG_TIMER_CTL(TIMER5));

	clocksource_mmio_init(REG_TIMER_DR(TIMER5),
		"nuc980-timer5", rate, 200, 24, clocksource_mmio_readl_up);
	sched_clock_register(read_sched_clock, 24, rate);
}

static int __init nuc980_timer_init(struct device_node *np)
{
    struct clk *clk;
	int ret;

    printk("Initializing nuc980_timer\n");
    ret = timer_of_init(np, &nuc980_to);
	if (ret)
		return ret;

	/* Enable the clock for timer1, if it exists */
	clk = of_clk_get(np, 1);
	if (clk) {
		if (!IS_ERR(clk))
			clk_prepare_enable(clk);
		else
			pr_warn("%pOF: Failed to get clock for timer1: %pe", np, clk);
	}

	nuc980_clocksource_init();
	nuc980_clockevents_init();

	pr_info("Enabling NUC980 clocksource timer base: %px, IRQ: %d ",
		timer_of_base(&nuc980_to), timer_of_irq(&nuc980_to));

    return 0;
}

TIMER_OF_DECLARE(nuc980, "nuvoton,nuc980-timer", nuc980_timer_init);
