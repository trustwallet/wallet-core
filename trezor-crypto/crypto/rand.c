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

#ifdef WIN32

#include <Windows.h>
#include <bcrypt.h>

static volatile LONG random_refc = 0;
static volatile BOOL random_lock = 0;
static volatile PVOID random_provider = NULL;

// [wallet-core]
void *random_init() {
    BCRYPT_ALG_HANDLE prov;
    while (InterlockedCompareExchange(&random_lock, 1, 0))
    {
        SwitchToThread();
    }
    LONG inc = InterlockedIncrement(&random_refc);
    if (inc == 1)
    {
        // Create new provider
        if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&prov, BCRYPT_RNG_ALGORITHM, NULL, 0))) {
            prov = NULL;
        }
        (void)InterlockedExchangePointer(&random_provider, prov);
    }
    else
    {
        // Get existing provider
        prov = InterlockedCompareExchangePointer(&random_provider, 0, 0);
    }
    if (!prov)
    {
        InterlockedDecrement(&random_refc);
    }
    (void)InterlockedExchange(&random_lock, 0);
    return prov;
}

// [wallet-core]
void random_release() {
    while (InterlockedCompareExchange(&random_lock, 1, 0))
    {
        SwitchToThread();
    }
    LONG dec = InterlockedDecrement(&random_refc);
    if (dec == 0)
    {
        BCryptCloseAlgorithmProvider(random_provider, 0);
        random_provider = NULL;
    }
    (void)InterlockedExchange(&random_lock, 0);
}

// [wallet-core]
uint32_t random32() {
    BCRYPT_ALG_HANDLE prov;
    uint32_t res;
    if (!(prov = random_init())) {
        return 0;
    }
    if (!BCRYPT_SUCCESS(BCryptGenRandom(prov, (PUCHAR)(&res), sizeof(res), 0))) {
        res = 0;
    }
    random_release();
    return res;
}

void random_buffer(uint8_t *buf, size_t len) {
    BCRYPT_ALG_HANDLE prov;
    if (!(prov = random_init())) {
        return;
    }
    for (size_t i = 0; i < len;)
    {
        size_t l = min(0x80000000UL, len);
        (void)BCryptGenRandom(prov, (PUCHAR)(buf + i), (ULONG)l, 0);
        i += l;
    }
    random_release();
}

#else

#include <sys/uio.h>
#include <unistd.h>

 // [wallet-core]
void *random_init() {
    return (void *)open; // return a valid pointer
}

// [wallet-core]
void random_release() {
    // no-op
}

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
