// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWCoinType.h>

#include "Coin.h"
#include "HexCoding.h"
#include "HDWallet.h"
#include "Bitcoin/Address.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

TEST(PivxAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("DLCk9wuF3r8CRbawUhrDpDHXAGfkHAC7if")));
    ASSERT_TRUE(Address::isValid(std::string("DSHhD6WxT5RfG2deUzVfuC1jDuRfdDpsSo")));
}

TEST(PivxAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0273132ffeafdc0eec3aaef5684a320a677cc77ba43df81de944af4a7fc6de66c9"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypePivx));
    EXPECT_EQ(address.string(), "D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH");

    auto wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "");
    auto addr = wallet.deriveAddress(TWCoinTypePivx);
    EXPECT_EQ(addr, "D81AqC8zKma3Cht4TbVuh4jyVVyLkZULCm");
}

TEST(PivxAddress, FromString) {
    auto address = Address("D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH");
    ASSERT_EQ(address.string(), "D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH");

    address = Address("DSHhD6WxT5RfG2deUzVfuC1jDuRfdDpsSo");
    ASSERT_EQ(address.string(), "DSHhD6WxT5RfG2deUzVfuC1jDuRfdDpsSo");
}

TEST(PivxAddress, AddressData) {
    const auto publicKey = PublicKey(parse_hex("0273132ffeafdc0eec3aaef5684a320a677cc77ba43df81de944af4a7fc6de66c9"), TWPublicKeyTypeSECP256k1);
    auto address = TW::deriveAddress(TWCoinTypePivx, publicKey);

    auto data = TW::addressToData(TWCoinTypePivx, "D6E4vLacYhKc6RVxLNQg5j8rtWbAH8fybH");
    ASSERT_EQ(hex(data), "0be8da968f9bc6c1c16b8c635544e757aade7013");
    ASSERT_EQ(data, TW::addressToData(TWCoinTypePivx, address));

    data = TW::addressToData(TWCoinTypePivx, "1G15VvshDxwFTnahZZECJfFwEkq9fP79"); // invalid address
    ASSERT_EQ(data.size(), 0ul);
}