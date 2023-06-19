// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Substrate/Address.h"
#include "TestUtilities.h"

using namespace TW;
namespace TW::Substrate {

TEST(SubstrateAddress, Validation) {
    ASSERT_TRUE(Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64));
    ASSERT_FALSE(Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 64));

    // polymesh
    ASSERT_TRUE(Address::isValid("2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG", 12));
    ASSERT_FALSE(Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 12));
}

TEST(SubstrateAddress, FromPublicKey) {
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

TEST(SubstrateAddress, FromString) {
    auto addressKusama = Address("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
    ASSERT_EQ(addressKusama.string(), "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");

    auto addressParallel = Address("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
    ASSERT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");

    // polymesh
    auto addressPolymesh = Address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW", 12);
    ASSERT_EQ(addressPolymesh.string(), "2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
}

TEST(SubstrateAddress, Keybytes) {
    auto pubKeyBytes = parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");
    auto publicKey = PublicKey(pubKeyBytes, TWPublicKeyTypeED25519);

    auto addressAstar = Address(publicKey, 5);
    ASSERT_EQ(addressAstar.keyBytes(), pubKeyBytes);

    auto addressParallel = Address(publicKey, 172);
    ASSERT_EQ(addressParallel.keyBytes(), pubKeyBytes);

    // polymesh
    auto addressPolymesh = Address(publicKey, 12);
    ASSERT_EQ(addressPolymesh.keyBytes(), pubKeyBytes);
}

} // namespace TW::Substrate