// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        ASSERT_EQ(signature, "9bb6d11ec8a6a3fb686a8f55b123e7ec4e9746a26157f6f9e854dd72f5683b450397a7b0a9653865658de8f9243f877539882891bad30c7286c3bf5622b900471b");
        auto pubKey = tronKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Tron::MessageSigner::verifyMessage(pubKey, msg, signature));
    }
    
    TEST(TWTronMessageSigner, SignAndVerifyLegacy) {
        const auto privKeyData = "75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Hello World");
        
        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeTron));
        const auto signature = WRAPS(TWTronMessageSignerSignMessage(privateKey.get(), message.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "9bb6d11ec8a6a3fb686a8f55b123e7ec4e9746a26157f6f9e854dd72f5683b450397a7b0a9653865658de8f9243f877539882891bad30c7286c3bf5622b900471b");
        EXPECT_TRUE(TWTronMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));
    }
}
