// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Tezos/BinaryCoding.h"
#include "proto/Tezos.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Tezos::tests {

TEST(TWAnySignerTezos, SignFA12) {
    // https://ghostnet.tzkt.io/ooTBu7DLbeC7DmVfXEsp896A6WTwimedbsM9QRqUVtqA8Vxt6D3/2993172
    auto key = parse_hex("363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6");

    Proto::SigningInput input;
    input.set_private_key(key.data(), key.size());
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp");

    auto& transaction = *operations.add_operations();
    auto& txData = *transaction.mutable_transaction_operation_data();
    txData.set_amount(0);
    txData.set_destination("KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5");
    txData.mutable_parameters()->mutable_fa12_parameters()->set_entrypoint("transfer");
    txData.mutable_parameters()->mutable_fa12_parameters()->set_from("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    txData.mutable_parameters()->mutable_fa12_parameters()->set_to("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    txData.mutable_parameters()->mutable_fa12_parameters()->set_value("123");
    transaction.set_source("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transaction.set_fee(100000);
    transaction.set_counter(2993172);
    transaction.set_gas_limit(100000);
    transaction.set_storage_limit(0);
    transaction.set_kind(Proto::Operation::TRANSACTION);
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTezos);
    ASSERT_EQ(hex(output.encoded()), "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb012914d768155fba2df319a81136e8e3e573b9cadb1676834490c90212615d271da029b6b0531e290e9063bcdb40bea43627af048b18e036f02be2b6b22fc8b307");
}

TEST(TWAnySignerTezos, SignFA2) {
    // https://ghostnet.tzkt.io/onxLBoPaf23M3A8kHTwncSFG2GVXPfnGXUhkC8BhKj8QDdCEbng
    auto key = parse_hex("363265a0b3f06661001cab8b4f3ca8fd97ae70608184979cf7300836f57ec2d6");

    Proto::SigningInput input;
    input.set_private_key(key.data(), key.size());
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BKvEAX9HXfJZWYfTQbR1C7B3ADoKY6a1aKVRF7qQqvc9hS8Rr3m");

    auto& transaction = *operations.add_operations();

    auto* transactionOperationData = transaction.mutable_transaction_operation_data();
    transactionOperationData->set_amount(0);
    transactionOperationData->set_destination("KT1DYk1XDzHredJq1EyNkDindiWDqZyekXGj");

    auto& fa2 = *transactionOperationData->mutable_parameters()->mutable_fa2_parameters();
    fa2.set_entrypoint("transfer");
    auto& txObject = *fa2.add_txs_object();
    txObject.set_from("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    auto& tx = *txObject.add_txs();
    tx.set_amount("10");
    tx.set_token_id("0");
    tx.set_to("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");

    transaction.set_source("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transaction.set_fee(100000);
    transaction.set_counter(2993173);
    transaction.set_gas_limit(100000);
    transaction.set_storage_limit(0);
    transaction.set_kind(Proto::Operation::TRANSACTION);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTezos);
    ASSERT_EQ(hex(output.encoded()), "1b1f9345dc9f77bd24b09034d1d2f9a28f02ac837f49db54b8d68341f53dc4b76c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a552d24710d6c59383286700c6c2917b25a6c1fa8b587e593c289dd47704278796792f1e522c1623845ec991e292b0935445e6994850bd03f035a006c5ed93806");
}

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

} // namespace TW::Tezos::tests
