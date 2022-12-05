// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <PrivateKey.h>
#include <HexCoding.h>
#include <Ethereum/EIP191.h>
#include <TrustWalletCore/TWEthereumMessageSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Ethereum {
    TEST(EthereumEip191, signMessageAndVerify) {
        PrivateKey ethKey(parse_hex("3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84"));
        auto msg = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3";
        auto signature = Ethereum::MessageSigner::signMessage(ethKey, msg);
        ASSERT_EQ(signature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(TWEthereumMessageSigner, SignAndVerify) {
        const auto privKeyData = "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3");

        const auto pubKey = TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum);
        const auto signature = WRAPS(TWEthereumMessageSignerSignMessage(privateKey.get(), message.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey, message.get(), signature.get()));
        delete pubKey;
    }
}
