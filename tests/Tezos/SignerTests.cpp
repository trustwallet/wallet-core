// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/OperationList.h"
#include "Tezos/Signer.h"
#include "Tezos/Transaction.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosSigner, SignString) {
    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));

    auto signature = Signer().signHexString(key, "ffaa");
    auto expected = Data({234, 171, 127, 64, 102, 33, 123, 7, 43, 121, 96, 154, 159, 118, 205, 250, 221, 147, 248, 221, 228, 23, 99, 136, 126, 19, 28, 2, 50, 79, 24, 200, 228, 27, 16, 9, 227, 52, 186, 248, 127, 157, 46, 145, 123, 244, 192, 231, 49, 101, 98, 46, 85, 34, 64, 154, 12, 88, 23, 35, 74, 72, 204, 2});
    ASSERT_EQ(signature, expected);
}

TEST(TezosSigner, SignOperationList) {
  auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
  auto op_list = OperationList(branch);
  auto tx1 = Transaction(
    Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
    1272,
    30738,
    10100,
    257,
    1,
    PublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"),
    OperationKind::REVEAL
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
    OperationKind::TRANSACTION
  );
  op_list.add_operation(tx2);

  auto expected = Data({217, 36, 203, 62, 86, 196, 185, 245, 94, 80, 115, 94, 70, 24, 153, 162, 246, 22, 162, 107, 251, 10, 160, 93, 11, 53, 107, 102, 245, 23, 176, 35, 223, 51, 10, 211, 98, 31, 11, 243, 157, 81, 129, 49, 161, 190, 205, 106, 123, 46, 34, 110, 210, 145, 72, 58, 243, 104, 37, 53, 209, 244, 83, 15});
  auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));

  auto signature = Signer().signOperationList(key, op_list);
  ASSERT_EQ(signature, expected);
}
