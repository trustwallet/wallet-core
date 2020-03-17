// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Tezos.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Tezos;

TEST(TWAnySignerTezos, Sign) {
    auto key = parse_hex("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f");
    auto revealKey = parse_hex("311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95");

    Proto::SigningInput input;
    input.set_private_key(key.data(), key.size());
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp");

    auto& reveal = *operations.add_operations();
    auto& revealData = *reveal.mutable_reveal_operation_data();
    revealData.set_public_key(revealKey.data(), revealKey.size());
    reveal.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    reveal.set_fee(1272);
    reveal.set_counter(30738);
    reveal.set_gas_limit(10100);
    reveal.set_storage_limit(257);
    reveal.set_kind(Proto::Operation::REVEAL);

    auto& transaction = *operations.add_operations();
    auto& txData = *transaction.mutable_transaction_operation_data();
    txData.set_amount(1);
    txData.set_destination("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_fee(1272);
    transaction.set_counter(30739);
    transaction.set_gas_limit(10100);
    transaction.set_storage_limit(257);
    transaction.set_kind(Proto::Operation::TRANSACTION);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTezos);

    EXPECT_EQ(hex(output.encoded()), "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05");
}

TEST(TWAnySignerTezos, SignJSON) {
    auto json = STRING(R"({"operationList": {"branch": "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp","operations": [{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30738,"gasLimit": 10100,"storageLimit": 257,"kind": 107,"revealOperationData": {"publicKey": "QpqYbIBypAofOj4qtaWBm7Gy+2mZPFAEg3gVudxVkj4="}},{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30739,"gasLimit": 10100,"storageLimit": 257,"kind": 108,"transactionOperationData": {"destination": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","amount": 1}}]}})");
    auto key = DATA("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeTezos));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeTezos));
    assertStringsEqual(result, "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b957210001b86398d5b9be737dca8e4106ea18d70e69b75e92f892fb283546a99152b8d7794b919c0fbf1c31de386069a60014491c0e7505adef5781cead1cfe6608030b");
}
