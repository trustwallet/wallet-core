// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Nebl/TAddress.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std ;
namespace TW::Nebl {

TEST(NeblAddress, FromPrivateKey) {
    const auto privateKey =  PrivateKey(parse_hex("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9"));
    const auto pbkey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto address = TAddress(pbkey);

    EXPECT_EQ(address.string(), "NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    EXPECT_EQ(address.bytes[0], 0x35);
}

TEST(NeblAddress, FromPublicKey) {
    const auto privateKey = PrivateKey(parse_hex("BE88DF1D0BF30A923CB39C3BB953178BAAF3726E8D3CE81E7C8462E046E0D835"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto address = TAddress(publicKey);

    EXPECT_EQ(address.string(), "NidLccuLD8J4oK25PwPg5ipLj5L9VVrwi5");
    EXPECT_EQ(address.bytes[0], 0x35);
}

} // namespace TW::Nebl
