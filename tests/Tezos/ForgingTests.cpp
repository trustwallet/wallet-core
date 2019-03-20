// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/Address.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include "Tezos/Forging.h"

#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>
#include <string>
#include <array>

using namespace TW;
using namespace TW::Tezos;

TEST(Forging, ForgeBoolTrue) {
    auto expected = "ff";

    auto output = forgeBool(true);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeBoolFalse) {
    auto expected = "00";

    auto output = forgeBool(false);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithZero) {
    auto expected = "00";
  
    auto output = forgeZarith(0);
  
    ASSERT_EQ(hex(output), hex(parse_hex(expected)));
}

TEST(Forging, ForgeZarithTen) {
    auto expected = "0a";
  
    auto output = forgeZarith(10);
  
    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithTwenty) {
    auto expected = "14";
  
    auto output = forgeZarith(20);
  
    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithOneHundredFifty) {
    auto expected = "9601";
  
    auto output = forgeZarith(150);
  
    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithLarge) {
    auto expected = "82fc43";
  
    auto output = forgeZarith(1113602);
  
    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, forge_tz1) {
    auto expected = "00cfa4aae60f5d9389752d41e320da224d43287fe2";

    auto output = forgePublicKeyHash("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, forge_tz2) {
    auto expected = "01be99dd914e38388ec80432818b517759e3524f16";

    auto output = forgePublicKeyHash("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, forge_tz3) {
    auto expected = "02358cbffa97149631cfb999fa47f0035fb1ea8636";

    auto output = forgePublicKeyHash("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forgin, ForgePublicKey) {
  auto expected = "00311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95";
  
  auto privateKey = PrivateKey(parse_hex("c6377a4cc490dc913fc3f0d9cf67d293a32df4547c46cb7e9e33c3b7b97c64d8"));
  auto publicKey = privateKey.getPublicKey(PublicKeyType::ed25519);
  auto output = forgePublicKey(publicKey);
  
  ASSERT_EQ(output, parse_hex(expected));
}

