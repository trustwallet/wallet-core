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

TEST(SubstrateExtrinsic, Polymesh_encodeCall) {
    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);

    auto *transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_module_index(0x05);
    transfer->set_method_index(0x01);
    transfer->set_to_address("2EB7wW2fYfFskkSx2d65ivn34ewpuEjcowfJYBL79ty5FsZF");

    auto value = store(1);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");

    auto result = Substrate::Extrinsic::encodeCall(input);
    EXPECT_EQ(hex(result), "050100"
                            "4c6c63e3dc083959f876788716b78885460b5f3c7ed9379f8d5f408e08639e02"
                            "04"
                            "01"
                            "4d454d4f20504144444544205749544820535041434553202020202020202020");
}

} // namespace TW::Substrate::tests