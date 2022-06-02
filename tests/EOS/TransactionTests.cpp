// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EOS/Transaction.h"
#include "EOS/Signer.h"
#include "EOS/Action.h"
#include "EOS/Address.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "Hash.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::EOS;

static std::string k1Sigs[5] {
    "SIG_K1_KfCdjsrTnx5cBpbA5cUdHZAsRYsnC9uKzuS1shFeqfMCfdZwX4PBm9pfHwGRT6ffz3eavhtkyNci5GoFozQAx8P8PBnDmj",
    "SIG_K1_K6wW678ngyWT7fgR4nNqm5XoKZBp9NDN4tKsctyzzADjXn15iAH9tcnQ393t6uvsqYxHKjdnxxduT1nyKMLiZbRqL7dHYr",
    "SIG_K1_K6cUbZX6xfWcV5iotVprnf12Lc5AmV8SKmN5hVdv39gcM8wfEcwcNScvTuGLWpWzDT463dyhNmUfMB4nqt7tJVFnzx8mSi",
    "SIG_K1_Khj7xhMd8HxrT6dUzuwiFM1MfMHtog5jCygJj7ADvdmUGkzZkmjymZXucEAud3whJ2qsMcxHcKtLWs8Ndm6be14qjTAH2a",
    "SIG_K1_K93MjjE39CSH7kwJBgoRsSF2VaH6a8psQKU29nSg4xxxrVhz2iQuubyyB5t2ACZFFYSkNHSdYia5efhnW6Z9SPtbQTquMY"
};

static std::string r1Sigs[5] { 
    "SIG_R1_Kbywy4Mjun4Ymrh23Xk5yRtKJxcDWaDjQjLKERAny6Vs6oT1DYoEdoAj9AJK9iukHdEd9HBYnf3GmLtA55JFY5VaNszJMa",
    "SIG_R1_KAhJJg4QGYBWY7hG6BKGAbW57fg6g8xTh3LG3Sss3bGv4BwiwHmRV1jsgh6hrnVRUoCaKMbJQzzWy9HXy6PnDmfJ6fbZMJ",
    "SIG_R1_KxAwVKfpLr2MeK4aSAp5LSi2Vohsp94Uhk5UvZZDUJqd7ccBkhc2kYY1L6z5rjRNNo7BeP1Qr6H2xPFqo54YQ6DjczAqLW",
    "SIG_R1_K1isJT8pJhkrHi3mcvrfY12nY6jirMCWaAHWuBXvu2ondcm3QHkgdaTwERskftZ9cqB5k2r8ajoYS4VWsiivjbd56D6pxX",
    "SIG_R1_KWtgvnj2LaaYdtBTjM7bTR23LPBytDHFE7gPEfGTZ7PWc4yc6piPuPUHsVJVkvKmpW2gEUhq3toCfjkt34itSxMgekovdG"
};

TEST(EOSTransaction, Serialization) {
    ASSERT_THROW(TransferAction("token", "eosio", "token", Asset::fromString("-20.1234 TKN"), "my first transfer"), std::invalid_argument);

    Data referenceBlockId = parse_hex("000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c");
    int32_t referenceBlockTime = 1554121728;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");

    Transaction tx {referenceBlockId, referenceBlockTime};
    tx.actions.push_back(TransferAction("token", "eosio", "token", Asset::fromString("20.1234 TKN"), "my first transfer"));
    ASSERT_EQ(tx.actions.back().serialize().dump(), "{\"account\":\"token\",\"authorizations\":[{\"actor\":\"eosio\",\"permission\":\"active\"}],\"data\":\"0000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e73666572\",\"name\":\"transfer\"}");

    Data buf;
    tx.serialize(buf);

    Signer signer {chainId};

    ASSERT_EQ(
        hex(buf),
        "1e04a25cdc46a452d92c00000000010000000080a920cd000000572d3ccdcd010000000000ea305500000000a8ed3232320000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e7366657200"
    );

    ASSERT_EQ(
        hex(signer.hash(tx)),
        "5de974bb90b940b462688609735a1dd522fa853aba765c30d14bedd27d719dd1"
    );

    // make transaction invalid and see if signing succeeds
    tx.maxNetUsageWords = UINT32_MAX;
    ASSERT_THROW(signer.sign(PrivateKey(Hash::sha256(std::string("A"))), Type::ModernK1, tx), std::invalid_argument);

    referenceBlockId = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    referenceBlockTime = 1554209118;

    Transaction tx2 {referenceBlockId, referenceBlockTime};
    tx2.actions.push_back(TransferAction("token", "token", "eosio", Asset::fromString("30.0000 TKN"), "my second transfer"));

    buf.clear();
    tx2.serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "7c59a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200"
    );

    ASSERT_EQ(
        hex(signer.hash(tx2)),
        "4dac38a8ad7f095a09ec0eb0cbd060c9d8ea0a842535d369c9ce526cdf1b5d85"
    );

    ASSERT_NO_THROW(tx2.serialize());

    // verify k1 sigs
    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernK1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            k1Sigs[i]
        );
    }

    // verify r1 sigs
    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernR1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            r1Sigs[i]
        );
    }
}

TEST(EOSTransaction, formatDate) {
    EXPECT_EQ(Transaction::formatDate(1554209148), "2019-04-02T12:45:48");
    EXPECT_EQ(Transaction::formatDate(1654160000), "2022-06-02T08:53:20");
    EXPECT_EQ(Transaction::formatDate(0), "1970-01-01T00:00:00");
    EXPECT_EQ(Transaction::formatDate(std::numeric_limits<int32_t>::max()), "2038-01-19T03:14:07");
}
