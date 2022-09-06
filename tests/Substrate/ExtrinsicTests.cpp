// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Substrate/Extrinsic.h"
#include "Data.h"
#include "HexCoding.h"
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
    EXPECT_EQ(hex(result), "050100"
                            "4c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e02"
                            "04"
                            "01"
                            "4d454d4f20504144444544205749544820535041434553202020202020202020");
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

} // namespace TW::Substrate::tests