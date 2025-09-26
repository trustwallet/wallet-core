// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "TestUtilities.h"
#include "proto/Cosmos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWAnySignerDydx, SignGovernanceProposal) {
    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1234);
    input.set_chain_id("dydx-mainnet-1");
    input.set_memo("");
    input.set_sequence(0);
    input.set_private_key(privateKey.data(), privateKey.size());

    // Create DYDX Governance Proposal message
    auto msg = input.add_messages();
    auto& proposal = *msg->mutable_dydx_governance_proposal();
    proposal.set_title("Title of TextProposal message");
    proposal.set_description("Description of TextProposal message");
    proposal.set_authority("dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("adydx");
    amountOfFee->set_amount("5000000000000000000"); // 5 DYDX

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeDydx);

    // Verify successful signing
    EXPECT_GT(output.signature().size(), 0);
    EXPECT_GT(output.serialized().size(), 0);
    EXPECT_EQ(output.signature().size(), 64);
    EXPECT_EQ(output.error(), Common::Proto::OK);
}

} // namespace TW::Cosmos::tests
