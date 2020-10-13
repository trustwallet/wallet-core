// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/NEAR.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(TWAnySignerNEAR, Sign) {

    auto privateKey = parse_hex("8737b99bf16fba78e1e753e23ba00c4b5423ac9c45d9b9caae9a519434786568");
    auto blockHash = parse_hex("0fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6");
    // uint128_t / little endian byte order
    auto deposit = parse_hex("01000000000000000000000000000000");

    Proto::SigningInput input;
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    
    auto& action = *input.add_actions();
    auto& transfer = *action.mutable_transfer();
    transfer.set_deposit(deposit.data(), deposit.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    ASSERT_EQ(hex(output.signed_transaction()), "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef601000000030100000000000000000000000000000000969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c4f22deeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01");
}

} // namespace TW::NEAR
