// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Tezos/Address.h"
#include "Tezos/Forging.h"

#include <TrustWalletCore/TWCoinType.h>

#include <array>
#include <gtest/gtest.h>
#include <string>

using namespace TW;

namespace TW::Tezos::tests {

TEST(TezosAddress, forge_tz1) {
    auto input = Address("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don");
    auto expected = "0000cfa4aae60f5d9389752d41e320da224d43287fe2";

    ASSERT_EQ(input.forge(), parse_hex(expected));
}

TEST(TezosAddress, forge_tz2) {
    auto input = Address("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo");
    auto expected = "0001be99dd914e38388ec80432818b517759e3524f16";

    ASSERT_EQ(input.forge(), parse_hex(expected));
}

TEST(TezosAddress, forge_tz3) {
    auto input = Address("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9");
    auto expected = "0002358cbffa97149631cfb999fa47f0035fb1ea8636";

    ASSERT_EQ(input.forge(), parse_hex(expected));
}

TEST(TezosAddress, forge_kt1) {
    auto input = Address("KT1XnTn74bUtxHfDtBmm2bGZAQfhPbvKWR8o");
    auto expected = "01fe810959c3d6127a41cbd471e7cb4e91a61b780b00";

    ASSERT_EQ(input.forge(), parse_hex(expected));
}

TEST(TezosAddress, isInvalid) {
    std::array<std::string, 3> invalidAddresses{
        "NmH7tmeJUmHcncBDvpr7aJNEBk7rp5zYsB1qt", // Invalid prefix, valid checksum
        "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3AAAA", // Valid prefix, invalid checksum
        "1tzeZwq8b5cvE2bPKokatLkVMzkxz24zAAAAA"  // Invalid prefix, invalid checksum
    };

    for (auto& address : invalidAddresses) {
        ASSERT_FALSE(Address::isValid(address));
    }
}

TEST(TezosAddress, isValid) {
    std::array<std::string, 4> validAddresses {
        "tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt",
        "tz2PdGc7U5tiyqPgTSgqCDct94qd6ovQwP6u",
        "tz3VEZ4k6a4Wx42iyev6i2aVAptTRLEAivNN",
        "KT1XnTn74bUtxHfDtBmm2bGZAQfhPbvKWR8o",
    };

    for (auto& address : validAddresses) {
        ASSERT_TRUE(Address::isValid(address));
    }
}

TEST(TezosAddress, string) {
    auto addressString = "tz1d1qQL3mYVuiH4JPFvuikEpFwaDm85oabM";
    auto address = Address(addressString);
    ASSERT_EQ(address.string(), addressString);
}

TEST(TezosAddress, deriveOriginatedAddress) {
    auto operationHash = "oo7VeTEPjEusPKnsHtKcGYbYa7i4RWpcEhUVo3Suugbbs6K62Ro";
    auto operationIndex = 0;
    auto expected = "KT1WrtjtAYQSrUVvSNJPTZTebiUWoopQL5hw";

    ASSERT_EQ(Address::deriveOriginatedAddress(operationHash, operationIndex), expected);
}

TEST(TezosAddress, PublicKeyInit) {
    Data bytes = parse_hex("01fe157cc8011727936c592f856c9071d39cf4acdadfa6d76435e4619c9dc56f63");
    const auto publicKey = PublicKey(bytes, TWPublicKeyTypeED25519);
    auto address = Address(publicKey);

    auto expected = "tz1cG2jx3W4bZFeVGBjsTxUAG8tdpTXtE8PT";
    ASSERT_EQ(address.string(), expected);
}

} // namespace TW::Tezos::tests
