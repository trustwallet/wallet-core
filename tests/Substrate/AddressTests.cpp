//
// Created by Fitz on 2022/3/1.
//

#include "HexCoding.h"
#include "Substrate/Address.h"
#include "../tests/interface/TWTestUtilities.h"

using namespace TW;
using namespace TW::Substrate;

TEST(SubstrateAddress, Validation) {
    ASSERT_TRUE(Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64));
    ASSERT_FALSE(Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 64));
}

TEST(SubstrateAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3"), TWPublicKeyTypeED25519);
    auto addressPolkadot = Address(publicKey, 0);
    ASSERT_EQ(addressPolkadot.string(), "14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC");

    auto addressAstar = Address(publicKey, 5);
    ASSERT_EQ(addressAstar.string(), "ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");

    auto addressParallel = Address(publicKey, 172);
    ASSERT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
}

TEST(SubstrateAddress, FromString) {
    auto addressKusama = Address("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
    ASSERT_EQ(addressKusama.string(), "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");

    auto addressParallel = Address("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
    ASSERT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
}

TEST(SubstrateAddress, Keybytes) {
    auto pubKeyBytes = parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");
    auto publicKey = PublicKey(pubKeyBytes, TWPublicKeyTypeED25519);

    auto addressAstar = Address(publicKey, 5);
    ASSERT_EQ(addressAstar.keyBytes(), pubKeyBytes);

    auto addressParallel = Address(publicKey, 172);
    ASSERT_EQ(addressParallel.keyBytes(), pubKeyBytes);
}