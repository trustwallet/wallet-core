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

#include <TrezorCrypto/rand.h>

#include <fcntl.h>
#include <sys/types.h>

#ifdef _MSC_VER

#if 1

#include <Windows.h>
#include <bcrypt.h>

#pragma comment(lib, "Bcrypt")

uint32_t random32() {
    BCRYPT_ALG_HANDLE prov;
    uint32_t res;
    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&prov, BCRYPT_RNG_ALGORITHM, NULL, 0))) {
        return 0;
    }
    if (!BCRYPT_SUCCESS(BCryptGenRandom(prov, (PUCHAR)(&res), sizeof(res), 0))) {
        res = 0;
    }
    BCryptCloseAlgorithmProvider(prov, 0);
    return res;
}

void random_buffer(uint8_t *buf, size_t len) {
    BCRYPT_ALG_HANDLE prov;
    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&prov, BCRYPT_RNG_ALGORITHM, NULL, 0))) {
        return;
    }
    (void)BCryptGenRandom(prov, (PUCHAR)buf, len, 0);
    BCryptCloseAlgorithmProvider(prov, 0);
}

#else

// TODO: Use a secure random number generator
// https://docs.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom
// https://docs.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptgenrandom
// Assumes seed() has been called

uint32_t random32() {
    uint32_t res = 0;
    res = rand() & 0xFF;
    res <<= 8;
    res = rand() & 0xFF;
    res <<= 8;
    res = rand() & 0xFF;
    res <<= 8;
    res = rand() & 0xFF;
    return res;
}

void random_buffer(uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() & 0xFF;
    }
}

#endif

#else

#include <sys/uio.h>
#include <unistd.h>

// [wallet-core]
uint32_t __attribute__((weak)) random32() {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        return 0;
    }

    uint32_t result;
    if (read(randomData, &result, sizeof(result)) < 0) {
        return 0;
    }

    close(randomData);

    return result;
}

void __attribute__((weak)) random_buffer(uint8_t *buf, size_t len) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        return;
    }
    if (read(randomData, buf, len) < 0) {
        return;
    }
    close(randomData);
}

#endif
