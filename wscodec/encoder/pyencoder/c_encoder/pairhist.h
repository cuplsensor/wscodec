/*!
 * @file pairhist.h
 * @brief This maintains a circular buffer named #pairhistory, which is stored in RAM. It contains all pairs that are
 * in the NDEF message circular buffer that is stored on the NFC-readable EEPROM.
 *
 * Its purpose is to allow a hash to be taken of the unencoded circular buffer contents. The decoder uses this hash
 * to verify that it has decoded the circular buffer properly; outputting the same samples in the same order.
 *
 * The <a href="https://en.wikipedia.org/wiki/MD5">MD5</a> algorithm is used by default.
 * It is simple enough to be used on a microcontroller with limited resources. As an option,
 * the <a href="https://en.wikipedia.org/wiki/HMAC">HMAC-MD5</a> hash can be used instead.
 *
 * When a pair is added or changed in the NDEF message, then the same change must be made to pairhistory.
 */

#ifndef _PAIRHIST_H_
#define _PAIRHIST_H_

#include <stdbool.h>


/**
 *  Structure to hold one sample consisting of two 12-bit readings.
 */
typedef struct
{
        unsigned char rd0Msb;    /*!< Reading0 Most significant byte. */
        unsigned char rd1Msb;    /*!< Reading1 Most significant byte. */
        unsigned char Lsb;      /*!< Least significant 4-bit nibbles of reading0 and reading1. */
} pair_t;

/**
 *  Structure to hold hash and npairs as per CODEC_SPEC_14.
 */
typedef struct
{
    unsigned char hash[7];      /*!< Last 7 bytes of the MD5 or HMAC-MD5 hash. */
    unsigned char npairs[2];    /*!< Number of valid pairs in the circular buffer. */
} hashn_t;

/// Have doxygen ignore this
/// @cond
int pairhist_ovr(pair_t sample);
int pairhist_push(pair_t sample);
hashn_t pairhist_hash(int npairs, int usehmac, unsigned int loopcount, unsigned int resetsalltime, unsigned int batv_resetcause, int cursorpos);
pair_t pairhist_read(unsigned int index, int * error);

#ifndef NOT_CFFI
const int buflenpairs;
#endif
/// @endcond

#endif //_PAIRHIST_H_
