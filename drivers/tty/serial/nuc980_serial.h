/*
 *  linux/drivers/serial/nuc980_serial.h
 *
 *  NUC980 serial driver header file
 *
 *
 *  Copyright (C) 2017 Nuvoton Technology Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __NUC980_SERIAL_H__
#define __NUC980_SERIAL_H__

#define UART_REG_RBR	0x00
#define UART_REG_THR	0x00

#define UART_REG_IER	0x04
#define RDA_IEN		0x00000001
#define THRE_IEN	0x00000002
#define RLS_IEN		0x00000004
#define RTO_IEN		0x00000010
#define BUFERR_IEN	0x00000020
#define TIME_OUT_EN	0x00000800
#define TXPDMAEN	0x00004000
#define RXPDMAEN	0x00008000

#define UART_REG_FCR	0x08
#define RFR		0x00000002
#define TFR		0x00000004

#define UART_REG_LCR	0x0C
#define	NSB		0x00000004
#define PBE		0x00000008
#define EPE		0x00000010
#define SPE		0x00000020
#define BCB		0x00000040

#define UART_REG_MCR	0x10
#define UART_REG_MSR	0x14

#define UART_REG_FSR	0x18
#define RX_OVER_IF	0x00000001
#define TX_OVER_IF	0x01000000
#define PEF		0x00000010
#define FEF		0x00000020
#define BIF		0x00000040
#define RX_EMPTY	0x00004000
#define TX_EMPTY	0x00400000
#define TX_FULL		0x00800000
#define RX_FULL		0x00008000
#define TE_FLAG		0x10000000

#define UART_REG_ISR	0x1C
#define RDA_IF		0x00000001
#define THRE_IF		0x00000002
#define TOUT_IF		0x00000010
#define THRE_INT	0x00000200
#define HWRLS_IF	0x00040000
#define HWBUFE_IF	0x00200000

#define UART_REG_TOR	0x20
#define UART_REG_BAUD	0x24

#define UART_REG_IRCR	0x28

#define UART_REG_ALT_CSR	0x2C

#define UART_FUN_SEL	0x30
#define FUN_SEL_UART	0x00000000
#define FUN_SEL_LIN		0x00000001
#define FUN_SEL_IrDA	0x00000002
#define FUN_SEL_RS485	0x00000003
#define FUN_SEL_Msk		0x00000007

#define UART_REG_WKCTL	0x40
#define UART_REG_WKSTS	0x44

#ifdef __cplusplus
#define   __I     volatile             /*!< Defines 'read only' permissions */
#else
#define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/**
   @addtogroup REGISTER Control Register
   @{
*/

/**
    @addtogroup PDMA Peripheral Direct Memory Access Controller(PDMA)
    Memory Mapped Structure for PDMA Controller
@{ */


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

/*---------------------------------------------------------------------------------------------------------*/
/*  Peripheral Transfer Mode Constant Definitions                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define PDMA_MEM          0UL
#define PDMA_UART0_TX     4UL
#define PDMA_UART0_RX     5UL
#define PDMA_UART1_TX     6UL
#define PDMA_UART1_RX     7UL
#define PDMA_UART2_TX     8UL
#define PDMA_UART2_RX     9UL
#define PDMA_UART3_TX    10UL
#define PDMA_UART3_RX    11UL
#define PDMA_UART4_TX    12UL
#define PDMA_UART4_RX    13UL
#define PDMA_UART5_TX    14UL
#define PDMA_UART5_RX    15UL
#define PDMA_UART6_TX    16UL
#define PDMA_UART6_RX    17UL
#define PDMA_UART7_TX    18UL
#define PDMA_UART7_RX    19UL
#define PDMA_SPI0_TX     20UL
#define PDMA_SPI0_RX     21UL
#define PDMA_SPI1_TX     22UL
#define PDMA_SPI1_RX     23UL
#define PDMA_SPI2_TX     24UL
#define PDMA_SPI2_RX     25UL
#define PDMA_UART8_TX     26UL
#define PDMA_UART8_RX     27UL
#define PDMA_UART9_TX     28UL
#define PDMA_UART9_RX     29UL

/**@}*/ /* PDMA_CONST */
/**@}*/ /* end of PDMA register group */
/**@}*/ /* end of REGISTER group */

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#ifndef __ASSEMBLY__
#define NUC980_ADDR(x)		((void __iomem *)(0xF0000000 + (x)))
#else
#define NUC980_ADDR(x)		(0xF0000000 + (x))
#endif

/*
 * interrupt controller is the first thing we put in, to make
 * the assembly code for the irq detection easier
 */
#define NUC980_VA_IRQ		NUC980_ADDR(0x00000000)
#define NUC980_PA_IRQ		(0xB0042000)
#define NUC980_SZ_IRQ		SZ_4K

/* GCR, Clock management */
#define NUC980_VA_GCR_CLK	NUC980_ADDR(0x08002000)
#define NUC980_PA_GCR_CLK	(0xB0000000)
#define NUC980_SZ_GCR_CLK	SZ_4K

#define NUC980_VA_GCR		(NUC980_VA_GCR_CLK)
#define NUC980_PA_GCR		(0xB0000000)
#define NUC980_SZ_GCR		SZ_512

/* Clock management */
#define NUC980_VA_CLK		(NUC980_VA_GCR+0x200)
#define NUC980_PA_CLK		(0xB0000200)
#define NUC980_SZ_CLK		SZ_512

/* SDIC management */
#define NUC980_VA_SDIC		NUC980_ADDR(0x00002000)
#define NUC980_PA_SDIC		(0xB0002000)
#define NUC980_SZ_SDIC		SZ_4K

/* EBI management */
#define NUC980_VA_EBI		NUC980_ADDR(0x00010000)
#define NUC980_PA_EBI		(0xB0010000)
#define NUC980_SZ_EBI		SZ_4K

/* GPIO management */
#define NUC980_VA_GPIO		NUC980_ADDR(0x00004000)
#define NUC980_PA_GPIO		(0xB0004000)
#define NUC980_SZ_GPIO		SZ_4K

/* External MAC0 control*/
#define NUC980_VA_EMAC0		NUC980_ADDR(0x00012000)
#define NUC980_PA_EMAC0		(0xB0012000)
#define NUC980_SZ_EMAC0		SZ_4K

/* External MAC1 control*/
#define NUC980_VA_EMAC1		NUC980_ADDR(0x00022000)
#define NUC980_PA_EMAC1		(0xB0022000)
#define NUC980_SZ_EMAC1		SZ_4K

/* PDMA0 */
#define NUC980_VA_PDMA0		NUC980_ADDR(0x00008000)
#define NUC980_PA_PDMA0		(0xB0008000)
#define NUC980_SZ_PDMA0		SZ_4K

/* PDMA1 */
#define NUC980_VA_PDMA1		NUC980_ADDR(0x00009000)
#define NUC980_PA_PDMA1		(0xB0009000)
#define NUC980_SZ_PDMA1		SZ_4K

/* USB host controller*/
#define NUC980_VA_EHCI		NUC980_ADDR(0x00015000)
#define NUC980_PA_EHCI		(0xB0015000)
#define NUC980_SZ_EHCI		SZ_4K

#define NUC980_VA_OHCI		NUC980_ADDR(0x00017000)
#define NUC980_PA_OHCI		(0xB0017000)
#define NUC980_SZ_OHCI		SZ_4K

/* USB Device port */
#define NUC980_VA_USBDEV	NUC980_ADDR(0x00016000)
#define NUC980_PA_USBDEV	(0xB0016000)
#define NUC980_SZ_USBDEV	SZ_4K

/* I2S */
#define NUC980_VA_I2S		NUC980_ADDR(0x00020000)
#define NUC980_PA_I2S		(0xB0020000)
#define NUC980_SZ_I2S		SZ_4K

/* SDH Controller */
#define NUC980_VA_SDH		NUC980_ADDR(0x00018000)
#define NUC980_PA_SDH		(0xB0018000)
#define NUC980_SZ_SDH		SZ_4K

/* FMI Controller */
#define NUC980_VA_FMI		NUC980_ADDR(0x00019000)
#define NUC980_PA_FMI		(0xB0019000)
#define NUC980_SZ_FMI		SZ_4K

/* VCAP Interface */
#define NUC980_VA_CCAP0		NUC980_ADDR(0x00024000)
#define NUC980_PA_CCAP0		(0xB0024000)
#define NUC980_SZ_CCAP0		SZ_4K

#define NUC980_VA_CCAP1		NUC980_ADDR(0x00014000)
#define NUC980_PA_CCAP1		(0xB0014000)
#define NUC980_SZ_CCAP1		SZ_4K


/* Crypto Engine */
#define NUC980_VA_CRYPTO	NUC980_ADDR(0x0001C000)
#define NUC980_PA_CRYPTO	(0xB001C000)
#define NUC980_SZ_CRYPTO	SZ_16K	// use 12K only

/* Watchdog Timer (WDT) & Window Watchdog Timer (WWDT)  */
#define NUC980_VA_WDT_WWDT	NUC980_ADDR(0x00040000)
#define NUC980_PA_WDT_WWDT	(0xB0040000)
#define NUC980_SZ_WDT_WWDT	SZ_4K

/* Watchdog Timer (WDT) */
#define NUC980_VA_WDT		NUC980_ADDR(0x00040000)
#define NUC980_PA_WDT		(0xB0040000)
#define NUC980_SZ_WDT		SZ_256

/* Window Watchdog Timer (WWDT) */
#define NUC980_VA_WWDT		NUC980_ADDR(0x00040100)
#define NUC980_PA_WWDT		(0xB0040100)
#define NUC980_SZ_WWDT		SZ_256

/* RTC */
#define NUC980_VA_RTC		NUC980_ADDR(0x00041000)
#define NUC980_PA_RTC		(0xB0041000)
#define NUC980_SZ_RTC		SZ_4K

/* Smart card host*/
#define NUC980_VA_SC0		NUC980_ADDR(0x00090000)
#define NUC980_PA_SC0		(0xB0090000)
#define NUC980_SZ_SC0		SZ_4K

#define NUC980_VA_SC1		NUC980_ADDR(0x00091000)
#define NUC980_PA_SC1		(0xB0091000)
#define NUC980_SZ_SC1		SZ_4K

/* I2C hardware controller */
#define NUC980_VA_I2C0		NUC980_ADDR(0x00080000)
#define NUC980_PA_I2C0		(0xB0080000)
#define NUC980_SZ_I2C0		SZ_4K

#define NUC980_VA_I2C1		NUC980_ADDR(0x00081000)
#define NUC980_PA_I2C1		(0xB0081000)
#define NUC980_SZ_I2C1		SZ_4K

#define NUC980_VA_I2C2		NUC980_ADDR(0x00082000)
#define NUC980_PA_I2C2		(0xB0082000)
#define NUC980_SZ_I2C2		SZ_4K

#define NUC980_VA_I2C3		NUC980_ADDR(0x00083000)
#define NUC980_PA_I2C3		(0xB0083000)
#define NUC980_SZ_I2C3		SZ_4K

/* UART */
#define NUC980_VA_UART0		NUC980_ADDR(0x00070000)
#define NUC980_PA_UART0		(0xB0070000)
#define NUC980_SZ_UART0		SZ_4K

#define NUC980_VA_UART1		NUC980_ADDR(0x00071000)
#define NUC980_PA_UART1		(0xB0071000)
#define NUC980_SZ_UART1		SZ_4K

#define NUC980_VA_UART2		NUC980_ADDR(0x00072000)
#define NUC980_PA_UART2		(0xB0072000)
#define NUC980_SZ_UART2		SZ_4K

#define NUC980_VA_UART3		NUC980_ADDR(0x00073000)
#define NUC980_PA_UART3		(0xB0073000)
#define NUC980_SZ_UART3		SZ_4K

#define NUC980_VA_UART4		NUC980_ADDR(0x00074000)
#define NUC980_PA_UART4		(0xB0074000)
#define NUC980_SZ_UART4		SZ_4K

#define NUC980_VA_UART5		NUC980_ADDR(0x00075000)
#define NUC980_PA_UART5		(0xB0075000)
#define NUC980_SZ_UART5		SZ_4K

#define NUC980_VA_UART6		NUC980_ADDR(0x00076000)
#define NUC980_PA_UART6		(0xB0076000)
#define NUC980_SZ_UART6		SZ_4K

#define NUC980_VA_UART7		NUC980_ADDR(0x00077000)
#define NUC980_PA_UART7		(0xB0077000)
#define NUC980_SZ_UART7		SZ_4K

#define NUC980_VA_UART8		NUC980_ADDR(0x00078000)
#define NUC980_PA_UART8		(0xB0078000)
#define NUC980_SZ_UART8		SZ_4K

#define NUC980_VA_UART9		NUC980_ADDR(0x00079000)
#define NUC980_PA_UART9		(0xB0079000)
#define NUC980_SZ_UART9		SZ_4K

/* SPI Controller */
#define NUC980_VA_SPI0 		NUC980_ADDR(0x00060000)
#define NUC980_PA_SPI0 		(0xB0060000)
#define NUC980_SZ_SPI0		SZ_4K

#define NUC980_VA_SPI1		NUC980_ADDR(0x00061000)
#define NUC980_PA_SPI1		(0xB0061000)
#define NUC980_SZ_SPI1		SZ_4K

#define NUC980_VA_SPI2		NUC980_ADDR(0x00062000)
#define NUC980_PA_SPI2		(0xB0062000)
#define NUC980_SZ_SPI2		SZ_4K

/* TIMER Controller */
#define NUC980_VA_TIMER01	NUC980_ADDR(0x00050000)
#define NUC980_PA_TIMER01	(0xB0050000)
#define NUC980_SZ_TIMER01	SZ_4K

#define NUC980_VA_TIMER23	NUC980_ADDR(0x00051000)
#define NUC980_PA_TIMER23	(0xB0051000)
#define NUC980_SZ_TIMER23	SZ_4K

#define NUC980_VA_TIMER45	NUC980_ADDR(0x00052000)
#define NUC980_PA_TIMER45	(0xB0052000)
#define NUC980_SZ_TIMER45	SZ_4K

/* Pulse Width Modulation(PWM) Registers */
#define NUC980_VA_PWM0		NUC980_ADDR(0x00058000)
#define NUC980_PA_PWM0		(0xB0058000)
#define NUC980_SZ_PWM0		SZ_4K

#define NUC980_VA_PWM1		NUC980_ADDR(0x00059000)
#define NUC980_PA_PWM1		(0xB0059000)
#define NUC980_SZ_PWM1		SZ_4K

/* ADC */
#define NUC980_VA_ADC		NUC980_ADDR(0x00043000)
#define NUC980_PA_ADC		(0xB0043000)
#define NUC980_SZ_ADC		SZ_4K

/* CAN Controller */
#define NUC980_VA_CAN0		NUC980_ADDR(0x000A0000)
#define NUC980_PA_CAN0		(0xB00A0000)
#define NUC980_SZ_CAN0		SZ_4K

#define NUC980_VA_CAN1		NUC980_ADDR(0x000A1000)
#define NUC980_PA_CAN1		(0xB00A1000)
#define NUC980_SZ_CAN1		SZ_4K

#define NUC980_VA_CAN2		NUC980_ADDR(0x000A2000)
#define NUC980_PA_CAN2		(0xB00A2000)
#define NUC980_SZ_CAN2		SZ_4K

#define NUC980_VA_CAN3		NUC980_ADDR(0x000A3000)
#define NUC980_PA_CAN3		(0xB00A3000)
#define NUC980_SZ_CAN3		SZ_4K


/* SRAM */
#define NUC980_VA_SRAM		NUC980_ADDR(0x0C000000)
#define NUC980_PA_SRAM		(0xBC000000)
#define NUC980_SZ_SRAM		SZ_16K

#endif // __NUC980_SERIAL_H__
