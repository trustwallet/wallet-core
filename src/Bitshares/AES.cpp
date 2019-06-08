// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AES.h"

#include <TrezorCrypto/aes.h>

using namespace TW::Bitshares;
using Data = TW::Data;

Data TW::Bitshares::aesEncrypt(const uint8_t *message, size_t messageLength, const uint8_t *key, const uint8_t *initializationVector) {
    if (messageLength > static_cast<size_t>(INT_MAX)) {
        throw std::invalid_argument("Message size must be smaller than " + std::to_string(INT_MAX));
    }

    // create context
    aes_encrypt_ctx context;
    if (aes_encrypt_key256(key, &context) == EXIT_FAILURE) {
        throw std::runtime_error("Encryption error: Error initializing the key");
    }

    size_t fullBlockBytes = (messageLength / 16) * 16;        // no. of bytes that make up the full 16-byte blocks
    size_t remainingBytes = messageLength % 16;

    size_t outputSize = fullBlockBytes + 16;
    Data output(outputSize);

    // create a non-const copy of the iv
    Data iv(initializationVector, initializationVector + 16);

    // encrypt the full blocks at a go
    if (fullBlockBytes) {
        if (aes_cbc_encrypt(message, output.data(), static_cast<int>(fullBlockBytes), iv.data(), &context) == EXIT_FAILURE) {
            throw std::runtime_error("Encryption error: Error encrypting the message");
        }
    }

    // create a __PKCS#5-padded__ buffer for the remaining bytes and encrypt that too
    Data lastBlock(16, static_cast<unsigned char>(16 - remainingBytes));
    std::memcpy(lastBlock.data(), message + fullBlockBytes, remainingBytes);
    if (aes_cbc_encrypt(lastBlock.data(), output.data() + fullBlockBytes, 16, iv.data(), &context) == EXIT_FAILURE) {
        throw std::runtime_error("Encryption error: Error encrypting the message");
    }

    return output;
}