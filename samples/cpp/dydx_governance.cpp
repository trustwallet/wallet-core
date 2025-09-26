// Copyright © 2017-2025 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include <iostream>

using namespace TW;

int main() {
    std::cout << "=== DYDX Governance Proposal Example ===" << std::endl;

    // Create signing input
    Cosmos::Proto::SigningInput input;
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_account_number(1234);
    input.set_chain_id("dydx-mainnet-1");
    input.set_memo("");
    input.set_sequence(0);

    // Set private key (example - in practice this should be loaded securely)
    const auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    input.set_private_key(privateKey.data(), privateKey.size());

    // Set fee
    auto fee = input.mutable_fee();
    fee->set_gas(200000);
    auto amount = fee->add_amounts();
    amount->set_denom("adydx");
    amount->set_amount("5000000000000000000"); // 5 DYDX

    // Create DYDX Governance Proposal message
    auto message = input.add_messages();
    auto* proposal = message->mutable_dydx_governance_proposal();
    proposal->set_title("Title of test proposal");
    proposal->set_description("Description of the test proposal");
    proposal->set_authority("dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky");

    // Sign the transaction
    auto result = TWAnySignerSign(input, TWCoinTypeDydx);

    if (!result.empty()) {
        Cosmos::Proto::SigningOutput output;
        output.ParseFromString(result);

        std::cout << "Signature: " << hex(output.signature()) << std::endl;
        std::cout << "Broadcast JSON: " << output.serialized() << std::endl;
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failed to sign transaction" << std::endl;
        return 1;
    }

    return 0;
}
