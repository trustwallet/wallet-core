// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWPublicKeyType.h>

#include <gtest/gtest.h>

namespace TW::FIO::tests {

TEST(FIOAddress, ValidateString) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    ASSERT_FALSE(Address::isValid("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));

    ASSERT_TRUE(Address::isValid("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o"));
}

TEST(FIOAddress, ValidateData) {
    Address address("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
    EXPECT_EQ(address.bytes.size(), 37ul);
    Data addrData = TW::data(address.bytes.data(), address.bytes.size());

    EXPECT_EQ(Address::isValid(addrData), true);

    // create invalid data, too short
    Data addressDataShort = subData(addrData, 0, addrData.size() - 1);
    EXPECT_EQ(Address::isValid(addressDataShort), false);
}

TEST(FIOAddress, FromString) {
    Address addr("FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
    ASSERT_EQ(addr.string(), "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
    ASSERT_EQ(hex(addr.bytes), "0271195c66ec2799e436757a70cd8431d4b17733a097b18a5f7f1b6b085978ff0f343fc54e");
}

TEST(FIOAddress, FromStringInvalid) {
    try {
        Address address("WRP5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
    } catch (std::invalid_argument&) {
        return; // ok
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(FIOAddress, FromPublicKey) {
    auto key = PrivateKey(parse_hex("ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(hex(publicKey.bytes), "0271195c66ec2799e436757a70cd8431d4b17733a097b18a5f7f1b6b085978ff0f");
    auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "FIO5kJKNHwctcfUM5XZyiWSqSTM5HTzznJP9F3ZdbhaQAHEVq575o");
}

TEST(FIOAddress, GetPublicKey) {
    const auto publicKeyHex = "0271195c66ec2799e436757a70cd8431d4b17733a097b18a5f7f1b6b085978ff0f";
    const PublicKey publicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey);
    EXPECT_EQ(hex(address.publicKey().bytes), publicKeyHex);
}

} // namespace TW::FIO::tests
