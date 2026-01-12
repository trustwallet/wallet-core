// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@import Security;

int random32(uint32_t *result) {
    if (SecRandomCopyBytes(kSecRandomDefault, sizeof(uint32_t), &result) != errSecSuccess) {
        // failed to generate random number
        return -1;
    }
    return 0;
}

int random_buffer(uint8_t *buf, size_t len) {
    if (SecRandomCopyBytes(kSecRandomDefault, len, buf) != errSecSuccess) {
        return -1;
    }
    return 0;
}
