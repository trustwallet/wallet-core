// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ARK/Address.h"
#include "ARK/Signer.h"
#include "ARK/Transaction.h"
#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::ARK;

TEST(ARKSigner, sign) {

    auto key = PrivateKey(parse_hex("d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712"));
    auto tx = Transaction(
        /* type */ 0,
        /* amount */ 123123123,
        /* fee */ 10000000,
        /* timestamp */ 67447770, // unix epoch - 1490101200
        /* */ Address("ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU"),
        /* */ key.getPublicKey(TWPublicKeyTypeSECP256k1).bytes);

    auto output = Signer::sign(key, tx);

    ASSERT_EQ(hex(tx.publicKey),
              "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192");

    ASSERT_EQ(hex(tx.signature),
              "304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc01"
              "5fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3");

    ASSERT_EQ(hex(tx.encoded()),
              "00"
              "da2b0504"
              "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192"
              "176d67bd926e417336456be869bc16a268b3705cbe"
              "000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
              "00000000000000000000000000000000000000000000"
              "b3b5560700000000"
              "8096980000000000"
              "304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc01"
              "5fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3");

    ASSERT_EQ(tx.encodedJson(),
              "{\"amount\":123123123,"
              "\"asset\":{},"
              "\"fee\":10000000,"
              "\"id\":\"219b1cc99ec804df02230a9e913ccb45edb7819f22328e3cd15030174a8c4167\","
              "\"recipientId\":\"ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU\","
              "\"senderPublicKey\":"
              "\"034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192\","
              "\"signature\":"
              "\"304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc"
              "015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3\","
              "\"timestamp\":67447770,"
              "\"type\":0"
              "}");
}
