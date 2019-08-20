// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "NULS/Address.h"

#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "DerivationPath.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::NULS;

// https://github.com/kiss1987f4/blockbook/blob/master/bchain/coins/nuls/nulsparser_test.go#L347

TEST(NULSAddress, DeriveKeyFromXPrv) {
    const std::string xprv = "xprv9yEvwSfPanK5gLYVnYvNyF2CEWJx1RsktQtKDeT6jnCnqASBiPCvFYHFSApXv39bZbF6hRaha1kWQBVhN1xjo7NHuhAn5uUfzy79TBuGiHh";
    const std::string privateKeyList[5] = {
        "995c98115809359eb57a5e179558faddd55ef88f88e5cf58617a5f9f3d6bb3a1",
        "0f65dee42d3c974c1a4bcc79f141be89715dc8d6406faa9ad4f1f55ca95fabc8",
        "6fd98d1d9c3f3ac1ff61bbf3f20e89f00ffa8d43a554f2a7d73fd464b6666f45",
        "21412d9e33aba493faf4bc7d408ed5290bea5b36a7beec554b858051f8d4bff3",
        "dc3d290e32a4e0f38bc26c25a78ceb1c8779110883d9cb0be54629043c1f8724"
    };

    const std::string publicKeyList[5] = {
        "028855d37e8b1d2760289ea51996df05f3297d86fae4e113aea696a0f02a420ae2",
        "0216f460ea59194464a6c981560e3f52899203496ed8a20f8f9a57a9225d841293",
        "0327ef15c2eaf99365610d6ef89d9ad1e89d1ddf888fc0ec7eb8a94d97153ee482",
        "02a73aebd08c6f70fa97f616b1c0b63756efe9eb070a14628de3d850b2b970a9a7",
        "02f87eb70b985a857d7238bc9423dab7d5930f3fcfc2118ccac0634a9342b9d324"
    };

    for (int i = 0; i < 5; ++i) {
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv,DerivationPath(TWPurposeBIP44,TWCoinTypeNULS,0,0,i));
        ASSERT_EQ(hex(privateKey->bytes),privateKeyList[i]);
        auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKey.bytes),publicKeyList[i]);
    }
}

TEST(NULSAddress,DeriveAddressFromXPrv) {
    auto xprv = "xprv9yEvwSfPanK5gLYVnYvNyF2CEWJx1RsktQtKDeT6jnCnqASBiPCvFYHFSApXv39bZbF6hRaha1kWQBVhN1xjo7NHuhAn5uUfzy79TBuGiHh";

    const std::string addressList[5]={"NsdtwhD8hb8H72J7FyQpGta2sqLngrXZ",
                                "Nse51sBAzRTVtm48wYQLb4TH7MGAHAER",
                                "NsdvoFSwfh1oW238SFM6p5wL4J834Gv2",
                                "Nse4wVWsJ4v3jPcpE4vRkAiZLFyQSNKd",
                                "Nse5NzUcZybsvFQeNgqfuWmmmwCfhdxF"};

    for (int i = 0; i < 5; ++i) {
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv,DerivationPath(TWPurposeBIP44,TWCoinTypeNULS,0,0,i));
        auto address = Address(privateKey->getPublicKey(TWPublicKeyTypeSECP256k1));
        ASSERT_EQ(addressList[i],address.string());
    }
}

TEST(NULSAddress, DeriveAddressFromXPub) {
    auto xpub = "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw";

    const std::string addressList[5]={"NsdtwhD8hb8H72J7FyQpGta2sqLngrXZ",
                                "Nse51sBAzRTVtm48wYQLb4TH7MGAHAER",
                                "NsdvoFSwfh1oW238SFM6p5wL4J834Gv2",
                                "Nse4wVWsJ4v3jPcpE4vRkAiZLFyQSNKd",
                                "Nse5NzUcZybsvFQeNgqfuWmmmwCfhdxF"};

    for (int i = 0; i < 5; ++i) {
        auto publicKey = HDWallet::getPublicKeyFromExtended(xpub,DerivationPath(TWPurposeBIP44,TWCoinTypeNULS,0,0,i));
        auto address = Address(*publicKey);
        ASSERT_EQ(addressList[i],address.string());
    }
}
