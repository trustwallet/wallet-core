// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aeternity/Address.h>
#include <HexCoding.h>
#include <PrivateKey.h>
#include <gtest/gtest.h>

namespace TW::Aeternity::tests {

TEST(AeternityAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");

    const auto privateKey = PrivateKey(parse_hex("a1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a"));
    const auto publicKey2 = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    EXPECT_ANY_THROW(new Address(publicKey2));
}

TEST(AeternityAddress, FromString) {
    auto address = Address("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");
    ASSERT_EQ(address.string(), "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");

    EXPECT_ANY_THROW(new Address(""));
}

} // namespace TW::Aeternity::tests
