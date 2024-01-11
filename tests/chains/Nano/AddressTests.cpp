// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Nano/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Nano::tests {

TEST(NanoAddress, FromPublicKey) {
    {
        const auto publicKey = PublicKey(parse_hex("5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50"), TWPublicKeyTypeED25519Blake2b);
        const auto address = Address(publicKey);
        ASSERT_EQ(string("nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg"), address.string());
    }

    {
        const auto publicKey = PublicKey(parse_hex("03e20ec6b4a39a629815ae02c0a1393b9225e3b890cae45b59f42fa29be9668d"), TWPublicKeyTypeED25519);
        ASSERT_THROW(Address address(publicKey), std::invalid_argument);
    }
}

TEST(NanoAddress, FromString) {
    {
        string nanoAddress = "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg";
        const auto address = Address(nanoAddress);
        ASSERT_EQ(address.string(), nanoAddress);
        ASSERT_EQ(hex(address.bytes), "5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50");
    }

    {
        string xrbAddress = "xrb_1111111111111111111111111111111111111111111111111111hifc8npp";
        string nanoAddress = "nano_1111111111111111111111111111111111111111111111111111hifc8npp";
        const auto address = Address(xrbAddress);
        ASSERT_EQ(address.string(), nanoAddress);
        ASSERT_EQ(hex(address.bytes), "0000000000000000000000000000000000000000000000000000000000000000");
    }
}

TEST(NanoAddress, isValid) {
    string nanodeAddress = "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg";
    string faultyChecksumAddress = "xrb_1111111111111111111111111111111111111111111111111111hi111111";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(nanodeAddress));
    ASSERT_FALSE(Address::isValid(faultyChecksumAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
}

} // namespace TW::Nano::tests
