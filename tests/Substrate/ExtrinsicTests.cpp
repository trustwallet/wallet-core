// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "HexCoding.h"
#include "Substrate/Extrinsic.h"
#include "proto/Substrate.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Substrate::tests {

TEST(SubstrateExtrinsic, Polymesh_encodeTransferWithMemo) {
    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_module_index(0x05);
    transfer->set_method_index(0x01);
    transfer->set_to_address("2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF");

    auto value = store(1);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");

    auto result = Substrate::Extrinsic(input).encodeCall();
    EXPECT_EQ(hex(result), "0501004c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e0204014d454d4f20504144444544205749544820535041434553000000000000000000");
}

TEST(SubstrateExtrinsic, Polymesh_encodeAuthorizationJoinIdentity) {
    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* identity = input.mutable_polymesh_call()->mutable_authorization_call()->mutable_join_identity();
    identity->set_module_index(0x07);
    identity->set_method_index(0x0d);
    identity->set_target("2FM6FpjQ6r5HTt7FGYSzskDNkwUyFsonMtwBpsnr9vwmCjhc");

    auto result = Substrate::Extrinsic(input).encodeCall();
    EXPECT_EQ(hex(result), "070d0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320501000100010000");

    auto* authData = identity->mutable_data();
    authData->mutable_asset()->set_data({0x00});
    authData->mutable_extrinsic()->set_data({0x00});
    authData->mutable_portfolio()->set_data({0x00});

    EXPECT_EQ(hex(result), hex(Substrate::Extrinsic(input).encodeCall()));

    // clear data
    authData->clear_asset();
    authData->clear_extrinsic();
    authData->clear_portfolio();

    EXPECT_EQ(hex(Substrate::Extrinsic(input).encodeCall()), "070d0180436894d47a18e0bcfea6940bd90226f7104fbd037a259aeff6b47b8257c1320500000000");
}

TEST(SubstrateExtrinsic, Polymesh_encodeIdentity) {
    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto* key = input.mutable_polymesh_call()->mutable_identity_call()->mutable_join_identity_as_key();
    key->set_module_index(0x07);
    key->set_method_index(0x05);
    key->set_auth_id(4875);

    auto result = Substrate::Extrinsic(input).encodeCall();
    EXPECT_EQ(hex(result), "07050b13000000000000");
}

TEST(SubstrateExtrinsic, Statemint_encodeAssetTransfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2619512-2
    
    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_module_index(0x32);
    transfer->set_method_index(0x05);
    transfer->set_to_address("14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD");

    auto value = store(999500000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);

    auto result = Substrate::Extrinsic(input).encodeCall();
    // clang-format off
    EXPECT_EQ(hex(result), "3205"
                           "011f"
                           "00"
                           "a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d"
                           "82a34cee");
    // clang-format on
}

TEST(SubstrateExtrinsic, Statemint_encodeBatchAssetTransfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2571849-2
    
    Substrate::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);

    auto* transfer = input.mutable_balance_call()->mutable_batch_asset_transfer();
    transfer->set_module_index(0x28);
    transfer->set_method_index(0x00);
    transfer->set_fee_asset_id(0x00);
    auto* t = transfer->add_transfers();
    t->set_to_address("13wQDQTMM6E9g5WD27e6UsWWTwHLaW763FQxnkbVaoKmsBQy");

    auto value = store(808081);
    t->set_module_index(0x32);
    t->set_method_index(0x06);
    t->set_value(std::string(value.begin(), value.end()));
    t->set_asset_id(1984);

    auto result = Substrate::Extrinsic(input).encodeCall();
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

} // namespace TW::Substrate::tests