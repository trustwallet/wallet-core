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
    auto test = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("7468697320697320612074657374"));
    auto stake = TWIoTeXStakingStake(canName, 1001, true, test);

    ASSERT_EQ(d, data(TWDataBytes(stake), TWDataSize(stake)));
}

TEST(TWIoTeXStaking, Unstake) {
    auto d = parse_hex("c8fd6ed000000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");

    auto test = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("7468697320697320612074657374"));
    auto unstake = TWIoTeXStakingUnstake(1001, test);

    ASSERT_EQ(d, data(TWDataBytes(unstake), TWDataSize(unstake)));
}

TEST(TWIoTeXStaking, Withdraw) {
    auto d = parse_hex("030ba25d00000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");

    auto test = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("7468697320697320612074657374"));
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

TEST(TWIoTeXStaking, SignStake) {
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
    auto data = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("7468697320697320612074657374"));
    // data = "this is a test" here, it could be null (user leaves data empty when signing the tx)
    TWData* stake = TWIoTeXStakingStake(canName, 1001, true, data);
    staking->set_data(TWDataBytes(stake), TWDataSize(stake));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "cc3c8f7a0129455d70457c4be42a8b31d8e1df59594c99041b6b6d091b295b32");
    // build() signs the tx
    auto output = signer.build();
    // signed action's serialized bytes
    auto encoded = output.encoded();
    ASSERT_EQ(hex(encoded.begin(), encoded.end()), "0a86020801107b18f806220339393962f7010a033435361229696f3178707136326177383575717a72636367397935686e727976386c64326e6b7079636333677a611ac40107c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000e74686973206973206120746573740000000000000000000000000000000000001241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41a558bc9a4bfba920242ccd4d5c5da363ec534d4dd5eb67f88e9db7aaad5c50ad62dfe298c0e54e311ebba045f48cea1136e42a123a8e6b03d3e6ed82d4ec2b9401");
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "41b1f8be5f6b884c06556fba2611716e8e514b507f5a653fc02ac50ba13fbd6c");
}

TEST(TWIoTeXStaking, SignUnstake) {
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
    TWData* unstake = TWIoTeXStakingUnstake(1001, nullptr);
    staking->set_data(TWDataBytes(unstake), TWDataSize(unstake));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "784f3d71246dfe897c1cb02da94e8ef1ac2381ac7f25ecfee80eaa78237db95b");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "b93a2874a72ce4eb8a41a20c209cf3fd188671ed8be8239a57960cbed887e962");
}

TEST(TWIoTeXStaking, SignWithdraw) {
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
    TWData* withdraw = TWIoTeXStakingWithdraw(1001, nullptr);
    staking->set_data(TWDataBytes(withdraw), TWDataSize(withdraw));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "ff55882624b2a1d6ae2d9fdec5f8a0f13b2f23c8b28c8ba91773b63f49b97fcc");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "2b2657247a72cb262de214b4e793c7a01fa2139fd5d12a46d43c24f87f9e2396");
}

TEST(TWIoTeXStaking, SignAddStake) {
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
    TWData* addStake = TWIoTeXStakingAddStake(1001, nullptr);
    staking->set_data(TWDataBytes(addStake), TWDataSize(addStake));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "7581e7f779429aa502879581fdc29f87917acfe638069255b6f033c45d7f24fe");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "c71058812a5febe5cdcdaf9499ba0b2c895f88d1acd3203e5097b307c2a5f1d1");
}

TEST(TWIoTeXStaking, SignMoveStake) {
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
    TWData* moveStake = TWIoTeXStakingMoveStake(1001, canName, nullptr);
    staking->set_data(TWDataBytes(moveStake), TWDataSize(moveStake));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "818637b9708ec9e075c7a17f23757cb6895eae6dd3331f7e44129efae6ca9a21");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "33290ded342efaebf795855be73d34cbac149a2415ff9558de10303e6126f30d");
}
