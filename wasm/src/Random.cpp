// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include <cstddef>
#include <cstdint>
#include <emscripten.h>

// clang-format off
static uint32_t
javascript_random(void)
{
    return EM_ASM_INT_V({
        return Module.getRandomValue();
    });
}

// https://github.com/jedisct1/libsodium/blob/master/src/libsodium/randombytes/randombytes.c#L53
static void
javascript_stir(void)
{
    EM_ASM({
        if (Module.getRandomValue === undefined) {
            try {
                var window_ = 'object' === typeof window ? window : self;
                var crypto_ = typeof window_.crypto !== 'undefined' ? window_.crypto : window_.msCrypto;
                var randomValuesStandard = function() {
                    var buf = new Uint32Array(1);
                    crypto_.getRandomValues(buf);
                    return buf[0] >>> 0;
                };
                randomValuesStandard();
                Module.getRandomValue = randomValuesStandard;
            } catch (e) {
                try {
                    var crypto = require('crypto');
                    var randomValueNodeJS = function() {
                        var buf = crypto['randomBytes'](4);
                        return (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]) >>> 0;
                    };
                    randomValueNodeJS();
                    Module.getRandomValue = randomValueNodeJS;
                } catch (e) {
                    throw 'No secure random number generator found';
                }
            }
        }
    });
}


static void
randombytes_init_if_needed(void)
{
    static bool initialized = false;
    if (!initialized) {
        javascript_stir();
        initialized = true;
    }
}

static void
javascript_buf(void * const buf, const size_t size)
{
    unsigned char *p = (unsigned char *) buf;
    size_t         i;

    for (i = (size_t) 0U; i < size; i++) {
        p[i] = (unsigned char) javascript_random();
    }
}
// clang-format on

extern "C" {
uint32_t random32(void) {
    randombytes_init_if_needed();
    return javascript_random();
}

void random_buffer(uint8_t* buf, size_t len) {
    randombytes_init_if_needed();
    javascript_buf(buf, len);
    return;
}

} // extern "C"
