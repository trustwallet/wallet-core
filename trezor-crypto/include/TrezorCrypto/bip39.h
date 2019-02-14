/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BIP39_H__
#define __BIP39_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIP39_PBKDF2_ROUNDS 2048

/// Generates a random mnemonic phrase with the given strength in bits.
///
/// \param strength mnemonic strength in bits. Must be a multiple of 32 between 128 and 256.
/// \param menmonic [out] mnemonic phrase, must have capacity for 240 bytes.
/// \returns whether a mnemonic phrase was generated.
bool mnemonic_generate(int strength, char* mnemonic);

/// Generates a mnemonic phrase from the provided data.
///
/// \param data array of data bytes.
/// \param len data array size.
/// \param mnemonic [out] mnemonic phrase, must have capacity for 240 bytes.
/// \returns whether a mnemonic phrase was generated.
bool mnemonic_from_data(const uint8_t *data, size_t len, char* mnemonic);

int mnemonic_check(const char *mnemonic);

int mnemonic_to_entropy(const char *mnemonic, uint8_t *entropy);

// passphrase must be at most 256 characters otherwise it would be truncated
void mnemonic_to_seed(const char *mnemonic, const char *passphrase, uint8_t seed[512 / 8], void (*progress_callback)(uint32_t current, uint32_t total));

const char * const *mnemonic_wordlist(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
