// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Polkadot/SS58Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <map>

using namespace TW;

namespace TW::Polkadot::tests {

TEST(SS58Address, IsValid) {
    EXPECT_TRUE(SS58Address::isValid("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu", 0));
    
    EXPECT_TRUE(SS58Address::isValid("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 5));
    EXPECT_FALSE(SS58Address::isValid("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd", 6));

    EXPECT_TRUE(SS58Address::isValid("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2));
    EXPECT_FALSE(SS58Address::isValid("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 5));

    EXPECT_TRUE(SS58Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 64));
    EXPECT_FALSE(SS58Address::isValid("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb", 65));
    EXPECT_FALSE(SS58Address::isValid("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A", 64));    

    EXPECT_TRUE(SS58Address::isValid("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172));
    EXPECT_FALSE(SS58Address::isValid("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 171));

    EXPECT_TRUE(SS58Address::isValid("VDSyeURSP7ykE1zJPJGeqx6GcDZQF2DT3hAKhPMuwM5FuN9HE", 4096));
    EXPECT_FALSE(SS58Address::isValid("VDSyeURSP7ykE1zJPJGeqx6GcDZQF2DT3hAKhPMuwM5FuN9HE", 64));

    EXPECT_TRUE(SS58Address::isValid("YDTv3GdhXPP3pQMqQtntGVg5hMno4jqanfYUgMPX2rLGJBKX6", 8219));
    EXPECT_FALSE(SS58Address::isValid("YDTv3GdhXPP3pQMqQtntGVg5hMno4jqanfYUgMPX2rLGJBKX6", 322));
}

const auto pubkeyString1 = "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3";

TEST(SS58Address, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex(pubkeyString1), TWPublicKeyTypeED25519);
    auto addressPolkadot = SS58Address(publicKey, 0);
    EXPECT_EQ(addressPolkadot.string(), "14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC");
    EXPECT_EQ(hex(addressPolkadot.keyBytes()), pubkeyString1);

    auto addressAstar = SS58Address(publicKey, 5);
    EXPECT_EQ(addressAstar.string(), "ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");
    EXPECT_EQ(hex(addressAstar.keyBytes()), pubkeyString1);

    auto addressParallel = SS58Address(publicKey, 172);
    EXPECT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
    EXPECT_EQ(hex(addressParallel.keyBytes()), pubkeyString1);
}

TEST(SS58Address, FromPublicKeyInvalid) {
    auto publicKey = PublicKey(parse_hex(pubkeyString1), TWPublicKeyTypeED25519);
    EXPECT_EXCEPTION(SS58Address(publicKey, 32771), "network out of range 32771");
}

TEST(SS58Address, FromString) {
    auto addressKusama = SS58Address("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D", 2);
    EXPECT_EQ(addressKusama.string(), "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");

    auto addressParallel = SS58Address("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 172);
    EXPECT_EQ(addressParallel.string(), "p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
}

TEST(SS58Address, FromStringInvalid) {
    EXPECT_EXCEPTION(SS58Address("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL", 130), "Invalid address string");
}

std::map<uint32_t, std::string> networkData = {
    {0x00, "00"},
    {0x01, "01"},
    {0x02, "02"},
    {0x03, "03"},
    {0x04, "04"},
    {0x08, "08"},
    {0x0b, "0b"},
    {0x10, "10"},
    {0x20, "20"},
    {0x23, "23"},
    {0x30, "30"},
    {0x3f, "3f"},
    {0x40, "5000"},
    {0x41, "5040"},
    {0x80, "6000"},
    {0x0100, "4001"},
    {0x0123, "48c1"},
    {0x0200, "4002"},
    {0x0300, "4003"},
    {0x0400, "4004"},
    {0x0800, "4008"},
    {0x0fff, "7fcf"},
    {0x1000, "4010"},
    {0x1003, "40d0"},
    {0x2000, "4020"},
    {0x3000, "4030"},
    {0x3fff, "7fff"},
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

    // 1. byte from invalid range
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("ab" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("8000" "000102030405"), networkSize, network));

    // 2-byte, but decoded network is < 64
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4000" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4040" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4080" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4100" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4200" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4400" "000102030405"), networkSize, network));
    EXPECT_FALSE(SS58Address::decodeNetwork(parse_hex("4800" "000102030405"), networkSize, network));
}

TEST(SS58Address, EncodeNetwork) {
    Data data;
    for (auto& d: networkData) {
        EXPECT_TRUE(SS58Address::encodeNetwork(d.first, data));
        EXPECT_EQ(hex(data), d.second);
    }

    // network > 16383
    EXPECT_FALSE(SS58Address::encodeNetwork(0x4000, data));
    EXPECT_FALSE(SS58Address::encodeNetwork(0x8000, data));
}

} // namespace TW::Polkadot::tests
