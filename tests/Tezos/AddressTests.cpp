// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <array>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosAddress, isValid) {
    auto address = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
    ASSERT_TRUE(Address::isValid(address));
}

TEST(TezosAddress, string) {
    auto addressString = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
    auto address = Address(addressString);
    ASSERT_EQ(address.string(), addressString);
}

TEST(TezosAddress, PublicKeyInit) {
    std::array<uint8_t, 33> bytes {3, 249, 155, 77, 241, 23, 253, 247, 67, 73, 230, 138, 75, 159, 164, 53, 139, 19, 144, 243, 254, 68, 145, 220, 146, 163, 138, 154, 23, 67, 129, 243, 228};
    const auto publicKey = PublicKey(bytes);
    auto address = Address(publicKey);

    auto expected = "tz1STrmVM4Uk7HYAjCe8H3b7HE7rnBY2L4xk";
    ASSERT_EQ(address.string(), expected);
}

TEST(TezosAddress, isInvalid) {
    std::array<std::string, 3> invalidAddresses {
      "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt", // Invalid prefix, valid checksum
      "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA", // Valid prefix, invalid checksum
      "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"  // Invalid prefix, invalid checksum
    };

    for (std::string address : invalidAddresses) {
        ASSERT_FALSE(Address::isValid(address));
    }
}

