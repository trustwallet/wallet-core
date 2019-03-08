// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/Address.h"
#include "Tezos/BinaryCoding.h"
#include "Tezos/OperationList.h"
#include "Tezos/Transaction.h"

#include <gtest/gtest.h>
using namespace TW::Tezos;

TEST(TezosOperationList, ForgeBranch) {
    auto input = OperationList("BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW");
    auto expected = "da8eb4f57f98a647588b47d29483d1edfdbec1428c11609cee0da6e0f27cfc38";

    ASSERT_EQ(input.forgeBranch(), expected);
}

TEST(TezosOperationList, ForgeOperationList_TransactionOnly) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);

    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        Address("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt"),
        operationtype::TRANSACTION
    );

    op_list.add_operation(tx1);

    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0108000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";

    ASSERT_EQ(op_list.forge(), expected);
}

TEST(TezosOperationList, ForgeOperationList_RevealOnly) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);

    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"),
        operationtype::REVEAL
      );

      op_list.add_operation(tx1);
      auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
      ASSERT_EQ(op_list.forge(), expected);
}

TEST(TezosOperationList, ForgeOperationList_TransactionAndReveal) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);
    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"),
        operationtype::REVEAL
    );
    op_list.add_operation(tx1);
    auto tx2 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30739,
        10100,
        257,
        1,
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        operationtype::TRANSACTION
    );
    op_list.add_operation(tx2);

    auto expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b9572100";
    ASSERT_EQ(op_list.forge(), expected);
}
