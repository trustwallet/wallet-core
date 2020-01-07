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
#include "TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::IoTeX;

static const char *_Nonnull IOTEX_STAKING_CONTRACT = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
static const char * IOTEX_STAKING_TEST = "this is a test";

TEST(TWIoTeXStaking, Stake) {
    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto candidate = WRAPD(TWDataCreateWithBytes(name, 12));

    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    auto stake = WRAPD(TWIoTeXStakingStake(candidate.get(), 1001, true, test.get()));

    auto result = dataFromTWData(stake.get());

    ASSERT_EQ(hex(*result), "07c35fc00102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");
}

TEST(TWIoTeXStaking, Unstake) {
    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    auto unstake = WRAPD(TWIoTeXStakingUnstake(1001, test.get()));

    auto result = dataFromTWData(unstake.get());

    ASSERT_EQ(hex(*result), "c8fd6ed000000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");
}

TEST(TWIoTeXStaking, Withdraw) {

    auto test = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    auto withdraw = WRAPD(TWIoTeXStakingWithdraw(1001, test.get()));

    auto result = dataFromTWData(withdraw.get());

    ASSERT_EQ(hex(*result), "030ba25d00000000000000000000000000000000000000000000000000000000000003e90000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000000e7468697320697320612074657374000000000000000000000000000000000000");
}

TEST(TWIoTeXStaking, AddStake) {

    auto test = WRAPD(TWDataCreateWithSize(0));
    auto add = WRAPD(TWIoTeXStakingAddStake(1001, test.get()));

    auto result = dataFromTWData(add.get());

    ASSERT_EQ(hex(*result), "6e7b301700000000000000000000000000000000000000000000000000000000000003e900000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000000");
}

TEST(TWIoTeXStaking, MoveStake) {

    byte name[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto candidate = TWDataCreateWithBytes(name, 12);
    auto test = WRAPD(TWDataCreateWithSize(0));
    auto add = WRAPD(TWIoTeXStakingMoveStake(1001, candidate, test.get()));

    auto result = dataFromTWData(add.get());

    ASSERT_EQ(hex(*result), "d3e41fd200000000000000000000000000000000000000000000000000000000000003e90102030405060708090a0b0c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000");
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
    auto candidate = WRAPD(TWDataCreateWithBytes(name, 12));
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)IOTEX_STAKING_TEST, 14));
    // data = "this is a test" here, it could be null (user leaves data empty when signing the tx)
    auto stake = WRAPD(TWIoTeXStakingStake(candidate.get(), 1001, true, data.get()));
    staking->set_data(TWDataBytes(stake.get()), TWDataSize(stake.get()));
    
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
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto unstake = WRAPD(TWIoTeXStakingUnstake(1001, data.get()));
    staking->set_data(TWDataBytes(unstake.get()), TWDataSize(unstake.get()));
    
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
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto withdraw = WRAPD(TWIoTeXStakingWithdraw(1001, data.get()));
    staking->set_data(TWDataBytes(withdraw.get()), TWDataSize(withdraw.get()));
    
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
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto addStake = WRAPD(TWIoTeXStakingAddStake(1001, data.get()));
    staking->set_data(TWDataBytes(addStake.get()), TWDataSize(addStake.get()));
    
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
    auto candidate = WRAPD(TWDataCreateWithBytes(name, 12));
    auto data = WRAPD(TWDataCreateWithSize(0));
    auto moveStake = WRAPD(TWIoTeXStakingMoveStake(1001, candidate.get(), data.get()));
    staking->set_data(TWDataBytes(moveStake.get()), TWDataSize(moveStake.get()));
    
    auto signer = IoTeX::Signer(std::move(input));
    // raw action's hash
    ASSERT_EQ(hex(signer.hash()), "818637b9708ec9e075c7a17f23757cb6895eae6dd3331f7e44129efae6ca9a21");
    // build() signs the tx
    auto output = signer.build();
    // signed action's hash
    ASSERT_EQ(hex(output.hash()), "33290ded342efaebf795855be73d34cbac149a2415ff9558de10303e6126f30d");
}
