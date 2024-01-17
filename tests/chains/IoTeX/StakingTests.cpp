// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "HexCoding.h"
#include "IoTeX/Staking.h"
#include "proto/IoTeX.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

namespace TW::IoTeX::tests {

TEST(TWIoTeXStaking, Create) {
    std::string IOTEX_STAKING_CANDIDATE = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    std::string IOTEX_STAKING_AMOUNT = "100";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());

    auto stake = stakingCreate(candidate, amount, 10000, true, payload);
    ASSERT_EQ(hex(stake), "0a29696f313964307033616834673877773964376b63786671383779786537666e723872"
                          "7074683573686a120331303018904e20012a077061796c6f6164");
}

TEST(TWIoTeXStaking, AddDeposit) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    std::string IOTEX_STAKING_AMOUNT = "10";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());

    auto stake = stakingAddDeposit(10, amount, payload);

    ASSERT_EQ(hex(stake), "080a120231301a077061796c6f6164");
}

TEST(TWIoTeXStaking, Unstake) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingUnstake(10, payload);

    ASSERT_EQ(hex(stake), "080a12077061796c6f6164");
}

TEST(TWIoTeXStaking, Withdraw) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingWithdraw(10, payload);

    ASSERT_EQ(hex(stake), "080a12077061796c6f6164");
}

TEST(TWIoTeXStaking, Restake) {
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingRestake(10, 1000, true, payload);

    ASSERT_EQ(hex(stake), "080a10e807180122077061796c6f6164");
}

TEST(TWIoTeXStaking, ChangeCandidate) {
    std::string IOTEX_STAKING_CANDIDATE = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingChangeCandidate(10, candidate, payload);

    ASSERT_EQ(hex(stake), "080a1229696f3178707136326177383575717a72636367397935686e727976386c"
                          "64326e6b7079636333677a611a077061796c6f6164");
}

TEST(TWIoTeXStaking, Transfer) {
    std::string IOTEX_STAKING_CANDIDATE = "io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data candidate(IOTEX_STAKING_CANDIDATE.begin(), IOTEX_STAKING_CANDIDATE.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake = stakingTransfer(10, candidate, payload);

    ASSERT_EQ(hex(stake), "080a1229696f3178707136326177383575717a72636367397935686e727976386c6432"
                          "6e6b7079636333677a611a077061796c6f6164");
}

TEST(TWIoTeXStaking, CandidateRegister) {
    std::string IOTEX_STAKING_NAME = "test";
    std::string IOTEX_STAKING_OPERATOR = "io10a298zmzvrt4guq79a9f4x7qedj59y7ery84he";
    std::string IOTEX_STAKING_REWARD = "io13sj9mzpewn25ymheukte4v39hvjdtrfp00mlyv";
    std::string IOTEX_STAKING_OWNER = "io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj";
    std::string IOTEX_STAKING_AMOUNT = "100";
    std::string IOTEX_STAKING_PAYLOAD = "payload";
    Data name(IOTEX_STAKING_NAME.begin(), IOTEX_STAKING_NAME.end());
    Data operatorAddress(IOTEX_STAKING_OPERATOR.begin(), IOTEX_STAKING_OPERATOR.end());
    Data reward(IOTEX_STAKING_REWARD.begin(), IOTEX_STAKING_REWARD.end());
    Data amount(IOTEX_STAKING_AMOUNT.begin(), IOTEX_STAKING_AMOUNT.end());
    Data owner(IOTEX_STAKING_OWNER.begin(), IOTEX_STAKING_OWNER.end());
    Data payload(IOTEX_STAKING_PAYLOAD.begin(), IOTEX_STAKING_PAYLOAD.end());

    auto stake =
        candidateRegister(name, operatorAddress, reward, amount, 10000, false, owner, payload);

    ASSERT_EQ(hex(stake),
              "0a5c0a04746573741229696f3130613239387a6d7a7672743467757137396139663478377165646a3539"
              "7937657279383468651a29696f3133736a396d7a7065776e3235796d6865756b74653476333968766a64"
              "7472667030306d6c7976120331303018904e2a29696f313964307033616834673877773964376b637866"
              "71383779786537666e7238727074683573686a32077061796c6f6164");
}

TEST(TWIoTeXStaking, CandidateUpdate) {
    std::string IOTEX_STAKING_NAME = "test";
    std::string IOTEX_STAKING_OPERATOR = "io1cl6rl2ev5dfa988qmgzg2x4hfazmp9vn2g66ng";
    std::string IOTEX_STAKING_REWARD = "io1juvx5g063eu4ts832nukp4vgcwk2gnc5cu9ayd";
    Data name(IOTEX_STAKING_NAME.begin(), IOTEX_STAKING_NAME.end());
    Data operatorAddress(IOTEX_STAKING_OPERATOR.begin(), IOTEX_STAKING_OPERATOR.end());
    Data reward(IOTEX_STAKING_REWARD.begin(), IOTEX_STAKING_REWARD.end());

    auto stake = candidateUpdate(name, operatorAddress, reward);

    ASSERT_EQ(hex(stake), "0a04746573741229696f31636c36726c32657635646661393838716d677a6732783468"
                          "66617a6d7039766e326736366e671a29696f316a757678356730363365753474733833"
                          "326e756b7034766763776b32676e6335637539617964");
}

Proto::SigningInput createSigningInput() {
    auto keyhex = parse_hex("cfa6ef757dee2e50351620dca002d32b9c090cfda55fb81f37f1d26b273743f1");
    auto input = Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(0);
    input.set_gaslimit(1000000);
    input.set_gasprice("10");
    input.set_privatekey(keyhex.data(), keyhex.size());
    return input;
}

TEST(TWIoTeXStaking, SignAll) {
    { // sign stakecreate
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakecreate();
        action.set_candidatename("io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj");
        action.set_stakedamount("100");
        action.set_stakedduration(10000);
        action.set_autostake(true);
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(hex(output.encoded()),
                  "0a4b080118c0843d22023130c2023e0a29696f313964307033616834673877773964376b63786671"
                  "3837797865"
                  "37666e7238727074683573686a120331303018904e20012a077061796c6f6164124104755ce6d890"
                  "3f6b3793bd"
                  "db4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c"
                  "0bc76ef30d"
                  "d6a1038ed9da8daf331a412e8bac421bab88dcd99c26ac8ffbf27f11ee57a41e7d2537891bfed5ae"
                  "d8e2e026d4"
                  "6e55d1b856787bc1cd7c1216a6e2534c5b5d1097c3afe8e657aa27cbbb0801");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "f1785e47b4200c752bb6518bd18097a41e075438b8c18c9cb00e1ae2f38ce767");
    }
    { // sign stakeadddeposit
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakeadddeposit();
        action.set_bucketindex(10);
        action.set_amount("10");
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a1c080118c0843d22023130da020f080a120231301a077061796c6f6164124104755ce6d8903f6b3793"
            "bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0b"
            "c76ef30dd6a1038ed9da8daf331a41a48ab1feba8181d760de946aefed7d815a89fd9b1ab503d2392bb5"
            "5e1bb75eec42dddc8bd642f89accc3a37b3cf15a103a95d66695fdf0647b202869fdd66bcb01");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "ca8937d6f224a4e4bf93cb5605581de2d26fb0481e1dfc1eef384ee7ccf94b73");
    }
    { // sign stakeunstake
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakeunstake();
        action.set_bucketindex(10);
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a18080118c0843d22023130ca020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5"
            "d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30d"
            "d6a1038ed9da8daf331a4100adee39b48e1d3dbbd65298a57c7889709fc4df39987130da306f6997374a"
            "184b7e7c232a42f21e89b06e6e7ceab81303c6b7483152d08d19ac829b22eb81e601");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "bed58b64a6c4e959eca60a86f0b2149ce0e1dd527ac5fd26aef725ebf7c22a7d");
    }
    { // sign stakewithdraw
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakewithdraw();
        action.set_bucketindex(10);
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a18080118c0843d22023130d2020b080a12077061796c6f6164124104755ce6d8903f6b3793bddb4ea5"
            "d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30d"
            "d6a1038ed9da8daf331a4152644d102186be6640d46b517331f3402e24424b0d85129595421d28503d75"
            "340b2922f5a0d4f667bbd6f576d9816770286b2ce032ba22eaec3952e24da4756b00");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "28049348cf34f1aa927caa250e7a1b08778c44efaf73b565b6fa9abe843871b4");
    }
    { // sign stakerestake
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakerestake();
        action.set_bucketindex(10);
        action.set_stakedduration(1000);
        action.set_autostake(true);
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a1d080118c0843d22023130e20210080a10e807180122077061796c6f6164124104755ce6d8903f6b37"
            "93bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c"
            "0bc76ef30dd6a1038ed9da8daf331a41e2e763aed5b1fd1a8601de0f0ae34eb05162e34b0389ae3418ee"
            "dbf762f64959634a968313a6516dba3a97b34efba4753bbed3a33d409ecbd45ac75007cd8e9101");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "8816e8f784a1fce40b54d1cd172bb6976fd9552f1570c73d1d9fcdc5635424a9");
    }
    { // sign stakechangecandidate
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_stakechangecandidate();
        action.set_bucketindex(10);
        action.set_candidatename("io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza");
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a43080118c0843d22023130ea0236080a1229696f3178707136326177383575717a7263636739793568"
            "6e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4e"
            "a5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef3"
            "0dd6a1038ed9da8daf331a41d519eb3747163b945b862989b7e82a7f8468001e9683757cb88d5ddd95f8"
            "1895047429e858bd48f7d59a88bfec92de231d216293aeba1e4fbe11461d9c9fc99801");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "186526b5b9fe74e25beb52c83c41780a69108160bef2ddaf3bffb9f1f1e5e73a");
    }
    { // sign staketransfer
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_staketransferownership();
        action.set_bucketindex(10);
        action.set_voteraddress("io1xpq62aw85uqzrccg9y5hnryv8ld2nkpycc3gza");
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a43080118c0843d22023130f20236080a1229696f3178707136326177383575717a7263636739793568"
            "6e727976386c64326e6b7079636333677a611a077061796c6f6164124104755ce6d8903f6b3793bddb4e"
            "a5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef3"
            "0dd6a1038ed9da8daf331a41fa26db427ab87a56a129196c1604f2e22c4dd2a1f99b2217bc916260757d"
            "00093d9e6dccdf53e3b0b64e41a69d71c238fbf9281625164694a74dfbeba075d0ce01");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "74b2e1d6a09ba5d1298fa422d5850991ae516865077282196295a38f93c78b85");
    }
    { // sign candidateupdate
        auto input = createSigningInput();
        Proto::SigningOutput output;
        auto& action = *input.mutable_candidateupdate();
        action.set_name("test");
        action.set_operatoraddress("io1cl6rl2ev5dfa988qmgzg2x4hfazmp9vn2g66ng");
        action.set_rewardaddress("io1juvx5g063eu4ts832nukp4vgcwk2gnc5cu9ayd");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(
            hex(output.encoded()),
            "0a69080118c0843d2202313082035c0a04746573741229696f31636c36726c3265763564666139383871"
            "6d677a673278346866617a6d7039766e326736366e671a29696f316a7576783567303633657534747338"
            "33326e756b7034766763776b32676e6335637539617964124104755ce6d8903f6b3793bddb4ea5d3589d"
            "637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99a5c1335b583c0bc76ef30dd6a103"
            "8ed9da8daf331a4101885c9c6684a4a8f2f5bf11f8326f27be48658f292e8f55ec8a11a604bb0c563a11"
            "ebf12d995ca1c152e00f8e0f0edf288db711aa10dbdfd5b7d73b4a28e1f701");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "ca1a28f0e9a58ffc67037cc75066dbdd8e024aa2b2e416e4d6ce16c3d86282e5");
    }
    { // sign candidateregister
        auto input = createSigningInput();
        Proto::SigningOutput output;
        input.set_gasprice("1000");
        auto& cbi = *input.mutable_candidateregister()->mutable_candidate();
        cbi.set_name("test");
        cbi.set_operatoraddress("io10a298zmzvrt4guq79a9f4x7qedj59y7ery84he");
        cbi.set_rewardaddress("io13sj9mzpewn25ymheukte4v39hvjdtrfp00mlyv");

        auto& action = *input.mutable_candidateregister();
        action.set_stakedamount("100");
        action.set_stakedduration(10000);
        action.set_autostake(false);
        action.set_owneraddress("io19d0p3ah4g8ww9d7kcxfq87yxe7fnr8rpth5shj");
        action.set_payload("payload");
        ANY_SIGN(input, TWCoinTypeIoTeX);
        ASSERT_EQ(hex(output.encoded()),
                  "0aaa01080118c0843d220431303030fa029a010a5c0a04746573741229696f3130613239387a6d7a"
                  "7672743467"
                  "757137396139663478377165646a35397937657279383468651a29696f3133736a396d7a7065776e"
                  "3235796d68"
                  "65756b74653476333968766a647472667030306d6c7976120331303018904e2a29696f3139643070"
                  "3361683467"
                  "3877773964376b63786671383779786537666e7238727074683573686a32077061796c6f61641241"
                  "04755ce6d8"
                  "903f6b3793bddb4ea5d3589d637de2d209ae0ea930815c82db564ee8cc448886f639e8a0c7e94e99"
                  "a5c1335b58"
                  "3c0bc76ef30dd6a1038ed9da8daf331a417819b5bcb635e3577acc8ca757f2c3d6afa451c2b6ff8a"
                  "9179b141ac"
                  "68e2c50305679e5d09d288da6f0fb52876a86c74deab6a5247edc6d371de5c2f121e159400");
        // signed action's hash
        ASSERT_EQ(hex(output.hash()),
                  "35f53a536e014b32b85df50483ef04849b80ad60635b3b1979c5ba1096b65237");
    }
}

} // namespace TW::IoTeX::tests
