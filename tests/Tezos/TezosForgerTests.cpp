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

  ASSERT_EQ(ouput, expected)
}

TEST(TWTezosForger, ForgeBoolTrue) {
  auto expected = "00";
  
  auto output = forgeBool(true);
  
  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeBoolFalse) {
  auto expected = "FF";
  
  auto output = forgeBool(false);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_tz1) {
  auto input = "KT1HiGcq47Yek7dcre7So2yfWTLU83FTBaf1";
  auto expected = "0164244bbdc7790d7896b6a52ac49802cfe4eafc4b00";
  
  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeAddress_KT1) {
  auto input = "KT1HiGcq47Yek7dcre7So2yfWTLU83FTBaf1";
  auto expected = "0164244bbdc7790d7896b6a52ac49802cfe4eafc4b00";
  
  auto output = forgeAddress(input);

  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeZarith_small) {
  auto input = "50";
  auto expected = "32";
  
  auto output = forgeAddress(input);
  
  ASSERT_EQ(output, expected);  
}

TEST(TWTezosForger, ForgeZarith_large) {
  auto input = "245";
  auto expected = "f501";
  
  auto output = forgeAddress(input);
  
  ASSERT_EQ(output, expected);  
}


TEST(TWTezosForger, ForgeTransactionOperation) {
  auto transactionOperationData = TW::Tezos::Proto::TransactionOperationData();
  transactionOperationData.set_amount(1);
  transactionOperationData.set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

  auto transactionOperation = TW::Tezos::Proto::Operation();
  transactionOperation.set_fee(1272);
  transactionOperation.set_kind(::TW::Tezos::Proto::Operation_OperationKind_TRANSACTION);
  transactionOperation.set_counter(30738);
  transactionOperation.set_gas_limit(10100);
  transactionOperation.set_storage_limit(257);
  transactionOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
  transactionOperation.set_allocated_transaction_operation_data(&transactionOperationData);

  auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0108000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";

  auto output = forgeOperation(transactionOperation);
    
  ASSERT_EQ(output, expected);    
}