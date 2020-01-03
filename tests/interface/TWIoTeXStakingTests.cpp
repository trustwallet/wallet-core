// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXStaking.h>

#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "IoTeX/Signer.h"
#include "proto/IoTeX.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IoTeX;

TEST(TWIoTeXStaking, Stake) {
    auto d = parse_hex("07c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");

    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto canName = TWDataCreateWithBytes(name, 12);
    byte t[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};
    auto test = TWDataCreateWithBytes(t, 14);
    auto stake = TWIoTeXStakingStake(canName, 1001, true, test);

    ASSERT_EQ(d, data(TWDataBytes(stake), TWDataSize(stake)));
}

TEST(TWIoTeXStaking, Unstake) {
    auto d = parse_hex("c8fd6ed000000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");

    byte t[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};
    auto test = TWDataCreateWithBytes(t, 14);
    auto unstake = TWIoTeXStakingUnstake(1001, test);

    ASSERT_EQ(d, data(TWDataBytes(unstake), TWDataSize(unstake)));
}

TEST(TWIoTeXStaking, Withdraw) {
    auto d = parse_hex("030ba25d00000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");

    byte t[] = {'t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's', 't'};
    auto test = TWDataCreateWithBytes(t, 14);
    auto withdraw = TWIoTeXStakingWithdraw(1001, test);

    ASSERT_EQ(d, data(TWDataBytes(withdraw), TWDataSize(withdraw)));
}

TEST(TWIoTeXStaking, AddStake) {
    auto d = parse_hex("6e7b301700000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000000");

    auto add = TWIoTeXStakingAddStake(1001, nullptr);

    ASSERT_EQ(d, data(TWDataBytes(add), TWDataSize(add)));
}

TEST(TWIoTeXStaking, MoveStake) {
    auto d = parse_hex("d3e41fd200000000000000000000000000000000000000000000000000000000000003e90102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000");

    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto canName = TWDataCreateWithBytes(name, 12);
    auto move = TWIoTeXStakingMoveStake(1001, canName, nullptr);

    ASSERT_EQ(d, data(TWDataBytes(move), TWDataSize(move)));
}

TEST(TWIoTeXStaking, Sign) {
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto keyhex = parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_privatekey(keyhex.data(), keyhex.size());

    // staking is implemented using the Execution message
    auto staking = input.mutable_execution();
    staking->set_amount("456");
    staking->set_contract(IOTEX_STAKING_CONTRACT);
    // call staking API to generate calldata
    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto canName = TWDataCreateWithBytes(name, 12);
    TWData* data = TWIoTeXStakingStake(canName, 1001, true, nullptr);
    staking->set_data(TWDataBytes(data), TWDataSize(data));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    auto h = signer.hash();
    ASSERT_EQ(hex(h.begin(), h.end()), "7119f1eb87b57f4f9a4886c1fba255a5c9cce21765ff4fbb794af59a1ab53307");
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "0ae6010801107b18f806220339393962d7010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611aa40107c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a416ca6f4dfbfcf4bb4d21254e6b5df2e8ec391ed7be92c31a1c437221846a71d6c502d9d470caf028d60835746bae535c55ad7a717d800718687b2c3d2f2fe869900");
    // signed action's hash
    auto h1 = output.hash();
    ASSERT_EQ(hex(h1.begin(), h1.end()), "14858f049bf009f38ce9382d99eaa33f70c7dd61aa312c4f477afccf79e963ac");
}
