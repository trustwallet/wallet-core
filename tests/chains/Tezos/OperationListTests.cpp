// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Tezos/Address.h"
#include "Tezos/BinaryCoding.h"
#include "Tezos/OperationList.h"
#include "proto/Tezos.pb.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Tezos::tests {

TEST(TezosOperationList, ForgeBranch) {
    auto input = TW::Tezos::OperationList("BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW");
    auto expected = "da8eb4f57f98a647588b47d29483d1edfdbec1428c11609cee0da6e0f27cfc38";

    ASSERT_EQ(input.forgeBranch(), parse_hex(expected));
}

TEST(TezosOperationList, ForgeOperationList_TransactionOnly) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = TW::Tezos::OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");

    auto transactionOperationData = new Proto::TransactionOperationData();
    transactionOperationData->set_amount(1);
    transactionOperationData->set_destination("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt");

    auto transactionOperation = Proto::Operation();
    transactionOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transactionOperation.set_fee(1272);
    transactionOperation.set_counter(30738);
    transactionOperation.set_gas_limit(10100);
    transactionOperation.set_storage_limit(257);
    transactionOperation.set_kind(Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);

    op_list.addOperation(transactionOperation);

    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";
    auto forged = op_list.forge(key);
    ASSERT_EQ(hex(forged), expected);
}

TEST(TezosOperationList, ForgeOperationList_RevealOnly) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = TW::Tezos::OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");
    PublicKey publicKey = parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");

    auto revealOperationData = new Proto::RevealOperationData();
    revealOperationData->set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto revealOperation = Proto::Operation();
    revealOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    revealOperation.set_fee(1272);
    revealOperation.set_counter(30738);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(257);
    revealOperation.set_kind(TW::Tezos::Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);

    op_list.addOperation(revealOperation);
    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
    auto forged = op_list.forge(key);

    ASSERT_EQ(hex(forged), expected);
}

TEST(TezosOperationList, ForgeOperationList_Delegation_ClearDelegate) {
    auto branch = "BLGJfQDFEYZBRLj5GSHskj8NPaRYhk7Kx5WAfdcDucD3q98WdeW";
    auto op_list = OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");
    auto delegationOperationData = new Proto::DelegationOperationData();
    delegationOperationData->set_delegate("tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M");

    auto delegationOperation = Proto::Operation();
    delegationOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    delegationOperation.set_fee(1257);
    delegationOperation.set_counter(67);
    delegationOperation.set_gas_limit(10000);
    delegationOperation.set_storage_limit(0);
    delegationOperation.set_kind(Proto::Operation::DELEGATION);
    delegationOperation.set_allocated_delegation_operation_data(delegationOperationData);

    op_list.addOperation(delegationOperation);

    auto expected = "48b63d801fa824013a195f7885ba522503c59e0580f7663e15c52f03ccc935e66e003e47f837f0467b4acde406ed5842f35e2414b1a8e90943904e00ff00e42504da69a7c8d5baeaaeebe157a02db6b22ed8";
    ASSERT_EQ(hex(op_list.forge(key)), expected);
}

TEST(TezosOperationList, ForgeOperationList_Delegation_AddDelegate) {
    auto branch = "BLa4GrVQTxUgQWbHv6cF7RXWSGzHGPbgecpQ795R3cLzw4cGfpD";
    auto op_list = OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");
    auto delegationOperationData = new Proto::DelegationOperationData();
    delegationOperationData->set_delegate("tz1dYUCcrorfCoaQCtZaxi1ynGrP3prTZcxS");

    auto delegationOperation = Proto::Operation();
    delegationOperation.set_source("KT1D5jmrBD7bDa3jCpgzo32FMYmRDdK2ihka");
    delegationOperation.set_fee(1257);
    delegationOperation.set_counter(68);
    delegationOperation.set_gas_limit(10000);
    delegationOperation.set_storage_limit(0);
    delegationOperation.set_kind(Proto::Operation::DELEGATION);
    delegationOperation.set_allocated_delegation_operation_data(delegationOperationData);

    op_list.addOperation(delegationOperation);
    auto expected = "7105102c032807994dd9b5edf219261896a559876ca16cbf9d31dbe3612b89f26e00315b1206ec00b1b1e64cc3b8b93059f58fa2fc39e90944904e00ff00c4650fd609f88c67356e5fe01e37cd3ff654b18c";
    auto forged = op_list.forge(key);
    ASSERT_EQ(hex(forged), expected);
}

TEST(TezosOperationList, ForgeOperationList_TransactionAndReveal) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");
    auto publicKey = parsePublicKey("edpkuNb9N2UHtGeSc2BZCBHN8ETx7E4DwkSfz5Hw3m3tF3dLZTU8qp");

    auto revealOperationData = new Proto::RevealOperationData();
    revealOperationData->set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto revealOperation = Proto::Operation();
    revealOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    revealOperation.set_fee(1272);
    revealOperation.set_counter(30738);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(257);
    revealOperation.set_kind(Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);

    auto transactionOperationData = new Proto::TransactionOperationData();
    transactionOperationData->set_amount(1);
    transactionOperationData->set_destination("tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M");

    auto transactionOperation = Proto::Operation();
    transactionOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    transactionOperation.set_fee(1272);
    transactionOperation.set_counter(30739);
    transactionOperation.set_gas_limit(10100);
    transactionOperation.set_storage_limit(257);
    transactionOperation.set_kind(Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);

    op_list.addOperation(revealOperation);
    op_list.addOperation(transactionOperation);

    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b003e47f837f0467b4acde406ed5842f35e2414b1a8f80992f001f44e810200603247bbf52501498293686da89ad8b2aca85f83b90903d4521dd2aba66054eb6c003e47f837f0467b4acde406ed5842f35e2414b1a8f80993f001f44e8102010000e42504da69a7c8d5baeaaeebe157a02db6b22ed800";
    auto forged = op_list.forge(key);

    ASSERT_EQ(hex(forged), expected);
}

TEST(TezosOperationList, ForgeOperationList_RevealWithoutPublicKey) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);
    auto key = parsePrivateKey("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");

    auto revealOperationData = new Proto::RevealOperationData();

    auto revealOperation = Proto::Operation();
    revealOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    revealOperation.set_fee(1272);
    revealOperation.set_counter(30738);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(257);
    revealOperation.set_kind(Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);

    op_list.addOperation(revealOperation);

    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b003e47f837f0467b4acde406ed5842f35e2414b1a8f80992f001f44e810200603247bbf52501498293686da89ad8b2aca85f83b90903d4521dd2aba66054eb";
    auto forged = op_list.forge(key);

    ASSERT_EQ(hex(forged), expected);
}

} // namespace TW::Tezos::tests
