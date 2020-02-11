// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Encryption.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

#include <stdlib.h>

using namespace TW;
using namespace TW::FIO;

TEST(FIOEncryption, checkEncrypt) {
    // test derived from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-check.test.ts
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data plaintext = TW::data("secret message");
    Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");
        
    Data result = Encryption::checkEncrypt(secret, plaintext, iv);
    EXPECT_EQ(hex(result), "f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab");
}

TEST(FIOEncryption, checkDecrypt) {
    // test derived from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-check.test.ts
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data encrypted = parse_hex("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab");
    const Data expectedPlaintext = TW::data("secret message");
        
    Data result = Encryption::checkDecrypt(secret, encrypted);
    EXPECT_EQ(hex(result), hex(expectedPlaintext));
}

TEST(FIOEncryption, checkEncryptInvalidIvLength) {
    try {
        Data iv = Data(5); // wrong size
        Encryption::checkEncrypt(Data(64), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(FIOEncryption, checkDecryptInvalidMessageHMAC) {
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data encrypted = parse_hex("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab00");
    try {       
        Encryption::checkDecrypt(secret, encrypted);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(FIOEncryption, checkDecryptMessageTooShort) {
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    try {       
        Encryption::checkDecrypt(secret, Data(60));
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

Data randomBuffer(size_t size) {
    Data d(size);
    for (auto i = 0; i < size; ++i) {
        d[i] = (byte)(256.0 * rand() / RAND_MAX);
    }
    return d;
}

void checkEncryptDecryptRandomOne(size_t messageLen) {
    // Encrypt, decrypt, and obtain the same
    // random (but reproducible) input
    Data secret = randomBuffer(64);
    Data message = randomBuffer(messageLen);
    Data iv = randomBuffer(16);

    Data encrypted = Encryption::checkEncrypt(secret, message, iv);
    Data decrypted = Encryption::checkDecrypt(secret, encrypted);

    EXPECT_EQ(message.size(), decrypted.size());
}

TEST(FIOEncryption, checkEncryptDecryptRandom) {
    // Encrypt, decrypt, and obtain the same
    // Random, but reproducible input
    srand(1);
    for (auto i = 0; i < 5; ++i) {
        // random message length
        size_t messageLen = 1 + (rand() % 60);
        checkEncryptDecryptRandomOne(messageLen);
    }
    // some even-block lengths
    checkEncryptDecryptRandomOne(16);
    checkEncryptDecryptRandomOne(32);
    checkEncryptDecryptRandomOne(64);
}
