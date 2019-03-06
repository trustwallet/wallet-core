// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/Address.h"
#include "Tezos/BinaryCoding.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>
#include <string>
#include <iostream>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosAddress, isValid) {
    auto address = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
    ASSERT_TRUE(Address::isValid(address));
}

TEST(TezosAddress, PublicKeyInit) {
    uint8_t bytes[] = {3, 75, 238, 252, 167, 77, 154, 50, 92, 241, 214, 182, 145, 29, 97, 166, 92, 50, 175, 168, 224, 43, 213, 231, 142, 46, 74, 194, 145, 11, 171, 69, 245};
    const auto publicKey = PublicKey(bytes);
    auto address = Address(publicKey);

    // auto expected = "00002b57963f8b7dd930af143d056522c480328752ec";
    // ASSERT_EQ(address.string(), expected);
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

TEST(TezosAddress, ForgeBoolTrue) {
    auto expected = "ff";

    auto output = forgeBool(true);

    ASSERT_EQ(output, expected);
}

TEST(TezosAddress, ForgeBoolFalse) {
    auto expected = "00";

    auto output = forgeBool(false);

    ASSERT_EQ(output, expected);
}

TEST(TezosAddress, forge_tz1) {
    auto input = Address("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don");
    auto expected = "0000cfa4aae60f5d9389752d41e320da224d43287fe2";

    ASSERT_EQ(input.string(), expected);
}

TEST(TezosAddress, forge_tz2) {
    auto input = Address("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo");
    auto expected = "0001be99dd914e38388ec80432818b517759e3524f16";

    ASSERT_EQ(input.string(), expected);
}

TEST(TezosAddress, forge_tz3) {
    auto input = Address("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9");
    auto expected = "0002358cbffa97149631cfb999fa47f0035fb1ea8636";

    ASSERT_EQ(input.string(), expected);
}

TEST(TezosAddress, forge_KT1) {
    auto input = Address("KT1HiGcq47Yek7dcre7So2yfWTLU83FTBaf1");
    auto expected = "0164244bbdc7790d7896b6a52ac49802cfe4eafc4b00";

    ASSERT_EQ(input.string(), expected);
}
