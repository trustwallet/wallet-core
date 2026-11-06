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

#include <stdlib.h>

#if defined(_WIN32)
#include <limits.h>
#include <windows.h>
#include <bcrypt.h>

void random_buffer(uint8_t *buf, size_t len) {
    while (len != 0) {
        const ULONG count = len > ULONG_MAX ? ULONG_MAX : (ULONG)len;
        if (BCryptGenRandom(NULL, buf, count, BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0) {
            abort();  // Critical: cannot proceed without a secure random source
        }
        buf += count;
        len -= count;
    }
}

uint32_t random32(void) {
    uint32_t result;
    random_buffer((uint8_t *)&result, sizeof(result));
    return result;
}
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// [wallet-core]
uint32_t __attribute__((weak)) random32(void) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        abort();  // Critical: cannot proceed without random source
    }

    uint32_t result;
    ssize_t readLen = read(randomData, &result, sizeof(result));
    close(randomData);
    if (readLen != sizeof(result)) {
        abort();  // Critical: failed to read random data
    }

    return result;
}

void __attribute__((weak)) random_buffer(uint8_t *buf, size_t len) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        abort();  // Critical: cannot proceed without random source
    }
    ssize_t readLen = read(randomData, buf, len);
    close(randomData);
    if (readLen != (ssize_t)len) {
        abort();  // Critical: failed to read random data
    }
}
#endif
