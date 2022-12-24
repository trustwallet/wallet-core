// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Cosmos/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Cosmos::tests {

TEST(AgoricAddress, Valid) {
    ASSERT_TRUE(Address::isValid(TWCoinTypeSecret, "secret16vw3fp7x35tzmwlkdkyzr8vgscn0zewtduyjuf"));
    ASSERT_TRUE(Address::isValid(TWCoinTypeSecret, "secret15rgv8teecnt53h0gdvngzt3am3yuz3rxh4fnle"));
}

TEST(AgoricAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(TWCoinTypeAgoric, "agoric1cqvwa8jr6pmt45jndanc8lqmdsxjkhw0yert212"));
    ASSERT_FALSE(Address::isValid(TWCoinTypeAgoric, "osmo1nvt7lu3e0l8wqahlya6m98zmma6qc9r40evp0v"));
}

TEST(AgoricAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("037048190544fa57651452f477c096de4f3073e7835cf3845b04602563a73f73"));
    auto address = Address(TWCoinTypeAgoric, privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5");
}

TEST(AgoricAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("03df9a5e4089f89d45913fb2b856de984c7e8bf1344cc6444cc9705899a48c939d"), TWPublicKeyTypeSECP256k1);
    auto address = Address(TWCoinTypeAgoric, publicKey);
    ASSERT_EQ(address.string(), "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5");
}

TEST(AgoricAddress, FromString) {
   Address address;
   EXPECT_TRUE(Address::decode("agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5", address));
   EXPECT_EQ(address.string(), "agoric18zvvgk6j3eq5wd7mqxccgt20gz2w94cy88aek5");
}
}
