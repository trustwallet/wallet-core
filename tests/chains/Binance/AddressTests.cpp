// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Binance/Address.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <gtest/gtest.h>
#include "TestUtilities.h"

using namespace TW;
using namespace TW::Binance;

TEST(BinanceAddress, AddressToData) {
    // mainnet
    auto data = TW::addressToData(TWCoinTypeBinance, "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8");
    EXPECT_EQ(hex(data), "b9cc92dd7d870bdc7c7d2d2ad9cd993a5186f6bd");

    // testnet
    data = TW::addressToData(TWCoinTypeTBinance, "tbnb1x4hxmtdf7pwea9dghq73dufh3qspm8gp5qht9c");
    EXPECT_EQ(hex(data), "356e6dada9f05d9e95a8b83d16f13788201d9d01");

    EXPECT_EQ(Address(data, TWCoinTypeTBinance).string(), "tbnb1x4hxmtdf7pwea9dghq73dufh3qspm8gp5qht9c");

    // invalid address
    data = TW::addressToData(TWCoinTypeTBinance, "tbnb1x4hxmtdf7pwea9dghq73dufh3qspm8gp");
    EXPECT_EQ(hex(data), "");
}

TEST(BinanceAddress, DeriveAddress) {
    // mainnet
    auto pubkey = parse_hex("02bf9a5e2b514492326e7ba9a5161b6e47df7a4aa970dd2d13c398bec89608d8d0");
    auto address = TW::deriveAddress(TWCoinTypeBinance, PublicKey(pubkey, TW::publicKeyType(TWCoinTypeBinance)));
    EXPECT_EQ(address, "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8");

    // testnet
    address = TW::deriveAddress(TWCoinTypeTBinance, PublicKey(pubkey, TW::publicKeyType(TWCoinTypeTBinance)));
    EXPECT_EQ(address, "tbnb1h8xf9htasu9aclra954dnnve8fgcda4ahtfdak");
}
