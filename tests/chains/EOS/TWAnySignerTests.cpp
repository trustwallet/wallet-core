// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "proto/EOS.pb.h"

#include <gtest/gtest.h>

namespace TW::EOS::tests {

TEST(TWAnySignerEOS, Sign) {
    Proto::SigningInput input;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");
    auto refBlock = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    auto key = parse_hex("559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd");

    auto& asset = *input.mutable_asset();
    asset.set_amount(300000);
    asset.set_decimals(4);
    asset.set_symbol("TKN");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_reference_block_id(refBlock.data(), refBlock.size());
    input.set_reference_block_time(1554209118);
    input.set_currency("token");
    input.set_sender("token");
    input.set_recipient("eosio");
    input.set_memo("my second transfer");
    input.set_private_key(key.data(), key.size());
    input.set_private_key_type(Proto::KeyType::MODERNK1);

    {
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEOS);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.json_encoded(), R"({"compression":"none","packed_context_free_data":"","packed_trx":"6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200","signatures":["SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7"]})");
    }

    input.set_private_key_type(Proto::KeyType::LEGACY);
    {
        Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeEOS);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_TRUE(output.json_encoded().empty());
    }
}

} // namespace TW::EOS::tests
