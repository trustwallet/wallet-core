// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

// Taken from https://github.com/jedisct1/libsodium/blob/stable/src/libsodium/sodium/utils.c

#pragma once

#include <stddef.h>

#ifdef HAVE_WEAK_SYMBOLS
__attribute__((weak)) void
_sodium_dummy_symbol_to_prevent_memcmp_lto(const unsigned char *b1,
                                           const unsigned char *b2,
                                           const size_t         len);
__attribute__((weak)) void
_sodium_dummy_symbol_to_prevent_memcmp_lto(const unsigned char *b1,
                                           const unsigned char *b2,
                                           const size_t         len)
{
    (void) b1;
    (void) b2;
    (void) len;
}
#endif


int
sodium_memcmp(const void *const b1_, const void *const b2_, size_t len)
{
#ifdef HAVE_WEAK_SYMBOLS
    const unsigned char *b1 = (const unsigned char *) b1_;
    const unsigned char *b2 = (const unsigned char *) b2_;
#else
    const volatile unsigned char *volatile b1 =
        (const volatile unsigned char *volatile) b1_;
    const volatile unsigned char *volatile b2 =
        (const volatile unsigned char *volatile) b2_;
#endif
    size_t                 i;
    volatile unsigned char d = 0U;

#if HAVE_WEAK_SYMBOLS
    _sodium_dummy_symbol_to_prevent_memcmp_lto(b1, b2, len);
#endif
    for (i = 0U; i < len; i++) {
        d |= b1[i] ^ b2[i];
    }
    return (1 & ((d - 1) >> 8)) - 1;
}
