/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Nuvoton NPCM7xx Clock Generator binding
 * clock binding number for all clocks supported by nuvoton,npcm7xx-clk
 *
 * Copyright (C) 2018 Nuvoton Technologies tali.perry@nuvoton.com
 *
 */

#ifndef __DT_BINDINGS_CLOCK_NPCM7XX_H
#define __DT_BINDINGS_CLOCK_NPCM7XX_H


#define NUC980_CLK_CPU 0
#define NUC980_CLK_DDR 1
#define NUC980_CLK_UART0 2
#define NUC980_CLK_UART1 3
#define NUC980_CLK_UART2 4
#define NUC980_CLK_UART3 5
#define NUC980_CLK_UART4 6
#define NUC980_CLK_UART5 7
#define NUC980_CLK_UART6 8
#define NUC980_CLK_UART7 9
#define NUC980_CLK_UART8 10
#define NUC980_CLK_UART9 11
#define NUC980_CLK_TIM0 12
#define NUC980_CLK_TIM1 13
#define NUC980_CLK_TIM2 14
#define NUC980_CLK_TIM3 15
#define NUC980_CLK_TIM4 16
#define NUC980_CLK_TIM5 17
#define NUC980_CLK_SDH  18
#define NUC980_CLK_PDMA0 19
#define NUC980_CLK_PDMA1 20
#define NUC980_CLK_EMAC0 21
#define NUC980_CLK_EMAC1 22
#define NUC980_CLK_CRYPTO 23
#define NUC980_CLK_XIN 24
#define NUC980_CLK_XIN32K 25

#define NUC980_NUM_CLOCKS	 (NUC980_CLK_XIN32K+1)

#endif
