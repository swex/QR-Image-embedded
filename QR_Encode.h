/*
 * QR_Encode.h
 *
 *  Created on: Jan 18, 2012
 *      Author: swex
 */

#ifndef _QR_ENCODE_H_
#define _QR_ENCODE_H_

/* Error correction level */
#define QR_LEVEL_L (0)
#define QR_LEVEL_M (1)
#define QR_LEVEL_Q (2)
#define QR_LEVEL_H (3)

/* Data Mode */
#define QR_MODE_NUMERAL  (0)
#define QR_MODE_ALPHABET (1)
#define QR_MODE_8BIT     (2)
#define QR_MODE_KANJI    (3)

/* Group version (model number) */
#define QR_VERSION_S (0)	/* 1 ~ 9 */
#define QR_VERSION_M (1)	/* 10 ~ 26 */
#define QR_VERSION_L (2)	/* 27 ~ 40 */

/* Sizes */
#define MAX_ALLCODEWORD  (3706)	/* The maximum total number of code words */
#define MAX_DATACODEWORD (2956)	/* Maximum data word code (version 40-L) */
#define MAX_CODEBLOCK    (153)	/* (Including RS code word) the maximum number
                              	   of block data code word */
#define MAX_MODULESIZE   (177)	/* Maximum number of modules in a side */
#define MAX_BITDATA      (3917)	/* finfile data */

int EncodeData(int, int, const char *, int, unsigned char *);
/* See README.md for usage. */

#endif
