// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"
#include "HexCoding.h"
#include "NEO/Address.h"
#include "NEO/Signer.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOSigner, FromPublicPrivateKey) {
   auto hexPrvKey = "4646464646464646464646464646464646464646464646464646464646464646";
   auto hexPubKey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
   auto signer = Signer(PrivateKey(parse_hex(hexPrvKey)));
   auto prvKey = signer.getPrivateKey();
   auto pubKey = signer.getPublicKey();

   EXPECT_EQ(hexPrvKey, hex(prvKey.bytes));
   EXPECT_EQ(hexPubKey, hex(pubKey.bytes));

   auto address = signer.getAddress();
   EXPECT_TRUE(Address::isValid(address.string()));

   EXPECT_EQ(Address(pubKey), address);
}

TEST(NEOSigner, SigningData) {
   auto signer = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
   auto verScript = "ba7908ddfe5a1177f2c9d3fa1d3dc71c9c289a3325b3bdd977e20c50136959ed02d1411efa5e8b897d970ef7e2325e6c0a3fdee4eb421223f0d86e455879a9ad";
   auto invocationScript = string("401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484401642b3d538e138f34b32330e381a7fe3f5151fcf958f2030991e72e2e25043143e4a1ebd239634efba279c96fa0ab04a15aa15179d73a7ef5a886ac8a06af484");
   invocationScript = string(invocationScript.rbegin(), invocationScript.rend());

   EXPECT_EQ(verScript, hex(signer.sign(parse_hex(invocationScript))));
}
