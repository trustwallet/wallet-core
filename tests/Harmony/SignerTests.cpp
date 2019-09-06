// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyChainID.h>
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

static uint256_t MAIN_NET = TWHarmonyChainIDMainNet;

static uint256_t LOCAL_NET = 0x2;

static uint256_t TEST_AMOUNT = uint256_t("0x6bfc8da5ee8220000");

static auto TEST_RECEIVER = Address("one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k");

static auto TEST_TRANSACTION = Transaction(/* nonce: */ 0x1,
                                           /* gasPrice: */ 0x0,
                                           /* gasLimit: */ 0x5208,
                                           /* fromShardID */ 0x1,
                                           /* toShardID */ 0x0,
                                           /* to: */ TEST_RECEIVER,
                                           /* amount: */ TEST_AMOUNT,
                                           /* payload: */ {});

TEST(HarmonySigner, RLPEncodingAndHashAssumeLocalNet) {
    auto rlp_unhashed_should_be = "ea0180825208018094514650ca30b3c79f693e14220"
                                  "115434236d44aeb8906bfc8da5ee822000080028080";
    auto rlp_hashed_should_be = "d239531674f7f2bfbdb1603c14c12999"
                                "eb412a4a3b696deafef9c7ee204ffc87";
    auto signer = SignerExposed(LOCAL_NET);
    auto rlp_hex = signer.txnAsRLPHex(TEST_TRANSACTION);
    auto hash = signer.hash(TEST_TRANSACTION);

    ASSERT_EQ(hex(hash), rlp_hashed_should_be);
    ASSERT_EQ(rlp_hex, rlp_unhashed_should_be);
}

TEST(HarmonySigner, SignAssumeLocalNet) {
    auto key =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    auto signer = SignerExposed(LOCAL_NET);

    uint256_t should_be_v("0x28");
    uint256_t should_be_r("0x84cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5c");
    uint256_t should_be_s("0x643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc");

    auto transaction = Transaction(TEST_TRANSACTION);

    signer.sign(key, transaction);

    ASSERT_EQ(transaction.v, should_be_v);
    ASSERT_EQ(transaction.r, should_be_r);
    ASSERT_EQ(transaction.s, should_be_s);
}

TEST(HarmonySigner, SignProtoBufAssumeLocalNet) {
    auto input = Proto::SigningInput();
    input.set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    auto payload = parse_hex("");
    input.set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(LOCAL_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    input.set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x6bfc8da5ee8220000"));
    input.set_amount(value.data(), value.size());

    auto proto_output = TW::Harmony::Signer::sign(input);

    auto shouldBeV = "28";
    auto shouldBeR = "84cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5c";
    auto shouldBeS = "643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc";

    ASSERT_EQ(hex(proto_output.v()), shouldBeV);
    ASSERT_EQ(hex(proto_output.r()), shouldBeR);
    ASSERT_EQ(hex(proto_output.s()), shouldBeS);
}

TEST(HarmonySigner, SignOverProtoBufAssumeMainNet) {
    auto input = Proto::SigningInput();
    input.set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    auto payload = parse_hex("");
    input.set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    input.set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x6bfc8da5ee8220000"));
    input.set_amount(value.data(), value.size());

    auto proto_output = TW::Harmony::Signer::sign(input);

    auto should_be_encoded = "f86a0180825208018094514650ca30b3c79f693e14220115434236d44aeb8906bfc8d"
                             "a5ee82200008026a05da3c716bf28756a"
                             "56593a6bef7169c8ec553a0121743a5e3a5b8e9ad587fc67a07b6a9cb0e421ac90ffa"
                             "f8ade9de5996bc20e880756b44685c0abbf1b2c266462";
    auto should_be_v = "26";
    auto should_be_r = "5da3c716bf28756a56593a6bef7169c8ec553a0121743a5e3a5b8e9ad587fc67";
    auto should_be_s = "7b6a9cb0e421ac90ffaf8ade9de5996bc20e880756b44685c0abbf1b2c266462";

    ASSERT_EQ(hex(proto_output.encoded()), should_be_encoded);
    ASSERT_EQ(hex(proto_output.v()), should_be_v);
    ASSERT_EQ(hex(proto_output.r()), should_be_r);
    ASSERT_EQ(hex(proto_output.s()), should_be_s);
}
} // namespace TW::Harmony
