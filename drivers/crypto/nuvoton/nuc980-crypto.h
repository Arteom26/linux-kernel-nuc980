/*
 * arch/arm/mach-nuc980/include/mach/regs-crypto.h
 *
 * Copyright (c) 2017 Nuvoton technology corporation
 * All rights reserved.
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

#ifndef __ASM_ARM_REGS_CRYPTO_H
#define __ASM_ARM_REGS_CRYPTO_H

#include <linux/types.h>
#include <linux/ioctl.h>

int nuc980_crypto_raw_probe(struct platform_device *pdev);
int nuc980_crypto_raw_remove(struct platform_device *pdev);

struct nuc980_crypto_regs {
	u32 CRPT_INTEN;				/*!< Offset: 0x0000   Crypto Interrupt Enable Control Register           */
	u32 CRPT_INTSTS;			/*!< Offset: 0x0004   Crypto Interrupt Flag                              */
	u32 CRPT_PRNG_CTL;          /*!< Offset: 0x0008   PRNG Control Register                              */
	u32 CRPT_PRNG_SEED;			/*!< Offset: 0x000C   Seed for PRNG                                      */
	u32 CRPT_PRNG_KEY[8];		/*!< Offset: [0x0010] ~ [0x002c] PRNG Generated Key 0~7                  */
	u32 RESERVED1[8];		    /*!< Offset: 0x0030 ~ 0x004C   Reserved                                  */

	u32 CRPT_AES_FDBCK[4];		/*!< Offset: 0x0050 ~ 0x005C   AES Engine Output Feedback Data after Cryptographic Operation */
	u32 RESERVED2[40];		    /*!< Offset: 0x0068 ~ 0x00FC   Reserved                                  */
	u32 CRPT_AES_CTL;			/*!< Offset: 0x0100   AES engine setting                                 */
	u32 CRPT_AES_STS;			/*!< Offset: 0x0104   AES engine flag                                    */
	u32 CRPT_AES_DATIN;			/*!< Offset: 0x0108   AES engine data input port                         */
	u32 CRPT_AES_DATOUT;		/*!< Offset: 0x010C   AES engine data output port                        */
	u32 CRPT_AES0_KEY[8];		/*!< Offset: [0x0110] ~ [0x012c] AES Key Word 0~7 Register               */
	u32 CRPT_AES0_IV[4];		/*!< Offset: [0x0130] ~ [0x013c] AES Initial Vector Word 0 ~ 3 Register  */
	u32 CRPT_AES0_SADDR;		/*!< Offset: 0x0140   AES0 Source Address Register                       */
	u32 CRPT_AES0_DADDR;		/*!< Offset: 0x0144   AES0 Destination Address Register                  */
	u32 CRPT_AES0_CNT;			/*!< Offset: 0x0148   AES0 Byte Count Register                           */
	u32 AES_RESERVED1[109];	    /*!< Offset: 0x014C ~ 0x0FC   Reserved                                   */

	u32 CRPT_HMAC_CTL;		    /*!< Offset: 0x0300   HMAC/SHA engine parameter setting                  */
	u32 CRPT_HMAC_STS;		    /*!< Offset: 0x0304   HMAC/SHA status flag                               */
	u32 CRPT_HMAC_DGST[16];		/*!< Offset: 0x0308   HMAC/SHA digest message word 0~15                  */
	u32 CRPT_HMAC_KEYCNT;		/*!< Offset: 0x0348   HMAC/SHA key byte length                           */
	u32 CRPT_HMAC_SADDR;	    /*!< Offset: 0x034C   HMAC DMA Source Address Register                   */
	u32 CRPT_HMAC_DMACNT;		/*!< Offset: 0x0350   HMAC DMA count                                     */
	u32 CRPT_HMAC_DATIN;		/*!< Offset: 0x0354   HMAC software write to engine port                 */
    u32 RESERVE6[298];

    u32 ECC_CTL;                /* Offset 0x800:         ECC Control Register                            */
    u32 ECC_STS;                /* Offset 0x804:         ECC Status Register                             */
    u32 ECC_X1[18];             /* Offset 0x808 ~ 0x84C: The X-coordinate of the first point             */
    u32 ECC_Y1[18];             /* Offset 0x850 ~ 0x894: The Y-coordinate of the first point             */
    u32 ECC_X2[18];             /* Offset 0x898 ~ 0x8DC: The X-coordinate of the second point            */
    u32 ECC_Y2[18];             /* Offset 0x8E0 ~ 0x924: The Y-coordinate of the second point            */
    u32 ECC_A[18];      	    /* Offset 0x928 ~ 0x96C: Elliptic curve parameter A                      */
    u32 ECC_B[18];     	        /* Offset 0x970 ~ 0x9B4: Elliptic curve parameter B                      */
    u32 ECC_N[18];     	        /* Offset 0x9B8 ~ 0x9FC: Elliptic curve parameter N                      */
    u32 ECC_K[18];              /* Offset 0xA00 ~ 0xA44: Elliptic curve scalar K point multiplication    */
    u32 ECC_SADDR;              /* Offset 0xA48:         ECC DMA Source Address Register                 */
    u32 ECC_DADDR;              /* Offset 0xA4C:         ECC DMA Destination Address Register            */
    u32 ECC_STARTREG;           /* Offset 0xA50:         ECC Starting Address of Updated Registers       */
    u32 ECC_WORDCNT;            /* Offset 0xA54:         ECC Data Word Count                             */
    uint32_t RESERVE7[358];
    u32 ECC_FSM_DBG;            /* Offset 0xFF0:         ECC FSM Debug Register                          */
    uint32_t RESERVE8[3];

    u32 RSA_CTL;                /* Offset 0x1000:        RSA Control Register                            */
    u32 RSA_STS;                /* Offset 0x1004:        RSA Status Register                             */
    u32 RSA_M[128];             /* Offset 0x1008:        RSA the base of exponentiation words            */
    u32 RSA_E[128];             /* Offset 0x1208:        RSA the exponent of exponentiation words        */
    u32 RSA_N[128];             /* Offset 0x1408:        RSA the base of modulus operation word          */
    u32 RSA_C[128];             /* Offset 0x1608:        RSA the constant value of Montgomery domain words */
    u32 RSA_SADDR;              /* Offset 0x1808:        RSA DMA Source Address Register                 */
    u32 RSA_DADDR;              /* Offset 0x180C:        RSA DMA Destination Address Register            */
    u32 RSA_STARTREG;           /* Offset 0x1810:        RSA Starting Address of Updated Registers       */
    u32 RSA_WORDCNT;            /* Offset 0x1814:        RSA DMA Word Count                              */
};

struct nuc980_aes_regs
{
	u32 key[8];
	u32 iv[4];
	u32 src_addr;
	u32 dst_addr;
	u32 count;
};


/********************* Bit definition of CRPT_INTEN  ************************/
#define AESIEN						((u32)0x00000001)			/*!<AES DMA finish interrupt flag enable */
#define AESERRIEN					((u32)0x00000002)			/*!<AES error flag enable */
#define TDESIEN						((u32)0x00000100)			/*!<TDES interrupt source enable */
#define TDESERRIEN					((u32)0x00000200)			/*!<TDES error flag enable */
#define PRNGIEN						((u32)0x00010000)			/*!<PRNG finish interrupt enable */
#define HMACIEN						((u32)0x01000000)			/*!<HMAC interrupt enable */
#define HMACERRIEN					((u32)0x02000000)			/*!<HMAC error interrupt enable */

/********************* Bit definition of CRPT_INTSTS *******************/
#define AESIF						((u32)0x00000001)			/*!<AES finish interrupt flag */
#define AESERRIF					((u32)0x00000002)			/*!<AES error flag */
#define TDESIF						((u32)0x00000100)			/*!<TDES finish interrupt flag */
#define TDESERRIF					((u32)0x00000200)			/*!<TDES error flag */
#define PRNGIF						((u32)0x00010000)			/*!<PRNG finish interrupt flag */
#define HMACIF						((u32)0x01000000)			/*!<HMAC finish interrupt flag */
#define HMACERRIF					((u32)0x02000000)		/*!<HMAC error flag */

/********************* Bit definition of CRPT_PRNG_CTL *******************/
#define PRNG_START					((u32)0x00000001)			/*!<Write 1 start PRNG engine generate new KEY and store KEYx register. */
#define SEEDRLD						((u32)0x00000002)			/*!1: reload new seed; 0: generate key base original seed. */
#define PRNG_KEYSZ_MASK				((u32)0x0000000C)			/*!<PRNG KEY SIZE bit mask */
#define PRNG_KEYSZ_64			  	((u32)0x00000000)			/*!<PRNG 64 bits key */
#define PRNG_KEYSZ_128  			((u32)0x00000004)			/*!<PRNG 128 bits key */
#define PRNG_KEYSZ_192  			((u32)0x00000008)			/*!<PRNG 192 bits key */
#define PRNG_KEYSZ_256  			((u32)0x0000000C)			/*!<PRNG 256 bits key */
#define PRNG_BUSY					((u32)0x00000100)			/*!<read only, 1:  indicate the PRNG engine under generating KEY */


/********************* Bit definition of CRPT_AES_CTL *******************/
#define AES_START					((u32)0x00000001)			/*!<Write 1, AES engine starting. AES_BUSY flag will be set. */
#define AES_STOP					((u32)0x00000002)			/*!<Write 1,  stop  AES engine instantly. */
#define AES_KEYSZ_MASK 				((u32)0x0000000C)			/*!<AES KEY_SIZE bit mask */
#define AES_KEYSZ_128				((u32)0x00000000)			/*!<AES 128 bits key */
#define AES_KEYSZ_192				((u32)0x00000004)			/*!<AES 192 bits key */
#define AES_KEYSZ_256				((u32)0x00000008)			/*!<AES 256 bits key */
#define AES_EXTERNAL_KEY			((u32)0x00000010)			/*!<1: AES KEY use extend key. 0:  AES KEY source form  AES_KEYx registers. */
#define AES_DMALAST					((u32)0x00000020)			/*!<Index current operation is  last one. */
#define AES_DMACSCAD				((u32)0x00000040)			/*!<Cascade AES encrypt/decrypt result */
#define AES_DMAEN					((u32)0x00000080)			/*!<enable AES DMA engine */
#define AES_OPMODE_MASK		 		((u32)0x0000FF00)			/*!<AES engine operation mode bit mask */
#define AES_ECB_MODE				((u32)0x00000000)			/*!<AES ECB mode */
#define AES_CBC_MODE				((u32)0x00000100)			/*!<AES CBC mode */
#define AES_CFB_MODE				((u32)0x00000200)			/*!<AES CFB mode */
#define AES_OFB_MODE				((u32)0x00000300)			/*!<AES OFB mode */
#define AES_CTR_MODE				((u32)0x00000400)			/*!<AES CTR mode */
#define AES_CBCCS1_MODE				((u32)0x00001000)			/*!<AES CBC CS1 mode */
#define AES_CBCCS2_MODE				((u32)0x00001100)			/*!<AES CBC CS2 mode */
#define AES_CBCCS3_MODE				((u32)0x00001200)			/*!<AES CBC CS3 mode */
#define AES_ENCRYPT					((u32)0x00010000)			/*!<AES engine execute encryption */
#define AES_DECRYPT					((u32)0x00000000)			/*!<AES engine execute decryption */
#define AES_OUTSWAP					((u32)0x00400000)			/*!<AES engine output data transform */
#define AES_INSWAP					((u32)0x00800000)			/*!<AES engine input data transform */
#define AES_CHANNEL_MASK			((u32)0x03000000)			/*!<AES working channel bit mask */
#define AES_KEYPRT_MASK				((u32)0xFC000000)			/*!<AES unprotect key bit mask */
#define AES_KEY_UNPRT        		((u32)0x58000000)			/*!<Set AES unprotect key */
#define AES_KEY_PRT					((u32)0x80000000)			/*!<AES key protect */

/********************* Bit definition of CRPT_AES_STS *******************/
#define AES_BUSY					((u32)0x00000001)			/*!<AES engine under archiving */
#define AES_INBUFEMPTY				((u32)0x00000100)			/*!<AES input buffer empty */
#define AES_INBUFFULL				((u32)0x00000200)			/*!<AES input buffer full */
#define AES_INBUFERR				((u32)0x00000400)			/*!<ERROR during feed AES engine data. */
#define AES_CNTERR					((u32)0x00001000)			/*!<AES_CNT setting error */
#define AES_OUTBUFEMPTY				((u32)0x00010000)			/*!<AES output buffer empty */
#define AES_OUTBUFFULL				((u32)0x00020000)			/*!<AES output buffer full */
#define AES_OUTBUFERR				((u32)0x00040000)			/*!<ERROR during get AES engine result */
#define AES_BUSERR					((u32)0x00100000)			/*!<AES bus error */

/********************* Bit definition of HMAC_CTL ********************/
#define HMAC_START					((u32)0x00000001)			/*!<HMAC engine starting */
#define HMAC_STOP					((u32)0x00000002)			/*!<HMAC engine stop */
#define HMAC_EN						((u32)0x00000010)			/*!<execute HMAC function */
#define HMAC_DMALAST				((u32)0x00000020)			/*!<last DMA cascade round */
#define HMAC_DMAEN					((u32)0x00000080)			/*!<enable HMAC_DMA engine */
#define HMAC_OPMODE_MASK			((u32)0x00000700)			/*!<HMAC engine operation modes mask */
#define HMAC_SHA1					((u32)0x00000000)			/*!<SHA1 */
#define HMAC_SHA224					((u32)0x00000500)			/*!<SHA224 */
#define HMAC_SHA256					((u32)0x00000400)			/*!<SHA256 */
#define HMAC_SHA384					((u32)0x00000700)			/*!<SHA384 */
#define HMAC_SHA512					((u32)0x00000600)			/*!<SHA512 */
#define HMAC_OUTSWAP				((u32)0x00400000)			/*!<HMAC engine output data transform */
#define HMAC_INSWAP					((u32)0x00800000)			/*!<HMAC engine input data transform */

/********************* Bit definition of HMAC_FLAG *******************/
#define HMAC_BUSY					((u32)0x00000001)			/*!<HMAC engine busy */
#define HMAC_DMABUSY				((u32)0x00000002)			/*!<HMAC engine is under active */
#define HMAC_DMAERR					((u32)0x00000100)			/*!<HMAC DMA error */
#define HMAC_DINREQ					((u32)0x00010000)			/*!<HMAC_SOFTWARE mode Data input  request */

#define CRPT_INTEN_ECCIEN_Pos       (22)                                    /*!< CRPT INTEN: ECCIEN Position            */
#define CRPT_INTEN_ECCIEN_Msk       (0x1ul << CRPT_INTEN_ECCIEN_Pos)        /*!< CRPT INTEN: ECCIEN Mask                */

#define CRPT_INTEN_ECCEIEN_Pos      (23)                                    /*!< CRPT INTEN: ECCEIEN Position           */
#define CRPT_INTEN_ECCEIEN_Msk      (0x1ul << CRPT_INTEN_ECCEIEN_Pos)       /*!< CRPT INTEN: ECCEIEN Mask               */

#define CRPT_INTSTS_ECCIF_Pos       (22)                                    /*!< CRPT INTSTS: ECCIF Position            */
#define CRPT_INTSTS_ECCIF_Msk       (0x1ul << CRPT_INTSTS_ECCIF_Pos)        /*!< CRPT INTSTS: ECCIF Mask                */

#define CRPT_INTSTS_ECCEIF_Pos      (23)                                    /*!< CRPT INTSTS: ECCEIF Position           */
#define CRPT_INTSTS_ECCEIF_Msk      (0x1ul << CRPT_INTSTS_ECCEIF_Pos)       /*!< CRPT INTSTS: ECCEIF Mask               */

#define CRPT_ECC_CTL_START_Pos      (0)                                     /*!< CRPT ECC_CTL: START Position            */
#define CRPT_ECC_CTL_START_Mask     (0x1ul << CRPT_ECC_CTL_START_Pos)       /*!< CRPT ECC_CTL: START Mask                */

#define CRPT_ECC_CTL_STOP_Pos       (1)                                     /*!< CRPT ECC_CTL: STOP Position             */
#define CRPT_ECC_CTL_STOP_Mask      (0x1ul << CRPT_ECC_CTL_STOP_Pos)        /*!< CRPT ECC_CTL: STOP Mask                 */

#define CRPT_ECC_CTL_DMAEN_Pos      (7)                                     /*!< CRPT ECC_CTL: DMAEN Position            */
#define CRPT_ECC_CTL_DMAEN_Mask     (0x1ul << CRPT_ECC_CTL_DMAEN_Pos)       /*!< CRPT ECC_CTL: DMAEN Mask                */

#define CRPT_ECC_CTL_FSEL_Pos       (8)                                     /*!< CRPT ECC_CTL: FSEL Position             */
#define CRPT_ECC_CTL_FSEL_Mask      (0x1ul << CRPT_ECC_CTL_FSEL_Pos)        /*!< CRPT ECC_CTL: FSEL Mask                 */

#define CRPT_ECC_CTL_ECCOP_Pos      (9)                                     /*!< CRPT ECC_CTL: ECCOP Position            */
#define CRPT_ECC_CTL_ECCOP_Mask     (0x3ul << CRPT_ECC_CTL_ECCOP_Pos)       /*!< CRPT ECC_CTL: ECCOP Mask                */

#define CRPT_ECC_CTL_MODOP_Pos      (11)                                    /*!< CRPT ECC_CTL: MOPOP Position            */
#define CRPT_ECC_CTL_MODOP_Mask     (0x3ul << CRPT_ECC_CTL_MODOP_Pos)       /*!< CRPT ECC_CTL: MOPOP Mask                */

#define CRPT_ECC_CTL_LDP1_Pos       (16)                                    /*!< CRPT ECC_CTL: LDP1 Position             */
#define CRPT_ECC_CTL_LDP1_Mask      (0x1ul << CRPT_ECC_CTL_LDP1_Pos)        /*!< CRPT ECC_CTL: LDP1 Mask                 */

#define CRPT_ECC_CTL_LDP2_Pos       (17)                                    /*!< CRPT ECC_CTL: LDP2 Position             */
#define CRPT_ECC_CTL_LDP2_Mask      (0x1ul << CRPT_ECC_CTL_LDP2_Pos)        /*!< CRPT ECC_CTL: LDP2 Mask                 */

#define CRPT_ECC_CTL_LDA_Pos        (18)                                    /*!< CRPT ECC_CTL: LDA Position              */
#define CRPT_ECC_CTL_LDA_Mask       (0x1ul << CRPT_ECC_CTL_LDA_Pos)         /*!< CRPT ECC_CTL: LDA Mask                  */

#define CRPT_ECC_CTL_LDB_Pos        (19)                                    /*!< CRPT ECC_CTL: LDB Position              */
#define CRPT_ECC_CTL_LDB_Mask       (0x1ul << CRPT_ECC_CTL_LDB_Pos)         /*!< CRPT ECC_CTL: LDB Mask                  */

#define CRPT_ECC_CTL_LDN_Pos        (20)                                    /*!< CRPT ECC_CTL: LDN Position              */
#define CRPT_ECC_CTL_LDN_Mask       (0x1ul << CRPT_ECC_CTL_LDN_Pos)         /*!< CRPT ECC_CTL: LDN Mask                  */

#define CRPT_ECC_CTL_LDK_Pos        (21)                                    /*!< CRPT ECC_CTL: LDK Position              */
#define CRPT_ECC_CTL_LDK_Mask       (0x1ul << CRPT_ECC_CTL_LDK_Pos)         /*!< CRPT ECC_CTL: LDK Mask                  */

#define CRPT_ECC_CTL_CURVE_M_Pos    (22)                                    /*!< CRPT ECC_CTL: CURVE_M Position          */
#define CRPT_ECC_CTL_CURVE_M_Mask   (0x3ful << CRPT_ECC_CTL_CURVE_M_Pos)    /*!< CRPT ECC_CTL: CURVE_M Mask              */

#define CRPT_ECC_STS_BUSY_Pos       (0)                                     /*!< CRPT ECC_STS: BUSY Position             */
#define CRPT_ECC_STS_BUSY_Mask      (0x1ul << CRPT_ECC_STS_BUSY_Pos)        /*!< CRPT ECC_STS: BUSY Mask                 */

#define CRPT_ECC_STS_DMABUSY_Pos    (1)                                     /*!< CRPT ECC_STS: DMABUSY Position          */
#define CRPT_ECC_STS_DMABUSY_Mask   (0x1ul << CRPT_ECC_STS_DMABUSY_Pos)     /*!< CRPT ECC_STS: DMABUSY Mask              */

#define CRPT_ECC_STS_BUSERR_Pos     (16)                                    /*!< CRPT ECC_STS: BUSERR Position           */
#define CRPT_ECC_STS_BUSERR_Mask    (0x1ul << CRPT_ECC_STS_BUSERR_Pos)      /*!< CRPT ECC_STS: BUSERR Mask               */

#define CRPT_RSA_CTL_START_Pos      (0)                                     /*!< CRPT RSA_CTL: START Position            */
#define CRPT_RSA_CTL_START_Msk      (0x1ul << CRPT_RSA_CTL_START_Pos)       /*!< CRPT RSA_CTL: START Mask                */

#define CRPT_RSA_CTL_STOP_Pos       (1)                                     /*!< CRPT RSA_CTL: STOP Position             */
#define CRPT_RSA_CTL_STOP_Msk       (0x1ul << CRPT_RSA_CTL_STOP_Pos)        /*!< CRPT RSA_CTL: STOP Mask                 */

#define CRPT_RSA_CTL_DMAEN_Pos      (7)                                     /*!< CRPT RSA_CTL: DMAEN Position            */
#define CRPT_RSA_CTL_DMAEN_Msk      (0x1ul << CRPT_RSA_CTL_DMAEN_Pos)       /*!< CRPT RSA_CTL: DMAEN Mask                */

#define CRPT_RSA_CTL_LDM_Pos        (8)                                     /*!< CRPT RSA_CTL: LDM Position              */
#define CRPT_RSA_CTL_LDM_Msk        (0x1ul << CRPT_RSA_CTL_LDM_Pos)         /*!< CRPT RSA_CTL: LDM Mask                  */

#define CRPT_RSA_CTL_LDE_Pos        (9)                                     /*!< CRPT RSA_CTL: LDE Position              */
#define CRPT_RSA_CTL_LDE_Msk        (0x1ul << CRPT_RSA_CTL_LDE_Pos)         /*!< CRPT RSA_CTL: LDE Mask                  */

#define CRPT_RSA_CTL_LDN_Pos        (10)                                    /*!< CRPT RSA_CTL: LDN Position              */
#define CRPT_RSA_CTL_LDN_Msk        (0x1ul << CRPT_RSA_CTL_LDN_Pos)         /*!< CRPT RSA_CTL: LDN Mask                  */

#define CRPT_RSA_CTL_LDC_Pos        (11)                                    /*!< CRPT RSA_CTL: LDC Position              */
#define CRPT_RSA_CTL_LDC_Msk        (0x1ul << CRPT_RSA_CTL_LDC_Pos)         /*!< CRPT RSA_CTL: LDC Mask                  */

#define CRPT_RSA_CTL_KEYLEN_Pos     (16)                                    /*!< CRPT RSA_CTL: KEYLEN Position           */
#define CRPT_RSA_CTL_KEYLEN_Msk     (0x1FFFul << CRPT_RSA_CTL_KEYLEN_Pos)   /*!< CRPT RSA_CTL: KEYLEN Mask               */

#define CRPT_RSA_STS_BUSY_Pos       (0)                                     /*!< CRPT RSA_STS: BUSY Position             */
#define CRPT_RSA_STS_BUSY_Msk       (0x1ul << CRPT_RSA_STS_BUSY_Pos)        /*!< CRPT RSA_STS: BUSY Mask                 */

#define CRPT_RSA_STS_DMABUSY_Pos    (1)                                     /*!< CRPT RSA_STS: DMABUSY Position          */
#define CRPT_RSA_STS_DMABUSY_Msk    (0x1ul << CRPT_RSA_STS_DMABUSY_Pos)     /*!< CRPT RSA_STS: DMABUSY Mask              */

#define CRPT_RSA_STS_BUSERR_Pos     (16)                                    /*!< CRPT ECC_RSA: BUSERR Position           */
#define CRPT_RSA_STS_BUSERR_Msk     (0x1ul << CRPT_RSA_STS_BUSERR_Pos)      /*!< CRPT ECC_RSA: BUSERR Mask               */


#define ECCOP_POINT_MUL     (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_MODULE        (0x1UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_ADD     (0x2UL << CRPT_ECC_CTL_ECCOP_Pos)
#define ECCOP_POINT_DOUBLE  (0x0UL << CRPT_ECC_CTL_ECCOP_Pos)

#define MODOP_DIV           (0x0UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_MUL           (0x1UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_ADD           (0x2UL << CRPT_ECC_CTL_MODOP_Pos)
#define MODOP_SUB           (0x3UL << CRPT_ECC_CTL_MODOP_Pos)

struct nuc980_crypto_dev {
	struct device	*dev;
	struct nuc980_crypto_regs __iomem *regs;
    struct clk *crypto_clk;
	struct mutex	aes_lock;
	struct mutex	sha_lock;
	u8		*aes_inbuf;		/* AES input buffer                          */
	u32		aes_inbuf_size;		/* AES input buffer size                     */
	dma_addr_t	aes_inbuf_dma_addr;	/* physical address of AES input buffer      */
	u32		*aes_outbuf;		/* AES output buffer                         */
	u32		aes_outbuf_size;	/* AES output buffer size                    */
	dma_addr_t	aes_outbuf_dma_addr;	/* physical address of AES output buffer     */

	u8		*hmac_inbuf;		/* SHA/HAMC input buffer                     */
	u32		hmac_inbuf_size;	/* SHA/HMAC input buffer size                */
	dma_addr_t	hmac_inbuf_dma_addr;	/* physical address of SHA/HMAC input buffer */
	u32		sha_len;
};

#define ECC_CURVE_P_192		0x100192
#define ECC_CURVE_P_224		0x100224
#define ECC_CURVE_P_256		0x100256
#define ECC_CURVE_P_384		0x100384
#define ECC_CURVE_P_521		0x100521
#define ECC_CURVE_K_163		0x200163
#define ECC_CURVE_K_233		0x200233
#define ECC_CURVE_K_283		0x200283
#define ECC_CURVE_K_409		0x200409
#define ECC_CURVE_K_571		0x200571
#define ECC_CURVE_B_163		0x300163
#define ECC_CURVE_B_233		0x300233
#define ECC_CURVE_B_283		0x300283
#define ECC_CURVE_B_409		0x300409
#define ECC_CURVE_B_571		0x300571
#define ECC_CURVE_KO_192	0x400192
#define ECC_CURVE_KO_224	0x400224
#define ECC_CURVE_KO_256	0x400256
#define ECC_CURVE_BP_256	0x500256
#define ECC_CURVE_BP_384	0x500384
#define ECC_CURVE_BP_512	0x500512
#define ECC_CURVE_25519		0x025519

enum {
	CURVE_GF_P,
	CURVE_GF_2M,
};

#define CRYPTO_IOC_MAGIC	'C'

#define AES_IOC_SET_MODE	_IOW(CRYPTO_IOC_MAGIC,  1, unsigned long)
#define AES_IOC_SET_LEN		_IOW(CRYPTO_IOC_MAGIC,  2, unsigned long)
#define AES_IOC_GET_BUFSIZE     _IOW(CRYPTO_IOC_MAGIC,  3, unsigned long *)
#define AES_IOC_SET_IV		_IOW(CRYPTO_IOC_MAGIC,  5, unsigned long *)
#define AES_IOC_SET_KEY		_IOW(CRYPTO_IOC_MAGIC,  6, unsigned long *)
#define AES_IOC_START		_IOW(CRYPTO_IOC_MAGIC,  8, unsigned long)
#define AES_IOC_C_START		_IOW(CRYPTO_IOC_MAGIC,  9, unsigned long)
#define AES_IOC_UPDATE_IV	_IOW(CRYPTO_IOC_MAGIC, 11, unsigned long *)

#define SHA_IOC_INIT            _IOW(CRYPTO_IOC_MAGIC, 21, unsigned long)
#define SHA_IOC_FINISH		_IOW(CRYPTO_IOC_MAGIC, 25, unsigned long)

#define ECC_IOC_SEL_CURVE       _IOW(CRYPTO_IOC_MAGIC, 51, unsigned long)
#define ECC_IOC_SET_PRI_KEY     _IOW(CRYPTO_IOC_MAGIC, 52, unsigned char *)
#define ECC_IOC_SET_PUB_K1      _IOW(CRYPTO_IOC_MAGIC, 53, unsigned char *)
#define ECC_IOC_SET_PUB_K2      _IOW(CRYPTO_IOC_MAGIC, 54, unsigned char *)
#define ECC_IOC_SET_SCALAR_K    _IOW(CRYPTO_IOC_MAGIC, 55, unsigned char *)
#define ECC_IOC_SET_MSG         _IOW(CRYPTO_IOC_MAGIC, 56, unsigned char *)
#define ECC_IOC_SET_SIG_R       _IOW(CRYPTO_IOC_MAGIC, 57, unsigned char *)
#define ECC_IOC_SET_SIG_S       _IOW(CRYPTO_IOC_MAGIC, 58, unsigned char *)
#define ECC_IOC_GET_PUB_K1      _IOW(CRYPTO_IOC_MAGIC, 61, unsigned char *)
#define ECC_IOC_GET_PUB_K2      _IOW(CRYPTO_IOC_MAGIC, 62, unsigned char *)
#define ECC_IOC_GET_SIG_R       _IOW(CRYPTO_IOC_MAGIC, 63, unsigned char *)
#define ECC_IOC_GET_SIG_S       _IOW(CRYPTO_IOC_MAGIC, 64, unsigned char *)
#define ECC_IOC_GEN_PUB_KEY     _IOW(CRYPTO_IOC_MAGIC, 71, unsigned long)
#define ECC_IOC_ECDSA_SIGN      _IOW(CRYPTO_IOC_MAGIC, 72, unsigned long)
#define ECC_IOC_ECDSA_VERIFY    _IOW(CRYPTO_IOC_MAGIC, 73, unsigned long)
#define ECC_IOC_POINT_MUL       _IOW(CRYPTO_IOC_MAGIC, 81, unsigned long)

#define RSA_IOC_SET_BIT_LEN     _IOW(CRYPTO_IOC_MAGIC, 90, unsigned long)
#define RSA_IOC_SET_N           _IOW(CRYPTO_IOC_MAGIC, 91, unsigned char *)
#define RSA_IOC_SET_D           _IOW(CRYPTO_IOC_MAGIC, 92, unsigned char *)
#define RSA_IOC_SET_E           _IOW(CRYPTO_IOC_MAGIC, 93, unsigned char *)
#define RSA_IOC_SET_C           _IOW(CRYPTO_IOC_MAGIC, 94, unsigned char *)
#define RSA_IOC_SET_MSG         _IOW(CRYPTO_IOC_MAGIC, 95, unsigned char *)
#define RSA_IOC_GET_MSG         _IOW(CRYPTO_IOC_MAGIC, 96, unsigned char *)
#define RSA_IOC_GET_SIG         _IOW(CRYPTO_IOC_MAGIC, 97, unsigned char *)
#define RSA_IOC_SET_SIG         _IOW(CRYPTO_IOC_MAGIC, 98, unsigned char *)
#define RSA_IOC_DO_SIGN         _IOW(CRYPTO_IOC_MAGIC, 101, unsigned long)
#define RSA_IOC_DO_VERIFY       _IOW(CRYPTO_IOC_MAGIC, 102, unsigned long)

#endif /* __ASM_ARM_REGS_CRYPTO_H */
