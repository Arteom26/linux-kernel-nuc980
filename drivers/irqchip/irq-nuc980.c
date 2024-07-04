/*
 * linux/arch/arm/mach-nuc980/irq.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <asm/exception.h>

#include <asm/irq.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/irqchip.h>
#include <asm/mach/irq.h>

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
#define		REG_AIC_EOFS		(0x154)	/* End of FIQ service command register */

#define NUC980_IRQ(x)	(x)

/* Main cpu interrupts */

#define IRQ_WDT		NUC980_IRQ(1)
#define IRQ_WWDT	NUC980_IRQ(2)
#define IRQ_LVD		NUC980_IRQ(3)
#define IRQ_EXT0	NUC980_IRQ(4)
#define IRQ_EXT1	NUC980_IRQ(5)
#define IRQ_EXT2	NUC980_IRQ(6)
#define IRQ_EXT3	NUC980_IRQ(7)
#define IRQ_GPA		NUC980_IRQ(8)
#define IRQ_GPB		NUC980_IRQ(9)
#define IRQ_GPC		NUC980_IRQ(10)
#define IRQ_GPD		NUC980_IRQ(11)
#define IRQ_I2S		NUC980_IRQ(12)

#define IRQ_CAP0	NUC980_IRQ(14)
#define IRQ_RTC		NUC980_IRQ(15)
#define IRQ_TIMER0	NUC980_IRQ(16)
#define IRQ_TIMER1	NUC980_IRQ(17)
#define IRQ_ADC		NUC980_IRQ(18)
#define IRQ_EMC0RX	NUC980_IRQ(19)
#define IRQ_EMC1RX	NUC980_IRQ(20)
#define IRQ_EMC0TX	NUC980_IRQ(21)
#define IRQ_EMC1TX	NUC980_IRQ(22)
#define IRQ_EHCI	NUC980_IRQ(23)
#define IRQ_OHCI	NUC980_IRQ(24)
#define IRQ_PDMA0	NUC980_IRQ(25)
#define IRQ_PDMA1	NUC980_IRQ(26)
#define IRQ_SDH		NUC980_IRQ(27)
#define IRQ_FMI		NUC980_IRQ(28)
#define IRQ_UDC		NUC980_IRQ(29)
#define IRQ_TIMER2	NUC980_IRQ(30)
#define IRQ_TIMER3	NUC980_IRQ(31)
#define IRQ_TIMER4	NUC980_IRQ(32)
#define IRQ_CAP1	NUC980_IRQ(33)
#define IRQ_TIMER5	NUC980_IRQ(34)
#define IRQ_CRYPTO	NUC980_IRQ(35)
#define IRQ_UART0	NUC980_IRQ(36)
#define IRQ_UART1	NUC980_IRQ(37)
#define IRQ_UART2	NUC980_IRQ(38)
#define IRQ_UART4	NUC980_IRQ(39)
#define IRQ_UART6	NUC980_IRQ(40)
#define IRQ_UART8	NUC980_IRQ(41)
#define IRQ_CAN3	NUC980_IRQ(42)
#define IRQ_UART3	NUC980_IRQ(43)
#define IRQ_UART5	NUC980_IRQ(44)
#define IRQ_UART7	NUC980_IRQ(45)
#define IRQ_UART9	NUC980_IRQ(46)
#define IRQ_I2C2	NUC980_IRQ(47)
#define IRQ_I2C3	NUC980_IRQ(48)
#define IRQ_GPE		NUC980_IRQ(49)
#define IRQ_SPI2	NUC980_IRQ(50)
#define IRQ_SPI0	NUC980_IRQ(51)
#define IRQ_SPI1	NUC980_IRQ(52)
#define IRQ_I2C0	NUC980_IRQ(53)
#define IRQ_I2C1	NUC980_IRQ(54)
#define IRQ_SMC0	NUC980_IRQ(55)
#define IRQ_SMC1	NUC980_IRQ(56)
#define IRQ_GPF		NUC980_IRQ(57)
#define IRQ_CAN0	NUC980_IRQ(58)
#define IRQ_CAN1	NUC980_IRQ(59)
#define IRQ_PWM0	NUC980_IRQ(60)
#define IRQ_PWM1	NUC980_IRQ(61)
#define IRQ_CAN2	NUC980_IRQ(62)
#define IRQ_GPG		NUC980_IRQ(63)

#ifndef CONFIG_GPIO_NUC980
#define SPARE_IRQS	(64)
#define NOF_IRQS		(IRQ_GPG + SPARE_IRQS + 1)

#else
#define EXT0_BASE 100
#define IRQ_EXT0_A0           NUC980_IRQ(EXT0_BASE + 0)
#define IRQ_EXT1_A1           NUC980_IRQ(EXT0_BASE + 1)
#define IRQ_EXT2_D0           NUC980_IRQ(EXT0_BASE + 2)
#define IRQ_EXT3_D1           NUC980_IRQ(EXT0_BASE + 3)

#define IRQ_EXT0_A13          NUC980_IRQ(EXT0_BASE + 4)
#define IRQ_EXT1_A14          NUC980_IRQ(EXT0_BASE + 5)
#define IRQ_EXT2_E10          NUC980_IRQ(EXT0_BASE + 6)
#define IRQ_EXT3_E12          NUC980_IRQ(EXT0_BASE + 7)

#define IRQ_EXT2_B3           NUC980_IRQ(EXT0_BASE + 8)
#define IRQ_EXT2_B13          NUC980_IRQ(EXT0_BASE + 9)
#define IRQ_EXT3_G15          NUC980_IRQ(EXT0_BASE +10)

#define IRQ_GPIO_START	NUC980_IRQ(NUC980_IRQ(0x100))
#define IRQ_GPIO_END		NUC980_IRQ(NUC980_IRQ(0x100 + 0xE0))

#define SPARE_IRQS		(64)

#define NOF_IRQS			(IRQ_GPIO_END + SPARE_IRQS +1)

#endif

#if 0
#define ENTRY()                 printk("[%-20s] : Enter...\n", __FUNCTION__)
#define LEAVE()                 printk("[%-20s] : Leave...\n", __FUNCTION__)
#else
#define ENTRY()
#define LEAVE()
#endif

static void __iomem *reg_base;

static void nuc980_irq_mask(struct irq_data *d)
{
	if (d->hwirq < 32)
		__raw_writel(1 << (d->hwirq), reg_base + REG_AIC_INTDIS0);
	else
		__raw_writel(1 << (d->hwirq - 32), reg_base + REG_AIC_INTDIS1);
}

static void nuc980_irq_ack(struct irq_data *d)
{
	__raw_writel(0x01, reg_base + REG_AIC_EOIS);
}

static void nuc980_irq_unmask(struct irq_data *d)
{
	if (d->hwirq < 32)
		__raw_writel(1 << (d->hwirq), reg_base + REG_AIC_INTEN0);
	else
		__raw_writel(1 << (d->hwirq - 32), reg_base + REG_AIC_INTEN1);
}

static int nuc980_irq_set_wake(struct irq_data *d, unsigned int on)
{
	return 0;
}

static struct irq_chip nuc980_irq_chip = {
	.irq_disable = nuc980_irq_mask,
	.irq_enable = nuc980_irq_unmask,
	.irq_ack = nuc980_irq_ack,
	.irq_mask = nuc980_irq_mask,
	.irq_unmask = nuc980_irq_unmask,
	.irq_set_wake = nuc980_irq_set_wake,
};

#if defined(CONFIG_GPIO_NUC980)

static const unsigned int Port[7] = {
	(unsigned int)REG_GPIOA_MODE,
	(unsigned int)REG_GPIOB_MODE,
	(unsigned int)REG_GPIOC_MODE,
	(unsigned int)REG_GPIOD_MODE,
	(unsigned int)REG_GPIOE_MODE,
	(unsigned int)REG_GPIOF_MODE,
	(unsigned int)REG_GPIOG_MODE
};

static unsigned int gpio_type[8];
static unsigned int gpio_inten[8];

static void nuc980_irq_gpio_mask(struct irq_data *d)
{
	unsigned int port,num;
	ENTRY();
	port =(d->irq-IRQ_GPIO_START)/GPIO_OFFSET;
	num  =(d->irq-IRQ_GPIO_START)%GPIO_OFFSET;
	__raw_writel(__raw_readl((volatile unsigned int *)(Port[port]+0x1C)) &~(0x1<<(num+16)),(volatile unsigned int *)(Port[port]+0x1C));
	__raw_writel(__raw_readl((volatile unsigned int *)(Port[port]+0x1C)) &~(0x1<<num),(volatile unsigned int *)(Port[port]+0x1C));
	LEAVE();
}

static void nuc980_irq_gpio_ack(struct irq_data *d)
{
	ENTRY();
	__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}

static void nuc980_irq_gpio_unmask(struct irq_data *d)
{
	unsigned int port,num,tmp;
	ENTRY();
	port =(d->irq-IRQ_GPIO_START)/GPIO_OFFSET;
	num  =(d->irq-IRQ_GPIO_START)%GPIO_OFFSET;
	tmp = gpio_type[port] & (0x1<<(num));
	__raw_writel(__raw_readl((volatile unsigned int *)(Port[port]+0x18))|tmp,(volatile unsigned int *)(Port[port]+0x18));
	tmp = gpio_inten[port] & (0x1<<(num+16));
	__raw_writel(__raw_readl((volatile unsigned int *)(Port[port]+0x1C))|tmp,(volatile unsigned int *)(Port[port]+0x1C));
	tmp = gpio_inten[port] & (0x1<<num);
	__raw_writel(__raw_readl((volatile unsigned int *)(Port[port]+0x1C))|tmp,(volatile unsigned int *)(Port[port]+0x1C));
	LEAVE();

}

static int nuc980_irq_gpio_type(struct irq_data *d, unsigned int type)
{
	unsigned int port, num;
	ENTRY();
	port = (d->irq - IRQ_GPIO_START) / GPIO_OFFSET;
	num = (d->irq - IRQ_GPIO_START) % GPIO_OFFSET;

	if (type == IRQ_TYPE_PROBE) {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x18)) & ~(0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x18));
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) | (0x1 << num) | ((0x1<<num)<<16),
		             (volatile unsigned int *)(Port[port] + 0x1C));
		gpio_type[port] &= ~(0x1<<num);
		gpio_inten[port] |= (0x1<<num);
		gpio_inten[port] |= (0x1<<(num+16));
		return 0;
	}
	if (type & IRQ_TYPE_LEVEL_MASK) {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x18)) | (0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x18));
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) & ~((0x1 << num)|((0x1<<num)<<16)),
		             (volatile unsigned int *)(Port[port] + 0x1C));
		gpio_type[port] |= (0x1<<num);
		gpio_inten[port] &= ~(0x1<<num);
		gpio_inten[port] &= ~(0x1<<(num+16));
		if (type == IRQ_TYPE_LEVEL_HIGH) {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) | ((0x1 <<
			                                                num)<<16),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] |=(0x1<<(num+16));
			return 0;
		}
		if (type == IRQ_TYPE_LEVEL_LOW) {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) | (0x1 <<
			                                                num),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] |=(0x1<<num);
			return 0;
		}
	} else {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x18)) & ~(0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x18));
		gpio_type[port] &= ~(0x1<<num);
		if (type & IRQ_TYPE_EDGE_RISING) {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) | ((0x1 <<
			                                                num)<<16),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] |=(0x1<<(num+16));
		} else {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) & ~((0x1<<
			                                                num)<<16),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] &= ~(0x1<<(num+16));
		}
		if (type & IRQ_TYPE_EDGE_FALLING) {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) | (0x1 <<
			                                                num),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] |= (0x1<<num);
		} else {
			__raw_writel(__raw_readl
			             ((volatile unsigned int *)(Port[port] +
			                                        0x1C)) & ~(0x1
			                                                <<
			                                                num),
			             (volatile unsigned int *)(Port[port] +
			                                       0x1C));
			gpio_inten[port] &= ~(0x1<<num);
		}
	}
	return 0;
}

static struct irq_chip nuc980_irq_gpio = {
	.name = "GPIO-IRQ",
	.irq_disable = nuc980_irq_gpio_mask,
	.irq_enable = nuc980_irq_gpio_unmask,
	.irq_ack = nuc980_irq_gpio_ack,
	.irq_mask = nuc980_irq_gpio_mask,
	.irq_unmask = nuc980_irq_gpio_unmask,
	.irq_set_type = nuc980_irq_gpio_type,
	.irq_set_wake = nuc980_irq_set_wake,
};

static void nuc980_irq_demux_intgroupA(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOA_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOA_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   0 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupB(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOB_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOB_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   1 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupC(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOC_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOC_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   2 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupD(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOD_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOD_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   3 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupE(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOE_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOE_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   4 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupF(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOF_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOF_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   5 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}
static void nuc980_irq_demux_intgroupG(struct irq_desc *desc)
{
	unsigned int j, isr;
	unsigned int flag=0;
	ENTRY();
	isr = __raw_readl(reg_base + REG_GPIOG_INTSRC);
	__raw_writel(isr, reg_base + REG_GPIOG_INTSRC);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag=1;
			generic_handle_irq(IRQ_GPIO_START +
			                   6 * 0x20 + j);
		}
		isr = isr >> 1;
	}
	if(flag==0)
		__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}

//------------------------------------------------------------------------------

static const unsigned int EXT[11]= {
	(unsigned int)NUC980_PA0,
	(unsigned int)NUC980_PA1,
	(unsigned int)NUC980_PD0,
	(unsigned int)NUC980_PD1,
	(unsigned int)NUC980_PA13,
	(unsigned int)NUC980_PA14,
	(unsigned int)NUC980_PE10,
	(unsigned int)NUC980_PE12,
	(unsigned int)NUC980_PB3,
	(unsigned int)NUC980_PB13,
	(unsigned int)NUC980_PG15,
};

static void nuc980_irq_ext_mask(struct irq_data *d)
{
	ENTRY();
	if(d->irq==IRQ_EXT0_A0 || d->irq==IRQ_EXT0_A13)
		__raw_writel(1<<IRQ_EXT0, reg_base + REG_AIC_INTDIS0);
	else if(d->irq==IRQ_EXT1_A1 || d->irq==IRQ_EXT1_A14)
		__raw_writel(1 <<IRQ_EXT1, reg_base + REG_AIC_INTDIS0);
	else if(d->irq==IRQ_EXT2_D0 || d->irq==IRQ_EXT2_E10 || d->irq==IRQ_EXT2_B3 || d->irq==IRQ_EXT2_B13)
		__raw_writel(1 <<IRQ_EXT2, reg_base + REG_AIC_INTDIS0);
	else if(d->irq==IRQ_EXT3_D1 || d->irq==IRQ_EXT3_E12 || d->irq==IRQ_EXT3_G15)
		__raw_writel(1 <<IRQ_EXT3, reg_base + REG_AIC_INTDIS0);
	LEAVE();
}

static void nuc980_irq_ext_ack(struct irq_data *d)
{
	ENTRY();
	__raw_writel(0x01, REG_AIC_EOIS);
	LEAVE();
}

static void nuc980_irq_ext_unmask(struct irq_data *d)
{
	ENTRY();
	if(d->irq==IRQ_EXT0_A0 || d->irq==IRQ_EXT0_A13)
		__raw_writel(1<<IRQ_EXT0, reg_base + REG_AIC_INTEN0);
	else if(d->irq==IRQ_EXT1_A1 || d->irq==IRQ_EXT1_A14)
		__raw_writel(1 <<IRQ_EXT1, reg_base + REG_AIC_INTEN0);
	else if(d->irq==IRQ_EXT2_D0 || d->irq==IRQ_EXT2_E10 || d->irq==IRQ_EXT2_B3 || d->irq==IRQ_EXT2_B13)
		__raw_writel(1 <<IRQ_EXT2, reg_base + REG_AIC_INTEN0);
	else if(d->irq==IRQ_EXT3_D1 || d->irq==IRQ_EXT3_E12 || d->irq==IRQ_EXT3_G15)
		__raw_writel(1 <<IRQ_EXT3, reg_base + REG_AIC_INTEN0);
	LEAVE();
}

static int nuc980_irq_ext_type(struct irq_data *d, unsigned int type)
{
	unsigned int port, num;
	ENTRY();
	port = (EXT[d->irq - EXT0_BASE]) / GPIO_OFFSET;
	num = (EXT[d->irq - EXT0_BASE]) % GPIO_OFFSET;
	if (type == IRQ_TYPE_PROBE) {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) | (0x1 << (num+16)),
		             (volatile unsigned int *)(Port[port] + 0x1C));
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) | (0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x1C));
		return 0;
	}

	if (type & IRQ_TYPE_EDGE_RISING) {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) | (0x1 << (num+16)),
		             (volatile unsigned int *)(Port[port] + 0x1C));
	} else
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) & ~(0x1 << (num+16)),
		             (volatile unsigned int *)(Port[port] + 0x1C));

	if (type & IRQ_TYPE_EDGE_FALLING) {
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) | (0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x1C));
	} else
		__raw_writel(__raw_readl
		             ((volatile unsigned int *)(Port[port] +
		                                        0x1C)) & ~(0x1 << num),
		             (volatile unsigned int *)(Port[port] + 0x1C));
	LEAVE();
	return 0;
}

static struct irq_chip nuc980_irq_ext = {
	.name = "EXT-IRQ",
	.irq_disable = nuc980_irq_ext_mask,
	.irq_enable = nuc980_irq_ext_unmask,
	.irq_ack = nuc980_irq_ext_ack,
	.irq_mask = nuc980_irq_ext_mask,
	.irq_unmask = nuc980_irq_ext_unmask,
	.irq_set_type = nuc980_irq_ext_type,
	.irq_set_wake = nuc980_irq_set_wake,
};

static void nuc980_irq_demux_intgroup2(struct irq_desc *desc)
{
	unsigned int port0, num0, port1, num1;
	unsigned int port2, num2, port3, num3;
	unsigned int irq = irq_desc_get_irq(desc);
	ENTRY();
	port0 = EXT[irq - 4] / GPIO_OFFSET;
	num0 = EXT[irq - 4] % GPIO_OFFSET;
	port1 = EXT[irq - 4 + 4] / GPIO_OFFSET;
	num1 = EXT[irq - 4 + 4] % GPIO_OFFSET;
	switch (irq) {
	case IRQ_EXT0:
		if (__raw_readl((volatile unsigned int *)(Port[port0] + 0x20)) &
		    (1 << num0)) {
			generic_handle_irq(IRQ_EXT0_A0);
			__raw_writel(0x1 << num0,
			             (volatile unsigned int *)(Port[port0] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port1] +
		                                      0x20)) & (1 << num1)) {
			generic_handle_irq(IRQ_EXT0_A13);
			__raw_writel(0x1 << num1,
			             (volatile unsigned int *)(Port[port1] +
			                                       0x20));
		} else
			__raw_writel(0x01, reg_base + REG_AIC_EOIS);
		break;
	case IRQ_EXT1:
		if (__raw_readl((volatile unsigned int *)(Port[port0] + 0x20)) &
		    (1 << num0)) {
			generic_handle_irq(IRQ_EXT1_A1);
			__raw_writel(0x1 << num0,
			             (volatile unsigned int *)(Port[port0] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port1] +
		                                      0x20)) & (1 << num1)) {
			generic_handle_irq(IRQ_EXT1_A14);
			__raw_writel(0x20 << num1,
			             (volatile unsigned int *)(Port[port1] +
			                                       0x20));
		} else
			__raw_writel(0x01, reg_base + REG_AIC_EOIS);
		break;
	case IRQ_EXT2:
		port2 = EXT[8] / GPIO_OFFSET;
		num2 = EXT[8] % GPIO_OFFSET;
		port3 = EXT[9] / GPIO_OFFSET;
		num3 = EXT[9] % GPIO_OFFSET;
		if (__raw_readl((volatile unsigned int *)(Port[port0] + 0x20)) &
		    (1 << num0)) {
			generic_handle_irq(IRQ_EXT2_D0);
			__raw_writel(0x1 << num0,
			             (volatile unsigned int *)(Port[port0] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port1] +
		                                      0x20)) & (1 << num1)) {
			generic_handle_irq(IRQ_EXT2_E10);
			__raw_writel(0x1 << num1,
			             (volatile unsigned int *)(Port[port1] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port2] +
		                                      0x20)) & (1 << num2)) {
			generic_handle_irq(IRQ_EXT2_B3);
			__raw_writel(0x1 << num2,
			             (volatile unsigned int *)(Port[port2] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port3] +
		                                      0x20)) & (1 << num3)) {
			generic_handle_irq(IRQ_EXT2_B13);
			__raw_writel(0x1 << num3,
			             (volatile unsigned int *)(Port[port3] +
			                                       0x20));
		} else
			__raw_writel(0x01, reg_base + REG_AIC_EOIS);
		break;
	case IRQ_EXT3:
		port2 = EXT[10] / GPIO_OFFSET;
		num2 = EXT[10] % GPIO_OFFSET;
		if (__raw_readl((volatile unsigned int *)(Port[port0] + 0x20)) &
		    (1 << num0)) {
			generic_handle_irq(IRQ_EXT3_D1);
			__raw_writel(0x1 << num0,
			             (volatile unsigned int *)(Port[port0] +
			                                       0x20));
		} else if (__raw_readl
		           ((volatile unsigned int *)(Port[port1] +
		                                      0x20)) & (1 << num1)) {
			generic_handle_irq(IRQ_EXT3_E12);
			__raw_writel(0x1 << num1,
			             (volatile unsigned int *)(Port[port1] +
			                                       0x20));
		} else  if (__raw_readl
		            ((volatile unsigned int *)(Port[port2] +
		                                       0x20)) & (1 << num2)) {
			generic_handle_irq(IRQ_EXT3_G15);
			__raw_writel(0x1 << num2,
			             (volatile unsigned int *)(Port[port2] +
			                                       0x20));
		} else
			__raw_writel(0x01, reg_base + REG_AIC_EOIS);
		break;
	}
	LEAVE();
}
#endif

static struct irq_domain *nuc980_aic_domain;

static int nuc980_aic_irq_map(struct irq_domain *h, unsigned int virq,
                              irq_hw_number_t hw)
{
	// printk("nuc980_aic_irq_map: %d %d\n", virq, (int)hw);

	if ((IRQ_WDT <= hw) && (hw < NOF_IRQS - SPARE_IRQS)) {
		irq_set_chip_and_handler(virq, &nuc980_irq_chip,
		                         handle_level_irq);
		irq_clear_status_flags(virq, IRQ_NOREQUEST);

#if defined(CONFIG_GPIO_NUC980)

		{
			int irqno;
			irq_set_chip(IRQ_GPA, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPB, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPC, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPD, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPE, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPF, &nuc980_irq_chip);
			irq_set_chip(IRQ_GPG, &nuc980_irq_chip);
			irq_set_chained_handler(IRQ_GPA, nuc980_irq_demux_intgroupA);
			irq_set_chained_handler(IRQ_GPB, nuc980_irq_demux_intgroupB);
			irq_set_chained_handler(IRQ_GPC, nuc980_irq_demux_intgroupC);
			irq_set_chained_handler(IRQ_GPD, nuc980_irq_demux_intgroupD);
			irq_set_chained_handler(IRQ_GPE, nuc980_irq_demux_intgroupE);
			irq_set_chained_handler(IRQ_GPF, nuc980_irq_demux_intgroupF);
			irq_set_chained_handler(IRQ_GPG, nuc980_irq_demux_intgroupG);
			for (irqno = IRQ_GPIO_START; irqno < IRQ_GPIO_END; irqno++) {
				irq_set_chip_and_handler(irqno, &nuc980_irq_gpio,
				                         handle_level_irq);
				irq_clear_status_flags(irqno, IRQ_NOREQUEST);
			}

			/*
			 * Install handler for GPIO external interrupts
			 */
			for (irqno = IRQ_EXT0; irqno <= IRQ_EXT3; irqno++) {
				//printk("registering irq %d (extended nuc980 irq)\n", irqno);
				irq_set_chip(irqno, &nuc980_irq_chip);
				irq_set_chained_handler(irqno, nuc980_irq_demux_intgroup2);
			}

			for (irqno = IRQ_EXT0_A0; irqno <= IRQ_EXT3_G15; irqno++) {
				irq_set_chip_and_handler(irqno, &nuc980_irq_ext,
				                         handle_level_irq);
				irq_clear_status_flags(irqno, IRQ_NOREQUEST);
			}
		}
#endif

	}

	else
		return -EINVAL;

	return 0;
}

static int nuc980_aic_irq_domain_xlate(struct irq_domain *d,
                                       struct device_node *ctrlr,
                                       const u32 * intspec,
                                       unsigned int intsize,
                                       irq_hw_number_t * out_hwirq,
                                       unsigned int *out_type)
{
	if (WARN_ON(intsize < 2))
		return -EINVAL;
	if (WARN_ON(intspec[0] >= NOF_IRQS))
		return -EINVAL;

	*out_hwirq = intspec[0];
	*out_type = IRQ_TYPE_NONE;

	return 0;
}

static struct irq_domain_ops nuc980_aic_irq_ops = {
	.map = nuc980_aic_irq_map,
	.xlate = nuc980_aic_irq_domain_xlate,
};

static asmlinkage void __exception_irq_entry aic_handle(struct pt_regs *regs)
{
	u32 irqnr = __raw_readl(reg_base + REG_AIC_IRQNUM);

	generic_handle_domain_irq(nuc980_aic_domain, irqnr);
}

static int __init nuc980_of_init_irq(struct device_node *node, struct device_node *parent)
{
    struct resource res;

	nuc980_aic_domain = irq_domain_add_linear(node, SPARE_IRQS, &nuc980_aic_irq_ops, NULL);
	if (!nuc980_aic_domain)
		panic("Failed to add irq domain!!\n");

	irq_set_default_host(nuc980_aic_domain);

    // Retrieve memory resource from device tree
    if(of_address_to_resource(node, 0, &res)) {
        pr_err("Failed to get memory resource\n");
        return -ENODEV;
    }

    // Request memory region
    reg_base = ioremap(res.start, resource_size(&res));
    if (!reg_base) {
        pr_err("Failed to remap memory\n");
        return -ENOMEM;
    }

	__raw_writel(0xFFFFFFFC, reg_base + REG_AIC_INTDIS0);
	__raw_writel(0xFFFFFFFF, reg_base + REG_AIC_INTDIS1);
	set_handle_irq(aic_handle);

	return 0;
}
IRQCHIP_DECLARE(irq_nuc980, "nuvoton,nuc980-aic", nuc980_of_init_irq);
