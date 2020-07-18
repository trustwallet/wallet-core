// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Filecoin/Address.h"
#include "Filecoin/Transaction.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::Filecoin {

TEST(FilecoinTransaction, Serialize) {
    const PrivateKey privateKey(
        parse_hex("2f0f1d2c8de955c7c3fb4d9cae02539fadcb13fa998ccd9a1e871bed95f1941e"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const Address fromAddress(publicKey);
    const Address toAddress("f1hvadvq4rd2pyayrigjx2nbqz2nvemqouslw4wxi");

    Transaction tx(toAddress, fromAddress,
                   /*nonce*/ 0x1234567890,
                   /*value*/ 1000,
                   /*gasPrice*/ 1,
                   /*gasLimit*/ 50);

    ASSERT_EQ(hex(tx.message().encoded()),
              "890055013d403ac3911e9f806228326fa68619d36a4641d455013d413d4c3fe3"
              "d89f99495a48c6046224a71f0cd71b0000001234567890430003e842000118320040");
    ASSERT_EQ(hex(tx.cid()),
              "0171a0e40220bebd2d8facba996f5e04e7c64e1b0f088c63078263a4587b838ed3573419d743");
}

} // namespace TW::Filecoin
