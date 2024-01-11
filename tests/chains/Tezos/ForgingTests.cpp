// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Tezos/Address.h"
#include "Tezos/BinaryCoding.h"
#include "Tezos/Forging.h"
#include "proto/Tezos.pb.h"
#include <TrustWalletCore/TWCoinType.h>
#include <array>
#include <gtest/gtest.h>
#include <string>

namespace TW::Tezos::tests {

TEST(Forging, ForgeBoolTrue) {
    auto expected = "ff";

    auto output = forgeBool(true);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeBoolFalse) {
    auto expected = "00";

    auto output = forgeBool(false);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithZero) {
    auto expected = "00";

    auto output = forgeZarith(0);

    ASSERT_EQ(hex(output), hex(parse_hex(expected)));
}

TEST(Forging, ForgeZarithTen) {
    auto expected = "0a";

    auto output = forgeZarith(10);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithTwenty) {
    auto expected = "14";

    auto output = forgeZarith(20);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithOneHundredFifty) {
    auto expected = "9601";

    auto output = forgeZarith(150);

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeZarithLarge) {
    auto expected = "bbd08001";

    auto output = forgeZarith(2107451);

    ASSERT_EQ(hex(output), expected);
}

TEST(Forging, forge_tz1) {
    auto expected = "00cfa4aae60f5d9389752d41e320da224d43287fe2";

    auto output = forgePublicKeyHash("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, forge_tz2) {
    auto expected = "01be99dd914e38388ec80432818b517759e3524f16";

    auto output = forgePublicKeyHash("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, forge_tz3) {
    auto expected = "02358cbffa97149631cfb999fa47f0035fb1ea8636";

    auto output = forgePublicKeyHash("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9");

    ASSERT_EQ(output, parse_hex(expected));
}

TEST(Forging, ForgeED25519PublicKey) {
    auto expected = "00311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95";

    auto privateKey = PrivateKey(parse_hex("c6377a4cc490dc913fc3f0d9cf67d293a32df4547c46cb7e9e33c3b7b97c64d8"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    auto output = forgePublicKey(publicKey);

    ASSERT_EQ(hex(output), expected);
}

TEST(Forging, ForgeInt32) {
    auto expected = "01";
    ASSERT_EQ(hex(forgeInt32(1, 1)), expected);
}

TEST(Forging, ForgeString) {
    auto expected = "087472616e73666572";
    ASSERT_EQ(hex(forgeString("transfer", 1)), expected);
}

TEST(Forging, ForgeEntrypoint) {
    auto expected = "ff087472616e73666572";
    ASSERT_EQ(hex(forgeEntrypoint("transfer")), expected);
}

TEST(Forging, ForgeMichelsonFA12) {
    Tezos::Proto::FA12Parameters data;
    data.set_entrypoint("transfer");
    data.set_from("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    data.set_to("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    data.set_value("123");
    auto v = FA12ParameterToMichelson(data);
    ASSERT_EQ(hex(forgeMichelson(v)), "07070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb01");
}
TEST(Forging, ForgeSECP256k1PublicKey) {
    auto expected = "0102b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfe";

    auto privateKey = PrivateKey(parse_hex("3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto output = forgePublicKey(publicKey);

    ASSERT_EQ(hex(output), expected);
}

TEST(TezosTransaction, forgeTransaction) {
    auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
    transactionOperationData->set_amount(1);
    transactionOperationData->set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

    auto transactionOperation = TW::Tezos::Proto::Operation();
    transactionOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transactionOperation.set_fee(1272);
    transactionOperation.set_counter(30738);
    transactionOperation.set_gas_limit(10100);
    transactionOperation.set_storage_limit(257);
    transactionOperation.set_kind(TW::Tezos::Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);

    auto expected = "6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";
    auto serialized = forgeOperation(transactionOperation);

    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosTransaction, forgeTransactionFA12) {
    auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
    transactionOperationData->set_amount(0);
    transactionOperationData->set_destination("KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5");
    transactionOperationData->mutable_parameters()->mutable_fa12_parameters()->set_from("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transactionOperationData->mutable_parameters()->mutable_fa12_parameters()->set_to("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transactionOperationData->mutable_parameters()->mutable_fa12_parameters()->set_entrypoint("transfer");
    transactionOperationData->mutable_parameters()->mutable_fa12_parameters()->set_value("123");

    auto transactionOperation = TW::Tezos::Proto::Operation();
    transactionOperation.set_source("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transactionOperation.set_fee(100000);
    transactionOperation.set_counter(2993172);
    transactionOperation.set_gas_limit(100000);
    transactionOperation.set_storage_limit(0);
    transactionOperation.set_kind(TW::Tezos::Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);

    auto expected = "6c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb01";
    auto serialized = forgeOperation(transactionOperation);

    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosTransaction, forgeTransactionFA2) {
    auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
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

    auto transactionOperation = TW::Tezos::Proto::Operation();
    transactionOperation.set_source("tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP");
    transactionOperation.set_fee(100000);
    transactionOperation.set_counter(2993173);
    transactionOperation.set_gas_limit(100000);
    transactionOperation.set_storage_limit(0);
    transactionOperation.set_kind(TW::Tezos::Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);
    auto serialized = forgeOperation(transactionOperation);
    auto expected = "6c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a";
    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosTransaction, forgeReveal) {
    PublicKey publicKey = parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");

    auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
    revealOperationData->set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto revealOperation = TW::Tezos::Proto::Operation();
    revealOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    revealOperation.set_fee(1272);
    revealOperation.set_counter(30738);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(257);
    revealOperation.set_kind(TW::Tezos::Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);

    auto expected = "6b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
    auto serialized = forgeOperation(revealOperation);

    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosTransaction, forgeDelegate) {
    auto delegateOperationData = new TW::Tezos::Proto::DelegationOperationData();
    delegateOperationData->set_delegate("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");

    auto delegateOperation = TW::Tezos::Proto::Operation();
    delegateOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    delegateOperation.set_fee(1272);
    delegateOperation.set_counter(30738);
    delegateOperation.set_gas_limit(10100);
    delegateOperation.set_storage_limit(257);
    delegateOperation.set_kind(TW::Tezos::Proto::Operation::DELEGATION);
    delegateOperation.set_allocated_delegation_operation_data(delegateOperationData);

    auto expected = "6e0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e8102ff003e47f837f0467b4acde406ed5842f35e2414b1a8";
    auto serialized = forgeOperation(delegateOperation);

    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosTransaction, forgeUndelegate) {
    auto delegateOperationData = new TW::Tezos::Proto::DelegationOperationData();
    delegateOperationData->set_delegate("");

    auto delegateOperation = TW::Tezos::Proto::Operation();
    delegateOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    delegateOperation.set_fee(1272);
    delegateOperation.set_counter(30738);
    delegateOperation.set_gas_limit(10100);
    delegateOperation.set_storage_limit(257);
    delegateOperation.set_kind(TW::Tezos::Proto::Operation::DELEGATION);
    delegateOperation.set_allocated_delegation_operation_data(delegateOperationData);

    auto expected = "6e0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200";
    auto serialized = forgeOperation(delegateOperation);

    ASSERT_EQ(hex(serialized), expected);
}

} // namespace TW::Tezos::tests
