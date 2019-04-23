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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, E1PRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <TrezorCrypto/nano.h>
#include <TrezorCrypto/blake2b.h>
#include <TrezorCrypto/base32.h>

#include <string.h>

const char *BASE32_ALPHABET_NANO = "13456789abcdefghijkmnopqrstuwxyz";

#define NANO_ADDRESS_BASE_LENGTH 60
#define NANO_CHECKSUM_LEN 5

typedef union {
    uint8_t bytes[40];
    struct {
        uint8_t padding[3];
        ed25519_public_key public_key;
        uint8_t checksum[NANO_CHECKSUM_LEN];
    } data;
} nano_address_raw;

typedef union {
    char chars[65];
    struct {
        char padding[4]; // 1111
        char address[NANO_ADDRESS_BASE_LENGTH];
        char terminator; // \0
    } data;
} nano_address_encoded;

size_t nano_get_address(
    const ed25519_public_key public_key,
    const char *prefix,
    const size_t prefix_len,
    char *out,
    size_t out_len
) {
    if (out_len < prefix_len + NANO_ADDRESS_BASE_LENGTH + 1) {
        return 0;
    }

    // Construct raw address which is going to be base32 encoded
    nano_address_raw raw;
    memset(&raw, 0, sizeof(nano_address_raw));

    uint8_t checksum[NANO_CHECKSUM_LEN];
    blake2b_state hash;
    blake2b_Init(&hash, NANO_CHECKSUM_LEN);
    blake2b_Update(&hash, public_key, sizeof(ed25519_public_key));
    blake2b_Final(&hash, checksum, NANO_CHECKSUM_LEN);

    for (int i = 0; i < NANO_CHECKSUM_LEN; i++) {
        raw.data.checksum[NANO_CHECKSUM_LEN - (i + 1)] = checksum[i];
    }
    memcpy(raw.data.public_key, public_key, sizeof(ed25519_public_key));

    // Encode the address into a buffer and compose the final output
    nano_address_encoded encoded;
    memset(&encoded, 0, sizeof(nano_address_encoded));
    char *ret = base32_encode(
        raw.bytes, sizeof(nano_address_raw),
        encoded.chars, sizeof(encoded.chars),
        BASE32_ALPHABET_NANO);
    if (ret == NULL) {
        return 0;
    }

    size_t w = 0;
    memcpy(&out[w], prefix, prefix_len);
    w += prefix_len;
    memcpy(&out[w], encoded.data.address, NANO_ADDRESS_BASE_LENGTH);
    w += NANO_ADDRESS_BASE_LENGTH;
    out[w] = 0;
    w += 1;
    return w;
}

bool nano_validate_address(
    const char *prefix,
    const size_t prefix_len,
    const char *address,
    const size_t address_len,
    ed25519_public_key out_public_key
) {
    if (address_len != prefix_len + NANO_ADDRESS_BASE_LENGTH) {
        return false;
    }

    // Validate that the prefix matches
    if (memcmp(address, prefix, prefix_len) != 0) {
        return false;
    }

    // Try to decode the address
    nano_address_encoded encoded;
    memcpy(encoded.data.padding, "1111", sizeof(encoded.data.padding));
    memcpy(encoded.data.address, &address[prefix_len], NANO_ADDRESS_BASE_LENGTH);
    encoded.data.terminator = '\0';

    nano_address_raw raw;
    memset(&raw, 0, sizeof(nano_address_raw));
    uint8_t *ret = base32_decode(
        encoded.chars, strlen(encoded.chars),
        raw.bytes, sizeof(nano_address_raw),
        BASE32_ALPHABET_NANO
    );
    if (ret == NULL) {
        return false;
    }

    // Validate the checksum
    uint8_t checksum[NANO_CHECKSUM_LEN];
    blake2b_state hash;
    blake2b_Init(&hash, NANO_CHECKSUM_LEN);
    blake2b_Update(&hash, raw.data.public_key, sizeof(ed25519_public_key));
    blake2b_Final(&hash, checksum, NANO_CHECKSUM_LEN);

    for (int i = 0; i < NANO_CHECKSUM_LEN; i++) {
        if (raw.data.checksum[NANO_CHECKSUM_LEN - (i + 1)] != checksum[i]) {
            return false;
        }
    }

    // Output the public key if the caller is interested in it
    if (out_public_key != NULL) {
        memcpy(out_public_key, raw.data.public_key, sizeof(ed25519_public_key));
    }

    return true;
}
