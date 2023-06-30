// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>

#include "Ethereum/RLP.h"
#include "Harmony/Address.h"
#include "Harmony/Signer.h"
#include "HexCoding.h"
#include "proto/Harmony.pb.h"

namespace TW::Harmony {

using namespace boost::multiprecision;

class SignerExposed : public Signer {
  public:
    SignerExposed(uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

static uint256_t MAIN_NET = 0x1;

static uint256_t LOCAL_NET = 0x2;

static uint256_t TEST_AMOUNT = uint256_t("0x4c53ecdc18a60000");

static Address TEST_RECEIVER;
static bool testReceiverDecodeResult =
    Address::decode("one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc", TEST_RECEIVER);

static auto TEST_TRANSACTION = Transaction(/* nonce: */ 0x9,
                                           /* gasPrice: */ 0x0,
                                           /* gasLimit: */ 0x5208,
                                           /* fromShardID */ 0x1,
                                           /* toShardID */ 0x0,
                                           /* to: */ TEST_RECEIVER,
                                           /* amount: */ TEST_AMOUNT,
                                           /* payload: */ {});

TEST(HarmonySigner, RLPEncodingAndHashAssumeLocalNet) {
    auto rlpUnhashedShouldBe = "e909808252080180946a87346f3ba9958d08d09484a"
                               "2b7fdbbe42b0df6884c53ecdc18a6000080028080";
    auto rlpHashedShouldBe = "610238ad72e4492af494f49bf5d92"
                             "13626a0ee5adb8256bb2558e990ee4da8f0";
    auto signer = SignerExposed(LOCAL_NET);
    auto rlpHex = signer.txnAsRLPHex(TEST_TRANSACTION);
    auto hash = signer.hash(TEST_TRANSACTION);

    ASSERT_EQ(rlpHex, rlpUnhashedShouldBe);
    ASSERT_EQ(hex(hash), rlpHashedShouldBe);
}

TEST(HarmonySigner, SignAssumeLocalNet) {
    auto key =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    auto signer = SignerExposed(LOCAL_NET);

    uint256_t v("0x28");
    uint256_t r("0x325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d");
    uint256_t s("0x6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b");

    auto transaction = Transaction(TEST_TRANSACTION);
    auto hash = signer.hash(transaction);

    signer.sign(key, hash, transaction);

    ASSERT_EQ(transaction.v, v);
    ASSERT_EQ(transaction.r, r);
    ASSERT_EQ(transaction.s, s);
}

TEST(HarmonySigner, SignProtoBufAssumeLocalNet) {
    auto input = Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();

    trasactionMsg->set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(LOCAL_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0x9"));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x4c53ecdc18a60000"));
    trasactionMsg->set_amount(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto shouldBeV = "28";
    auto shouldBeR = "325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d";
    auto shouldBeS = "6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b";

    ASSERT_EQ(hex(proto_output.v()), shouldBeV);
    ASSERT_EQ(hex(proto_output.r()), shouldBeR);
    ASSERT_EQ(hex(proto_output.s()), shouldBeS);
}

TEST(HarmonySigner, SignOverProtoBufAssumeMainNet) {
    auto input = Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();
    trasactionMsg->set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());
    
    value = store(uint256_t("0xa"));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x4c53ecdc18a60000"));
    trasactionMsg->set_amount(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto expectEncoded = "f8690a808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a"
                         "600008026a074acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b1543045053667"
                         "08a0616ab8262ee6f6fb30ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a";

    auto v = "26";
    auto r = "74acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b154304505366708";
    auto s = "616ab8262ee6f6fb30ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonySigner, BuildSigningOutput) {
    auto input = Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();
    trasactionMsg->set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0xa"));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x4c53ecdc18a60000"));
    trasactionMsg->set_amount(value.data(), value.size());

    Data signature = parse_hex("74acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b154304505366708616ab8262ee6f6fb30ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a01");

    Signer signer(uint256_t(load(input.chain_id())));
    auto proto_output = signer.buildSigningOutput(input, signature);

    auto expectEncoded = "f8690a808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a"
                         "600008026a074acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b1543045053667"
                         "08a0616ab8262ee6f6fb30ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a";

    auto v = "26";
    auto r = "74acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b154304505366708";
    auto s = "616ab8262ee6f6fb30ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonySigner, BuildUnsignedTxBytes) {
    auto input = Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();
    trasactionMsg->set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0xa"));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x4c53ecdc18a60000"));
    trasactionMsg->set_amount(value.data(), value.size());

    Signer signer(uint256_t(load(input.chain_id())));
    auto unsignedTxBytes = signer.buildUnsignedTxBytes(input);

    auto expectEncoded = "e90a808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a6000080018080";

    ASSERT_EQ(hex(unsignedTxBytes), expectEncoded);
}

TEST(HarmonySigner, BuildUnsignedStakingTxBytes) {
    auto input = Proto::SigningInput();
    auto stakingMsg = input.mutable_staking_message();
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0xa"));
    stakingMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    stakingMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    stakingMsg->set_gas_limit(value.data(), value.size());

    // delegate message
    auto delegateMsg = stakingMsg->mutable_delegate_message();
        delegateMsg->set_delegator_address(TEST_RECEIVER.string());
    delegateMsg->set_validator_address(TEST_RECEIVER.string());

    value = store(uint256_t("0x4c53ecdc18a60000"));
    delegateMsg->set_amount(value.data(), value.size());

    Signer signer(uint256_t(load(input.chain_id())));
    auto unsignedTxBytes = signer.buildUnsignedTxBytes(input);

    auto expectEncoded = "f83d02f3946a87346f3ba9958d08d09484a2b7fdbbe42b0df6946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600000a80825208018080";

    ASSERT_EQ(hex(unsignedTxBytes), expectEncoded);
}
} // namespace TW::Harmony
