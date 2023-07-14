// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Polkadot/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Polkadot::tests {

TEST(PolkadotAddress, Validation) {
    // Substrate ed25519
    ASSERT_FALSE(Address::isValid("5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ"));
    // Bitcoin
    ASSERT_FALSE(Address::isValid("1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA"));
    // Kusama ed25519
    ASSERT_FALSE(Address::isValid("FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ"));
    // Kusama secp256k1
    ASSERT_FALSE(Address::isValid("FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx"));
    // Kusama sr25519
    ASSERT_FALSE(Address::isValid("EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe"));

    // Polkadot ed25519
    ASSERT_TRUE(Address::isValid("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu"));
    // Polkadot sr25519
    ASSERT_TRUE(Address::isValid("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony"));

    ASSERT_TRUE(Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64));
    ASSERT_FALSE(Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 64));

    // Polymesh
    ASSERT_TRUE(Address::isValid("2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG", 12));
    ASSERT_FALSE(Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 12));
}

TEST(PolkadotAddress, FromPrivateKey) {
    // subkey phrase `chief menu kingdom stereo hope hazard into island bag trick egg route`
    auto privateKey = PrivateKey(parse_hex("0x612d82bc053d1b4729057688ecb1ebf62745d817ddd9b595bc822f5f2ba0e41a"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
}

TEST(PolkadotAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("0xbeff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
}

TEST(PolkadotAddress, FromPublicKeyWithPrefix) {
    auto publicKey = PublicKey(parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3"), TWPublicKeyTypeED25519);
    auto addressPolkadot = Address(publicKey, 0);
    ASSERT_EQ(addressPolkadot.string(), "14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC");

    auto addressAstar = Address(publicKey, 5);
    ASSERT_EQ(addressAstar.string(), "ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");

    auto addressParallel = Address(publicKey, 172);
    ASSERT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");

    // polymesh
    publicKey = PublicKey(parse_hex("849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f83"), TWPublicKeyTypeED25519);
    auto addressPolymesh = Address(publicKey, 12);
    ASSERT_EQ(addressPolymesh.string(), "2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
}

TEST(PolkadotAddress, FromString) {
    auto address = Address("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
    ASSERT_EQ(address.string(), "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
}

TEST(PolkadotAddress, FromStringWithPrefix) {
    auto addressKusama = Address("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
    ASSERT_EQ(addressKusama.string(), "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");

    auto addressParallel = Address("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
    ASSERT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");

    // polymesh
    auto addressPolymesh = Address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW", 12);
    ASSERT_EQ(addressPolymesh.string(), "2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
}

} // namespace TW::Polkadot::tests
