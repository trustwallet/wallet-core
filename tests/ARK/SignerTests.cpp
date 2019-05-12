// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "PublicKey.h"
#include <gtest/gtest.h>
#include "PrivateKey.h"
#include "Data.h"
#include "HexCoding.h"
#include "ARK/Transaction.h"
#include "ARK/Signer.h"
#include "ARK/Address.h"

using namespace TW;
using namespace TW::ARK;

TEST(ARKSigner, sign){
    std::string s = "d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712";
    PrivateKey priv = PrivateKey(parse_hex(s));

    Transaction tx = Transaction();
    tx.type = 0;

    uint32_t t = 67447770+1490101200;
    tx.setTimestamp(t);
    ASSERT_EQ(tx.getTimestamp(), 67447770);

    tx.amount = 123123123;
    tx.fee = 10000000;
    tx.recipientId = Address("ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU");
    
    ARK::Signer::sign(priv, tx);

    std::string expected_sig = "304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3";
    
    ASSERT_EQ(tx.publicKey, parse_hex("034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192"));
    ASSERT_EQ(hex(tx.encoded()), "00"
    "da2b0504"
    "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192"
    "176d67bd926e417336456be869bc16a268b3705cbe"
    "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "b3b5560700000000"
    "8096980000000000"
    "304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3");
    ASSERT_EQ(tx.signature, parse_hex(expected_sig));
    std::string id = hex(Hash::sha256(tx.encoded()));
    ASSERT_EQ(tx.toJson(), "{\"amount\":123123123,"
    "\"asset\":{},"
    "\"fee\":10000000,"
    "\"id\":\"219b1cc99ec804df02230a9e913ccb45edb7819f22328e3cd15030174a8c4167\","
    "\"recipientId\":\"ARkMaRcjcwRgr6vmDtAWo7bFqUgy9wG3NU\","
    "\"senderPublicKey\":\"034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192\","
    "\"signature\":\"304402205e6365f4c3b49c28f03afd89d308736dca56671ea707dd3dd5af42272a0cc8ed02207fa7fc015fba7ae527d22a058cc4ebd8e9867c563ace7effc2dbaad2af8976c3\","
    "\"timestamp\":67447770,"
    "\"type\":0"
    "}");
}