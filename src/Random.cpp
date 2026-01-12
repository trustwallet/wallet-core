// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// Originally from Trezor Crypto:
// Copyright (c) 2013-2014 Tomas Dzetkulic
// Copyright (c) 2013-2014 Pavol Rusnak

#include <TrezorCrypto/rand.h>

#include <fcntl.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

extern "C" {

// Weak implementations that can be overridden by platform-specific versions:
// - wasm/src/Random.cpp (for WebAssembly builds)
// - jni/cpp/Random.cpp (for Android/JNI builds)
// - swift/Sources/SecRandom.m (for iOS/Swift builds)

uint32_t __attribute__((weak)) random32(void) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        throw std::runtime_error("Random device not available");
    }

    uint32_t result;
    if (read(randomData, &result, sizeof(result)) < 0) {
        close(randomData);
        throw std::runtime_error("Failed to read random data");
    }

    close(randomData);
    return result;
}

void __attribute__((weak)) random_buffer(uint8_t *buf, size_t len) {
    int randomData = open("/dev/urandom", O_RDONLY);
    if (randomData < 0) {
        throw std::runtime_error("Random device not available");
    }
    if (read(randomData, buf, len) < 0) {
        close(randomData);
        throw std::runtime_error("Failed to read random data");
    }
    close(randomData);
}

} // extern "C"
