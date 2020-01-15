/*
 * QR_Encode.h
 *
 *  Created on: Jan 18, 2012
 *      Author: swex
 */

#ifndef _QR_ENCODE_H_
#define _QR_ENCODE_H_
#include <stddef.h> //just for size_t
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Error correction level */
typedef enum {
    QR_LEVEL_L = (0),
    QR_LEVEL_M = (1),
    QR_LEVEL_Q = (2),
    QR_LEVEL_H = (3)
} QR_Level;

/* Data Mode */
typedef enum {
    QR_MODE_NUMERAL = (0),
    QR_MODE_ALPHABET = (1),
    QR_MODE_8BIT = (2),
    QR_MODE_KANJI = (3),
} QR_DataMode;

/* Mask pattern */
typedef enum {
    QR_MaskAuto = -1, //calculate penalty and take the best mask (takes longer)
    QR_Mask1 = 0, //(row + column) mod 2 == 0
    QR_Mask2, // (row) mod 2 == 0
    QR_Mask3, // (column) mod 3 == 0
    QR_Mask4, // (row + column) mod 3 == 0
    QR_Mask5, // ( floor(row / 2) + floor(column / 3) ) mod 2 == 0
    QR_Mask6, // ((row * column) mod 2) + ((row * column) mod 3) == 0
    QR_Mask7, // ( ((row * column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
    QR_Mask8, // ( ((row + column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
} QR_MaskPattern;

/*Encoding errors*/
typedef enum {
    QR_E_EmptyInput = -1,
    QR_E_InvalidVersion = -2,
    QR_E_VersionNotFit = -3,
} QR_EncodeError;

/* Sizes */
#define MAX_ALLCODEWORD (3706) /* The maximum total number of code words */
#define MAX_DATACODEWORD (2956) /* Maximum data word code (version 40-L) */
#define MAX_CODEBLOCK (153) /* (Including RS code word) the maximum number \
                               of block data code word */
#define MAX_MODULESIZE (177) /* Maximum number of modules in a side */
#define MAX_BITDATA (3917) /* finfile data */

/**
 * @brief EncodeData encodes data into QR code
 * @param level One of QR_LEVEL_{L,M,Q,H}.
 * @param version 0 for auto detect or required output version.
 * @param maskPattern Choose concrete mask pattern or use autodetect
 * @param data_in A pointer to the input data to encode.
 * @param in_size Size of data pointed to by data_in. If set to 0, the function will interpret source as a NUL terminated string.
 * @param output Pointer to storage of at MAX_BITDATA bytes in which to store produced data.
 * @return <0 on error @see QR_EncodeError or width of the produced QR code
 */
int EncodeData(QR_Level level, int version, QR_MaskPattern maskPattern, const char* data_in, size_t in_size, unsigned char* output);
/* See README.md for usage. */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _QR_ENCODE_H_ */
