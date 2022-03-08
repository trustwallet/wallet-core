// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/SS58Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>
#include <map>

using namespace TW;

TEST(SS58Address, IsValid) {
    EXPECT_TRUE(SS58Address::isValid("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu", 0));
    
    EXPECT_TRUE(SS58Address::isValid("VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx", 110));
    EXPECT_FALSE(SS58Address::isValid("VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx", 111));

    EXPECT_TRUE(SS58Address::isValid("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 5));
    EXPECT_FALSE(SS58Address::isValid("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 6));

    EXPECT_TRUE(SS58Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 4096));
    EXPECT_FALSE(SS58Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 64));
    EXPECT_FALSE(SS58Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 0));

    EXPECT_TRUE(SS58Address::isValid("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2));
    EXPECT_FALSE(SS58Address::isValid("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 5));

    EXPECT_TRUE(SS58Address::isValid("hJJWt4tUJdYwV4Hrm9uogFgHMomqeXWCzdqXvnHCmb9pt9RF7", 7040));
    EXPECT_FALSE(SS58Address::isValid("hJJWt4tUJdYwV4Hrm9uogFgHMomqeXWCzdqXvnHCmb9pt9RF7", 120));
}

TEST(SS58Address, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3"), TWPublicKeyTypeED25519);
    auto addressPolkadot = SS58Address(publicKey, 0);
    ASSERT_EQ(addressPolkadot.string(), "14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC");
    EXPECT_EQ(hex(addressPolkadot.keyBytes()), "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");

    auto addressAstar = SS58Address(publicKey, 5);
    ASSERT_EQ(addressAstar.string(), "ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");
    EXPECT_EQ(hex(addressAstar.keyBytes()), "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");

    auto addressParallel = SS58Address(publicKey, 110);
    ASSERT_EQ(addressParallel.string(), "VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx");
    EXPECT_EQ(hex(addressParallel.keyBytes()), "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");
}

TEST(SS58Address, FromPublicKeyInvalid) {
    auto publicKey = PublicKey(parse_hex("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3"), TWPublicKeyTypeED25519);
    EXPECT_EXCEPTION(SS58Address(publicKey, 32771), "network out of range 32771");
}

TEST(SS58Address, FromString) {
    auto addressKusama = SS58Address("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
    ASSERT_EQ(addressKusama.string(), "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");

    auto addressParallel = SS58Address("VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx", 110);
    ASSERT_EQ(addressParallel.string(), "VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx");
}

TEST(SS58Address, FromStringInvalid) {
    EXPECT_EXCEPTION(SS58Address("VNpyemgLHFKxYhWKzSKM5SoGYyAqtBSdyRaQ3DcNEsT9jpVnx", 130), "Invalid address string");
}

std::map<uint32_t, std::string> networkData = {
    {0, "00"},
    {3, "03"},
    {11, "0b"},
    {35, "23"},
    {63, "3f"},
    {64, "4040"},
    {65, "4041"},
    {291, "4123"},
    {4095, "4fff"},
    {4096, "5000"},
    {4099, "5003"},
    {16382, "7ffe"},
    {16383, "7fff"},
};

TEST(SS58Address, DecodeNetwork) {
    byte networkSize = 0;
    uint32_t network = 0;
    for (auto& d: networkData) {
        std::string input = d.second + std::string("000102030405");
        EXPECT_TRUE(SS58Address::decodeNetwork(parse_hex(input), networkSize, network));
        EXPECT_EQ(network, d.first);
        if (d.first < 64) {
            EXPECT_EQ(networkSize, 1);
        } else {
            EXPECT_EQ(networkSize, 2);
        }
    }

    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("ab" "000102030405"), networkSize, network));

    EXPECT_TRUE(SS58Address::decodeNetwork(parse_hex("4000" "000102030405"), networkSize, network));
    EXPECT_EQ(networkSize, 2);
    EXPECT_EQ(network, 0);
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("8000" "000102030405"), networkSize, network));
}

TEST(SS58Address, EncodeNetwork) {
    Data data;
    for (auto& d: networkData) {
        EXPECT_TRUE(SS58Address::encodeNetwork(d.first, data));
        EXPECT_EQ(hex(data), d.second);
    }
}
