// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <Tron/MessageSigner.h>
#include <TrustWalletCore/TWTronMessageSigner.h>
#include <HexCoding.h>
#include <PrivateKey.h>

#include <gtest/gtest.h>

namespace TW::Tron {
    TEST(TronMessageSigner, SignMessageAndVerify) {
        PrivateKey tronKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
        auto msg = "Hello World";
        auto signature = Tron::MessageSigner::signMessage(tronKey, msg);
        auto pubKey = tronKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

        ASSERT_EQ(signature, "bc0753c070cc55693097df11bc11e1a7c4bd5e1a40b9dc94c75568e59bcc9d6b50a7873ef25b469e494490a54de37327b4bc7fc825c81a377b555e34fb7261ba1c");
        ASSERT_TRUE(Tron::MessageSigner::verifyMessage(pubKey, msg, signature));

        auto msg2 = "A much longer message to test the signing and verification process";
        auto signature2 = Tron::MessageSigner::signMessage(tronKey, msg2);

        ASSERT_EQ(signature2, "93aee5f753cf889e0749c74dd0c5996cce889883ae079e09ede462e16d65d06a4f43d1ed2745e9f3c1690695628269bd58f057a4a93953cc50e66b4a05bc0f451b");
        ASSERT_TRUE(Tron::MessageSigner::verifyMessage(pubKey, msg2, signature2));
    }
    
    TEST(TWTronMessageSigner, SignAndVerifyLegacy) {
        const auto privKeyData = "75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Hello World");
        
        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeTron));
        const auto signature = WRAPS(TWTronMessageSignerSignMessage(privateKey.get(), message.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "bc0753c070cc55693097df11bc11e1a7c4bd5e1a40b9dc94c75568e59bcc9d6b50a7873ef25b469e494490a54de37327b4bc7fc825c81a377b555e34fb7261ba1c");
        EXPECT_TRUE(TWTronMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));

        const auto message2 = STRING("A much longer message to test the signing and verification process");
        const auto signature2 = WRAPS(TWTronMessageSignerSignMessage(privateKey.get(), message2.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature2.get())), "93aee5f753cf889e0749c74dd0c5996cce889883ae079e09ede462e16d65d06a4f43d1ed2745e9f3c1690695628269bd58f057a4a93953cc50e66b4a05bc0f451b");
        EXPECT_TRUE(TWTronMessageSignerVerifyMessage(pubKey.get(), message2.get(), signature2.get()));
    }
}
