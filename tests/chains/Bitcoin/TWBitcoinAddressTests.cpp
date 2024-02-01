// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWCoinType.h>
#include <HexCoding.h>

#include <gtest/gtest.h>

const auto addr1Valid = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx";
const auto addr1Data = "00769bdff96a02f9135a1d19b749db6a78fe07dc90";

TEST(TWBitcoinAddress, Create) {
    {
        TWBitcoinAddress* addr = TWBitcoinAddressCreateWithString(STRING(addr1Valid).get());
        EXPECT_TRUE(addr != nullptr);
        TWBitcoinAddressDelete(addr);
    }
    {
        TWBitcoinAddress* addr = TWBitcoinAddressCreateWithData(DATA(addr1Data).get());
        EXPECT_TRUE(addr != nullptr);
        TWBitcoinAddressDelete(addr);
    }
}

TEST(TWBitcoinAddress, IsValid) {
    EXPECT_TRUE(TWBitcoinAddressIsValidString(STRING(addr1Valid).get()));
    EXPECT_TRUE(TWBitcoinAddressIsValid(DATA(addr1Data).get()));
}

TEST(TWBitcoinAddress, CreateWithPublicKey) {
    const auto pubKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("0239de350077b204f8fa1b63542b33580b8f125c4b9b827d5fc65cbe47fc1d9a52").get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(pubKey != nullptr);
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey.get(), TWCoinTypeP2shPrefix(TWCoinTypeBitcoin)));
    EXPECT_TRUE(addr.get() != nullptr);
}

TEST(TWBitcoinAddress, Description) {
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithString(STRING(addr1Valid).get()));
    EXPECT_TRUE(addr.get() != nullptr);
    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWBitcoinAddressDescription(addr.get())).get())), addr1Valid);
}

TEST(TWBitcoinAddress, PrefixAndHash) {
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithString(STRING(addr1Valid).get()));
    EXPECT_TRUE(addr.get() != nullptr);
    EXPECT_EQ(TWBitcoinAddressPrefix(addr.get()), 0x00);
    const auto keyhash = WRAPD(TWBitcoinAddressKeyhash(addr.get()));
    EXPECT_EQ(TW::hex(TW::data(TWDataBytes(keyhash.get()), TWDataSize(keyhash.get()))), addr1Data + 2);
}

TEST(TWBitcoinAddress, Equal) {
    const auto addr1 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithString(STRING(addr1Valid).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    const auto addr2 = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithData(DATA(addr1Data).get()));
    EXPECT_TRUE(addr2.get() != nullptr);
    EXPECT_TRUE(TWBitcoinAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWBitcoinAddress, CreateWithStringInvalid) {
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithString(STRING("__INVALID__").get()));
    EXPECT_TRUE(addr.get() == nullptr);
}

TEST(TWBitcoinAddress, CreateWithDataInvalid) {
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithData(DATA("deadbeef").get()));
    EXPECT_TRUE(addr.get() == nullptr);
}

TEST(TWBitcoinAddress, CreateWithPublicKeyInvalid) {
    const auto pubKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3").get(),
        TWPublicKeyTypeED25519));
    EXPECT_TRUE(pubKey != nullptr);
    const auto addr = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(pubKey.get(), TWCoinTypeP2shPrefix(TWCoinTypeBitcoin)));
    EXPECT_TRUE(addr.get() == nullptr);
}
