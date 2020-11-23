// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@import Security;

uint32_t random32(void) {
    uint32_t value;
    if (SecRandomCopyBytes(kSecRandomDefault, sizeof(value), &value) != errSecSuccess) {
        // failed to generate random number
        abort();
    }
    return value;
}

void random_buffer(uint8_t *buf, size_t len) {
    if (SecRandomCopyBytes(kSecRandomDefault, len, buf) != errSecSuccess) {
        // failed to generate random number
        abort();
    }
}
