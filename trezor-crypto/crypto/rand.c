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
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

// getentropy() is available on:
// - macOS 10.12+
// - OpenBSD 5.6+
// - FreeBSD 12.0+
// - Linux with glibc 2.25+ (2017)
#if defined(__APPLE__) || defined(__OpenBSD__) || defined(__FreeBSD__) || \
    (defined(__linux__) && defined(__GLIBC__) && \
     (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 25)))
#define HAVE_GETENTROPY
#endif

// On Linux, getentropy() is declared in sys/random.h
// On Apple/BSD, it's declared in unistd.h (already included above)
#if defined(HAVE_GETENTROPY) && defined(__linux__)
#include <sys/random.h>
#endif

// [wallet-core]
// Helper function to read random data using getentropy() if available, otherwise /dev/urandom
static int read_random(uint8_t *buf, size_t len) {
#ifdef HAVE_GETENTROPY
    // getentropy() has a maximum buffer size of 256 bytes
    // If we need more, we must call it multiple times
    size_t offset = 0;
    while (offset < len) {
        size_t chunk = len - offset;
        if (chunk > 256) {
            chunk = 256;
        }

        if (getentropy(buf + offset, chunk) != 0) {
            // getentropy() failed, fall back to /dev/urandom
            // This should rarely happen (only if RNG is not initialized)
            break;
        }

        offset += chunk;
    }

    // If we successfully read all data, return success
    if (offset == len) {
        return 0;
    }

    // If getentropy() failed, fall through to /dev/urandom
#endif

    // Fallback to /dev/urandom
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        return randomData;
    }

    size_t ret = read(randomData, buf, len);
    close(randomData);

    if (ret != len) {
        return -1;
    }
    return 0;
}

int __attribute__((weak)) random32(uint32_t *result) {
    return read_random((uint8_t *)result, sizeof(uint32_t));
}

int __attribute__((weak)) random_buffer(uint8_t *buf, size_t len) {
    return read_random(buf, len);
}

