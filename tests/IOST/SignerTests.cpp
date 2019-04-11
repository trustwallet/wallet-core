// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Base58.h"
#include "proto/IOST.pb.h"
#include "IOST/Signer.h"
#include <gtest/gtest.h>

#include <iostream>

namespace TW::IOST {

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

    Data secKeyBytes = parse_hex("63095105a37b4e896e5ebbd740e751c6f9df7cca2410beba3261dc5680299cebe812b52ea9ad5cba9a9af03afcc6f2942a4524b0df3c0344dc195072831670c4");
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

    Signer signer;
    std::string signature = signer.sign(input).transaction().publisher_sigs(0).signature();

    ASSERT_EQ(hex(signature), "e8ce15214bad39683021c15dd318e963da8541fd8f3d8484df5042b4ea7fdafb7f46505b85841367d6e1736c7d3b433ca72089b88a23f43661dfb0429a10cb03");
}
} // namespace TW::IOST 

