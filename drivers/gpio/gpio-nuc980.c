/*
 *  linux/drivers/drivers/gpio/nuc980-gpio.c - Nuvoton NUC980 GPIO Driver
 *
 *  Copyright (c) 2018 Nuvoton Technology Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not,     write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  /gpio-tc3589x.c/
 */



#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>
#include <linux/irq.h>
#include <linux/of_irq.h>

#if 0
#define ENTRY()                 printk("[%-20s] : Enter...\n", __FUNCTION__)
#define LEAVE()                 printk("[%-20s] : Leave...\n", __FUNCTION__)
#else
#define ENTRY()
#define LEAVE()
#endif

#define	NUC980_PA0	(0x00 + 0)
#define	NUC980_PA1	(0x00 + 1)
#define	NUC980_PA2	(0x00 + 2)
#define	NUC980_PA3	(0x00 + 3)
#define	NUC980_PA4	(0x00 + 4)
#define	NUC980_PA5	(0x00 + 5)
#define	NUC980_PA6	(0x00 + 6)
#define	NUC980_PA7	(0x00 + 7)
#define	NUC980_PA8	(0x00 + 8)
#define	NUC980_PA9	(0x00 + 9)
#define	NUC980_PA10	(0x00 + 10)
#define	NUC980_PA11	(0x00 + 11)
#define	NUC980_PA12	(0x00 + 12)
#define	NUC980_PA13	(0x00 + 13)
#define	NUC980_PA14	(0x00 + 14)
#define	NUC980_PA15	(0x00 + 15)

#define	NUC980_PB0	(0x20 + 0)
#define	NUC980_PB1	(0x20 + 1)
#define	NUC980_PB2	(0x20 + 2)
#define	NUC980_PB3	(0x20 + 3)
#define	NUC980_PB4	(0x20 + 4)
#define	NUC980_PB5	(0x20 + 5)
#define	NUC980_PB6	(0x20 + 6)
#define	NUC980_PB7	(0x20 + 7)
#define	NUC980_PB8	(0x20 + 8)
#define	NUC980_PB9	(0x20 + 9)
#define	NUC980_PB10	(0x20 + 10)
#define	NUC980_PB11	(0x20 + 11)
#define	NUC980_PB12	(0x20 + 12)
#define	NUC980_PB13	(0x20 + 13)
#define	NUC980_PB14	(0x20 + 14)
#define	NUC980_PB15	(0x20 + 15)

#define	NUC980_PC0	(0x40 + 0)
#define	NUC980_PC1	(0x40 + 1)
#define	NUC980_PC2	(0x40 + 2)
#define	NUC980_PC3	(0x40 + 3)
#define	NUC980_PC4	(0x40 + 4)
#define	NUC980_PC5	(0x40 + 5)
#define	NUC980_PC6	(0x40 + 6)
#define	NUC980_PC7	(0x40 + 7)
#define	NUC980_PC8	(0x40 + 8)
#define	NUC980_PC9	(0x40 + 9)
#define	NUC980_PC10	(0x40 + 10)
#define	NUC980_PC11	(0x40 + 11)
#define	NUC980_PC12	(0x40 + 12)
#define	NUC980_PC13	(0x40 + 13)
#define	NUC980_PC14	(0x40 + 14)
#define	NUC980_PC15	(0x40 + 15)

#define	NUC980_PD0	(0x60 + 0)
#define	NUC980_PD1	(0x60 + 1)
#define	NUC980_PD2	(0x60 + 2)
#define	NUC980_PD3	(0x60 + 3)
#define	NUC980_PD4	(0x60 + 4)
#define	NUC980_PD5	(0x60 + 5)
#define	NUC980_PD6	(0x60 + 6)
#define	NUC980_PD7	(0x60 + 7)
#define	NUC980_PD8	(0x60 + 8)
#define	NUC980_PD9	(0x60 + 9)
#define	NUC980_PD10	(0x60 + 10)
#define	NUC980_PD11	(0x60 + 11)
#define	NUC980_PD12	(0x60 + 12)
#define	NUC980_PD13	(0x60 + 13)
#define	NUC980_PD14	(0x60 + 14)
#define	NUC980_PD15	(0x60 + 15)

#define	NUC980_PE0	(0x80 + 0)
#define	NUC980_PE1	(0x80 + 1)
#define	NUC980_PE2	(0x80 + 2)
#define	NUC980_PE3	(0x80 + 3)
#define	NUC980_PE4	(0x80 + 4)
#define	NUC980_PE5	(0x80 + 5)
#define	NUC980_PE6	(0x80 + 6)
#define	NUC980_PE7	(0x80 + 7)
#define	NUC980_PE8	(0x80 + 8)
#define	NUC980_PE9	(0x80 + 9)
#define	NUC980_PE10	(0x80 + 10)
#define	NUC980_PE11	(0x80 + 11)
#define	NUC980_PE12	(0x80 + 12)
#define	NUC980_PE13	(0x80 + 13)
#define	NUC980_PE14	(0x80 + 14)
#define	NUC980_PE15	(0x80 + 15)

#define	NUC980_PF0	(0xA0 + 0)
#define	NUC980_PF1	(0xA0 + 1)
#define	NUC980_PF2	(0xA0 + 2)
#define	NUC980_PF3	(0xA0 + 3)
#define	NUC980_PF4	(0xA0 + 4)
#define	NUC980_PF5	(0xA0 + 5)
#define	NUC980_PF6	(0xA0 + 6)
#define	NUC980_PF7	(0xA0 + 7)
#define	NUC980_PF8	(0xA0 + 8)
#define	NUC980_PF9	(0xA0 + 9)
#define	NUC980_PF10	(0xA0 + 10)
#define	NUC980_PF11	(0xA0 + 11)
#define	NUC980_PF12	(0xA0 + 12)
#define	NUC980_PF13	(0xA0 + 13)
#define	NUC980_PF14	(0xA0 + 14)
#define	NUC980_PF15	(0xA0 + 15)

#define	NUC980_PG0	(0xC0 + 0)
#define	NUC980_PG1	(0xC0 + 1)
#define	NUC980_PG2	(0xC0 + 2)
#define	NUC980_PG3	(0xC0 + 3)
#define	NUC980_PG4	(0xC0 + 4)
#define	NUC980_PG5	(0xC0 + 5)
#define	NUC980_PG6	(0xC0 + 6)
#define	NUC980_PG7	(0xC0 + 7)
#define	NUC980_PG8	(0xC0 + 8)
#define	NUC980_PG9	(0xC0 + 9)
#define	NUC980_PG10	(0xC0 + 10)
#define	NUC980_PG11	(0xC0 + 11)
#define	NUC980_PG12	(0xC0 + 12)
#define	NUC980_PG13	(0xC0 + 13)
#define	NUC980_PG14	(0xC0 + 14)
#define	NUC980_PG15	(0xC0 + 15)

#define GPIO_PMD_INPUT              0x0UL                  /*!< Input Mode */
#define GPIO_PMD_OUTPUT             0x1UL                  /*!< Output Mode */
#define GPIO_PMD_OPEN_DRAIN         0x2UL                  /*!< Open-Drain Mode */
#define GPIO_PMD_QUASI              0x3UL                  /*!< Quasi-bidirectional Mode */
#define GPIO_PMD_MODE(pin, mode)    ((mode) << ((pin)<<1)) /*!< Generate the PMD mode setting for each pin  */

#define REG_GPIOA_MODE      (0x000)
#define REG_GPIOA_DINOFF    (0x004)
#define REG_GPIOA_DOUT      (0x008)
#define REG_GPIOA_DATMSK    (0x00C)
#define REG_GPIOA_PIN       (0x010)
#define REG_GPIOA_DBEN      (0x014)
#define REG_GPIOA_INTTYPE 	(0x018)
#define REG_GPIOA_INTEN     (0x01C)
#define REG_GPIOA_INTSRC    (0x020)
#define REG_GPIOA_SMTEN     (0x024)
#define REG_GPIOA_SLEWCTL 	(0x028)
#define REG_GPIOA_PUSEL     (0x030)

#define REG_GPIOB_MODE      (0x040)
#define REG_GPIOB_DINOFF    (0x044)
#define REG_GPIOB_DOUT      (0x048)
#define REG_GPIOB_DATMSK    (0x04C)
#define REG_GPIOB_PIN       (0x050)
#define REG_GPIOB_DBEN      (0x054)
#define REG_GPIOB_INTTYPE 	(0x058)
#define REG_GPIOB_INTEN     (0x05C)
#define REG_GPIOB_INTSRC    (0x060)
#define REG_GPIOB_SMTEN     (0x064)
#define REG_GPIOB_SLEWCTL 	(0x068)
#define REG_GPIOB_PUSEL     (0x070)

#define REG_GPIOC_MODE      (0x080)
#define REG_GPIOC_DINOFF    (0x084)
#define REG_GPIOC_DOUT      (0x088)
#define REG_GPIOC_DATMSK    (0x08C)
#define REG_GPIOC_PIN       (0x090)
#define REG_GPIOC_DBEN      (0x094)
#define REG_GPIOC_INTTYPE 	(0x098)
#define REG_GPIOC_INTEN     (0x09C)
#define REG_GPIOC_INTSRC    (0x0A0)
#define REG_GPIOC_SMTEN     (0x0A4)
#define REG_GPIOC_SLEWCTL 	(0x0A8)
#define REG_GPIOC_PUSEL     (0x0B0)

#define REG_GPIOD_MODE      (0x0C0)
#define REG_GPIOD_DINOFF    (0x0C4)
#define REG_GPIOD_DOUT      (0x0C8)
#define REG_GPIOD_DATMSK    (0x0CC)
#define REG_GPIOD_PIN       (0x0D0)
#define REG_GPIOD_DBEN      (0x0D4)
#define REG_GPIOD_INTTYPE 	(0x0D8)
#define REG_GPIOD_INTEN     (0x0DC)
#define REG_GPIOD_INTSRC    (0x0E0)
#define REG_GPIOD_SMTEN     (0x0E4)
#define REG_GPIOD_SLEWCTL 	(0x0E8)
#define REG_GPIOD_PUSEL     (0x0F0)

#define REG_GPIOE_MODE      (0x100)
#define REG_GPIOE_DINOFF    (0x104)
#define REG_GPIOE_DOUT      (0x108)
#define REG_GPIOE_DATMSK    (0x10C)
#define REG_GPIOE_PIN       (0x110)
#define REG_GPIOE_DBEN      (0x114)
#define REG_GPIOE_INTTYPE 	(0x118)
#define REG_GPIOE_INTEN     (0x11C)
#define REG_GPIOE_INTSRC    (0x120)
#define REG_GPIOE_SMTEN     (0x124)
#define REG_GPIOE_SLEWCTL 	(0x128)
#define REG_GPIOE_PUSEL     (0x130)

#define REG_GPIOF_MODE      (0x140)
#define REG_GPIOF_DINOFF    (0x144)
#define REG_GPIOF_DOUT      (0x148)
#define REG_GPIOF_DATMSK    (0x14C)
#define REG_GPIOF_PIN       (0x150)
#define REG_GPIOF_DBEN      (0x154)
#define REG_GPIOF_INTTYPE 	(0x158)
#define REG_GPIOF_INTEN     (0x15C)
#define REG_GPIOF_INTSRC    (0x160)
#define REG_GPIOF_SMTEN     (0x164)
#define REG_GPIOF_SLEWCTL 	(0x168)
#define REG_GPIOF_PUSEL     (0x170)

#define REG_GPIOG_MODE      (0x180)
#define REG_GPIOG_DINOFF    (0x184)
#define REG_GPIOG_DOUT      (0x188)
#define REG_GPIOG_DATMSK    (0x18C)
#define REG_GPIOG_PIN       (0x190)
#define REG_GPIOG_DBEN      (0x194)
#define REG_GPIOG_INTTYPE 	(0x198)
#define REG_GPIOG_INTEN     (0x19C)
#define REG_GPIOG_INTSRC    (0x1A0)
#define REG_GPIOG_SMTEN     (0x1A4)
#define REG_GPIOG_SLEWCTL 	(0x1A8)
#define REG_GPIOG_PUSEL     (0x1B0)

#define REG_GPIO_DBNCECON   (0x440)

#define REG_WKUPSER0    (0x050)  /*!< System Wakeup Source Enable Register0 */
#define REG_MFP_GPA_L	(0x070)	/* GPIOA Low Byte Multiple Function Control Register */
#define REG_MFP_GPA_H	(0x074)	/* GPIOA High Byte Multiple Function Control Register */
#define REG_MFP_GPB_L	(0x078)	/* GPIOB Low Byte Multiple Function Control Register */
#define REG_MFP_GPB_H	(0x07C)	/* GPIOB High Byte Multiple Function Control Register */
#define REG_MFP_GPC_L	(0x080)	/* GPIOC Low Byte Multiple Function Control Register */
#define REG_MFP_GPC_H	(0x084)	/* GPIOC High Byte Multiple Function Control Register */
#define REG_MFP_GPD_L	(0x088)	/* GPIOD Low Byte Multiple Function Control Register */
#define REG_MFP_GPD_H	(0x08C)	/* GPIOD High Byte Multiple Function Control Register */
#define REG_MFP_GPE_L	(0x090)	/* GPIOE Low Byte Multiple Function Control Register */
#define REG_MFP_GPE_H	(0x094)	/* GPIOE High Byte Multiple Function Control Register */
#define REG_MFP_GPF_L	(0x098)	/* GPIOF Low Byte Multiple Function Control Register */
#define REG_MFP_GPF_H	(0x09C)	/* GPIOF High Byte Multiple Function Control Register */
#define REG_MFP_GPG_L	(0x0A0)	/* GPIOG Low Byte Multiple Function Control Register */
#define REG_MFP_GPG_H	(0x0A4)	/* GPIOG High Byte Multiple Function Control Register */
#define REG_MFP_GPH_L	(0x0A8)	/* GPIOH Low Byte Multiple Function Control Register */
#define REG_MFP_GPH_H	(0x0AC)	/* GPIOH High Byte Multiple Function Control Register */
#define REG_MFP_GPI_L	(0x0B0)	/* GPIOI Low Byte Multiple Function Control Register */
#define REG_MFP_GPI_H	(0x0B4)	/* GPIOI High Byte Multiple Function Control Register */
#define REG_MFP_GPJ_L	(0x0B8)	/* GPIOJ Low Byte Multiple Function Control Register */
#define REG_DDR_DS_CR	(0x0E0)	/* DDR I/O Driving Strength Control Register */
#define REG_PORDISCR    (0x100)	/* Power-On-Reset Disable Control Register */
#define REG_ICEDBGCR    (0x104)	/* ICE Debug Interface Control Register */
#define REG_WRPRTR	    (0x1FC)	/* Register Write-Protection Control Register */

#define GPIO_PIN_DATA_BASE (0x800)
/* Define GPIO Pin Data Input/Output. It could be used to control each I/O pin by pin address mapping. */
#define GPIO_PIN_DATA(reg_base, port, pin)    (*((volatile uint32_t *)((reg_base + GPIO_PIN_DATA_BASE + (0x40 * (port))) + ((pin) << 2))))

#define GPIO_OFFSET 0x20
#define NUMGPIO 0x20 * 7    //(PortA~PortG)

#define EXT0_BASE 			100
#define IRQ_EXT0_A0  		EXT0_BASE + 0
#define IRQ_EXT1_A1  		EXT0_BASE + 1
#define IRQ_EXT2_D0  		EXT0_BASE + 2
#define IRQ_EXT3_D1  		EXT0_BASE + 3
#define IRQ_EXT0_A13 		EXT0_BASE + 4
#define IRQ_EXT1_A14 		EXT0_BASE + 5
#define IRQ_EXT2_E10 		EXT0_BASE + 6
#define IRQ_EXT3_E12 		EXT0_BASE + 7	
#define IRQ_EXT2_B3  		EXT0_BASE + 8
#define IRQ_EXT2_B13 		EXT0_BASE + 9
#define IRQ_EXT3_G15 		EXT0_BASE +10

#define IRQ_GPA				8
#define IRQ_GPB				9
#define IRQ_GPC				10
#define IRQ_GPD				11
#define IRQ_GPE				49
#define IRQ_GPF				57
#define IRQ_GPG				63

static DEFINE_SPINLOCK(gpio_lock);

static __iomem void *gpio_base;
static __iomem void *pwr_base;

struct gpio_port {
	uint32_t dir;
	uint32_t out;
	uint32_t in;
};

static const struct gpio_port port_class[] = {
	{
		(uint32_t)REG_GPIOA_MODE, (uint32_t)REG_GPIOA_DOUT,
		(uint32_t)REG_GPIOA_PIN
	},
	{
		(uint32_t)REG_GPIOB_MODE, (uint32_t)REG_GPIOB_DOUT,
		(uint32_t)REG_GPIOB_PIN
	},
	{
		(uint32_t)REG_GPIOC_MODE, (uint32_t)REG_GPIOC_DOUT,
		(uint32_t)REG_GPIOC_PIN
	},
	{
		(uint32_t)REG_GPIOD_MODE, (uint32_t)REG_GPIOD_DOUT,
		(uint32_t)REG_GPIOD_PIN
	},
	{
		(uint32_t)REG_GPIOE_MODE, (uint32_t)REG_GPIOE_DOUT,
		(uint32_t)REG_GPIOE_PIN
	},
	{
		(uint32_t)REG_GPIOF_MODE, (uint32_t)REG_GPIOF_DOUT,
		(uint32_t)REG_GPIOF_PIN
	},
	{
		(uint32_t)REG_GPIOG_MODE, (uint32_t)REG_GPIOG_DOUT,
		(uint32_t)REG_GPIOG_PIN
	},
	{},
};

static const struct gpio_port *nuc980_gpio_cla_port(unsigned gpio_num,
                unsigned *group,unsigned *num) {
	*group = gpio_num / GPIO_OFFSET;
	*num = gpio_num % GPIO_OFFSET;
	return &port_class[*group];
}


static int nuc980_gpio_core_direction_in(struct gpio_chip *gc,
                unsigned gpio_num)
{
	int port_num,group_num;
	unsigned long value;
	unsigned long flags;
	const struct gpio_port *port =
	        nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	ENTRY();
	spin_lock_irqsave(&gpio_lock, flags);
	value = __raw_readl(gpio_base + port->dir);
	value &= ~GPIO_PMD_MODE(port_num, GPIO_PMD_QUASI);
	value |= GPIO_PMD_MODE(port_num, GPIO_PMD_INPUT);
	__raw_writel(value, gpio_base + port->dir);
	spin_unlock_irqrestore(&gpio_lock, flags);
	LEAVE();
	return 0;
}
EXPORT_SYMBOL_GPL(nuc980_gpio_core_direction_in);

static int nuc980_gpio_core_get(struct gpio_chip *gc, unsigned gpio_num)
{
	int port_num,group_num;
	const struct gpio_port *port;
	ENTRY();
	port = nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);

	LEAVE();
	return GPIO_PIN_DATA(gpio_base, group_num, port_num);
}
EXPORT_SYMBOL_GPL(nuc980_gpio_core_get);

static void nuc980_gpio_core_set(struct gpio_chip *gc, unsigned gpio_num,
                                 int val)
{
	int port_num,group_num;
	const struct gpio_port *port;
	ENTRY();
	port = nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	GPIO_PIN_DATA(gpio_base, group_num, port_num) = val;
	LEAVE();
}
EXPORT_SYMBOL_GPL(nuc980_gpio_core_set);

static int nuc980_gpio_core_direction_out(struct gpio_chip *gc,
                unsigned gpio_num, int val)
{
	int port_num,group_num;
	unsigned long value;
	unsigned long flags;
	const struct gpio_port *port =
	        nuc980_gpio_cla_port(gpio_num, &group_num, &port_num);
	ENTRY();
	spin_lock_irqsave(&gpio_lock, flags);
	value = __raw_readl(gpio_base + port->dir);
	value &= ~GPIO_PMD_MODE(port_num, GPIO_PMD_QUASI);
	value |= GPIO_PMD_MODE(port_num, GPIO_PMD_OUTPUT);
	__raw_writel(value, gpio_base + port->dir);
	spin_unlock_irqrestore(&gpio_lock, flags);
	nuc980_gpio_core_set(gc, gpio_num, val);
	LEAVE();
	return 0;
}
EXPORT_SYMBOL_GPL(nuc980_gpio_core_direction_out);

static int nuc980_gpio_core_to_request(struct gpio_chip *chip, unsigned offset)
{
	unsigned int group, num1, num, value;
	__iomem void *reg;
	ENTRY();
	group = offset / GPIO_OFFSET;
	num1  = num = offset % GPIO_OFFSET;
	reg   = pwr_base + REG_MFP_GPA_L + (group * 0x08);
	if(num>7) {
		num -= 8;
		reg = reg + 0x04 ;
	}

	value = ( __raw_readl(reg) & (0xf<<(num*4)))>>(num*4);
	if(value>0 && value<0xf) {
		printk(KERN_ERR "Please Check GPIO%c%02d's multi-function = 0x%x \n",(char)(65+group),num1,value);
		LEAVE();
		return -EINVAL;
	}
	LEAVE();
	return 0;
}

static void nuc980_gpio_core_to_free(struct gpio_chip *chip, unsigned offset)
{
	ENTRY();
}

static struct gpio_chip nuc980_gpio = {
	.label = "gpio-nuc980",
	.owner = THIS_MODULE,
	.direction_input = nuc980_gpio_core_direction_in,
	.get = nuc980_gpio_core_get,
	.direction_output = nuc980_gpio_core_direction_out,
	.set = nuc980_gpio_core_set,
	.request = nuc980_gpio_core_to_request,
	.free = nuc980_gpio_core_to_free,
	.base = -1,// Dynamically allocate gpio numbers
	.ngpio = NUMGPIO,
};

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
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	unsigned int port,num;
	ENTRY();
	port =(hwirq) / GPIO_OFFSET;
	num  =(hwirq) % GPIO_OFFSET;
	__raw_writel(__raw_readl(gpio_base + (Port[port]+0x1C)) &~(0x1<<(num+16)), gpio_base + (Port[port]+0x1C));
	__raw_writel(__raw_readl(gpio_base + (Port[port]+0x1C)) &~(0x1<<num), gpio_base + (Port[port]+0x1C));

	gpiochip_disable_irq(gc, hwirq);
	LEAVE();
}

static void nuc980_irq_gpio_ack(struct irq_data *d)
{
	ENTRY();
	// printk("nuc980_irq_gpio_ack %d\n", irqd_to_hwirq(d));
	
	LEAVE();
}

static void nuc980_irq_gpio_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	unsigned int port,num,tmp;
	ENTRY();
	port =(hwirq)/GPIO_OFFSET;
	num  =(hwirq)%GPIO_OFFSET;
	tmp = gpio_type[port] & (0x1<<(num));
	__raw_writel(__raw_readl(gpio_base + (Port[port]+0x18)) | tmp, gpio_base + (Port[port]+0x18));
	tmp = gpio_inten[port] & (0x1<<(num+16));
	__raw_writel(__raw_readl(gpio_base + (Port[port]+0x1C))|tmp, gpio_base + (Port[port]+0x1C));
	tmp = gpio_inten[port] & (0x1<<num);
	__raw_writel(__raw_readl(gpio_base + (Port[port]+0x1C))|tmp, gpio_base + (Port[port]+0x1C));

	gpiochip_enable_irq(gc, hwirq);
	LEAVE();
}

static int nuc980_irq_gpio_type(struct irq_data *d, unsigned int type)
{
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	unsigned int port, num;
	ENTRY();
	port = (hwirq) / GPIO_OFFSET;
	num = (hwirq) % GPIO_OFFSET;

	if (type == IRQ_TYPE_PROBE) {
		__raw_writel(__raw_readl
		             (gpio_base + (Port[port] + 0x18)) & ~(0x1 << num),
		             gpio_base + (Port[port] + 0x18));
		__raw_writel(__raw_readl
		             (gpio_base + (Port[port] + 0x1C)) | (0x1 << num) | ((0x1<<num)<<16),
		             gpio_base + (Port[port] + 0x1C));
		gpio_type[port] &= ~(0x1<<num);
		gpio_inten[port] |= (0x1<<num);
		gpio_inten[port] |= (0x1<<(num+16));
		return 0;
	}
	if (type & IRQ_TYPE_LEVEL_MASK) {
		__raw_writel(__raw_readl
		             (gpio_base + (Port[port] + 0x18)) | (0x1 << num),
		             gpio_base + (Port[port] + 0x18));
		__raw_writel(__raw_readl
		             (gpio_base + (Port[port] + 0x1C)) & ~((0x1 << num)|((0x1<<num)<<16)),
		             gpio_base + (Port[port] + 0x1C));
		gpio_type[port] |= (0x1<<num);
		gpio_inten[port] &= ~(0x1<<num);
		gpio_inten[port] &= ~(0x1<<(num+16));
		if (type == IRQ_TYPE_LEVEL_HIGH) {
			__raw_writel(__raw_readl
			             (gpio_base + (Port[port] + 0x1C)) | ((0x1 <<  num)<<16),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] |=(0x1<<(num+16));
			return 0;
		}
		if (type == IRQ_TYPE_LEVEL_LOW) {
			__raw_writel(__raw_readl(gpio_base + (Port[port] + 0x1C)) | (0x1 << num),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] |= (0x1<<num);
			return 0;
		}
	} else {
		__raw_writel(__raw_readl
		             (gpio_base + (Port[port] + 0x18)) & ~(0x1 << num),
		             gpio_base + (Port[port] + 0x18));
		gpio_type[port] &= ~(0x1<<num);
		if (type & IRQ_TYPE_EDGE_RISING) {
			__raw_writel(__raw_readl
			             (gpio_base + (Port[port] + 0x1C)) | ((0x1 << num)<<16),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] |= (0x1<<(num+16));
		} else {
			__raw_writel(__raw_readl
			             (gpio_base + (Port[port] + 0x1C)) & ~((0x1 << num)<<16),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] &= ~(0x1<<(num+16));
		}
		if (type & IRQ_TYPE_EDGE_FALLING) {
			__raw_writel(__raw_readl
			             (gpio_base + (Port[port] + 0x1C)) | (0x1 << num),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] |= (0x1<<num);
		} else {
			__raw_writel(__raw_readl
			             (gpio_base + (Port[port] + 0x1C)) & ~(0x1 << num),
			             gpio_base + (Port[port] + 0x1C));
			gpio_inten[port] &= ~(0x1<<num);
		}
	}
	return 0;
}

static irqreturn_t gpio_parent_handler(int irq, void *data)
{
	struct gpio_chip *chip = (struct gpio_chip*)data;
	struct irq_desc *desc = irq_to_desc(irq);
	irq_hw_number_t hwirq = desc->irq_data.hwirq;
	ENTRY();
	
	// Resolve the register base address to use
	int int_src_base = 0;
	int port = -1;
	switch (hwirq)
	{
		case IRQ_GPA:
			int_src_base = REG_GPIOA_INTSRC;
			port = 0;
			break;

		case IRQ_GPB:
			int_src_base = REG_GPIOB_INTSRC;
			port = 1;
			break;
		
		case IRQ_GPC:
			int_src_base = REG_GPIOC_INTSRC;
			port = 2;
			break;

		case IRQ_GPD:
			int_src_base = REG_GPIOD_INTSRC;
			port = 3;
			break;

		case IRQ_GPE:
			int_src_base = REG_GPIOE_INTSRC;
			port = 4;
			break;

		case IRQ_GPF:
			int_src_base = REG_GPIOF_INTSRC;
			port = 5;
			break;

		case IRQ_GPG:
			int_src_base = REG_GPIOG_INTSRC;
			port = 6;
			break;
		
		default:
			break;
	}
	BUG_ON(!int_src_base || port == -1);

	unsigned int j, isr;
	unsigned int flag=0;
	isr = __raw_readl(gpio_base + int_src_base);
	__raw_writel(isr, gpio_base + int_src_base);
	for (j = 0; j < 16; j++) {
		if (isr  & 0x1) {
			flag = 1;
			generic_handle_irq(irq_find_mapping(chip->irq.domain, port * 0x20 + j));
		}
		isr = isr >> 1;
	}
	LEAVE();

	return IRQ_HANDLED;
}

static const struct irq_chip nuc980_irq_gpio = {
	.name = "GPIO-IRQ",
	.irq_ack = nuc980_irq_gpio_ack,
	.irq_mask = nuc980_irq_gpio_mask,
	.irq_unmask = nuc980_irq_gpio_unmask,
	.irq_set_type = nuc980_irq_gpio_type,
	.flags = IRQCHIP_IMMUTABLE,
	GPIOCHIP_IRQ_RESOURCE_HELPERS
};

static irqreturn_t nuc980_eint0_interrupt(int irq, void *dev_id)
{
	printk("@0\n");
	return IRQ_HANDLED;
}
__attribute__ ((unused)) static irqreturn_t nuc980_eint1_interrupt(int irq, void *dev_id)
{
	printk("@1\n");
	return IRQ_HANDLED;
}
__attribute__ ((unused)) static irqreturn_t nuc980_eint2_interrupt(int irq, void *dev_id)
{
	printk("@2\n");
	return IRQ_HANDLED;
}
__attribute__ ((unused)) static irqreturn_t nuc980_eint3_interrupt(int irq, void *dev_id)
{
	printk("@3\n");
	return IRQ_HANDLED;
}

static int nuc980_enable_eint(uint32_t flag, struct platform_device *pdev)
{
	int err;
	u32 val32[3];
	u32 irqnum,irqflag;

	//eint 0
	if (of_property_read_u32_array(pdev->dev.of_node, "eint0-config", val32, 3) != 0) {
		printk("%s - eint0 can not get port-number!\n", __func__);
		return -EINVAL;
	}
	if(val32[0]==1) {
		irqnum=(val32[1]==0)?(IRQ_EXT0_A0):(IRQ_EXT0_A13);
		irqflag = (val32[2] | IRQF_NO_SUSPEND);
		if(flag==1) {
			__raw_writel((1<<4) | __raw_readl(pwr_base + REG_WKUPSER0), pwr_base + REG_WKUPSER0);
			enable_irq_wake(irqnum);
		}
		if ((err = request_irq(irqnum,nuc980_eint0_interrupt,irqflag, "eint0", 0)) != 0) {
			printk("%s - eint0 can not get irq!\n", __func__);
			return -EINVAL;
		}
	}

	//eint 1
	if (of_property_read_u32_array(pdev->dev.of_node, "eint1-config", val32, 3) != 0) {
		printk("%s - eint1 can not get port-number!\n", __func__);
		return -EINVAL;
	}
	if(val32[0]==1) {
		irqnum=(val32[1]==0)?(IRQ_EXT1_A1):(IRQ_EXT1_A14);
		irqflag = (val32[2] | IRQF_NO_SUSPEND);
		if(flag==1) {
			__raw_writel((1<<5) | __raw_readl(pwr_base + REG_WKUPSER0), pwr_base + REG_WKUPSER0);
			enable_irq_wake(irqnum);
		}
		if ((err = request_irq(irqnum,nuc980_eint1_interrupt,irqflag, "eint1", 0)) != 0) {
			printk("%s - eint1 can not get irq!\n", __func__);
			return -EINVAL;
		}
	}

	//eint 2
	if (of_property_read_u32_array(pdev->dev.of_node, "eint2-config", val32, 3) != 0) {
		printk("%s - eint2 can not get port-number!\n", __func__);
		return -EINVAL;
	}
	if(val32[0]==1) {
		if(val32[1]==0)
			irqnum = IRQ_EXT2_D0;
		else if(val32[1]==1)
			irqnum = IRQ_EXT2_E10;
		else if(val32[1]==2) {
			printk("======================>IRQ_EXT2_B3\n");
			irqnum = IRQ_EXT2_B3;
		} else
			irqnum = IRQ_EXT2_B13;
		irqflag = (val32[2] | IRQF_NO_SUSPEND);
		if(flag==1) {
			__raw_writel((1<<6) | __raw_readl(pwr_base + REG_WKUPSER0), pwr_base + REG_WKUPSER0);
			enable_irq_wake(irqnum);
		}
		if ((err = request_irq(irqnum,nuc980_eint2_interrupt,irqflag, "eint2", 0)) != 0) {
			printk("%s - eint2 can not get irq!\n", __func__);
			return -EINVAL;
		}
	}

	//eint 3
	if (of_property_read_u32_array(pdev->dev.of_node, "eint3-config", val32, 3) != 0) {
		printk("%s - eint3 can not get port-number!\n", __func__);
		return -EINVAL;
	}
	if(val32[0]==1) {
		if(val32[1]==0)
			irqnum = IRQ_EXT3_D1;
		else if(val32[1]==1)
			irqnum = IRQ_EXT3_E12;
		else
			irqnum = IRQ_EXT3_G15;

		irqflag = (val32[2] | IRQF_NO_SUSPEND);
		if(flag == 1) {
			__raw_writel((1<<7) | __raw_readl(pwr_base + REG_WKUPSER0), pwr_base + REG_WKUPSER0);
			enable_irq_wake(irqnum);
		}
		if ((err = request_irq(irqnum,nuc980_eint3_interrupt,irqflag, "eint3", 0)) != 0) {
			printk("%s - eint3 can not get irq!\n", __func__);
			return -EINVAL;
		}
	}
	return 0;
}

static int nuc980_gpio_of_xlate(struct gpio_chip *gc,
                             const struct of_phandle_args *gpiospec,
                             u32 *flags)
{
    if (gpiospec->args[0] > 0x20 * 7)
        return -EINVAL;

    if (flags){
        *flags = gpiospec->args[1];
    }

	return gpiospec->args[0];
}

static int nuc980_gpio_probe(struct platform_device *pdev)
{
	int err, i;
	struct clk *clk;
	struct device_node *np = pdev->dev.of_node;
	struct resource res;
	printk("%s - pdev = %s\n", __func__, pdev->name);
	/* Enable GPIO clock */
	clk = of_clk_get(np, 0);
	if (IS_ERR(clk)) {
		printk(KERN_ERR "nuc980-gpio:failed to get gpio clock source\n");
		err = PTR_ERR(clk);
		return err;
	}
	clk_prepare_enable(clk);

	i = of_property_match_string(np, "reg-names", "gpio");
	if (of_address_to_resource(np, i, &res))
		return -ENOMEM;
	
	gpio_base = devm_ioremap_resource(&pdev->dev, &res);
	if (!gpio_base) {
		dev_err(&pdev->dev, "Failed to remap gpio_base memory\n");
		return -ENOMEM;
	}

	i = of_property_match_string(np, "reg-names", "pwr");
	if (of_address_to_resource(np, i, &res))
		return -ENOMEM;
	pwr_base = devm_ioremap_resource(&pdev->dev, &res);
	if (!pwr_base) {
		dev_err(&pdev->dev, "Failed to remap pwr_base memory\n");
		return -ENOMEM;
	}

	// Map all hw gpio irqs to local interrupt handler
	int tmpMapping[] = {IRQ_GPA, IRQ_GPB, IRQ_GPC, IRQ_GPD, IRQ_GPE, IRQ_GPF, IRQ_GPG};
	for(int i = 0;i < 7;i++) {
		int ret = devm_request_threaded_irq(&pdev->dev, irq_create_mapping(NULL, tmpMapping[i]), NULL, gpio_parent_handler,
										IRQF_ONESHOT, "gpio-handler", &nuc980_gpio);
		if (ret < 0)
			return ret;
	}

	struct gpio_irq_chip *gpioirqchip = &nuc980_gpio.irq;
	gpio_irq_chip_set_chip(gpioirqchip, &nuc980_irq_gpio);
	gpioirqchip->parent_handler = NULL;
	gpioirqchip->num_parents = 0;
	gpioirqchip->parents = NULL;
	gpioirqchip->default_type = IRQ_TYPE_NONE;
	gpioirqchip->handler = handle_edge_irq;// Support both level and edge interrupts
	
	nuc980_gpio.of_xlate = nuc980_gpio_of_xlate;
	nuc980_gpio.of_gpio_n_cells = 2;
	nuc980_gpio.parent = &pdev->dev;
	devm_gpiochip_add_data(&pdev->dev, &nuc980_gpio, NULL);
	if (err < 0) {
		goto err_nuc980_gpio_port;
	}

	// TODO: Finish setting up external interrupts...
#ifdef CONFIG_GPIO_NUC980_EINT_WKUP
	nuc980_enable_eint(1, pdev);
#else
	nuc980_enable_eint(0, pdev);
#endif
	
	return 0;

err_nuc980_gpio_port:
	return err;
}

static int nuc980_gpio_remove(struct platform_device *pdev)
{
	struct clk *clk;

	iounmap(gpio_base);
	iounmap(pwr_base);

	/* Disable GPIO clock */
	clk = of_clk_get(pdev->dev.of_node, 0);
    if (IS_ERR(clk)) {
        int err;

        printk(KERN_ERR "nuc980-gpio:failed to get gpio clock source\n");
        err = PTR_ERR(clk);
        return err;
    }

	clk_disable(clk);

	return 0;
}

static int nuc980_gpio_resume(struct platform_device *pdev)
{
	ENTRY();
	LEAVE();
	return 0;
}

static int nuc980_gpio_suspend(struct platform_device *pdev,pm_message_t state)
{
	ENTRY();
	LEAVE();
	return 0;
}

static const struct of_device_id nuc980_gpio_of_match[] = {
	{ .compatible = "nuvoton,nuc980-gpio" },
	{},
};
MODULE_DEVICE_TABLE(of, nuc980_gpio_of_match);

static struct platform_driver nuc980_gpio_driver = {
	.probe      = nuc980_gpio_probe,
	.remove     = nuc980_gpio_remove,
	.resume     = nuc980_gpio_resume,
	.suspend    = nuc980_gpio_suspend,
	.driver     = {
		.name   = "nuc980-gpio",
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(nuc980_gpio_of_match),
	},
};
module_platform_driver(nuc980_gpio_driver);

MODULE_DESCRIPTION("GPIO interface for Nuvoton NUC980 GPIO Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:nuc980_gpio");
