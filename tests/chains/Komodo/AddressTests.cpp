// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Komodo::tests {

TEST(KomodoAddress, Valid) {
    ASSERT_TRUE(TW::validateAddress(TWCoinTypeKomodo, "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA"));
}

TEST(KomodoAddress, Invalid) {
    ASSERT_FALSE(TW::validateAddress(TWCoinTypeKomodo, "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY"));
}

TEST(KomodoAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("020e46e79a2a8d12b9b5d12c7a91adb4e454edfae43c0a0cb805427d2ac7613fd9"), TWPublicKeyTypeSECP256k1);
    auto address = TW::deriveAddress(TWCoinTypeKomodo, publicKey);
    ASSERT_EQ(address, "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA");

    auto wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "");
    auto addr = wallet.deriveAddress(TWCoinTypeKomodo);
    EXPECT_EQ(addr, "RCWJLXE5CSXydxdSnwcghzPgkFswERegyb");
}

} // namespace TW::Komodo::tests
