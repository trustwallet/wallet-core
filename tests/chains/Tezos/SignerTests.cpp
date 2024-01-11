// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Tezos/BinaryCoding.h"
#include "Tezos/OperationList.h"
#include "Tezos/Signer.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Tezos::tests {

TEST(TezosSigner, SignString) {
    Data bytesToSign = parse_hex("ffaa");
    Data expectedSignature = parse_hex("eaab7f4066217b072b79609a9f76cdfadd93f8dde41763887e131c02324f18c8e41b1009e334baf87f9d2e917bf4c0e73165622e5522409a0c5817234a48cc02");
    Data expected = Data();
    append(expected, bytesToSign);
    append(expected, expectedSignature);

    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
    auto signedBytes = Signer().signData(key, bytesToSign);

    ASSERT_EQ(signedBytes, expected);
}

TEST(TezosSigner, SignOperationList) {
    auto branch = "BLDnkhhVgwdBAtmDNQc5HtEMsrxq8L3t7NQbjUbbdTdw5Ug1Mpe";
    auto op_list = Tezos::OperationList(branch);

    auto transactionOperationData = new Proto::TransactionOperationData();
    transactionOperationData->set_amount(11100000);
    transactionOperationData->set_destination("tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M");

    auto transactionOperation = TW::Tezos::Proto::Operation();
    transactionOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    transactionOperation.set_fee(1283);
    transactionOperation.set_counter(1878);
    transactionOperation.set_gas_limit(10307);
    transactionOperation.set_storage_limit(0);
    transactionOperation.set_kind(Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);

    op_list.addOperation(transactionOperation);

    PublicKey publicKey = parsePublicKey("edpkuNb9N2UHtGeSc2BZCBHN8ETx7E4DwkSfz5Hw3m3tF3dLZTU8qp");
    auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
    revealOperationData->set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto revealOperation = Proto::Operation();
    revealOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    revealOperation.set_fee(1268);
    revealOperation.set_counter(1876);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(0);
    revealOperation.set_kind(Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);

    op_list.addOperation(revealOperation);

    auto delegateOperationData = new Tezos::Proto::DelegationOperationData();
    delegateOperationData->set_delegate("tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M");

    auto delegateOperation = Proto::Operation();
    delegateOperation.set_source("tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4");
    delegateOperation.set_fee(1257);
    delegateOperation.set_counter(1879);
    delegateOperation.set_gas_limit(10100);
    delegateOperation.set_storage_limit(0);
    delegateOperation.set_kind(Proto::Operation::DELEGATION);
    delegateOperation.set_allocated_delegation_operation_data(delegateOperationData);

    op_list.addOperation(delegateOperation);

    auto decodedPrivateKey = Base58::decodeCheck("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc");
    auto key = PrivateKey(Data(decodedPrivateKey.begin() + 4, decodedPrivateKey.end()));

    std::string expectedForgedBytesToSign = hex(op_list.forge(key));
    std::string expectedSignature = "871693145f2dc72861ff6816e7ac3ce93c57611ac09a4c657a5a35270fa57153334c14cd8cae94ee228b6ef52f0e3f10948721e666318bc54b6c455404b11e03";
    std::string expectedSignedBytes = expectedForgedBytesToSign + expectedSignature;

    auto signedBytes = Signer().signOperationList(key, op_list);
    auto signedBytesHex = hex(signedBytes);

    ASSERT_EQ(hex(signedBytes), expectedSignedBytes);
}

} // namespace TW::Tezos::tests
