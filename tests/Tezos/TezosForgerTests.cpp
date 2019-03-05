// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/TWTezosForger.h"

#include <gtest/gtest.h>

TEST(TWTezosForger, ForgeBranch) {
  auto input = "BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW";
  auto expected = "da8eb4f57f98a647588b47d29483d1edfdbec1428c11609cee0da6e0f27cfc38";

  auto output = forgeBranch(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeBoolTrue) {
  auto expected = "ff";

  auto output = forgeBool(true);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeBoolFalse) {
  auto expected = "00";

  auto output = forgeBool(false);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_tz1) {
  auto input = "tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don";
  auto expected = "0000cfa4aae60f5d9389752d41e320da224d43287fe2";

  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_tz2) {
  auto input = "tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo";
  auto expected = "0001be99dd914e38388ec80432818b517759e3524f16";

  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_tz3) {
  auto input = "tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9";
  auto expected = "0002358cbffa97149631cfb999fa47f0035fb1ea8636";

  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_KT1) {
  auto input = "KT1HiGcq47Yek7dcre7So2yfWTLU83FTBaf1";
  auto expected = "0164244bbdc7790d7896b6a52ac49802cfe4eafc4b00";

  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgePublicKey) {
  auto input = "edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A";
  auto expected = "00429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";

  auto output = forgePublicKey(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeTransactionOperation) {
  auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
  transactionOperationData->set_amount(1);
  transactionOperationData->set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

  auto transactionOperation = TW::Tezos::Proto::Operation();
  transactionOperation.set_fee(1272);
  transactionOperation.set_kind(::TW::Tezos::Proto::Operation_OperationKind_TRANSACTION);
  transactionOperation.set_counter(30738);
  transactionOperation.set_gas_limit(10100);
  transactionOperation.set_storage_limit(257);
  transactionOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);
  auto expected = "08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";
  auto output = forgeOperation(transactionOperation);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeRevealOperation) {
  auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
  revealOperationData->set_public_key("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");

  auto revealOperation = TW::Tezos::Proto::Operation();
  revealOperation.set_fee(1272);
  revealOperation.set_kind(::TW::Tezos::Proto::Operation_OperationKind_REVEAL);
  revealOperation.set_counter(30738);
  revealOperation.set_gas_limit(10100);
  revealOperation.set_storage_limit(257);
  revealOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  revealOperation.set_allocated_reveal_operation_data(revealOperationData);

  auto expected = "07000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";

  auto output = forgeOperation(revealOperation);
  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeOperationList_TransactionOnly) {
  auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
  auto operationList = TW::Tezos::Proto::OperationList();
  auto transactionOperation = operationList.add_operations();

  auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
  transactionOperationData->set_amount(1);
  transactionOperationData->set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

  transactionOperation -> set_fee(1272);
  transactionOperation -> set_kind(::TW::Tezos::Proto::Operation_OperationKind_TRANSACTION);
  transactionOperation -> set_counter(30738);
  transactionOperation -> set_gas_limit(10100);
  transactionOperation -> set_storage_limit(257);
  transactionOperation -> set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  transactionOperation -> set_allocated_transaction_operation_data(transactionOperationData);

  auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0108000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";

  auto output = forgeOperationList(operationList);
  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeOperationList_RevealOnly) {
  auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
  auto operationList = TW::Tezos::Proto::OperationList();
  auto revealOperation = operationList.add_operations();

  auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
  revealOperationData->set_public_key("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");

  revealOperation -> set_fee(1272);
  revealOperation -> set_kind(::TW::Tezos::Proto::Operation_OperationKind_REVEAL);
  revealOperation -> set_counter(30738);
  revealOperation -> set_gas_limit(10100);
  revealOperation -> set_storage_limit(257);
  revealOperation -> set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  revealOperation -> set_allocated_reveal_operation_data(revealOperationData);

  auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";

  auto output = forgeOperationList(operationList);
  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeOperationList_TransactionAndReveal) {
  auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
  auto operationList = TW::Tezos::Proto::OperationList();

  auto revealOperation = operationList.add_operations();

  auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
  revealOperationData->set_public_key("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");

  revealOperation -> set_fee(1272);
  revealOperation -> set_kind(::TW::Tezos::Proto::Operation_OperationKind_REVEAL);
  revealOperation -> set_counter(30739);
  revealOperation -> set_gas_limit(10100);
  revealOperation -> set_storage_limit(257);
  revealOperation -> set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  revealOperation -> set_allocated_reveal_operation_data(revealOperationData);

  auto transactionOperation = operationList.add_operations();

  auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
  transactionOperationData->set_amount(1);
  transactionOperationData->set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

  transactionOperation -> set_fee(1272);
  transactionOperation -> set_kind(::TW::Tezos::Proto::Operation_OperationKind_TRANSACTION);
  transactionOperation -> set_counter(30738);
  transactionOperation -> set_gas_limit(10100);
  transactionOperation -> set_storage_limit(257);
  transactionOperation -> set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  transactionOperation -> set_allocated_transaction_operation_data(transactionOperationData);

  auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b9572100";

  auto output = forgeOperationList(operationList);
  ASSERT_EQ(output, expected);
}
