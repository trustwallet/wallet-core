// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Address.h"
#include "Bitcoin/SegwitAddress.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWDerivation.h>

#include <gtest/gtest.h>
#include <string>

using namespace TW;

namespace TW::Bitcoin::tests {

TEST(LitecoinAddress, deriveAddress_legacy) {
    const auto pubKey = PublicKey(parse_hex("03b49081a4d7ad24b20e209bc6fe10491aadb5607777baf0509a036cce96025db0"), TWPublicKeyTypeSECP256k1);
    auto addrStr = deriveAddress(TWCoinTypeLitecoin, pubKey, TWDerivationLitecoinLegacy);
    EXPECT_EQ(addrStr, "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr");

    const auto address = Address(pubKey, TWCoinTypeP2pkhPrefix(TWCoinTypeLitecoin));
    EXPECT_EQ(address.string(), addrStr);
}

TEST(LitecoinAddress, deriveAddress_segwit) {
    const auto pubKey = PublicKey(parse_hex("030fc2fdd1a0b5d43b31227a4b1cd57e7d35a6edc93fb12f9315e67762abeb8be0"), TWPublicKeyTypeSECP256k1);
    auto addrStr = deriveAddress(TWCoinTypeLitecoin, pubKey, TWDerivationBitcoinSegwit);
    EXPECT_EQ(addrStr, "ltc1q3m3ujh350qrqdl33pv7pjw0d0m9qnm6qjcjpga");

    const auto address = SegwitAddress(pubKey, stringForHRP(TWCoinTypeHRP(TWCoinTypeLitecoin)));
    EXPECT_EQ(address.string(), addrStr);
}

} // namespace TW::Bitcoin::tests
