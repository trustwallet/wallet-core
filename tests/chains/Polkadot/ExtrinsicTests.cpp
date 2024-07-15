// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "HexCoding.h"
#include "Polkadot/Extrinsic.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Polkadot::tests {

TEST(PolkadotExtrinsic, Polymesh_encodeTransferWithMemo) {
    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_to_address("2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF");
    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x05);
    callIndices->set_method_index(0x01);

    auto value = store(1);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");

    auto result = Polkadot::Extrinsic(input).encodeCall(input);
    EXPECT_EQ(hex(result), "0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000");
}

TEST(PolkadotExtrinsic, Polymesh_encodeAuthorizationJoinIdentity) {
    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* identity = input.mutable_polymesh_call()->mutable_identity_call()->mutable_add_authorization();
    identity->set_target("2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc");
    auto* authCallIndices = identity->mutable_call_indices()->mutable_custom();
    authCallIndices->set_module_index(0x07);
    authCallIndices->set_method_index(0x0d);

    auto result = Polkadot::Extrinsic(input).encodeCall(input);
    EXPECT_EQ(hex(result), "070d0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000");

    auto* authData = identity->mutable_data();
    authData->mutable_asset()->set_data({0x00});
    authData->mutable_extrinsic()->set_data({0x00});
    authData->mutable_portfolio()->set_data({0x00});

    EXPECT_EQ(hex(result), hex(Polkadot::Extrinsic(input).encodeCall(input)));

    // clear data
    authData->clear_asset();
    authData->clear_extrinsic();
    authData->clear_portfolio();

    EXPECT_EQ(hex(Polkadot::Extrinsic(input).encodeCall(input)), "070d0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000");
}

TEST(PolkadotExtrinsic, Polymesh_encodeIdentity) {
    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* key = input.mutable_polymesh_call()->mutable_identity_call()->mutable_join_identity_as_key();
    key->set_auth_id(4875);
    auto* callIndices = key->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x07);
    callIndices->set_method_index(0x05);

    auto result = Polkadot::Extrinsic(input).encodeCall(input);
    EXPECT_EQ(hex(result), "07050b13000000000000");
}

TEST(PolkadotExtrinsic, Statemint_encodeAssetTransfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2619512-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD");
    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x32);
    callIndices->set_method_index(0x05);

    auto value = store(999500000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);

    auto result = Polkadot::Extrinsic(input).encodeCall(input);
    // clang-format off
    EXPECT_EQ(hex(result), "3205"
                           "011f"
                           "00"
                           "a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d"
                           "82a34cee");
    // clang-format on
}

TEST(PolkadotExtrinsic, Statemint_encodeBatchAssetTransfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2571849-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_batch_asset_transfer();
    auto* batchCallIndices = transfer->mutable_call_indices()->mutable_custom();
    batchCallIndices->set_module_index(0x28);
    batchCallIndices->set_method_index(0x00);
    transfer->set_fee_asset_id(0x00);

    auto* t = transfer->add_transfers();
    t->set_to_address("13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy");
    auto value = store(808081);
    t->set_value(std::string(value.begin(), value.end()));
    t->set_asset_id(1984);

    auto* transferCallIndices = t->mutable_call_indices()->mutable_custom();
    transferCallIndices->set_module_index(0x32);
    transferCallIndices->set_method_index(0x06);

    auto result = Polkadot::Extrinsic(input).encodeCall(input);
    // clang-format off
    EXPECT_EQ(hex(result), "2800"
                           "04"
                           "3206"
                           "011f"
                           "00"
                           "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91"
                           "46523100");
    // clang-format on
}

TEST(PolkadotExtrinsic, Kusama_encodeAssetTransferNoCallIndices) {
    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_batch_asset_transfer();
    transfer->set_fee_asset_id(0x00);

    auto* t = transfer->add_transfers();
    t->set_to_address("13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy");
    auto value = store(808081);
    t->set_value(std::string(value.begin(), value.end()));
    t->set_asset_id(1984);

    EXPECT_THROW(Polkadot::Extrinsic(input).encodeCall(input), std::invalid_argument);
}

TEST(PolkadotExtrinsic, Polkadot_EncodePayloadWithNewSpec) {
    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD");
    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x32);
    callIndices->set_method_index(0x05);

    auto value = store(999500000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);

    input.set_spec_version(1002000);  // breaking change happens at version 1002005
    auto result = Polkadot::Extrinsic(input).encodePayload();
    EXPECT_EQ(hex(result), "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee00000000104a0f0000000000");

    input.set_spec_version(1002005);  // >= 1002005
    result = Polkadot::Extrinsic(input).encodePayload();
    EXPECT_EQ(hex(result), "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee0000000000154a0f000000000000");

    input.set_spec_version(1002006);  // >= 1002005
    result = Polkadot::Extrinsic(input).encodePayload();
    EXPECT_EQ(hex(result), "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee0000000000164a0f000000000000");
}


} // namespace TW::Polkadot::tests
