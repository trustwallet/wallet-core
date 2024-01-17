// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <StarkEx/MessageSigner.h>
#include <HexCoding.h>
#include <TrustWalletCore/TWStarkExMessageSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::StarkEx::tests {

TEST(StarkExMessageSigner, SignAndVerify) {
    PrivateKey starkPrivKey(parse_hex("04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de", true));
    auto starkPubKey  = starkPrivKey.getPublicKey(TWPublicKeyTypeStarkex);
    auto starkMsg = "463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf";
    auto starkSignature = StarkEx::MessageSigner::signMessage(starkPrivKey, starkMsg);
    ASSERT_EQ(starkSignature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528");
    ASSERT_TRUE(StarkEx::MessageSigner::verifyMessage(starkPubKey, starkMsg, starkSignature));
}

TEST(TWStarkExMessageSigner, SignAndVerify) {
    const auto privKeyData = "04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de";
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
    const auto message = STRING("463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf");
    
    const auto pubKey = TWPrivateKeyGetPublicKeyByType(privateKey.get(), TWPublicKeyTypeStarkex);
    const auto signature = WRAPS(TWStarkExMessageSignerSignMessage(privateKey.get(), message.get()));
    EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528");
    EXPECT_TRUE(TWStarkExMessageSignerVerifyMessage(pubKey, message.get(), signature.get()));
    delete pubKey;
}

}
