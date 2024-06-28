/**
 * @file clk-nuc980.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/of_address.h>
#include <linux/slab.h>

#include <dt-bindings/clock/nuvoton,nuc980-clock.h>

// Register Defines
#define CLK_PMCON       (0x00)
#define CLK_HCLKEN      (0x10)
#define CLK_PCLKEN0     (0x18)
#define CLK_PCLKEN1     (0x1C)
#define CLK_DIVCTL0     (0x20)
#define CLK_DIVCTL1     (0x24)
#define CLK_DIVCTL2     (0x28)
#define CLK_DIVCTL3     (0x2C)
#define CLK_DIVCTL4     (0x30)
#define CLK_DIVCTL5     (0x34)
#define CLK_DIVCTL6     (0x38)
#define CLK_DIVCTL7     (0x3C)
#define CLK_DIVCTL8     (0x40)
#define CLK_DIVCTL9     (0x44)
#define CLK_APLLCON     (0x60)
#define CLK_UPLLCON     (0x64)
#define CLK_PLLSTBCNTR  (0x80)

/*
 * Single copy of strings used to refer to clocks within this driver indexed by
 * below enums.
 */
#define NUC980_CLK_S_XIN        	"xin"
#define NUC980_CLK_S_XIN32K     	"xin32k"
#define NUC980_CLK_S_XIN512_DIV 	"xin_div512"// 512 divider used for watchdog
#define NUC980_CLK_S_APLL       	"apll"
#define NUC980_CLK_S_UPLL       	"upll"
#define NUC980_CLK_S_SYSMUX     	"sys_mux"// System clock muxed - before divider
#define NUC980_CLK_S_SYSCLK     	"sys_clk"// System clock muxed and divided
#define NUC980_CLK_S_CPUCLK_GATE    "cpu_clk_gate"// CPU Clock before gate
#define NUC980_CLK_S_CPUCLK    		"cpu_clk"// CPU clock
#define NUC980_CLK_S_SYSCLK2_DIV	"sys_clk_div2"// Sys clock divider for hclk/pclk
#define NUC980_CLK_S_DDR			"ddr_clk"
#define NUC980_CLK_S_HCLK			"hclk"
#define NUC980_CLK_S_HCLK1			"hclk1"
#define NUC980_CLK_S_HCLK3			"hclk3"
#define NUC980_CLK_S_HCLK4			"hclk4"
#define NUC980_CLK_S_HCLK1_DIV2		"hclk1_div2"
#define NUC980_CLK_S_PCLK0			"pclk0"
#define NUC980_CLK_S_PCLK1			"pclk1"
#define NUC980_CLK_S_PCLK2			"pclk2"
#define NUC980_CLK_S_PCLK_DIV4096	"pclk_div4096"
#define NUC980_CLK_S_UART_MUX(uart_num)    	"uart" #uart_num "_clk_mux"
#define NUC980_CLK_S_UART_DIV(uart_num)    	"uart" #uart_num "_clk_div"
#define NUC980_CLK_S_UART_CLK(uart_num)		"uart" #uart_num "_clk"
#define NUC980_CLK_S_TIM_MUX(uart_num)    	"time" #uart_num "_clk_mux"
#define NUC980_CLK_S_TIM_CLK(uart_num)    	"tim" #uart_num "_clk"

struct nuc980_clk_pll_data {
	u32 reg;
	const char *name;
	const char *parent_name;
	unsigned long flags;
	/*
	 * If this clock is exported via DT, set onecell_idx to constant
	 * defined in include/dt-bindings/clock/nuvoton, NUC980-clock.h for
	 * this specific clock.  Otherwise, set to -1.
	 */
	int onecell_idx;
};

struct nuc980_clk_div_data {
	u32 reg;
	u8 shift;
	u8 width;
	const char *name;
	const char *parent_name;
	u8 clk_divider_flags;
	unsigned long flags;
	/*
	 * If this clock is exported via DT, set onecell_idx to constant
	 * defined in include/dt-bindings/clock/nuvoton, NUC980-clock.h for
	 * this specific clock.  Otherwise, set to -1.
	 */
	int onecell_idx;
};

struct nuc980_clk_mux_data {
	u32 reg;
	u8 shift;
	u8 mask;
	u32 *table;
	const char *name;
	const char * const *parent_names;
	u8 num_parents;
	unsigned long flags;
	/*
	 * If this clock is exported via DT, set onecell_idx to constant
	 * defined in include/dt-bindings/clock/nuvoton, NUC980-clock.h for
	 * this specific clock.  Otherwise, set to -1.
	 */
	int onecell_idx;
};

struct nuc980_clk_gate_data {
	u32 reg;
	u8 shift;
	const char *name;
	const char *parent_name;
	u8 clk_gate_flags;
	unsigned long flags;
	/*
	 * If this clock is exported via DT, set onecell_idx to constant
	 * defined in include/dt-bindings/clock/nuvoton, NUC980-clock.h for
	 * this specific clock.  Otherwise, set to -1.
	 */
	int onecell_idx;
};

// PLL Clock control methods
struct nuc980_clk_pll {
	struct clk_hw hw;
	void __iomem *base;
};
#define to_nuc980_clk_pll(_hw) container_of(_hw, struct nuc980_clk_pll, hw)

static unsigned long clk_pll_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct nuc980_clk_pll *pll = to_nuc980_clk_pll(hw);
	long long ll;
	u32 reg = readl(pll->base) & 0x0FFFFFFF;

	if (parent_rate != 12000000)
		return 0;

	switch (reg) {
        case 0x15:
            ll = 264000000;	/* system default, 264MHz */
            break;
        case 0x18:
            ll = 300000000;
            break;
        case 0x8027:
            ll = 98000000;	/* usbh */
            break;
        case 0x8028:
            ll = 98400000;	/* i2s */
            break;
        case 0x21f0:
            ll = 169500000;	/* i2s */
            break;
        default:
            ll = 264000000;
            break;
	}

	return ll;
}

static int clk_pll_enable(struct clk_hw *hw)
{
	struct nuc980_clk_pll *pll = to_nuc980_clk_pll(hw);

	u32 val = readl(pll->base);
	val &= ~0x10000000;	/* PD = 0, power down mode disable */
	val |= 0x40000000;	/* RESETN = 1 */
	writel(val, pll->base);

	return 0;
}

static void clk_pll_disable(struct clk_hw *hw)
{
	struct nuc980_clk_pll *pll = to_nuc980_clk_pll(hw);

	u32 val = readl(pll->base);
	val |= 0x10000000;	/* PD = 1, power down mode enable */
	val &= ~0x40000000;	/* RESETN = 1 */
	writel(val, pll->base);
}

static int clk_pll_set_rate(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
	struct nuc980_clk_pll *pll = to_nuc980_clk_pll(hw);
	u32 reg = readl(pll->base) & ~0x0FFFFFFF;

	switch (rate) {
        case 98000000:		/* usbh */
            reg |= 0x8027;
            break;
        case 98400000:		/* i2s */
            reg |= 0x8028;
            break;
        case 169500000:		/* i2s */
            reg |= 0x21f0;
            break;
        case 264000000:		/* system default, 264MHz */
            reg |= 0x15;
            break;
        case 300000000:
            reg |= 0x18;
            break;
        default:
            reg |= 0x15;
            break;
	}
	writel(reg, pll->base);

	return 0;
}

static long clk_pll_round_rate(struct clk_hw *hw, unsigned long rate, unsigned long *prate)
{
	return rate;
}

static struct clk_ops clk_pll_ops = {
	.recalc_rate = clk_pll_recalc_rate,
	.enable = clk_pll_enable,
	.disable = clk_pll_disable,
	.set_rate = clk_pll_set_rate,
	.round_rate = clk_pll_round_rate,
};

static struct clk_hw *nuc980_clk_register_pll(void __iomem *base, const char *name, const char *parent, unsigned long flags)
{
	struct nuc980_clk_pll *pll;
	struct clk_hw *clk;
	struct clk_init_data init;
	int ret;

	pll = kmalloc(sizeof(*pll), GFP_KERNEL);

	if (!pll)
		return ERR_PTR(-ENOMEM);

	pll->base = base;
	init.name = name;
	init.ops = &clk_pll_ops;
	init.flags = flags;
	init.parent_names = &parent;
	init.num_parents = 1;
	pll->hw.init = &init;

	clk = &pll->hw;
	ret = clk_hw_register(NULL, clk);
	if (ret) {
		kfree(pll);
		clk = ERR_PTR(ret);
	}

	return clk;
}

static const struct nuc980_clk_pll_data nuc980_plls[] __initconst = {
	{ CLK_APLLCON, NUC980_CLK_S_APLL, NUC980_CLK_S_XIN, 0, -1 },
	{ CLK_UPLLCON, NUC980_CLK_S_UPLL, NUC980_CLK_S_XIN, 0, -1 },
};

static const struct nuc980_clk_div_data nuc980_divs[] __initconst = {
    { CLK_DIVCTL0, 8, 1, NUC980_CLK_S_SYSCLK, NUC980_CLK_S_SYSMUX, 0, CLK_IS_CRITICAL, -1 },
    { CLK_DIVCTL0, 16, 1, NUC980_CLK_S_CPUCLK_GATE, NUC980_CLK_S_SYSCLK, 0, CLK_IS_CRITICAL, -1 },
	{ CLK_DIVCTL4, 5, 3, NUC980_CLK_S_UART_DIV(0), NUC980_CLK_S_UART_MUX(0), 0, 0, -1 },
	{ CLK_DIVCTL4, 13, 3, NUC980_CLK_S_UART_DIV(1), NUC980_CLK_S_UART_MUX(1), 0, 0, -1 },
	{ CLK_DIVCTL4, 21, 3, NUC980_CLK_S_UART_DIV(2), NUC980_CLK_S_UART_MUX(2), 0, 0, -1 },
	{ CLK_DIVCTL4, 29, 3, NUC980_CLK_S_UART_DIV(3), NUC980_CLK_S_UART_MUX(3), 0, 0, -1 },
	{ CLK_DIVCTL5, 5, 3, NUC980_CLK_S_UART_DIV(4), NUC980_CLK_S_UART_MUX(4), 0, 0, -1 },
	{ CLK_DIVCTL5, 13, 3, NUC980_CLK_S_UART_DIV(5), NUC980_CLK_S_UART_MUX(5), 0, 0, -1 },
	{ CLK_DIVCTL5, 21, 3, NUC980_CLK_S_UART_DIV(6), NUC980_CLK_S_UART_MUX(6), 0, 0, -1 },
	{ CLK_DIVCTL5, 29, 3, NUC980_CLK_S_UART_DIV(7), NUC980_CLK_S_UART_MUX(7), 0, 0, -1 },
	{ CLK_DIVCTL6, 5, 3, NUC980_CLK_S_UART_DIV(8), NUC980_CLK_S_UART_MUX(8), 0, 0, -1 },
	{ CLK_DIVCTL6, 13, 3, NUC980_CLK_S_UART_DIV(9), NUC980_CLK_S_UART_MUX(9), 0, 0, -1 },
};

static u32 sysclk_mux_table[] = {0, 2, 3};
static const char * const sysclk_mux_parents[] __initconst = { NUC980_CLK_S_XIN, NUC980_CLK_S_APLL, NUC980_CLK_S_UPLL };
static u32 uart_mux_table[] = {0, 1, 2, 3};
static const char * const uart_mux_parents[] __initconst = { NUC980_CLK_S_XIN, NUC980_CLK_S_XIN32K, NUC980_CLK_S_APLL, NUC980_CLK_S_UPLL };
static u32 timer_mux_table[] = {0, 1, 2, 3};
static const char * const timer01_45_mux_parents[] __initconst = { NUC980_CLK_S_XIN, NUC980_CLK_S_PCLK0, NUC980_CLK_S_PCLK_DIV4096, NUC980_CLK_S_XIN32K };
static const char * const timer23_mux_parents[] __initconst = { NUC980_CLK_S_XIN, NUC980_CLK_S_PCLK1, NUC980_CLK_S_PCLK_DIV4096, NUC980_CLK_S_XIN32K };
static const struct nuc980_clk_mux_data nuc980_muxes[] __initconst = {
	{ CLK_DIVCTL0, 3, 2, sysclk_mux_table, NUC980_CLK_S_SYSMUX, sysclk_mux_parents, ARRAY_SIZE(sysclk_mux_parents), 0, -1 },
	{ CLK_DIVCTL4, 3, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(0), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL4, 11, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(1), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL4, 19, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(2), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL4, 27, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(3), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL5, 3, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(4), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL5, 11, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(5), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL5, 19, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(6), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL5, 27, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(7), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL6, 3, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(8), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL6, 11, 2, uart_mux_table, NUC980_CLK_S_UART_MUX(9), uart_mux_parents, ARRAY_SIZE(uart_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 16, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(0), timer01_45_mux_parents, ARRAY_SIZE(timer01_45_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 18, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(1), timer01_45_mux_parents, ARRAY_SIZE(timer01_45_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 20, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(2), timer23_mux_parents, ARRAY_SIZE(timer23_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 22, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(3), timer23_mux_parents, ARRAY_SIZE(timer23_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 24, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(4), timer01_45_mux_parents, ARRAY_SIZE(timer01_45_mux_parents), 0, -1 },
	{ CLK_DIVCTL8, 26, 2, timer_mux_table, NUC980_CLK_S_TIM_MUX(5), timer01_45_mux_parents, ARRAY_SIZE(timer01_45_mux_parents), 0, -1 },
};

// TODO: For every periphal have to seperate clock setups - clock type selection & enable
static const struct nuc980_clk_gate_data nuc980_gates[] __initconst = {
	{ CLK_HCLKEN, 0, NUC980_CLK_S_CPUCLK, NUC980_CLK_S_CPUCLK_GATE, 0, CLK_IS_CRITICAL, NUC980_CLK_CPU },// CPU Clock gate
	{ CLK_HCLKEN, 10, NUC980_CLK_S_DDR, NUC980_CLK_S_SYSCLK, 0, CLK_IS_CRITICAL, NUC980_CLK_DDR }, // DDR Clock Gate
	{ CLK_HCLKEN, 1, NUC980_CLK_S_HCLK, NUC980_CLK_S_SYSCLK2_DIV, 0, CLK_IS_CRITICAL, -1 }, // HCLK Clock Gate
	{ CLK_HCLKEN, 2, NUC980_CLK_S_HCLK1, NUC980_CLK_S_SYSCLK2_DIV, 0, CLK_IS_CRITICAL, -1 }, // HCLK1 Clock Gate
	{ CLK_HCLKEN, 3, NUC980_CLK_S_HCLK3, NUC980_CLK_S_SYSCLK2_DIV, 0, CLK_IS_CRITICAL, -1 }, // HCLK3 Clock Gate
	{ CLK_HCLKEN, 4, NUC980_CLK_S_HCLK4, NUC980_CLK_S_SYSCLK2_DIV, 0, CLK_IS_CRITICAL, -1 }, // HCLK4 Clock Gate
	{ CLK_HCLKEN, 5, NUC980_CLK_S_PCLK0, NUC980_CLK_S_HCLK1, 0, CLK_IS_CRITICAL, -1 }, // PCLK0 Clock Gate
	{ CLK_HCLKEN, 6, NUC980_CLK_S_PCLK1, NUC980_CLK_S_HCLK1, 0, CLK_IS_CRITICAL, -1 }, // PCLK1 Clock Gate
	{ CLK_HCLKEN, 14, NUC980_CLK_S_PCLK2, NUC980_CLK_S_HCLK1_DIV2, 0, CLK_IS_CRITICAL, -1 }, // PCLK2 Clock Gate
	{ CLK_PCLKEN0, 16, NUC980_CLK_S_UART_CLK(0), NUC980_CLK_S_UART_DIV(0), 0, 0, NUC980_CLK_UART0 },
	{ CLK_PCLKEN0, 17, NUC980_CLK_S_UART_CLK(1), NUC980_CLK_S_UART_DIV(1), 0, 0, NUC980_CLK_UART1 },
	{ CLK_PCLKEN0, 18, NUC980_CLK_S_UART_CLK(2), NUC980_CLK_S_UART_DIV(2), 0, 0, NUC980_CLK_UART2 },
	{ CLK_PCLKEN0, 19, NUC980_CLK_S_UART_CLK(3), NUC980_CLK_S_UART_DIV(3), 0, 0, NUC980_CLK_UART3 },
	{ CLK_PCLKEN0, 20, NUC980_CLK_S_UART_CLK(4), NUC980_CLK_S_UART_DIV(4), 0, 0, NUC980_CLK_UART4 },
	{ CLK_PCLKEN0, 21, NUC980_CLK_S_UART_CLK(5), NUC980_CLK_S_UART_DIV(5), 0, 0, NUC980_CLK_UART5 },
	{ CLK_PCLKEN0, 22, NUC980_CLK_S_UART_CLK(6), NUC980_CLK_S_UART_DIV(6), 0, 0, NUC980_CLK_UART6 },
	{ CLK_PCLKEN0, 23, NUC980_CLK_S_UART_CLK(7), NUC980_CLK_S_UART_DIV(7), 0, 0, NUC980_CLK_UART7 },
	{ CLK_PCLKEN0, 24, NUC980_CLK_S_UART_CLK(8), NUC980_CLK_S_UART_DIV(8), 0, 0, NUC980_CLK_UART8 },
	{ CLK_PCLKEN0, 25, NUC980_CLK_S_UART_CLK(9), NUC980_CLK_S_UART_DIV(9), 0, 0, NUC980_CLK_UART9 },
	{ CLK_PCLKEN0, 8, NUC980_CLK_S_TIM_CLK(0), NUC980_CLK_S_TIM_MUX(0), 0, NUC980_CLK_TIM0 },
	{ CLK_PCLKEN0, 9, NUC980_CLK_S_TIM_CLK(1), NUC980_CLK_S_TIM_MUX(1), 0, NUC980_CLK_TIM1 },
	{ CLK_PCLKEN0, 10, NUC980_CLK_S_TIM_CLK(2), NUC980_CLK_S_TIM_MUX(2), 0, NUC980_CLK_TIM2 },
	{ CLK_PCLKEN0, 11, NUC980_CLK_S_TIM_CLK(3), NUC980_CLK_S_TIM_MUX(3), 0, NUC980_CLK_TIM3 },
	{ CLK_PCLKEN0, 12, NUC980_CLK_S_TIM_CLK(4), NUC980_CLK_S_TIM_MUX(4), 0, NUC980_CLK_TIM4 },
	{ CLK_PCLKEN0, 13, NUC980_CLK_S_TIM_CLK(5), NUC980_CLK_S_TIM_MUX(5), 0, NUC980_CLK_TIM5 },
};

static DEFINE_SPINLOCK(nuc980_clk_lock);

static void __init nuc980_clk_init(struct device_node *clk_np)
{
    printk("Initializing nuc980_clk......\n");
    struct clk_hw_onecell_data *nuc980_clk_data;
    void __iomem *clk_base;
    struct resource res;
    struct clk_hw *hw;

    // Get information about register mapping from device tree
    int ret = of_address_to_resource(clk_np, 0, &res);
    if (ret) {
		pr_err("%pOFn: failed to get resource, ret %d\n", clk_np,
			ret);
		return;
	}

    // Get the clock registers
    clk_base = ioremap(res.start, resource_size(&res));
	if (!clk_base) {
        pr_err("OFn: failed to ioremap: %p\n", clk_base);
        of_node_put(clk_np);
        return;
    }

    // Allocate enough room for all defined clock types
    nuc980_clk_data = kzalloc(struct_size(nuc980_clk_data, hws, NUC980_NUM_CLOCKS), GFP_KERNEL);
	if (!nuc980_clk_data) {
        pr_err("OFn: failed to kzalloc: %p\n", nuc980_clk_data);
        iounmap(clk_base);
        of_node_put(clk_np);
        return;
    }
	nuc980_clk_data->num = NUC980_NUM_CLOCKS;

    // Initially all clock configurations will return error
    for (int i = 0; i < NUC980_NUM_CLOCKS; i++)
		nuc980_clk_data->hws[i] = ERR_PTR(-EPROBE_DEFER);

    /* Register plls */
	for (int i = 0; i < ARRAY_SIZE(nuc980_plls); i++) {
		const struct nuc980_clk_pll_data *pll_data = &nuc980_plls[i];

		hw = nuc980_clk_register_pll(clk_base + pll_data->reg, pll_data->name, pll_data->parent_name, pll_data->flags);
		if (IS_ERR(hw)) {
			pr_err("nuc980_clk: Can't register pll\n");
			goto nuc980_init_fail;
		}

		if (pll_data->onecell_idx >= 0)
			nuc980_clk_data->hws[pll_data->onecell_idx] = hw;
	}

    /* Register fixed dividers */
	hw = clk_hw_register_fixed_factor(NULL, NUC980_CLK_S_XIN512_DIV, NUC980_CLK_S_XIN, 0, 1, 512);
	if (IS_ERR(hw)) {
		pr_err("nuc980_clk: Can't register fixed div\n");
		goto nuc980_init_fail;
	}
	hw = clk_hw_register_fixed_factor(NULL, NUC980_CLK_S_SYSCLK2_DIV, NUC980_CLK_S_SYSCLK, 0, 1, 2);
	if (IS_ERR(hw)) {
		pr_err("nuc980_clk: Can't register fixed div\n");
		goto nuc980_init_fail;
	}
	hw = clk_hw_register_fixed_factor(NULL, NUC980_CLK_S_HCLK1_DIV2, NUC980_CLK_S_HCLK1, 0, 1, 2);
	if (IS_ERR(hw)) {
		pr_err("nuc980_clk: Can't register fixed div\n");
		goto nuc980_init_fail;
	}
	hw = clk_hw_register_fixed_factor(NULL, NUC980_CLK_S_PCLK_DIV4096, NUC980_CLK_S_HCLK1_DIV2, 0, 1, 4096);
	if (IS_ERR(hw)) {
		pr_err("nuc980_clk: Can't register fixed div\n");
		goto nuc980_init_fail;
	}

    /* Register clock dividers specified in nuc980_divs */
    for (int i = 0; i < ARRAY_SIZE(nuc980_divs); i++) {
		const struct nuc980_clk_div_data *div_data = &nuc980_divs[i];

		hw = clk_hw_register_divider(NULL, div_data->name,
				div_data->parent_name,
				div_data->flags,
				clk_base + div_data->reg,
				div_data->shift, div_data->width,
				div_data->clk_divider_flags, &nuc980_clk_lock);
		if (IS_ERR(hw)) {
			pr_err("nuc980_clk: Can't register div table\n");
			goto nuc980_init_fail;
		}

		if (div_data->onecell_idx >= 0)
			nuc980_clk_data->hws[div_data->onecell_idx] = hw;
	}

    /* Register muxes */
	for (int i = 0; i < ARRAY_SIZE(nuc980_muxes); i++) {
		const struct nuc980_clk_mux_data *mux_data = &nuc980_muxes[i];

		hw = clk_hw_register_mux_table(NULL,
			mux_data->name,
			mux_data->parent_names, mux_data->num_parents,
			mux_data->flags, clk_base + mux_data->reg,
			mux_data->shift, mux_data->mask, 0,
			mux_data->table, &nuc980_clk_lock);

		if (IS_ERR(hw)) {
			pr_err("nuc980_clk: Can't register mux\n");
			goto nuc980_init_fail;
		}

		if (mux_data->onecell_idx >= 0)
			nuc980_clk_data->hws[mux_data->onecell_idx] = hw;
	}

	/* Register gates */
	for (int i = 0; i < ARRAY_SIZE(nuc980_gates); i++) {
		const struct nuc980_clk_gate_data *gate_data = &nuc980_gates[i];

		hw = clk_hw_register_gate(NULL, gate_data->name, gate_data->parent_name, 
			gate_data->flags,
			clk_base + gate_data->reg,
			gate_data->shift,
			gate_data->clk_gate_flags,
			&nuc980_clk_lock);

		if (IS_ERR(hw)) {
			pr_err("nuc980_clk: Can't register mux\n");
			goto nuc980_init_fail;
		}

		if (gate_data->onecell_idx >= 0)
			nuc980_clk_data->hws[gate_data->onecell_idx] = hw;
	}

    ret = of_clk_add_hw_provider(clk_np, of_clk_hw_onecell_get, nuc980_clk_data);
	if (ret)
		pr_err("failed to add DT provider: %d\n", ret);

	of_node_put(clk_np);

    return;

nuc980_init_fail:
	kfree(nuc980_clk_data);
	iounmap(clk_base);
	of_node_put(clk_np);
}

CLK_OF_DECLARE(nuc980_clk_init, "nuvoton,nuc980-clk", nuc980_clk_init);
