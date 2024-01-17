// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "IOST/Signer.h"
#include "proto/IOST.pb.h"
#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::IOST;

TEST(IOSTSigner, Sign) {
    auto tx = Proto::Transaction();
    tx.set_time(1550137587000000000);
    tx.set_expiration(tx.time() + int64_t(1000000000) * 300);
    tx.set_gas_ratio(1);
    tx.set_gas_limit(1000000);
    tx.set_chain_id(1024);

    tx.add_amount_limit();
    auto amountLimit = tx.mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");

    Data secKeyBytes = parse_hex("63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe"
                                 "812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
    std::string secKey(secKeyBytes.begin(), secKeyBytes.end());
    Proto::AccountInfo account;
    account.set_active_key(secKey);
    account.set_name("myname");

    Proto::SigningInput input;
    input.mutable_account()->CopyFrom(account);
    input.mutable_transaction_template()->CopyFrom(tx);
    input.set_transfer_destination("admin");
    input.set_transfer_amount("10");
    input.set_transfer_memo("");

    Signer signer(input);
    std::string signature = signer.sign(input).transaction().publisher_sigs(0).signature();

    ASSERT_EQ(hex(signature), "e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46"
                              "505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03");
}

TEST(IOSTSigner, EncodeTransaction) {
    auto tx = Proto::Transaction();
    tx.set_time(1550137587000000000);
    tx.set_expiration(tx.time() + int64_t(1000000000) * 300);
    tx.set_gas_ratio(1);
    tx.set_gas_limit(1000000);
    tx.set_chain_id(1024);

    tx.add_amount_limit();
    auto amountLimit = tx.mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");

    Data secKeyBytes = parse_hex("63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe"
                                 "812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
    std::string secKey(secKeyBytes.begin(), secKeyBytes.end());
    Proto::AccountInfo account;
    account.set_active_key(secKey);
    account.set_name("myname");

    tx.add_signers();
    *tx.mutable_signers(0) = secKey;

    const auto signature =
        parse_hex("1e5e2de66512658e9317fa56766678166abcf492d020863935723db2030f736710e13437cef0981f"
                  "cc376eae45349759508767d407b6c9963712910ada2c3606");

    tx.add_signatures();
    auto* sig = tx.mutable_signatures(0);
    sig->set_algorithm(Proto::Algorithm::ED25519);
    sig->set_public_key(secKey);
    sig->set_signature(std::string(signature.begin(), signature.end()));

    auto encoded = Signer::encodeTransaction(tx);
    ASSERT_EQ(hex(encoded), "158331ec221dbe0015833231fb82760000000000000000640000000005f5e10000000000000000000000040000000000000000010000004063095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4000000000000000100000012000000012a00000009756e6c696d69746564000000010000008902000000401e5e2de66512658e9317fa56766678166abcf492d020863935723db2030f736710e13437cef0981fcc376eae45349759508767d407b6c9963712910ada2c36060000004063095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
}
