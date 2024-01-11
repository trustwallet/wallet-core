// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HDWallet.h"
#include "HexCoding.h"
#include "Nervos/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Nervos::tests {

TEST(NervosAddress, Valid) {
    ASSERT_TRUE(Address::isValid("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9e"
                                 "rg8furras980hksatlslfaktks7epf25"));
    ASSERT_TRUE(Address::isValid("ckb1qyqvfdgvtjxswncx8mq2wl0dp6hlp7nmvhdqcecnt6"));
    ASSERT_TRUE(Address::isValid("ckb1qjda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xw3394p3wg6"
                                 "p60qclvpfmaa582lu860dja5h0fk0v"));
}

TEST(NervosAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9"
                                  "erg8furras980hksatlslfaktks7epf26"));
    ASSERT_FALSE(Address::isValid("ckb1qyqvfdgvtjxswncx8mq2wl0dp6hlp7nmvhdqcecnt7"));
    ASSERT_FALSE(Address::isValid("ckb1qjda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xw3394p3wg"
                                  "6p60qclvpfmaa582lu860dja5h0fk0w"));
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
    auto address1 = Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8fu"
                            "rras980hksatlslfaktks7epf25");
    ASSERT_EQ(address1.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9e"
                                 "rg8furras980hksatlslfaktks7epf25");
    auto address2 = Address("ckb1qyqvfdgvtjxswncx8mq2wl0dp6hlp7nmvhdqcecnt6");
    ASSERT_EQ(address2.string(), "ckb1qyqvfdgvtjxswncx8mq2wl0dp6hlp7nmvhdqcecnt6");
    auto address3 = Address("ckb1qjda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xw3394p3wg6p60qc"
                            "lvpfmaa582lu860dja5h0fk0v");
    ASSERT_EQ(address3.string(), "ckb1qjda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xw3394p3wg6"
                                 "p60qclvpfmaa582lu860dja5h0fk0v");
}

TEST(TWNervosAddress, AddressFromPublicKey) {
    auto privateKey =
        PrivateKey(parse_hex("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(publicKey.bytes.size(), 33ul);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(NervosAddress, AddressFromString) {
    auto address = Address("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8fur"
                           "ras980hksatlslfaktks7epf25");
    ASSERT_EQ(address.string(), "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}

TEST(NervosAddress, AddressFromWallet) {
    auto hdWallet = HDWallet(
        "alpha draw toss question picnic endless recycle wrong enable roast success palm", "");
    auto addressString = hdWallet.deriveAddress(TWCoinTypeNervos);
    ASSERT_EQ(addressString, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9erg8f"
                             "urras980hksatlslfaktks7epf25");
}

} // namespace TW::Nervos::tests
