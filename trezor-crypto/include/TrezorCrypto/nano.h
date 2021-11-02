/**
 * Copyright (c) 2019 Mart Roosmaa
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

#ifndef __NANO_H__
#define __NANO_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <TrezorCrypto/ed25519.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char *BASE32_ALPHABET_NANO;

size_t nano_get_address(
    const ed25519_public_key public_key,
    const char *prefix,
    const size_t prefix_len,
    char *out,
    size_t out_len);

bool nano_validate_address(
    const char *prefix,
    const size_t prefix_len,
    const char *address,
    const size_t address_len,
    ed25519_public_key out_public_key);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif