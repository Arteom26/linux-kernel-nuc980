/*
 * Copyright (c) 2018 Nuvoton Technology Corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <linux/clk.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/dmaengine.h>
#include <linux/module.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
/*
 * Copyright (c) 2018 Nuvoton Technology Corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <asm/io.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/platform_data/dma-nuc980.h>
#include <asm/irq.h>
#include <linux/platform_device.h>

#define   	__I     volatile const       /*!< Defines 'read only' permissions */
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

typedef struct {
	__IO uint32_t CTL;             /*!< [0x0000] Descriptor Table Control Register of PDMA Channel n.             */
	__IO uint32_t SA;              /*!< [0x0004] Source Address Register of PDMA Channel n                        */
	__IO uint32_t DA;              /*!< [0x0008] Destination Address Register of PDMA Channel n                   */
	__IO uint32_t NEXT;            /*!< [0x000c] First Scatter-Gather Descriptor Table Offset Address of PDMA Channel n */
} DSCT_T;


typedef struct {
	__IO uint32_t STCR;           /*!< [0x0500] Stride Transfer Count Register of PDMA Channel 0                 */
	__IO uint32_t ASOCR;          /*!< [0x0504] Address Stride Offset Register of PDMA Channel 0                 */
} STRIDE_T;

typedef struct {
	DSCT_T DSCT[16];
	__I  uint32_t CURSCAT[16];              /*!< [0x0100] Current Scatter-Gather Descriptor Table Address of PDMA Channel n */
	/// @cond HIDDEN_SYMBOLS
	__I  uint32_t RESERVE1[176];
	/// @endcond //HIDDEN_SYMBOLS
	__IO uint32_t CHCTL;                 /*!< [0x0400] PDMA Channel Control Register                                    */
	__O  uint32_t PAUSE;                 /*!< [0x0404] PDMA Transfer Pause Control Register                              */
	__O  uint32_t SWREQ;                 /*!< [0x0408] PDMA Software Request Register                                   */
	__I  uint32_t TRGSTS;                /*!< [0x040c] PDMA Channel Request Status Register                             */
	__IO uint32_t PRISET;                /*!< [0x0410] PDMA Fixed Priority Setting Register                             */
	__O  uint32_t PRICLR;                /*!< [0x0414] PDMA Fixed Priority Clear Register                               */
	__IO uint32_t INTEN;                 /*!< [0x0418] PDMA Interrupt Enable Register                                   */
	__IO uint32_t INTSTS;                /*!< [0x041c] PDMA Interrupt Status Register                                   */
	__IO uint32_t ABTSTS;                /*!< [0x0420] PDMA Channel Read/Write Target Abort Flag Register               */
	__IO uint32_t TDSTS;                 /*!< [0x0424] PDMA Channel Transfer Done Flag Register                         */
	__IO uint32_t ALIGN;                 /*!< [0x0428] PDMA Transfer Alignment Status Register                          */
	__I  uint32_t TACTSTS;               /*!< [0x042c] PDMA Transfer Active Flag Register                               */
	__IO uint32_t TOUTPSC;               /*!< [0x0430] PDMA Time-out Prescaler Register                                 */
	__IO uint32_t TOUTEN;                /*!< [0x0434] PDMA Time-out Enable Register                                    */
	__IO uint32_t TOUTIEN;               /*!< [0x0438] PDMA Time-out Interrupt Enable Register                          */
	__IO uint32_t SCATBA;                /*!< [0x043c] PDMA Scatter-Gather Descriptor Table Base Address Register       */
	__IO uint32_t TOC0_1;                /*!< [0x0440] PDMA Time-out Counter Ch1 and Ch0 Register                       */
	__IO uint32_t TOC2_3;               /*!< [0x0444]  PDMA Time-out Counter Ch3 and Ch2 Register                        */
	__IO uint32_t TOC4_5;               /*!< [0x0448]  PDMA Time-out Counter Ch5 and Ch4 Register                        */
	__IO uint32_t TOC6_7;           /*!< [0x044c]  PDMA Time-out Counter Ch7 and Ch6 Register                        */
	__IO uint32_t TOC8_9;           /*!< [0x0450]  PDMA Time-out Counter Ch9 and Ch8 Register                        */
	/// @cond HIDDEN_SYMBOLS
	__I  uint32_t RESERVE2[3];
	/// @endcond //HIDDEN_SYMBOLS
	__IO uint32_t CHRST;                 /*!< [0x0460] PDMA Channel Reset Register                                      */
	/// @cond HIDDEN_SYMBOLS
	__I  uint32_t RESERVE3[3];
	/// @endcond //HIDDEN_SYMBOLS
	__IO uint32_t TOUTPSC2;      /* Offset: 0x470  PDMA Time-out Prescaler Register                                  */
	/// @cond HIDDEN_SYMBOLS
	__I  uint32_t RESERVE4[3];
	/// @endcond //HIDDEN_SYMBOLS
	__IO uint32_t REQSEL0_3;             /*!< [0x0480] PDMA Request Source Select Register 0                            */
	__IO uint32_t REQSEL4_7;             /*!< [0x0484] PDMA Request Source Select Register 1                            */
	__IO uint32_t REQSEL8_11;            /*!< [0x0488] PDMA Request Source Select Register 2                            */
	__IO uint32_t REQSEL12_15;           /*!< [0x048c] PDMA Request Source Select Register 3                            */
	/// @cond HIDDEN_SYMBOLS
	__I  uint32_t RESERVE5[28];
	/// @endcond //HIDDEN_SYMBOLS
	STRIDE_T     STRIDE[6];
} PDMA_T;

/**
    @addtogroup PDMA_CONST PDMA Bit Field Definition
    Constant Definitions for PDMA Controller
@{ */

#define PDMA_DSCT_CTL_OPMODE_Pos        (0)                                               /*!< PDMA_T::DSCT_CTL: OPMODE Position     */
#define PDMA_DSCT_CTL_OPMODE_Msk        (0x3ul << PDMA_DSCT_CTL_OPMODE_Pos)               /*!< PDMA_T::DSCT_CTL: OPMODE Mask         */

#define PDMA_DSCT_CTL_TXTYPE_Pos        (2)                                               /*!< PDMA_T::DSCT_CTL: TXTYPE Position     */
#define PDMA_DSCT_CTL_TXTYPE_Msk        (0x1ul << PDMA_DSCT_CTL_TXTYPE_Pos)               /*!< PDMA_T::DSCT_CTL: TXTYPE Mask         */

#define PDMA_DSCT_CTL_BURSIZE_Pos       (4)                                               /*!< PDMA_T::DSCT_CTL: BURSIZE Position    */
#define PDMA_DSCT_CTL_BURSIZE_Msk       (0x7ul << PDMA_DSCT_CTL_BURSIZE_Pos)              /*!< PDMA_T::DSCT_CTL: BURSIZE Mask        */

#define PDMA_DSCT_CTL_TBINTDIS_Pos      (7)                                               /*!< PDMA_T::DSCT_CTL: TBINTDIS Position      */
#define PDMA_DSCT_CTL_TBINTDIS_Msk      (0x1ul << PDMA_DSCT_CTL_TBINTDIS_Pos)             /*!< PDMA_T::DSCT_CTL: TBINTDIS Mask          */

#define PDMA_DSCT_CTL_SAINC_Pos         (8)                                               /*!< PDMA_T::DSCT_CTL: SAINC Position      */
#define PDMA_DSCT_CTL_SAINC_Msk         (0x3ul << PDMA_DSCT_CTL_SAINC_Pos)                /*!< PDMA_T::DSCT_CTL: SAINC Mask          */

#define PDMA_DSCT_CTL_DAINC_Pos         (10)                                              /*!< PDMA_T::DSCT_CTL: DAINC Position      */
#define PDMA_DSCT_CTL_DAINC_Msk         (0x3ul << PDMA_DSCT_CTL_DAINC_Pos)                /*!< PDMA_T::DSCT_CTL: DAINC Mask          */

#define PDMA_DSCT_CTL_TXWIDTH_Pos       (12)                                              /*!< PDMA_T::DSCT_CTL: TXWIDTH Position    */
#define PDMA_DSCT_CTL_TXWIDTH_Msk       (0x3ul << PDMA_DSCT_CTL_TXWIDTH_Pos)              /*!< PDMA_T::DSCT_CTL: TXWIDTH Mask        */

#define PDMA_DSCT_CTL_TXACK_Pos         (14)                                              /*!< PDMA_T::DSCT_CTL: TXACK Position      */
#define PDMA_DSCT_CTL_TXACK_Msk         (0x1ul << PDMA_DSCT_CTL_TXACK_Pos)                /*!< PDMA_T::DSCT_CTL: TXACK Mask          */

#define PDMA_DSCT_CTL_STRIDEEN_Pos     (15)                                               /*!< PDMA_T::DSCT_CTL: STRIDEEN Position  */
#define PDMA_DSCT_CTL_STRIDEEN_Msk     (0x1ul << PDMA_DSCT_CTL_STRIDEEN_Pos)              /*!< PDMA_T::DSCT_CTL: STRIDEEN Mask      */

#define PDMA_DSCT_CTL_TXCNT_Pos         (16)                                              /*!< PDMA_T::DSCT_CTL: TXCNT Position      */
#define PDMA_DSCT_CTL_TXCNT_Msk         (0xfffful << PDMA_DSCT_CTL_TXCNT_Pos)             /*!< PDMA_T::DSCT_CTL: TXCNT Mask          */

#define PDMA_DSCT_SA_SA_Pos             (0)                                               /*!< PDMA_T::DSCT_SA: SA Position          */
#define PDMA_DSCT_SA_SA_Msk             (0xfffffffful << PDMA_DSCT_SA_SA_Pos)             /*!< PDMA_T::DSCT_SA: SA Mask              */

#define PDMA_DSCT_DA_DA_Pos             (0)                                               /*!< PDMA_T::DSCT_DA: DA Position          */
#define PDMA_DSCT_DA_DA_Msk             (0xfffffffful << PDMA_DSCT_DA_DA_Pos)             /*!< PDMA_T::DSCT_DA: DA Mask              */

#define PDMA_DSCT_NEXT_NEXT_Pos         (0)                                               /*!< PDMA_T::DSCT_NEXT: NEXT Position      */
#define PDMA_DSCT_NEXT_NEXT_Msk         (0xfffful << PDMA_DSCT_NEXT_NEXT_Pos)             /*!< PDMA_T::DSCT_NEXT: NEXT Mask          */

#define PDMA_DSCT_NEXT_EXENEXT_Pos      (16)                                              /*!< PDMA_T::DSCT_FIRST: NEXT Position     */
#define PDMA_DSCT_NEXT_EXENEXT_Msk      (0xfffful << PDMA_DSCT_NEXT_EXENEXT_Pos)           /*!< PDMA_T::DSCT_FIRST: NEXT Mask         */

#define PDMA_CURSCAT_CURADDR_Pos        (0)                                               /*!< PDMA_T::CURSCAT: CURADDR Position     */
#define PDMA_CURSCAT_CURADDR_Msk        (0xfffffffful << PDMA_CURSCAT_CURADDR_Pos)        /*!< PDMA_T::CURSCAT: CURADDR Mask         */

#define PDMA_CHCTL_CHENn_Pos            (0)                                               /*!< PDMA_T::CHCTL: CHENn Position          */
#define PDMA_CHCTL_CHENn_Msk            (0xfffful << PDMA_CHCTL_CHENn_Pos)                /*!< PDMA_T::CHCTL: CHENn Mask              */

#define PDMA_PAUSE_PAUSEn_Pos           (0)                                               /*!< PDMA_T::PAUSE: PAUSEn Position           */
#define PDMA_PAUSE_PAUSEn_Msk           (0xfffful << PDMA_PAUSE_PAUSEn_Pos)              /*!< PDMA_T::PAUSE: PAUSEn Mask               */

#define PDMA_SWREQ_SWREQn_Pos            (0)                                               /*!< PDMA_T::SWREQ: SWREQn Position         */
#define PDMA_SWREQ_SWREQn_Msk            (0xfffful << PDMA_SWREQ_SWREQn_Pos)               /*!< PDMA_T::SWREQ: SWREQn Mask             */

#define PDMA_TRGSTS_REQSTSn_Pos          (0)                                               /*!< PDMA_T::TRGSTS: REQSTSn Position       */
#define PDMA_TRGSTS_REQSTSn_Msk          (0xfffful << PDMA_TRGSTS_REQSTSn_Pos)             /*!< PDMA_T::TRGSTS: REQSTSn Mask           */

#define PDMA_PRISET_FPRISETn_Pos         (0)                                               /*!< PDMA_T::PRISET: FPRISETn Position      */
#define PDMA_PRISET_FPRISETn_Msk         (0xfffful << PDMA_PRISET_FPRISETn_Pos)            /*!< PDMA_T::PRISET: FPRISETn Mask          */

#define PDMA_PRICLR_FPRICLRn_Pos         (0)                                               /*!< PDMA_T::PRICLR: FPRICLRn Position      */
#define PDMA_PRICLR_FPRICLRn_Msk         (0xfffful << PDMA_PRICLR_FPRICLRn_Pos)            /*!< PDMA_T::PRICLR: FPRICLRn Mask          */

#define PDMA_INTEN_INTENn_Pos            (0)                                               /*!< PDMA_T::INTEN: INTENn Position         */
#define PDMA_INTEN_INTENn_Msk            (0xfffful << PDMA_INTEN_INTENn_Pos)               /*!< PDMA_T::INTEN: INTENn Mask             */

#define PDMA_INTSTS_ABTIF_Pos            (0)                                               /*!< PDMA_T::INTSTS: ABTIF Position         */
#define PDMA_INTSTS_ABTIF_Msk            (0x1ul << PDMA_INTSTS_ABTIF_Pos)                  /*!< PDMA_T::INTSTS: ABTIF Mask             */

#define PDMA_INTSTS_TDIF_Pos             (1)                                               /*!< PDMA_T::INTSTS: TDIF Position          */
#define PDMA_INTSTS_TDIF_Msk             (0x1ul << PDMA_INTSTS_TDIF_Pos)                   /*!< PDMA_T::INTSTS: TDIF Mask              */

#define PDMA_INTSTS_ALIGNF_Pos           (2)                                               /*!< PDMA_T::INTSTS: ALIGNF Position        */
#define PDMA_INTSTS_ALIGNF_Msk           (0x1ul << PDMA_INTSTS_ALIGNF_Pos)                 /*!< PDMA_T::INTSTS: ALIGNF Mask            */

#define PDMA_INTSTS_REQTOF0_Pos          (8)                                               /*!< PDMA_T::INTSTS: REQTOF0 Position       */
#define PDMA_INTSTS_REQTOF0_Msk          (0x1ul << PDMA_INTSTS_REQTOF0_Pos)                /*!< PDMA_T::INTSTS: REQTOF0 Mask           */

#define PDMA_INTSTS_REQTOF1_Pos          (9)                                               /*!< PDMA_T::INTSTS: REQTOF1 Position       */
#define PDMA_INTSTS_REQTOF1_Msk          (0x1ul << PDMA_INTSTS_REQTOF1_Pos)                /*!< PDMA_T::INTSTS: REQTOF1 Mask           */

#define PDMA_ABTSTS_ABTIF0_Pos           (0)                                               /*!< PDMA_T::ABTSTS: ABTIF0 Position        */
#define PDMA_ABTSTS_ABTIF0_Msk           (0x1ul << PDMA_ABTSTS_ABTIF0_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF0 Mask            */

#define PDMA_ABTSTS_ABTIF1_Pos           (1)                                               /*!< PDMA_T::ABTSTS: ABTIF1 Position        */
#define PDMA_ABTSTS_ABTIF1_Msk           (0x1ul << PDMA_ABTSTS_ABTIF1_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF1 Mask            */

#define PDMA_ABTSTS_ABTIF2_Pos           (2)                                               /*!< PDMA_T::ABTSTS: ABTIF2 Position        */
#define PDMA_ABTSTS_ABTIF2_Msk           (0x1ul << PDMA_ABTSTS_ABTIF2_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF2 Mask            */

#define PDMA_ABTSTS_ABTIF3_Pos           (3)                                               /*!< PDMA_T::ABTSTS: ABTIF3 Position        */
#define PDMA_ABTSTS_ABTIF3_Msk           (0x1ul << PDMA_ABTSTS_ABTIF3_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF3 Mask            */

#define PDMA_ABTSTS_ABTIF4_Pos           (4)                                               /*!< PDMA_T::ABTSTS: ABTIF4 Position        */
#define PDMA_ABTSTS_ABTIF4_Msk           (0x1ul << PDMA_ABTSTS_ABTIF4_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF4 Mask            */

#define PDMA_ABTSTS_ABTIF5_Pos           (5)                                               /*!< PDMA_T::ABTSTS: ABTIF5 Position        */
#define PDMA_ABTSTS_ABTIF5_Msk           (0x1ul << PDMA_ABTSTS_ABTIF5_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF5 Mask            */

#define PDMA_ABTSTS_ABTIF6_Pos           (6)                                               /*!< PDMA_T::ABTSTS: ABTIF6 Position        */
#define PDMA_ABTSTS_ABTIF6_Msk           (0x1ul << PDMA_ABTSTS_ABTIF6_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF6 Mask            */

#define PDMA_ABTSTS_ABTIF7_Pos           (7)                                               /*!< PDMA_T::ABTSTS: ABTIF7 Position        */
#define PDMA_ABTSTS_ABTIF7_Msk           (0x1ul << PDMA_ABTSTS_ABTIF7_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF7 Mask            */

#define PDMA_ABTSTS_ABTIF8_Pos           (8)                                               /*!< PDMA_T::ABTSTS: ABTIF8 Position        */
#define PDMA_ABTSTS_ABTIF8_Msk           (0x1ul << PDMA_ABTSTS_ABTIF8_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF8 Mask            */

#define PDMA_ABTSTS_ABTIF9_Pos           (9)                                               /*!< PDMA_T::ABTSTS: ABTIF9 Position        */
#define PDMA_ABTSTS_ABTIF9_Msk           (0x1ul << PDMA_ABTSTS_ABTIF9_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF9 Mask            */

#define PDMA_ABTSTS_ABTIF10_Pos           (10)                                               /*!< PDMA_T::ABTSTS: ABTIF10 Position        */
#define PDMA_ABTSTS_ABTIF10_Msk           (0x1ul << PDMA_ABTSTS_ABTIF10_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF10 Mask            */

#define PDMA_ABTSTS_ABTIF11_Pos           (11)                                               /*!< PDMA_T::ABTSTS: ABTIF11 Position        */
#define PDMA_ABTSTS_ABTIF11_Msk           (0x1ul << PDMA_ABTSTS_ABTIF11_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF11 Mask            */

#define PDMA_ABTSTS_ABTIF12_Pos           (12)                                               /*!< PDMA_T::ABTSTS: ABTIF12 Position        */
#define PDMA_ABTSTS_ABTIF12_Msk           (0x1ul << PDMA_ABTSTS_ABTIF12_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF12 Mask            */

#define PDMA_ABTSTS_ABTIF13_Pos           (13)                                               /*!< PDMA_T::ABTSTS: ABTIF13 Position        */
#define PDMA_ABTSTS_ABTIF13_Msk           (0x1ul << PDMA_ABTSTS_ABTIF13_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF13 Mask            */

#define PDMA_ABTSTS_ABTIF14_Pos           (14)                                               /*!< PDMA_T::ABTSTS: ABTIF14 Position        */
#define PDMA_ABTSTS_ABTIF14_Msk           (0x1ul << PDMA_ABTSTS_ABTIF14_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF14 Mask            */

#define PDMA_ABTSTS_ABTIF15_Pos           (15)                                               /*!< PDMA_T::ABTSTS: ABTIF15 Position        */
#define PDMA_ABTSTS_ABTIF15_Msk           (0x1ul << PDMA_ABTSTS_ABTIF15_Pos)                 /*!< PDMA_T::ABTSTS: ABTIF15 Mask            */

#define PDMA_TDSTS_TDIF0_Pos           (0)                                               /*!< PDMA_T::TDSTS: TDIF0 Position        */
#define PDMA_TDSTS_TDIF0_Msk           (0x1ul << PDMA_TDSTS_TDIF0_Pos)                   /*!< PDMA_T::TDSTS: TDIF0 Mask            */

#define PDMA_TDSTS_TDIF1_Pos           (1)                                               /*!< PDMA_T::TDSTS: TDIF1 Position        */
#define PDMA_TDSTS_TDIF1_Msk           (0x1ul << PDMA_TDSTS_TDIF1_Pos)                   /*!< PDMA_T::TDSTS: TDIF1 Mask            */

#define PDMA_TDSTS_TDIF2_Pos           (2)                                               /*!< PDMA_T::TDSTS: TDIF2 Position        */
#define PDMA_TDSTS_TDIF2_Msk           (0x1ul << PDMA_TDSTS_TDIF2_Pos)                   /*!< PDMA_T::TDSTS: TDIF2 Mask            */

#define PDMA_TDSTS_TDIF3_Pos           (3)                                               /*!< PDMA_T::TDSTS: TDIF3 Position        */
#define PDMA_TDSTS_TDIF3_Msk           (0x1ul << PDMA_TDSTS_TDIF3_Pos)                   /*!< PDMA_T::TDSTS: TDIF3 Mask            */

#define PDMA_TDSTS_TDIF4_Pos           (4)                                               /*!< PDMA_T::TDSTS: TDIF4 Position        */
#define PDMA_TDSTS_TDIF4_Msk           (0x1ul << PDMA_TDSTS_TDIF4_Pos)                   /*!< PDMA_T::TDSTS: TDIF4 Mask            */

#define PDMA_TDSTS_TDIF5_Pos           (5)                                               /*!< PDMA_T::TDSTS: TDIF5 Position        */
#define PDMA_TDSTS_TDIF5_Msk           (0x1ul << PDMA_TDSTS_TDIF5_Pos)                   /*!< PDMA_T::TDSTS: TDIF5 Mask            */

#define PDMA_TDSTS_TDIF6_Pos           (6)                                               /*!< PDMA_T::TDSTS: TDIF6 Position        */
#define PDMA_TDSTS_TDIF6_Msk           (0x1ul << PDMA_TDSTS_TDIF6_Pos)                   /*!< PDMA_T::TDSTS: TDIF6 Mask            */

#define PDMA_TDSTS_TDIF7_Pos           (7)                                               /*!< PDMA_T::TDSTS: TDIF7 Position        */
#define PDMA_TDSTS_TDIF7_Msk           (0x1ul << PDMA_TDSTS_TDIF7_Pos)                   /*!< PDMA_T::TDSTS: TDIF7 Mask            */

#define PDMA_TDSTS_TDIF8_Pos           (8)                                               /*!< PDMA_T::TDSTS: TDIF8 Position        */
#define PDMA_TDSTS_TDIF8_Msk           (0x1ul << PDMA_TDSTS_TDIF8_Pos)                   /*!< PDMA_T::TDSTS: TDIF8 Mask            */

#define PDMA_TDSTS_TDIF9_Pos           (9)                                               /*!< PDMA_T::TDSTS: TDIF9 Position        */
#define PDMA_TDSTS_TDIF9_Msk           (0x1ul << PDMA_TDSTS_TDIF9_Pos)                   /*!< PDMA_T::TDSTS: TDIF9 Mask            */

#define PDMA_TDSTS_TDIF10_Pos           (10)                                               /*!< PDMA_T::TDSTS: TDIF10 Position        */
#define PDMA_TDSTS_TDIF10_Msk           (0x1ul << PDMA_TDSTS_TDIF10_Pos)                   /*!< PDMA_T::TDSTS: TDIF10 Mask            */

#define PDMA_TDSTS_TDIF11_Pos           (11)                                               /*!< PDMA_T::TDSTS: TDIF11 Position        */
#define PDMA_TDSTS_TDIF11_Msk           (0x1ul << PDMA_TDSTS_TDIF11_Pos)                   /*!< PDMA_T::TDSTS: TDIF11 Mask            */

#define PDMA_TDSTS_TDIF12_Pos           (12)                                               /*!< PDMA_T::TDSTS: TDIF12 Position        */
#define PDMA_TDSTS_TDIF12_Msk           (0x1ul << PDMA_TDSTS_TDIF12_Pos)                   /*!< PDMA_T::TDSTS: TDIF12 Mask            */

#define PDMA_TDSTS_TDIF13_Pos           (13)                                               /*!< PDMA_T::TDSTS: TDIF13 Position        */
#define PDMA_TDSTS_TDIF13_Msk           (0x1ul << PDMA_TDSTS_TDIF13_Pos)                   /*!< PDMA_T::TDSTS: TDIF13 Mask            */

#define PDMA_TDSTS_TDIF14_Pos           (14)                                               /*!< PDMA_T::TDSTS: TDIF14 Position        */
#define PDMA_TDSTS_TDIF14_Msk           (0x1ul << PDMA_TDSTS_TDIF14_Pos)                   /*!< PDMA_T::TDSTS: TDIF14 Mask            */

#define PDMA_TDSTS_TDIF15_Pos           (15)                                               /*!< PDMA_T::TDSTS: TDIF15 Position        */
#define PDMA_TDSTS_TDIF15_Msk           (0x1ul << PDMA_TDSTS_TDIF15_Pos)                   /*!< PDMA_T::TDSTS: TDIF15 Mask            */

#define PDMA_ALIGN_ALIGNn_Pos           (0)                                                /*!< PDMA_T::ALIGN: ALIGNn Position        */
#define PDMA_ALIGN_ALIGNn_Msk           (0xfffful << PDMA_ALIGN_ALIGNn_Pos)                /*!< PDMA_T::ALIGN: ALIGNn Mask            */

#define PDMA_TACTSTS_TXACTFn_Pos         (0)                                               /*!< PDMA_T::TACTSTS: TXACTFn Position      */
#define PDMA_TACTSTS_TXACTFn_Msk         (0xfffful << PDMA_TACTSTS_TXACTFn_Pos)            /*!< PDMA_T::TACTSTS: TXACTFn Mask          */

#define PDMA_TOUTPSC_TOUTPSC0_Pos        (0)                                               /*!< PDMA_T::TOUTPSC: TOUTPSC0 Position     */
#define PDMA_TOUTPSC_TOUTPSC0_Msk        (0x7ul << PDMA_TOUTPSC_TOUTPSC0_Pos)              /*!< PDMA_T::TOUTPSC: TOUTPSC0 Mask         */

#define PDMA_TOUTPSC_TOUTPSC1_Pos        (4)                                               /*!< PDMA_T::TOUTPSC: TOUTPSC1 Position     */
#define PDMA_TOUTPSC_TOUTPSC1_Msk        (0x7ul << PDMA_TOUTPSC_TOUTPSC1_Pos)              /*!< PDMA_T::TOUTPSC: TOUTPSC1 Mask         */

#define PDMA_TOUTEN_TOUTENn_Pos          (0)                                               /*!< PDMA_T::TOUTEN: TOUTENn Position       */
#define PDMA_TOUTEN_TOUTENn_Msk          (0x3ul << PDMA_TOUTEN_TOUTENn_Pos)                /*!< PDMA_T::TOUTEN: TOUTENn Mask           */

#define PDMA_TOUTIEN_TOUTIENn_Pos        (0)                                               /*!< PDMA_T::TOUTIEN: TOUTIENn Position     */
#define PDMA_TOUTIEN_TOUTIENn_Msk        (0x3ul << PDMA_TOUTIEN_TOUTIENn_Pos)              /*!< PDMA_T::TOUTIEN: TOUTIENn Mask         */

#define PDMA_SCATBA_SCATBA_Pos           (16)                                              /*!< PDMA_T::SCATBA: SCATBA Position        */
#define PDMA_SCATBA_SCATBA_Msk           (0xfffful << PDMA_SCATBA_SCATBA_Pos)              /*!< PDMA_T::SCATBA: SCATBA Mask            */

#define PDMA_TOC0_1_TOC0_Pos             (0)                                               /*!< PDMA_T::TOC0_1: TOC0 Position          */
#define PDMA_TOC0_1_TOC0_Msk             (0xfffful << PDMA_TOC0_1_TOC0_Pos)                /*!< PDMA_T::TOC0_1: TOC0 Mask              */

#define PDMA_TOC0_1_TOC1_Pos             (16)                                              /*!< PDMA_T::TOC0_1: TOC1 Position          */
#define PDMA_TOC0_1_TOC1_Msk             (0xfffful << PDMA_TOC0_1_TOC1_Pos)                /*!< PDMA_T::TOC0_1: TOC1 Mask              */

#define PDMA_CHRST_CHnRST_Pos            (0)                                               /*!< PDMA_T::CHRST: CHnRST Position         */
#define PDMA_CHRST_CHnRST_Msk            (0xfffful << PDMA_CHRST_CHnRST_Pos)               /*!< PDMA_T::CHRST: CHnRST Mask             */

#define PDMA_REQSEL0_3_REQSRC0_Pos       (0)                                               /*!< PDMA_T::REQSEL0_3: REQSRC0 Position    */
#define PDMA_REQSEL0_3_REQSRC0_Msk       (0x7ful << PDMA_REQSEL0_3_REQSRC0_Pos)            /*!< PDMA_T::REQSEL0_3: REQSRC0 Mask        */

#define PDMA_REQSEL0_3_REQSRC1_Pos       (8)                                               /*!< PDMA_T::REQSEL0_3: REQSRC1 Position    */
#define PDMA_REQSEL0_3_REQSRC1_Msk       (0x7ful << PDMA_REQSEL0_3_REQSRC1_Pos)            /*!< PDMA_T::REQSEL0_3: REQSRC1 Mask        */

#define PDMA_REQSEL0_3_REQSRC2_Pos       (16)                                              /*!< PDMA_T::REQSEL0_3: REQSRC2 Position    */
#define PDMA_REQSEL0_3_REQSRC2_Msk       (0x7ful << PDMA_REQSEL0_3_REQSRC2_Pos)            /*!< PDMA_T::REQSEL0_3: REQSRC2 Mask        */

#define PDMA_REQSEL0_3_REQSRC3_Pos       (24)                                              /*!< PDMA_T::REQSEL0_3: REQSRC3 Position    */
#define PDMA_REQSEL0_3_REQSRC3_Msk       (0x7ful << PDMA_REQSEL0_3_REQSRC3_Pos)            /*!< PDMA_T::REQSEL0_3: REQSRC3 Mask        */

#define PDMA_REQSEL4_7_REQSRC4_Pos       (0)                                               /*!< PDMA_T::REQSEL4_7: REQSRC4 Position    */
#define PDMA_REQSEL4_7_REQSRC4_Msk       (0x7ful << PDMA_REQSEL4_7_REQSRC4_Pos)            /*!< PDMA_T::REQSEL4_7: REQSRC4 Mask        */

#define PDMA_REQSEL4_7_REQSRC5_Pos       (8)                                               /*!< PDMA_T::REQSEL4_7: REQSRC5 Position    */
#define PDMA_REQSEL4_7_REQSRC5_Msk       (0x7ful << PDMA_REQSEL4_7_REQSRC5_Pos)            /*!< PDMA_T::REQSEL4_7: REQSRC5 Mask        */

#define PDMA_REQSEL4_7_REQSRC6_Pos       (16)                                              /*!< PDMA_T::REQSEL4_7: REQSRC6 Position    */
#define PDMA_REQSEL4_7_REQSRC6_Msk       (0x7ful << PDMA_REQSEL4_7_REQSRC6_Pos)            /*!< PDMA_T::REQSEL4_7: REQSRC6 Mask        */

#define PDMA_REQSEL4_7_REQSRC7_Pos       (24)                                              /*!< PDMA_T::REQSEL4_7: REQSRC7 Position    */
#define PDMA_REQSEL4_7_REQSRC7_Msk       (0x7ful << PDMA_REQSEL4_7_REQSRC7_Pos)            /*!< PDMA_T::REQSEL4_7: REQSRC7 Mask        */

#define PDMA_REQSEL8_11_REQSRC8_Pos      (0)                                               /*!< PDMA_T::REQSEL8_11: REQSRC8 Position   */
#define PDMA_REQSEL8_11_REQSRC8_Msk      (0x7ful << PDMA_REQSEL8_11_REQSRC8_Pos)           /*!< PDMA_T::REQSEL8_11: REQSRC8 Mask       */

#define PDMA_REQSEL8_11_REQSRC9_Pos      (8)                                               /*!< PDMA_T::REQSEL8_11: REQSRC9 Position   */
#define PDMA_REQSEL8_11_REQSRC9_Msk      (0x7ful << PDMA_REQSEL8_11_REQSRC9_Pos)           /*!< PDMA_T::REQSEL8_11: REQSRC9 Mask       */

#define PDMA_REQSEL8_11_REQSRC10_Pos     (16)                                              /*!< PDMA_T::REQSEL8_11: REQSRC10 Position  */
#define PDMA_REQSEL8_11_REQSRC10_Msk     (0x7ful << PDMA_REQSEL8_11_REQSRC10_Pos)          /*!< PDMA_T::REQSEL8_11: REQSRC10 Mask      */

#define PDMA_REQSEL8_11_REQSRC11_Pos     (24)                                              /*!< PDMA_T::REQSEL8_11: REQSRC11 Position  */
#define PDMA_REQSEL8_11_REQSRC11_Msk     (0x7ful << PDMA_REQSEL8_11_REQSRC11_Pos)          /*!< PDMA_T::REQSEL8_11: REQSRC11 Mask      */

#define PDMA_REQSEL12_15_REQSRC12_Pos    (0)                                               /*!< PDMA_T::REQSEL12_15: REQSRC12 Position */
#define PDMA_REQSEL12_15_REQSRC12_Msk    (0x7ful << PDMA_REQSEL12_15_REQSRC12_Pos)         /*!< PDMA_T::REQSEL12_15: REQSRC12 Mask     */

#define PDMA_REQSEL12_15_REQSRC13_Pos    (8)                                               /*!< PDMA_T::REQSEL12_15: REQSRC13 Position */
#define PDMA_REQSEL12_15_REQSRC13_Msk    (0x7ful << PDMA_REQSEL12_15_REQSRC13_Pos)         /*!< PDMA_T::REQSEL12_15: REQSRC13 Mask     */

#define PDMA_REQSEL12_15_REQSRC14_Pos    (16)                                              /*!< PDMA_T::REQSEL12_15: REQSRC14 Position */
#define PDMA_REQSEL12_15_REQSRC14_Msk    (0x7ful << PDMA_REQSEL12_15_REQSRC14_Pos)         /*!< PDMA_T::REQSEL12_15: REQSRC14 Mask     */

#define PDMA_REQSEL12_15_REQSRC15_Pos    (24)                                              /*!< PDMA_T::REQSEL12_15: REQSRC15 Position */
#define PDMA_REQSEL12_15_REQSRC15_Msk    (0x7ful << PDMA_REQSEL12_15_REQSRC15_Pos)         /*!< PDMA_T::REQSEL12_15: REQSRC15 Mask     */

#define PDMA_STCRn_STC_Pos               (0)                                               /*!< PDMA_T::STCRn: STC Position            */
#define PDMA_STCRn_STC_Msk               (0xfffful << PDMA_STCRn_STC_Pos)                  /*!< PDMA_T::STCRn: STC Mask                */

#define PDMA_ASOCRn_SASOL_Pos            (0)                                               /*!< PDMA_T::ASOCRn: SASOL Position         */
#define PDMA_ASOCRn_SASOL_Msk            (0xfffful << PDMA_ASOCRn_SASOL_Pos)               /*!< PDMA_T::ASOCRn: SASOL Mask             */

#define PDMA_ASOCRn_DASOL_Pos            (16)                                              /*!< PDMA_T::ASOCRn: DASOL Position         */
#define PDMA_ASOCRn_DASOL_Msk            (0xfffful << PDMA_ASOCRn_DASOL_Pos)               /*!< PDMA_T::ASOCRn: DASOL Mask             */

/*---------------------------------------------------------------------------------------------------------*/
/*  Operation Mode Constant Definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PDMA_OP_STOP        0x00000000UL            /*!<DMA Stop Mode  \hideinitializer */
#define PDMA_OP_BASIC       0x00000001UL            /*!<DMA Basic Mode  \hideinitializer */
#define PDMA_OP_SCATTER     0x00000002UL            /*!<DMA Scatter-gather Mode  \hideinitializer */

//-----------------------------------------------------------------------------------
static void __iomem 		*pdma_reg_base;
#define PDMA0               ((PDMA_T *)  pdma_reg_base)
#define PDMA1               ((PDMA_T *)  (pdma_reg_base + 0x1000))

static int pdma0_irq = -1;
static int pdma1_irq = -1;

//-----------------------------------------------------------------------------------

#include "dmaengine.h"

#if 0
#define ENTRY()                 printk("[%-20s] : Enter...\n", __FUNCTION__)
#define LEAVE()                 printk("[%-20s] : Leave...\n", __FUNCTION__)
#else
#define ENTRY() {}
#define LEAVE()	{}
#endif

#if 0
#define DMA_DEBUG printk
#else
#define DMA_DEBUG(fmt,args...)
#endif


#define DMA_MAX_CHAN_DESCRIPTORS    32
#define DMA_MAX_CHAN_BYTES      0x10000

#define DMA_CHANNEL(_name, _base, _irq) \
        { .name = (_name), .base = (_base), .irq = (_irq) }

struct nuc980_dma_chan_data nuc980_dma_channels[] = {
	DMA_CHANNEL("ch0",  NULL, -1),
	DMA_CHANNEL("ch1",  NULL, -1),
	DMA_CHANNEL("ch2",  NULL, -1),
	DMA_CHANNEL("ch3",  NULL, -1),
	DMA_CHANNEL("ch4",  NULL, -1),
	DMA_CHANNEL("ch5",  NULL, -1),
	DMA_CHANNEL("ch6",  NULL, -1),
	DMA_CHANNEL("ch7",  NULL, -1),
	DMA_CHANNEL("ch8",  NULL, -1),
	DMA_CHANNEL("ch9",  NULL, -1),
	DMA_CHANNEL("ch10", NULL, -1),
	DMA_CHANNEL("ch11", NULL, -1),
	DMA_CHANNEL("ch12", NULL, -1),
	DMA_CHANNEL("ch13", NULL, -1),
	DMA_CHANNEL("ch14", NULL, -1),
	DMA_CHANNEL("ch15", NULL, -1),
	DMA_CHANNEL("ch16", NULL, -1),
	DMA_CHANNEL("ch17", NULL, -1),
	DMA_CHANNEL("ch18", NULL, -1),
	DMA_CHANNEL("ch19", NULL, -1),
};
struct nuc980_dma_platform_data nuc980_dma_data = {
	.channels               = nuc980_dma_channels,
	.num_channels           = ARRAY_SIZE(nuc980_dma_channels),
};

struct nuc980_dma_engine;

/**
 * struct nuc980_dma_desc - NUC980 specific transaction descriptor
 * @src_addr: source address of the transaction
 * @dst_addr: destination address of the transaction
 * @size: size of the transaction (in bytes)
 * @complete: this descriptor is completed
 * @txd: dmaengine API descriptor
 * @tx_list: list of linked descriptors
 * @node: link used for putting this into a channel queue
 */
struct nuc980_dma_desc {
	u32             src_addr;
	u32             dst_addr;
	size_t              size;
	u32                     ctl;
	bool                    complete;
	struct dma_async_tx_descriptor  txd;
	struct list_head        tx_list;
	struct list_head        node;
	struct nuc980_dma_config config;
	DSCT_T dsct[2];
	u32 			dir;
};

/**
 * struct nuc980_dma_chan - an NUC980 DMA M2M channel
 * @chan: dmaengine API channel
 * @edma: pointer to to the engine device
 * @regs: memory mapped registers
 * @irq: interrupt number of the channel
 * @clk: clock used by this channel
 * @tasklet: channel specific tasklet used for callbacks
 * @lock: lock protecting the fields following
 * @flags: flags for the channel
 * @buffer: which buffer to use next (0/1)
 * @active: flattened chain of descriptors currently being processed
 * @queue: pending descriptors which are handled next
 * @free_list: list of free descriptors which can be used
 * @runtime_addr: physical address currently used as dest/src (M2M only). This
 *                is set via %DMA_SLAVE_CONFIG before slave operation is
 *                prepared
 * @runtime_ctrl: M2M runtime values for the control register.
 *
 * As NUC980 DMA controller doesn't support real chained DMA descriptors we
 * will have slightly different scheme here: @active points to a head of
 * flattened DMA descriptor chain.
 *
 * @queue holds pending transactions. These are linked through the first
 * descriptor in the chain. When a descriptor is moved to the @active queue,
 * the first and chained descriptors are flattened into a single list.
 *
 * @chan.private holds pointer to &struct nuc980_dma_data which contains
 * necessary channel configuration information. For memcpy channels this must
 * be %NULL.
 */
struct nuc980_dma_chan {
	struct dma_chan         chan;
	const struct nuc980_dma_engine  *edma;
	void __iomem            *regs;
	int             irq;
	u32             id;
	struct tasklet_struct       tasklet;
	struct tasklet_struct       tasklet_sc;
	int sc_flag;
	/* protects the fields following */
	spinlock_t          lock;
	spinlock_t      wklock;
	unsigned long           flags;
	/* Channel is configured for cyclic transfers */
#define NUC980_DMA_IS_CYCLIC        0

	int             buffer;
	struct list_head        active;
	struct list_head        queue;
	struct list_head        free_list;
	u32             runtime_addr;
	u32             runtime_ctrl;
};

DEFINE_SPINLOCK(pdma0_lock); /* shared between the threads */
DEFINE_SPINLOCK(pdma1_lock); /* shared between the threads */

/**
 * struct nuc980_dma_engine - the NUC980 DMA engine instance
 * @dma_dev: holds the dmaengine device
 * @hw_setup: method which sets the channel up for operation
 * @hw_shutdown: shuts the channel down and flushes whatever is left
 * @hw_submit: pushes active descriptor(s) to the hardware
 * @hw_interrupt: handle the interrupt
 * @num_channels: number of channels for this instance
 * @channels: array of channels
 *
 * There is one instance of this struct for the M2M channels.
 * hw_xxx() methods are used to perform operations which are
 * different on M2M and M2P channels. These methods are called with channel
 * lock held and interrupts disabled so they cannot sleep.
 */
struct nuc980_dma_engine {
	struct dma_device   dma_dev;
	bool          m2m;
	int         (*hw_setup)(struct nuc980_dma_chan *);
	void        (*hw_shutdown)(struct nuc980_dma_chan *);
	void        (*hw_submit)(struct nuc980_dma_chan *);
	int         (*hw_interrupt)(struct nuc980_dma_chan *);
#define INTERRUPT_UNKNOWN   0
#define INTERRUPT_DONE      1
#define INTERRUPT_NEXT_BUFFER   2
#define INTERRUPT_TIMEOUT         3

	size_t          num_channels;
	struct nuc980_dma_chan  channels[];
};

static inline struct device *chan2dev(struct nuc980_dma_chan *edmac) {
	return &edmac->chan.dev->device;
}

static struct nuc980_dma_chan *to_nuc980_dma_chan(struct dma_chan *chan) {
	return container_of(chan, struct nuc980_dma_chan, chan);
}


static void nuc980_set_transfer_mode(PDMA_T * pdma,uint32_t u32Ch,uint32_t u32Peripheral)
{
	switch(u32Ch) {
		case 0ul:
			pdma->REQSEL0_3 = (pdma->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC0_Msk) | u32Peripheral;
			break;
		case 1ul:
			pdma->REQSEL0_3 = (pdma->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC1_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC1_Pos);
			break;
		case 2ul:
			pdma->REQSEL0_3 = (pdma->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC2_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC2_Pos);
			break;
		case 3ul:
			pdma->REQSEL0_3 = (pdma->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC3_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC3_Pos);
			break;
		case 4ul:
			pdma->REQSEL4_7 = (pdma->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC4_Msk) | u32Peripheral;
			break;
		case 5ul:
			pdma->REQSEL4_7 = (pdma->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC5_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC5_Pos);
			break;
		case 6ul:
			pdma->REQSEL4_7 = (pdma->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC6_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC6_Pos);
			break;
		case 7ul:
			pdma->REQSEL4_7 = (pdma->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC7_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC7_Pos);
			break;
		case 8ul:
			pdma->REQSEL8_11 = (pdma->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC8_Msk) | u32Peripheral;
			break;
		case 9ul:
			pdma->REQSEL8_11 = (pdma->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC9_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC9_Pos);
			break;
		case 10ul:
			pdma->REQSEL8_11 = (pdma->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC10_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC10_Pos);
			break;
		case 11ul:
			pdma->REQSEL8_11 = (pdma->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC11_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC11_Pos);
			break;
		case 12ul:
			pdma->REQSEL12_15 = (pdma->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC12_Msk) | u32Peripheral;
			break;
		case 13ul:
			pdma->REQSEL12_15 = (pdma->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC13_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC13_Pos);
			break;
		case 14ul:
			pdma->REQSEL12_15 = (pdma->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC14_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC14_Pos);
			break;
		case 15ul:
			pdma->REQSEL12_15 = (pdma->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC15_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC15_Pos);
			break;
		default:
			break;
	}
}

/**
 * nuc980_dma_set_active - set new active descriptor chain
 * @edmac: channel
 * @desc: head of the new active descriptor chain
 *
 * Sets @desc to be the head of the new active descriptor chain. This is the
 * chain which is processed next. The active list must be empty before calling
 * this function.
 *
 * Called with @edmac->lock held and interrupts disabled.
 */
static void nuc980_dma_set_active(struct nuc980_dma_chan *edmac,
                                  struct nuc980_dma_desc *desc)
{
	ENTRY();
	BUG_ON(!list_empty(&edmac->active));

	list_add_tail(&desc->node, &edmac->active);

	/* Flatten the @desc->tx_list chain into @edmac->active list */
	while (!list_empty(&desc->tx_list)) {
		struct nuc980_dma_desc *d = list_first_entry(&desc->tx_list,
		                            struct nuc980_dma_desc, node);

		/*
		 * We copy the callback parameters from the first descriptor
		 * to all the chained descriptors. This way we can call the
		 * callback without having to find out the first descriptor in
		 * the chain. Useful for cyclic transfers.
		 */
		d->txd.callback = desc->txd.callback;
		d->txd.callback_param = desc->txd.callback_param;

		list_move_tail(&d->node, &edmac->active);
	}
	LEAVE();
}

/* Called with @edmac->lock held and interrupts disabled */
static struct nuc980_dma_desc *
nuc980_dma_get_active(struct nuc980_dma_chan *edmac) {
	DMA_DEBUG("NUC980 GDMA %s\n", __FUNCTION__ );
	if (list_empty(&edmac->active))
		return NULL;
	return list_first_entry(&edmac->active, struct nuc980_dma_desc, node);
}

/**
 * nuc980_dma_advance_active - advances to the next active descriptor
 * @edmac: channel
 *
 * Function advances active descriptor to the next in the @edmac->active and
 * returns %true if we still have descriptors in the chain to process.
 * Otherwise returns %false.
 *
 * When the channel is in cyclic mode always returns %true.
 *
 * Called with @edmac->lock held and interrupts disabled.
 */
static bool nuc980_dma_advance_active(struct nuc980_dma_chan *edmac)
{
	struct nuc980_dma_desc *desc;
	DMA_DEBUG("NUC980 GDMA %s\n", __FUNCTION__ );
	list_rotate_left(&edmac->active);

	if (test_bit(NUC980_DMA_IS_CYCLIC, &edmac->flags))
		return true;

	desc = nuc980_dma_get_active(edmac);
	if (!desc)
		return false;

	/*
	 * If txd.cookie is set it means that we are back in the first
	 * descriptor in the chain and hence done with it.
	 */
	return !desc->txd.cookie;
}

/*
 000 = PDMA channel 1 time-out clock source is HCLK/(2^8).
 001 = PDMA channel 1 time-out clock source is HCLK/(2^9).
 010 = PDMA channel 1 time-out clock source is HCLK/(2^10).
 011 = PDMA channel 1 time-out clock source is HCLK/(2^11).
 100 = PDMA channel 1 time-out clock source is HCLK/(2^12).
 101 = PDMA channel 1 time-out clock source is HCLK/(2^13).
 */
static void nuc980_dma_SetTimeOut(struct nuc980_dma_chan *edmac,u32 prescaler,u32 counter)
{

	struct nuc980_dma_desc *desc;
	int ch;
	PDMA_T * pdma;
	ENTRY();
	desc = nuc980_dma_get_active(edmac);
	if (!desc) {
		dev_warn(chan2dev(edmac), "PDMA: empty descriptor list\n");
		return;
	}

	if(edmac->irq == pdma0_irq) {
		pdma=PDMA0;
	} else {
		pdma=PDMA1;
	}
	ch =edmac->id ;

	if(prescaler ==0 && counter ==0) {
		pdma->TOUTIEN &=~(1 << ch);
		pdma->TOUTEN &=~(1 << ch);  /* Enable time-out funciton */

		return;
	}

	if(ch<=7) {
		pdma->TOUTPSC &= ~(0x7 << (PDMA_TOUTPSC_TOUTPSC1_Pos * ch));
		pdma->TOUTPSC |= ((prescaler&0x7) << (PDMA_TOUTPSC_TOUTPSC1_Pos * ch));
	} else {
		pdma->TOUTPSC2 &= ~(0x7 << (PDMA_TOUTPSC_TOUTPSC1_Pos * (ch-8)));
		pdma->TOUTPSC2 |= ((prescaler&0x7) << (PDMA_TOUTPSC_TOUTPSC1_Pos * (ch-8)));
	}

	if(ch==0 || ch==1) {
		pdma->TOC0_1 &= ~( (0xffff) << (PDMA_TOC0_1_TOC1_Pos * ch));
		pdma->TOC0_1 |= ((counter&0xffff) << (PDMA_TOC0_1_TOC1_Pos * ch));
	} else if(ch==2 || ch==3) {
		pdma->TOC2_3 &= ~( (0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-2)));
		pdma->TOC2_3 |= ((counter&0xffff)<< (PDMA_TOC0_1_TOC1_Pos * (ch-2)));
	} else if(ch==4 || ch==5) {
		pdma->TOC4_5 &= ~( (0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-4)));
		pdma->TOC4_5 |= ((counter&0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-4)));
	} else if(ch==6 || ch==7) {
		pdma->TOC6_7 &= ~( (0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-6)));
		pdma->TOC6_7 |= ((counter&0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-6)));
	} else if(ch==8 || ch==9) {
		pdma->TOC8_9 &= ~( (0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-8)));
		pdma->TOC8_9 |= ((counter&0xffff) << (PDMA_TOC0_1_TOC1_Pos * (ch-8)));
	}


	pdma->TOUTEN |= (1 << ch);  /* Enable time-out funciton */
	pdma->TOUTIEN |= (1 << ch); /* Enable time-out interrupt */
	LEAVE();
}


/*
 * DMA implementation
 */

static int hw_setup(struct nuc980_dma_chan *edmac)
{
	return 0;
}

static void hw_shutdown(struct nuc980_dma_chan *edmac)
{
	ENTRY();
	/* Just disable the channel */

	if(edmac->irq == pdma0_irq) {
		PDMA0->CHCTL &= ~(1<<edmac->id);
	} else {
		PDMA1->CHCTL &= ~(1<<edmac->id);
	}
	LEAVE();
}

static void fill_desc(struct nuc980_dma_chan *edmac)
{
	struct nuc980_dma_desc *desc;
	u32 regT;
	PDMA_T * pdma;
	//u32 tcnt,config;

	ENTRY();
	desc = nuc980_dma_get_active(edmac);
	if (!desc) {
		dev_warn(chan2dev(edmac), "PDMA: empty descriptor list\n");
		return;
	}

	DMA_DEBUG("edmac->runtime_ctrl=0x%08x\n",edmac->runtime_ctrl);
	DMA_DEBUG("desc->ctl=0x%08x\n",desc->ctl);
	if(edmac->irq==pdma0_irq) {
		DMA_DEBUG("PDMA0[%d] CTL=0x%08x\n",edmac->id,PDMA0->DSCT[edmac->id].CTL);
		if((PDMA0->DSCT[edmac->id].CTL & 0x3)!=0) {
			regT = PDMA0->CHCTL;
			PDMA0->DSCT[edmac->id].CTL=0;
			PDMA0->CHRST = (1<<edmac->id);
			PDMA0->CHCTL = (regT | (1<<edmac->id));
		} else {
			PDMA0->DSCT[edmac->id].CTL=0;
			PDMA0->CHCTL |= (1<<edmac->id);
		}
		PDMA0->INTEN |= (1<<edmac->id);
		nuc980_set_transfer_mode(PDMA0, edmac->id, desc->config.reqsel);
		pdma = PDMA0;
	} else {
		DMA_DEBUG("PDMA1[%d] CTL=0x%08x\n",edmac->id,PDMA1->DSCT[edmac->id].CTL);
		if((PDMA1->DSCT[edmac->id].CTL & 0x3)!=0) {
			regT=PDMA1->CHCTL;
			PDMA1->DSCT[edmac->id].CTL=0;
			PDMA1->CHRST = (1<<edmac->id);
			PDMA1->CHCTL = (regT | (1<<edmac->id));
		} else {
			PDMA1->DSCT[edmac->id].CTL=0;
			PDMA1->CHCTL |= (1<<edmac->id);
		}
		PDMA1->INTEN |= (1<<edmac->id);
		nuc980_set_transfer_mode(PDMA1,edmac->id,desc->config.reqsel);
		pdma = PDMA1;
	}
	pdma->DSCT[edmac->id].CTL |= (edmac->runtime_ctrl | ((desc->size - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos));
	pdma->DSCT[edmac->id].SA   = desc->src_addr;
	pdma->DSCT[edmac->id].DA   = desc->dst_addr;

	DMA_DEBUG("===============pdma=============\n");
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].CTL=0x%08x\n",&pdma->DSCT[edmac->id].CTL,edmac->id,pdma->DSCT[edmac->id].CTL);
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].SA=0x%08x\n",&pdma->DSCT[edmac->id].SA,edmac->id,pdma->DSCT[edmac->id].SA);
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].DA=0x%08x\n",&pdma->DSCT[edmac->id].DA,edmac->id,pdma->DSCT[edmac->id].DA);
	DMA_DEBUG("(0x%08x)pdma->CHCTL=0x%08x\n",&pdma->CHCTL,pdma->CHCTL);
	DMA_DEBUG("(0x%08x)pdma->INTEN=0x%08x\n",&pdma->INTEN,pdma->INTEN);
	DMA_DEBUG("(0x%08x)pdma->INTSTS=0x%08x\n",&pdma->INTSTS,pdma->INTSTS);
	DMA_DEBUG("(0x%08x)pdma->TDSTS=0x%08x\n",&pdma->TDSTS,pdma->TDSTS);
	DMA_DEBUG("(0x%08x)pdma->REQSEL0_3=0x%08x\n",&pdma->REQSEL0_3,pdma->REQSEL0_3);
	DMA_DEBUG("===============================\n");
	LEAVE();
}

static void fill_desc_sc(struct nuc980_dma_chan *edmac)
{
	struct nuc980_dma_desc *desc=NULL;
	//DSCT_T *dsct=NULL;
	u32 regT;
	ENTRY();
	desc = nuc980_dma_get_active(edmac);

	if(edmac->irq==pdma0_irq) {
		DMA_DEBUG("SC PDMA0[%d] CTL=0x%08x\n",edmac->id,PDMA0->DSCT[edmac->id].CTL);
		if((PDMA0->DSCT[edmac->id].CTL & 0x3)!=0) {
			regT=PDMA0->CHCTL;
			PDMA0->DSCT[edmac->id].CTL=0;
			PDMA0->CHRST = (1<<edmac->id);
			PDMA0->CHCTL = (regT | (1<<edmac->id));
		} else {
			PDMA0->DSCT[edmac->id].CTL=0;
			PDMA0->CHCTL |= (1<<edmac->id);
		}
		nuc980_set_transfer_mode(PDMA0,edmac->id,desc->config.reqsel);
		PDMA0->INTEN |= (1<<edmac->id);
	} else {
		DMA_DEBUG("SC PDMA1[%d] CTL=0x%08x\n",edmac->id,PDMA1->DSCT[edmac->id].CTL);
		if((PDMA1->DSCT[edmac->id].CTL & 0x3)!=0) {
			regT=PDMA1->CHCTL;
			PDMA1->DSCT[edmac->id].CTL=0;
			PDMA1->CHRST = (1<<edmac->id);
			PDMA1->CHCTL = (regT | (1<<edmac->id));
		} else {
			PDMA1->DSCT[edmac->id].CTL=0;
			PDMA1->CHCTL |= (1<<edmac->id);
		}
		nuc980_set_transfer_mode(PDMA1,edmac->id,desc->config.reqsel);
		PDMA1->INTEN |= (1<<edmac->id);
	}

	if(desc->dir ==DMA_DEV_TO_MEM) {
		desc->dsct[0].CTL =   (edmac->runtime_ctrl | (((desc->size/2) - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos) | PDMA_OP_SCATTER);
		desc->dsct[0].SA =  desc->src_addr;
		desc->dsct[0].DA =  desc->dst_addr;
		desc->dsct[0].NEXT = virt_to_phys(&desc->dsct[1].CTL);

		desc->dsct[1].CTL =  (edmac->runtime_ctrl | (((desc->size/2) - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos) | PDMA_OP_SCATTER);
		desc->dsct[1].SA =  desc->src_addr;
		desc->dsct[1].DA =  desc->dst_addr+(desc->size/2);
		desc->dsct[1].NEXT = virt_to_phys(&desc->dsct[0].CTL);
	} else if(desc->dir ==DMA_MEM_TO_DEV) {
		desc->dsct[0].CTL =   (edmac->runtime_ctrl | (((desc->size/2) - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos) | PDMA_OP_SCATTER);
		desc->dsct[0].SA =  desc->src_addr;
		desc->dsct[0].DA =  desc->dst_addr;
		desc->dsct[0].NEXT = virt_to_phys(&desc->dsct[1].CTL);

		desc->dsct[1].CTL =  (edmac->runtime_ctrl | (((desc->size/2) - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos) | PDMA_OP_SCATTER);
		desc->dsct[1].SA =  desc->src_addr+(desc->size/2);
		desc->dsct[1].DA =  desc->dst_addr;
		desc->dsct[1].NEXT = virt_to_phys(&desc->dsct[0].CTL);
	}

	LEAVE();
}


static void hw_submit(struct nuc980_dma_chan *edmac)
{
	struct nuc980_dma_desc *desc;
	ENTRY();
	desc = nuc980_dma_get_active(edmac);

	if(edmac->irq==pdma0_irq)
		spin_lock(&pdma0_lock);
	else
		spin_lock(&pdma1_lock);

	/*
	 * Since we allow clients to configure PW (peripheral width) we always
	 * clear PW bits here and then set them according what is given in
	 * the runtime configuration.
	 */
	if(desc->config.en_sc==0) {
		fill_desc(edmac);
		nuc980_dma_SetTimeOut(edmac,desc->config.timeout_prescaler,desc->config.timeout_counter);
		if(edmac->irq == pdma0_irq) {
			PDMA0->DSCT[edmac->id].CTL = (PDMA0->DSCT[edmac->id].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_BASIC;
			if(desc->config.reqsel==0) {
				PDMA0->SWREQ = 1<<(edmac->id);
			}
		} else {
			PDMA1->DSCT[edmac->id].CTL = (PDMA1->DSCT[edmac->id].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_BASIC;
			if(desc->config.reqsel==0) {
				PDMA1->SWREQ = 1<<(edmac->id);
			}
		}
	} else {
		fill_desc_sc(edmac);
		nuc980_dma_SetTimeOut(edmac,desc->config.timeout_prescaler,desc->config.timeout_counter);
		if(edmac->irq==pdma0_irq) {
			PDMA0->DSCT[edmac->id].NEXT = virt_to_phys(&desc->dsct[0].CTL);
			PDMA0->DSCT[edmac->id].CTL = PDMA_OP_SCATTER;
			if(desc->config.reqsel==0) {
				PDMA0->SWREQ = 1<<(edmac->id);
			}
		} else {
			PDMA1->DSCT[edmac->id].NEXT =  virt_to_phys(&desc->dsct[0].CTL);
			PDMA1->DSCT[edmac->id].CTL = PDMA_OP_SCATTER;
			if(desc->config.reqsel==0) {
				PDMA1->SWREQ = 1<<(edmac->id);
			}
		}
	}

	PDMA_T * pdma;
	if(edmac->irq==pdma0_irq)
		pdma=PDMA0;
	else
		pdma=PDMA1;

	DMA_DEBUG("===============pdma=============\n");
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].CTL=0x%08x\n",&pdma->DSCT[edmac->id].CTL,edmac->id,pdma->DSCT[edmac->id].CTL);
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].SA=0x%08x\n",&pdma->DSCT[edmac->id].SA,edmac->id,pdma->DSCT[edmac->id].SA);
	DMA_DEBUG("(0x%08x)pdma->DSCT[%d].DA=0x%08x\n",&pdma->DSCT[edmac->id].DA,edmac->id,pdma->DSCT[edmac->id].DA);
	DMA_DEBUG("(0x%08x)pdma->CHCTL=0x%08x\n",&pdma->CHCTL,pdma->CHCTL);
	DMA_DEBUG("(0x%08x)pdma->INTEN=0x%08x\n",&pdma->INTEN,pdma->INTEN);
	DMA_DEBUG("(0x%08x)pdma->INTSTS=0x%08x\n",&pdma->INTSTS,pdma->INTSTS);
	DMA_DEBUG("(0x%08x)pdma->TDSTS=0x%08x\n",&pdma->TDSTS,pdma->TDSTS);
	DMA_DEBUG("(0x%08x)pdma->REQSEL0_3=0x%08x\n",&pdma->REQSEL0_3,pdma->REQSEL0_3);
	DMA_DEBUG("===============================\n");

	if(edmac->irq==pdma0_irq)
		spin_unlock(&pdma0_lock);
	else
		spin_unlock(&pdma1_lock);
	LEAVE();
}

/*
 * According to NUC980 User's Guide, we should receive DONE interrupt when all
 * M2M DMA controller transactions complete normally. This is not always the
 * case - sometimes NUC980 M2M DMA asserts DONE interrupt when the DMA channel
 * is still running (channel Buffer FSM in DMA_BUF_ON state, and channel
 * Control FSM in DMA_MEM_RD state, observed at least in IDE-DMA operation).
 * In effect, disabling the channel when only DONE bit is set could stop
 * currently running DMA transfer. To avoid this, we use Buffer FSM and
 * Control FSM to check current state of DMA channel.
 */
static int hw_interrupt(struct nuc980_dma_chan *edmac)
{
	bool last_done;
	struct nuc980_dma_desc *desc;
	ENTRY();
	if(edmac->irq==pdma0_irq) {
		DMA_DEBUG("PDMA0->TDSTS=0x%08x,edmac->id=%d\n",PDMA0->TDSTS,edmac->id);
		PDMA0->TDSTS = (1<<(edmac->id));
	} else {
		DMA_DEBUG("PDMA1->TDSTS=0x%08x,edmac->id=%d\n",PDMA1->TDSTS,edmac->id);
		PDMA1->TDSTS = (1<<(edmac->id));
	}

	/*
	 * Check whether we are done with descriptors or not. This, together
	 * with DMA channel state, determines action to take in interrupt.
	 */
	desc = nuc980_dma_get_active(edmac);
	last_done = !desc; //|| desc->txd.cookie;
	DMA_DEBUG("last_done=%d,desc=0x%08x,desc->txd.cookie=%d\n",last_done,(unsigned int)desc,desc->txd.cookie);
	if(!last_done) {
		if (nuc980_dma_advance_active(edmac)) {
			DMA_DEBUG("nuc980_dma_advance_active(edmac)!=NULL\n");
			fill_desc(edmac);
			if(edmac->irq==pdma0_irq) {
				PDMA0->DSCT[edmac->id].CTL = (PDMA0->DSCT[edmac->id].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_BASIC;
				if(desc->config.reqsel==0) {
					PDMA0->SWREQ = 1<<(edmac->id);
				}
			} else {
				PDMA1->DSCT[edmac->id].CTL = (PDMA1->DSCT[edmac->id].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_BASIC;
				if(desc->config.reqsel==0) {
					PDMA1->SWREQ = 1<<(edmac->id);
				}
			}
			return INTERRUPT_NEXT_BUFFER;
		} else {
			DMA_DEBUG("nuc980_dma_advance_active(edmac)=NULL\n");
			last_done = true;
		}
	}

	if(last_done) {
		return INTERRUPT_DONE;
	}
	LEAVE();
	return INTERRUPT_NEXT_BUFFER;
}

/*
 * DMA engine API implementation
 */

static struct nuc980_dma_desc *
nuc980_dma_desc_get(struct nuc980_dma_chan *edmac) {
	struct nuc980_dma_desc *desc, *_desc;
	struct nuc980_dma_desc *ret = NULL;
	ENTRY();
	spin_lock(&edmac->lock);
	list_for_each_entry_safe(desc, _desc, &edmac->free_list, node) {
		if (async_tx_test_ack(&desc->txd)) {
			list_del_init(&desc->node);

			/* Re-initialize the descriptor */
			desc->src_addr = 0;
			desc->dst_addr = 0;
			desc->size = 0;
			desc->complete = false;
			desc->txd.cookie = 0;
			desc->txd.callback = NULL;
			desc->txd.callback_param = NULL;

			ret = desc;
			break;
		}
	}
	spin_unlock(&edmac->lock);
	LEAVE();
	return ret;
}

static void nuc980_dma_desc_put(struct nuc980_dma_chan *edmac,
                                struct nuc980_dma_desc *desc)
{
	//ENTRY();
	if (desc) {
		spin_lock(&edmac->lock);
		list_splice_init(&desc->tx_list, &edmac->free_list);
		list_add(&desc->node, &edmac->free_list);
		spin_unlock(&edmac->lock);
	}
}

/**
 * nuc980_dma_advance_work - start processing the next pending transaction
 * @edmac: channel
 *
 * If we have pending transactions queued and we are currently idling, this
 * function takes the next queued transaction from the @edmac->queue and
 * pushes it to the hardware for execution.
 */
static void nuc980_dma_advance_work(struct nuc980_dma_chan *edmac)
{
	struct nuc980_dma_desc *new;
	ENTRY();
	spin_lock(&edmac->lock);
	if (!list_empty(&edmac->active) || list_empty(&edmac->queue)) {
		spin_unlock(&edmac->lock);
		DMA_DEBUG("nuc980_dma_advance_work  %d %d\n",list_empty(&edmac->active),list_empty(&edmac->queue));
		return;
	}

	/* Take the next descriptor from the pending queue */
	new = list_first_entry(&edmac->queue, struct nuc980_dma_desc, node);
	list_del_init(&new->node);

	nuc980_dma_set_active(edmac, new);
	DMA_DEBUG("hw_submit(edmac)\n");
	/* Push it to the hardware */
	edmac->edma->hw_submit(edmac);
	spin_unlock(&edmac->lock);
	LEAVE();
}

static void nuc980_dma_sc_tasklet(unsigned long data)
{
	struct nuc980_dma_chan *edmac = (struct nuc980_dma_chan *)data;
	struct nuc980_dma_desc *desc;
	struct nuc980_dma_done * done=NULL;
	dma_async_tx_callback callback = NULL;
	void *callback_param = NULL;
	ENTRY();
	//spin_lock_irq(&edmac->lock);
	desc = nuc980_dma_get_active(edmac);
	DMA_DEBUG("*desc=0x%08x\n",*desc);

	desc->complete = true;
	done =(struct nuc980_dma_done *)desc->txd.callback_param;
	if(done!=NULL) {
		done->ch = edmac->id;
		if(desc->config.en_sc==1) {
			if(done->base_addr!=1)
				done->base_addr = 1;
			else
				done->base_addr = 2;
		}
	}


	if (desc) {
		callback = desc->txd.callback;
		callback_param = desc->txd.callback_param;
	}
	//spin_unlock_irq(&edmac->lock);
	if (callback) {
		callback(callback_param);
		edmac->sc_flag = 0;
	}
	LEAVE();


}

static void nuc980_dma_tasklet(unsigned long data)
{
	struct nuc980_dma_chan *edmac = (struct nuc980_dma_chan *)data;
	struct nuc980_dma_desc *desc, *d;
	struct nuc980_dma_done * done=NULL;
	dma_async_tx_callback callback = NULL;
	void *callback_param = NULL;
	LIST_HEAD(list);
	ENTRY();
	//spin_lock_irq(&edmac->lock);
	/*
	 * If dma_terminate_all() was called before we get to run, the active
	 * list has become empty. If that happens we aren't supposed to do
	 * anything more than call nuc980_dma_advance_work().
	 */
	desc = nuc980_dma_get_active(edmac);
	desc->complete = true;
	done =(struct nuc980_dma_done *)desc->txd.callback_param;
	if(done!=NULL) {
		done->ch = edmac->id;
		if(desc->config.en_sc==1) {
			if(done->base_addr!=1)
				done->base_addr = 1;
			else
				done->base_addr = 2;
		}
	}

	DMA_DEBUG("*desc=0x%08x\n",*desc);
	if (desc) {
		DMA_DEBUG("desc->complete=%d\n",desc->complete);
		if (desc->complete) {
			/* mark descriptor complete for non cyclic case only */
			if (!test_bit(NUC980_DMA_IS_CYCLIC, &edmac->flags))
				dma_cookie_complete(&desc->txd);
			DMA_DEBUG("nuc980_dma_tasklet : ====>list_splice_init\n");
			list_splice_init(&edmac->active, &list);
		}
		callback = desc->txd.callback;
		callback_param = desc->txd.callback_param;
	}

	//spin_unlock_irq(&edmac->lock);

	/* Pick up the next descriptor from the queue */
	nuc980_dma_advance_work(edmac);

	/* Now we can release all the chained descriptors */
	list_for_each_entry_safe(desc, d, &list, node) {
		/*
		 * For the memcpy channels the API requires us to unmap the
		 * buffers unless requested otherwise.
		 */
		desc->txd.flags = DMA_CTRL_ACK;
		nuc980_dma_desc_put(edmac, desc);
	}
	if (callback) {
		callback(callback_param);
	}
	LEAVE();
}

static void nuc980_dma_emac_interrupt(struct nuc980_dma_chan *edmac,int status)
{
	struct nuc980_dma_desc *desc=NULL;
	struct nuc980_dma_done * done=NULL;
	ENTRY();
	desc = nuc980_dma_get_active(edmac);
	if (!desc) {
		dev_warn(chan2dev(edmac),
		         "got interrupt while active list is empty\n");
		LEAVE();
		return;
	}
	if(status==INTERRUPT_TIMEOUT) {
		done =(struct nuc980_dma_done *)desc->txd.callback_param;
		if(done!=NULL) {
			done->done = 0;
			done->timeout=1;
			if(edmac->irq==pdma0_irq)
				done->remain = (PDMA0->DSCT[edmac->id].CTL & PDMA_DSCT_CTL_TXCNT_Msk)>>PDMA_DSCT_CTL_TXCNT_Pos;
			else
				done->remain = (PDMA1->DSCT[edmac->id].CTL & PDMA_DSCT_CTL_TXCNT_Msk)>>PDMA_DSCT_CTL_TXCNT_Pos;
		}

		tasklet_schedule(&edmac->tasklet);
		return;
	}


	switch (edmac->edma->hw_interrupt(edmac)) {
		case INTERRUPT_DONE: 
		{
			DMA_DEBUG("INTERRUPT_DONE\n");
			done =(struct nuc980_dma_done *)desc->txd.callback_param;
			if(done!=NULL) {
				done->done = 1;
				done->timeout=0;
				done->remain = 0;
			}
			if(desc->config.en_sc==0) {
				tasklet_schedule(&edmac->tasklet);
			} else {
				if(edmac->sc_flag==0){
					tasklet_schedule(&edmac->tasklet_sc);
					edmac->sc_flag = 1;
				}
			}
		}
		break;

		case INTERRUPT_NEXT_BUFFER:
			DMA_DEBUG("INTERRUPT_NEXT_BUFFER\n");
			if (test_bit(NUC980_DMA_IS_CYCLIC, &edmac->flags))
				tasklet_schedule(&edmac->tasklet);
			break;

		default:
			dev_warn(chan2dev(edmac), "unknown interrupt!\n");
			break;
	}
	LEAVE();
}
static irqreturn_t nuc980_dma_interrupt(int irq, void *dev_id)
{
	int i;
	struct nuc980_dma_engine *edma= dev_id;
	unsigned int pdma0_int_status = PDMA0->INTSTS;
	unsigned int pdma1_int_status = PDMA1->INTSTS;
	unsigned int pdma0_status = PDMA0->TDSTS;
	unsigned int pdma1_status = PDMA1->TDSTS;

	irqreturn_t ret = IRQ_HANDLED;
	ENTRY();
	DMA_DEBUG("irqreturn_t\n");
	DMA_DEBUG("PDMA0->INTSTS=0x%08x,PDMA1->INTSTS=0x%08x\n",PDMA0->INTSTS,PDMA1->INTSTS);
	DMA_DEBUG("PDMA0->ABTSTS=0x%08x,PDMA1->ABTSTS=0x%08x\n",PDMA0->ABTSTS,PDMA1->ABTSTS);
	DMA_DEBUG("PDMA0->TDSTS=0x%08x,PDMA1->TDSTS=0x%08x\n",PDMA0->TDSTS,PDMA1->TDSTS);

	// Global Status Checks
	if (pdma0_int_status & PDMA_INTSTS_ABTIF_Msk) {
		// Find which channel has the abort flag
		int channel = -1;
		for(int i = 0;i < 10;i++) {
			if(PDMA0->ABTSTS & (1 << i)) {
				channel = i;
				break;
			}
		}
		nuc980_set_transfer_mode(PDMA0, channel, 0);
		PDMA0->ABTSTS |= (1 << channel);
		dev_warn(edma->dma_dev.dev, "Error! PDMA0 Target abort on channel %d\n", channel);
	}
	if (pdma1_int_status & PDMA_INTSTS_ABTIF_Msk) {
		// Find which channel has the abort flag
		int channel = -1;
		for(int i = 0;i < 10;i++) {
			if(PDMA1->ABTSTS & (1 << i)) {
				channel = i;
				break;
			}
		}
		nuc980_set_transfer_mode(PDMA1, channel, 0);
		PDMA1->ABTSTS |= (1 << channel);
		dev_warn(edma->dma_dev.dev, "Error! PDMA1 Target abort on channel %d\n", channel);
	}

	for(i=(edma->num_channels-1); i>=0; i--) {
		if((edma->channels[i].irq==pdma0_irq) && (irq==pdma0_irq)) {
			if(pdma0_status & (1<<(edma->channels[i].id))) {
				PDMA0->TDSTS = (1<<(edma->channels[i].id));
				nuc980_dma_emac_interrupt(&edma->channels[i],INTERRUPT_DONE);
			}

			if(pdma0_int_status & (1<<(edma->channels[i].id+8))) {
				DMA_DEBUG("PDMA0 INTERRUPT_TIMEOUT id=%d",edma->channels[i].id);
				nuc980_dma_emac_interrupt(&edma->channels[i],INTERRUPT_TIMEOUT);
				PDMA0->TOUTEN &= ~(1<<(edma->channels[i].id));
				//PDMA0->TOUTIEN  &= ~(1<<(edma->channels[i].id));
				PDMA0->INTSTS = (1<<(edma->channels[i].id+8));
			}

		} else if((edma->channels[i].irq==pdma1_irq) && (irq==pdma1_irq)) {
			if(pdma1_status & (1<<(edma->channels[i].id))) {
				PDMA1->TDSTS = (1<<(edma->channels[i].id));
				nuc980_dma_emac_interrupt(&edma->channels[i],INTERRUPT_DONE);
			}
			if(pdma1_int_status & (1<<(edma->channels[i].id+8))) {
				DMA_DEBUG("PDMA1 INTERRUPT_TIMEOUT id=%d",edma->channels[i].id);
				nuc980_dma_emac_interrupt(&edma->channels[i],INTERRUPT_TIMEOUT);
				PDMA1->TOUTEN &= ~(1<<(edma->channels[i].id));
				//PDMA1->TOUTIEN  &= ~(1<<(edma->channels[i].id));
				PDMA1->INTSTS = (1<<(edma->channels[i].id+8));
			}

		}
	}
	LEAVE();
	return ret;
}

/**
 * nuc980_dma_tx_submit - set the prepared descriptor(s) to be executed
 * @tx: descriptor to be executed
 *
 * Function will execute given descriptor on the hardware or if the hardware
 * is busy, queue the descriptor to be executed later on. Returns cookie which
 * can be used to poll the status of the descriptor.
 */
static dma_cookie_t nuc980_dma_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(tx->chan);
	struct nuc980_dma_desc *desc;
	dma_cookie_t cookie;
	ENTRY();
	//spin_lock_irqsave(&edmac->lock, flags);
	cookie = dma_cookie_assign(tx);

	desc = container_of(tx, struct nuc980_dma_desc, txd);

	/*
	 * If nothing is currently prosessed, we push this descriptor
	 * directly to the hardware. Otherwise we put the descriptor
	 * to the pending queue.
	 */
	if (list_empty(&edmac->active)) {
		nuc980_dma_set_active(edmac, desc);
		edmac->edma->hw_submit(edmac);
	} else {
		list_add_tail(&desc->node, &edmac->queue);
	}
	//spin_unlock_irqrestore(&edmac->lock, flags);

	LEAVE();
	return cookie;
}

/**
 * nuc980_dma_alloc_chan_resources - allocate resources for the channel
 * @chan: channel to allocate resources
 *
 * Function allocates necessary resources for the given DMA channel and
 * returns number of allocated descriptors for the channel. Negative errno
 * is returned in case of failure.
 */
static int nuc980_dma_alloc_chan_resources(struct dma_chan *chan)
{
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	struct nuc980_dma_data *data = chan->private;
	const char *name = dma_chan_name(chan);
	int ret, i;
	ENTRY();
	DMA_DEBUG("name =%s\n", name);

	if (data && data->name)
		name = data->name;

	DMA_DEBUG("edmac->irq =%d\n", edmac->irq);

	spin_lock_irq(&edmac->lock);
	dma_cookie_init(&edmac->chan);
	ret = edmac->edma->hw_setup(edmac);
	spin_unlock_irq(&edmac->lock);

	if (ret)
		return ret;

	for (i = 0; i < DMA_MAX_CHAN_DESCRIPTORS; i++) {
		struct nuc980_dma_desc *desc;

		desc = kzalloc(sizeof(*desc), GFP_KERNEL);
		if (!desc) {
			dev_warn(chan2dev(edmac), "not enough descriptors\n");
			break;
		}
		INIT_LIST_HEAD(&desc->tx_list);
		dma_async_tx_descriptor_init(&desc->txd, chan);
		desc->txd.flags = DMA_CTRL_ACK;
		desc->txd.tx_submit = nuc980_dma_tx_submit;
		nuc980_dma_desc_put(edmac, desc);
	}
	DMA_DEBUG("return %d\n",i);
	LEAVE();
	return i;

}

/**
 * nuc980_dma_free_chan_resources - release resources for the channel
 * @chan: channel
 *
 * Function releases all the resources allocated for the given channel.
 * The channel must be idle when this is called.
 */
static void nuc980_dma_free_chan_resources(struct dma_chan *chan)
{
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	struct nuc980_dma_desc *desc, *d;
	LIST_HEAD(list);
	ENTRY();
	BUG_ON(!list_empty(&edmac->queue));
	spin_lock(&edmac->lock);
	edmac->edma->hw_shutdown(edmac);
	edmac->runtime_addr = 0;
	edmac->runtime_ctrl = 0;
	list_splice_init(&edmac->free_list, &list);
	spin_unlock(&edmac->lock);
	list_for_each_entry_safe(desc, d, &list, node)
	kfree(desc);
	LEAVE();
}

/**
 * nuc980_dma_prep_dma_memcpy - prepare a memcpy DMA operation
 * @chan: channel
 * @dest: destination bus address
 * @src: source bus address
 * @len: size of the transaction
 * @flags: flags for the descriptor
 *
 * Returns a valid DMA descriptor or %NULL in case of failure.
 */
static struct dma_async_tx_descriptor *
nuc980_dma_prep_dma_memcpy(struct dma_chan *chan, dma_addr_t dest,
                           dma_addr_t src, size_t len, unsigned long flags) {
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	struct nuc980_dma_desc *desc, *first;
	size_t bytes, offset;
	ENTRY();
	first = NULL;
	for (offset = 0; offset < len; offset += bytes) {
		desc = nuc980_dma_desc_get(edmac);
		if (!desc) {
			dev_warn(chan2dev(edmac), "couln't get descriptor\n");
			goto fail;
		}

		bytes = min_t(size_t, len - offset, DMA_MAX_CHAN_BYTES);

		desc->src_addr = src + offset;
		desc->dst_addr = dest + offset;
		desc->size = bytes;
		desc->config.reqsel = 0;
		desc->config.timeout_counter=0;
		desc->config.timeout_prescaler=0;
		desc->config.en_sc = 0;
		desc->dir = DMA_MEM_TO_MEM;
		edmac->runtime_ctrl = 0;
		DMA_DEBUG("src_addr=0x%08x\n",desc->src_addr);
		DMA_DEBUG("dst_addr=0x%08x\n",desc->dst_addr);
		DMA_DEBUG("size=0x%08x\n",desc->size);
		DMA_DEBUG("offset=0x%08x\n",offset);
		if (!first)
			first = desc;
		else
			list_add_tail(&desc->node, &first->tx_list);
	}

	first->txd.cookie = -EBUSY;
	first->txd.flags = flags;
	LEAVE();

	return &first->txd;
fail:
	DMA_DEBUG("%s fail =>\n", __FUNCTION__);
	nuc980_dma_desc_put(edmac, first);
	LEAVE();
	return NULL;
}

/**
 * nuc980_dma_prep_slave_sg - prepare a slave DMA operation
 * @chan: channel
 * @sgl: list of buffers to transfer
 * @sg_len: number of entries in @sgl
 * @dir: direction of tha DMA transfer
 * @flags: flags for the descriptor
 * @context: operation context (ignored)
 *
 * Returns a valid DMA descriptor or %NULL in case of failure.
 */
static struct dma_async_tx_descriptor *
nuc980_dma_prep_slave_sg(struct dma_chan *chan, struct scatterlist *sgl,
                         unsigned int sg_len, enum dma_transfer_direction dir,
                         unsigned long flags, void *context) {
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	struct nuc980_dma_desc *desc, *first;
	struct scatterlist *sg;
	struct nuc980_dma_config *config;
	int i;
	ENTRY();

	struct nuc980_dma_desc *d;
	LIST_HEAD(list);
	list_splice_init(&edmac->active, &list);
	/* Now we can release all the chained descriptors */
	list_for_each_entry_safe(desc, d, &list, node) {
		desc->txd.flags = DMA_CTRL_ACK;
		nuc980_dma_desc_put(edmac, desc);
	}
	config =(struct nuc980_dma_config *)context;

	if (test_bit(NUC980_DMA_IS_CYCLIC, &edmac->flags)) {
		dev_warn(chan2dev(edmac),
		         "channel is already used for cyclic transfers\n");
		return NULL;
	}

	first = NULL;
	for_each_sg(sgl, sg, sg_len, i) {
		if (sg_dma_len(sg) > DMA_MAX_CHAN_BYTES) {
			dev_warn(chan2dev(edmac), "too big transfer size %d\n",
			         sg_dma_len(sg));
			goto fail;
		}

		desc = nuc980_dma_desc_get(edmac);
		if (!desc) {
			dev_warn(chan2dev(edmac), "couln't get descriptor\n");
			goto fail;
		}

		if (dir == DMA_MEM_TO_DEV) {
			desc->src_addr = sg_dma_address(sg);
			desc->dst_addr = edmac->runtime_addr;
			desc->dir = DMA_MEM_TO_DEV;
		} else {
			desc->src_addr = edmac->runtime_addr;
			desc->dst_addr = sg_dma_address(sg);
			desc->dir = DMA_DEV_TO_MEM;
		}
		desc->size = sg_dma_len(sg);
		desc->config.reqsel= config->reqsel;
		desc->config.timeout_counter=config->timeout_counter;
		desc->config.timeout_prescaler=config->timeout_prescaler;
		desc->config.en_sc = config->en_sc;
		DMA_DEBUG("desc->src_addr=%x\n",desc->src_addr);
		DMA_DEBUG("desc->dst_addr=%x\n",desc->dst_addr);
		DMA_DEBUG("desc->size=%x\n",desc->size);
		DMA_DEBUG("*context=%x\n",*(u32 *)context);

		if (!first)
			first = desc;
		else
			list_add_tail(&desc->node, &first->tx_list);
	}

	first->txd.cookie = -EBUSY;
	first->txd.flags = flags;
	LEAVE();
	return &first->txd;

fail:
	DMA_DEBUG("%s fail =>\n", __FUNCTION__);
	nuc980_dma_desc_put(edmac, first);
	LEAVE();
	return NULL;
}

/**
 * nuc980_dma_prep_dma_cyclic - prepare a cyclic DMA operation
 * @chan: channel
 * @dma_addr: DMA mapped address of the buffer
 * @buf_len: length of the buffer (in bytes)
 * @period_len: length of a single period
 * @dir: direction of the operation
 * @flags: tx descriptor status flags
 * @context: operation context (ignored)
 *
 * Prepares a descriptor for cyclic DMA operation. This means that once the
 * descriptor is submitted, we will be submitting in a @period_len sized
 * buffers and calling callback once the period has been elapsed. Transfer
 * terminates only when client calls dmaengine_terminate_all() for this
 * channel.
 *
 * Returns a valid DMA descriptor or %NULL in case of failure.
 */
static struct dma_async_tx_descriptor *
nuc980_dma_prep_dma_cyclic(struct dma_chan *chan, dma_addr_t dma_addr,
                           size_t buf_len, size_t period_len,
                           enum dma_transfer_direction dir, unsigned long flags) {
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	struct nuc980_dma_desc *desc, *first;
	size_t offset = 0;
	ENTRY();
	if (test_and_set_bit(NUC980_DMA_IS_CYCLIC, &edmac->flags)) {
		dev_warn(chan2dev(edmac),
		         "channel is already used for cyclic transfers\n");
		return NULL;
	}

	if (period_len > DMA_MAX_CHAN_BYTES) {
		dev_warn(chan2dev(edmac), "too big period length %d\n",
		         period_len);
		return NULL;
	}

	/* Split the buffer into period size chunks */
	first = NULL;
	for (offset = 0; offset < buf_len; offset += period_len) {
		desc = nuc980_dma_desc_get(edmac);
		if (!desc) {
			dev_warn(chan2dev(edmac), "couln't get descriptor\n");
			goto fail;
		}

		desc->size = period_len;

		if (!first)
			first = desc;
		else
			list_add_tail(&desc->node, &first->tx_list);
	}

	first->txd.cookie = -EBUSY;
	LEAVE();
	return &first->txd;

fail:
	nuc980_dma_desc_put(edmac, first);
	LEAVE();
	return NULL;
}

static int nuc980_dma_slave_config(struct dma_chan *chan,
                                   struct dma_slave_config *config)
{
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	enum dma_slave_buswidth width;
	u32 addr, ctrl;

	ENTRY();

	switch (config->direction) {
		case DMA_DEV_TO_MEM:
			ctrl    = PDMA_DSCT_CTL_SAINC_Msk|PDMA_DSCT_CTL_TXTYPE_Msk;
			width = config->src_addr_width;
			addr    = config->src_addr;
			break;

		case DMA_MEM_TO_DEV:
			ctrl    = PDMA_DSCT_CTL_DAINC_Msk|PDMA_DSCT_CTL_TXTYPE_Msk;
			width = config->dst_addr_width;
			addr    = config->dst_addr;
			break;

		default:
			return -EINVAL;
	}


	switch (width) {
		case DMA_SLAVE_BUSWIDTH_1_BYTE:
			ctrl |= 0<<PDMA_DSCT_CTL_TXWIDTH_Pos;
			break;
		case DMA_SLAVE_BUSWIDTH_2_BYTES:
			ctrl |= 1<<PDMA_DSCT_CTL_TXWIDTH_Pos;
			break;
		case DMA_SLAVE_BUSWIDTH_4_BYTES:
			ctrl |= 2<<PDMA_DSCT_CTL_TXWIDTH_Pos;
			break;
		default:
			return -EINVAL;
	}

	//spin_lock_irqsave(&edmac->lock, flags);
	edmac->runtime_addr = addr;
	edmac->runtime_ctrl = ctrl;
	//spin_unlock_irqrestore(&edmac->lock, flags);

	LEAVE();
	return 0;
}

/**
 * nuc980_dma_tx_status - check if a transaction is completed
 * @chan: channel
 * @cookie: transaction specific cookie
 * @state: state of the transaction is stored here if given
 *
 * This function can be used to query state of a given transaction.
 */
static enum dma_status nuc980_dma_tx_status(struct dma_chan *chan,
                dma_cookie_t cookie,
                struct dma_tx_state *state)
{
	struct nuc980_dma_chan *edmac = to_nuc980_dma_chan(chan);
	enum dma_status ret;

	spin_lock(&edmac->lock);
	ret = dma_cookie_status(chan, cookie, state);
	spin_unlock(&edmac->lock);
	return ret;
}

/**
 * nuc980_dma_issue_pending - push pending transactions to the hardware
 * @chan: channel
 *
 * When this function is called, all pending transactions are pushed to the
 * hardware and executed.
 */
static void nuc980_dma_issue_pending(struct dma_chan *chan)
{
	ENTRY();
	nuc980_dma_advance_work(to_nuc980_dma_chan(chan));
	LEAVE();
}

static int nuc980_dma_probe(struct platform_device *pdev)
{
	struct nuc980_dma_platform_data *pdata;
	struct nuc980_dma_engine *edma;
	struct dma_device *dma_dev;
	struct resource res;
	size_t edma_size;
	struct clk *clk;
	int ret, i;
	
	ENTRY();
	platform_device_add_data(pdev, &nuc980_dma_data, sizeof(nuc980_dma_data));
	printk("%s - pdev = %s\n", __func__, pdev->name);
	pdata = dev_get_platdata(&pdev->dev);
	edma_size = pdata->num_channels * sizeof(struct nuc980_dma_chan);
	edma = kzalloc(sizeof(*edma) + edma_size, GFP_KERNEL);

	if (!edma) {
		DMA_DEBUG("NUC980 GDMA -ENOMEM\n");
		return -ENOMEM;
	}
	DMA_DEBUG("NUC980 GDMA !!!\n");

	/* enable pdma0/pdma1 clock */
	clk = of_clk_get_by_name(pdev->dev.of_node, "pdma0_clk");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "cannot get clock\n");
		return -ENOENT;
	}
	dev_dbg(&pdev->dev, "clock source %p\n", clk);
	clk_prepare_enable(clk);

	clk = of_clk_get_by_name(pdev->dev.of_node, "pdma1_clk");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "cannot get clock\n");
		return -ENOENT;
	}
	dev_dbg(&pdev->dev, "clock source %p\n", clk);
	clk_prepare_enable(clk);

	/* ioremap register resources */
	if (of_address_to_resource(pdev->dev.of_node, 0, &res)){
		dev_err(&pdev->dev, "Failed to get memory resource\n");
		return -ENOMEM;
	}
	pdma_reg_base = ioremap(res.start, resource_size(&res));
	if (!pdma_reg_base) {
		dev_err(&pdev->dev, "Unable to remap register memory!\n");
		return -ENOMEM;
	}

	// Complete struct initialization
	pdma0_irq = of_irq_get_byname(pdev->dev.of_node, "pdma0_irq");
	pdma1_irq = of_irq_get_byname(pdev->dev.of_node, "pdma1_irq");
	for (int count = 0; count < pdata->num_channels; count++) {
		if (count % 2 == 0) { // A PDMA0 channel
			pdata->channels[count].base = ((void*)PDMA0) + ((count / 2) * 0x10);
			pdata->channels[count].irq = of_irq_get_byname(pdev->dev.of_node, "pdma0_irq");
		}
		else { // A PDMA1 channel
			pdata->channels[count].base = ((void*)PDMA1) + ((count / 2) * 0x10);
			pdata->channels[count].irq = of_irq_get_byname(pdev->dev.of_node, "pdma1_irq");
		}
	}
	dma_dev = &edma->dma_dev;
	edma->num_channels = pdata->num_channels;

	INIT_LIST_HEAD(&dma_dev->channels);
	for (i = 0; i < pdata->num_channels; i++) {
		const struct nuc980_dma_chan_data *cdata = &pdata->channels[i];
		struct nuc980_dma_chan *edmac = &edma->channels[i];
		DMA_DEBUG("ch=%d, base=%08X, irq=%d\n", i, pdata->channels[i].base, pdata->channels[i].irq);
		edmac->chan.device = dma_dev;
		edmac->chan.private = 0;
		edmac->regs = cdata->base;
		edmac->irq = cdata->irq;
		edmac->sc_flag = 0;
		edmac->edma = edma;
		edmac->id = (i / 2);
		spin_lock_init(&edmac->lock);
		spin_lock_init(&edmac->wklock);
		INIT_LIST_HEAD(&edmac->active);
		INIT_LIST_HEAD(&edmac->queue);
		INIT_LIST_HEAD(&edmac->free_list);
		tasklet_init(&edmac->tasklet, nuc980_dma_tasklet,
		             (unsigned long)edmac);
		tasklet_init(&edmac->tasklet_sc, nuc980_dma_sc_tasklet,
		             (unsigned long)edmac);

		list_add_tail(&edmac->chan.device_node,
		              &dma_dev->channels);
	}

	
	ret = request_irq(of_irq_get_byname(pdev->dev.of_node, "pdma0_irq"), nuc980_dma_interrupt, IRQF_SHARED, "PDMA0", edma);
	if (ret) {
		printk("request irq(IRQ_PDMA0) failed\n");
		return ret;
	}
	ret = request_irq(of_irq_get_byname(pdev->dev.of_node, "pdma1_irq"), nuc980_dma_interrupt, IRQF_SHARED, "PDMA1", edma);
	if (ret) {
		printk("request irq(IRQ_PDMA1) failed\n");
		return ret;
	}

	dma_cap_zero(dma_dev->cap_mask);
	dma_cap_set(DMA_SLAVE, dma_dev->cap_mask);
	dma_cap_set(DMA_CYCLIC, dma_dev->cap_mask);

	dma_dev->dev = &pdev->dev;
	dma_dev->device_alloc_chan_resources = nuc980_dma_alloc_chan_resources;
	dma_dev->device_free_chan_resources = nuc980_dma_free_chan_resources;
	dma_dev->device_prep_slave_sg = nuc980_dma_prep_slave_sg;
	dma_dev->device_prep_dma_cyclic = nuc980_dma_prep_dma_cyclic;
	dma_dev->device_config = nuc980_dma_slave_config;
	dma_dev->device_issue_pending = nuc980_dma_issue_pending;
	dma_dev->device_tx_status = nuc980_dma_tx_status;

	dma_set_max_seg_size(dma_dev->dev, DMA_MAX_CHAN_BYTES);

	dma_cap_set(DMA_MEMCPY, dma_dev->cap_mask);
	dma_dev->device_prep_dma_memcpy = nuc980_dma_prep_dma_memcpy;

	edma->hw_setup = hw_setup;
	edma->hw_shutdown = hw_shutdown;
	edma->hw_submit = hw_submit;
	edma->hw_interrupt = hw_interrupt;
	dma_cap_set(DMA_PRIVATE, dma_dev->cap_mask);
	spin_lock_init(&pdma0_lock);
	spin_lock_init(&pdma1_lock);

	platform_set_drvdata(pdev, edma);

	ret = dma_async_device_register(dma_dev);
	if (unlikely(ret)) {
		kfree(edma);
	} else {
		dev_info(dma_dev->dev, "NUC980 DMA ready\n");
	}
	LEAVE();

	return ret;
}

static int nuc980_dma_suspend(struct platform_device *pdev,pm_message_t state)
{
	ENTRY();
	LEAVE();
	return 0;
}

static int nuc980_dma_resume(struct platform_device *pdev)
{
	ENTRY();
	LEAVE();
	return 0;
}

static struct platform_device_id nuc980_dma_driver_ids[] = {
	{ "nuc980-dma", 0 },
	{ },
};

static const struct of_device_id nuc980_dma_of_match[] = {
	{ .compatible = "nuvoton,nuc980-dma" },
	{},
};

static struct platform_driver nuc980_dma_driver = {
	.driver     = {
		.name   = "nuc980-dma",
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(nuc980_dma_of_match),
	},
	.probe      = nuc980_dma_probe,
	.resume     = nuc980_dma_resume,
	.suspend    = nuc980_dma_suspend,
	.id_table   = nuc980_dma_driver_ids,
};
module_platform_driver(nuc980_dma_driver);


MODULE_DESCRIPTION("NUC980 DMA driver");
MODULE_LICENSE("GPL");
