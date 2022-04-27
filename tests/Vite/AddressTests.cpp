// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Vite/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Vite;

TEST(ViteAddress, FromPublicKey) {
    {
        const auto publicKey = PublicKey(parse_hex("5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50"), TWPublicKeyTypeED25519Blake2b);
        const auto address = Address(publicKey);
        ASSERT_EQ(string("vite_1vitede8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg"), address.string());
    }

    {
        const auto publicKey = PublicKey(parse_hex("03e20ec6b4a39a629815ae02c0a1393b9225e3b890cae45b59f42fa29be9668d"), TWPublicKeyTypeED25519);
        ASSERT_THROW(Address address(publicKey), std::invalid_argument);
    }
}

TEST(ViteAddress, FromString) {
    {
        string viteAddress = "vite_1vitede8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg";
        const auto address = Address(viteAddress);
        ASSERT_EQ(address.string(), viteAddress);
        ASSERT_EQ(hex(address.bytes), "5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50");
    }

    {
        string viteAddress = "vite_1111111111111111111111111111111111111111111111111111hifc8npp";
        ASSERT_EQ(address.string(), viteAddress);
        ASSERT_EQ(hex(address.bytes), "0000000000000000000000000000000000000000000000000000000000000000");
    }
}

TEST(ViteAddress, isValid) {
    string viteAddress = "vite_1vitede8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg";
    string faultyChecksumAddress = "vite_1111111111111111111111111111111111111111111111111111hi111111";
    string bitcoinAddress = "1Ma2DrB78K7jmAwaomqZNRMCvgQrNjE2QC";

    ASSERT_TRUE(Address::isValid(viteAddress));
    ASSERT_FALSE(Address::isValid(faultyChecksumAddress));
    ASSERT_FALSE(Address::isValid(bitcoinAddress));
}
