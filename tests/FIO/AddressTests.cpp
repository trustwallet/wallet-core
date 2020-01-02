// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWPublicKeyType.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FIO;

TEST(FIOAddress, Validation) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    ASSERT_FALSE(Address::isValid("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));

    ASSERT_TRUE(Address::isValid("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o"));
}

TEST(FIOAddress, FromString) {
    ASSERT_EQ(
        Address("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o").string(),
        "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o"
    );
}

TEST(FIOAddress, FromPrivateKey) {
    auto key = PrivateKey(parse_hex("ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
}
