// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "gtest/gtest.h"
#include "TWTestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

TEST(PublicKeyTests, Compressed) {
    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto privateKeyData = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(privateKeyData.get()));
    auto publicKey = TWPrivateKeyGetPublicKey(privateKey.get(), false);

    uint8_t expected[] = {0x03, 0x99, 0xc6, 0xf5, 0x1a, 0xd6, 0xf9, 0x8c, 0x9c, 0x58, 0x3f, 0x8e, 0x92, 0xbb, 0x77, 0x58, 0xab, 0x2c, 0xa9, 0xa0, 0x41, 0x10, 0xc0, 0xa1, 0x12, 0x6e, 0xc4, 0x3e, 0x54, 0x53, 0xd1, 0x96, 0xc1};
    auto compressed = TWPublicKeyCompressed(publicKey);

    for (auto i = 0; i < TWPublicKeyCompressedSize; i += 1) {
        ASSERT_EQ(expected[i], compressed.bytes[i]);
    }
}

TEST(PublicKeyTests, Verify) {
    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto privateKeyData = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(privateKeyData.get()));
    
    const char *message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t *)message, strlen(message)));
    auto digest = WRAPD(TWHashKeccak256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get()));

    auto publicKey = TWPrivateKeyGetPublicKey(privateKey.get(), false);
    ASSERT_TRUE(TWPublicKeyVerify(publicKey, signature.get(), digest.get()));
}
