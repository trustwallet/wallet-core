// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Nervos/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Nervos;

TEST(NervosAddress, Valid) {
    ASSERT_TRUE(Address::isValid("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9e"
                                 "rg8furras980hksatlslfaktks7epf25"));
}

TEST(NervosAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9"
                                  "erg8furras980hksatlslfaktks7epf26"));
}

TEST(NervosAddress, FromPrivateKey) {
    auto privateKey =
        PrivateKey(parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(NervosAddress, FromPublicKey) {
    auto publicKey =
        PublicKey(parse_hex("026c9e4cbb95d4b3a123c1fc80795feacc38029683a1b3e16bccf49bba25fb2858"),
                  TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(NervosAddress, FromString) {
    auto address = Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8fur"
                           "ras980hksatlslfaktks7epf25");
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}
