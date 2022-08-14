// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"
#include "HexCoding.h"
#include "Nervos/Address.h"
#include "PrivateKey.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Nervos;

TEST(TWNervosAddress, AddressFromPublicKey) {
    auto privateKey =
        PrivateKey(parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(publicKey.bytes.size(), 33ul);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(TWNervosAddress, AddressFromString) {
    auto address = Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8fur"
                           "ras980hksatlslfaktks7epf25");
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(TWNervosAddress, AddressFromWallet) {
    auto hdWallet = HDWallet(
        "alpha draw toss question picnic endless recycle wrong enable roast success palm", "");
    auto addressString = hdWallet.deriveAddress(TWCoinTypeNervos);
    ASSERT_EQ(addressString, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8f"
                             "urras980hksatlslfaktks7epf25");
}
