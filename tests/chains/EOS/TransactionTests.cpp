// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EOS/Action.h"
#include "EOS/Address.h"
#include "EOS/Signer.h"
#include "EOS/Transaction.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::EOS::tests {

static std::string k1Sigs[5]{
    "SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7",
    "SIG_K1_K4oXhfa8xJnAB26EeTz8FLfZtY4kw4kjqUqQLz5snryP7USRJ7yGyuBBfYzTBtZ8djBo87pAW53xHsUxjvQvaKHGQRhKd5",
    "SIG_K1_K2BumXj5Qtk2CtaYe6EvMSZ2JXLCFeNsTQirx4cnZFURBFpkchuS4sNFxGLH5Qrdv4R7cN4rax6ZF9HBMz4f4d6GFoTNN2",
    "SIG_K1_K8LNseWYiePTM646LZduWevssozJ9t3gaNe2ipZfXbbSFsx36dJFXnk5UBasT2G3cX1Niu7LSUVFspkDYSSPxMFGbWcAvk",
    "SIG_K1_Jx2JFftzdx28PZXkmoeWk2afm3KHYsn5knYxynA3GrGevMEJVd1GhcuS5h3f2wdUS2ZUojqycyyVizyJFVSajeR2LZGnJr"};

static std::string r1Sigs[5]{
    "SIG_R1_KC4qBxibZpXLESzN37F46Jv8w7dQtyPDeRmeFs8Z4DmEFhr3FACLkjcbSLkVN7acBt4eb6zUa9N76UfJncr4htxCSe7huZ",
    "SIG_R1_KaWNQefJReykjKUsS51caChJRgeywUoeuucFReyKY1SPNveSoFFVgxT3jEzW56ZLtpN7qGgekoSNsKs1BzzyZ9snhCALcG",
    "SIG_R1_KarN7JJxHeKgRJLmscWzCsdDpfdktWrBGJLvVFN7RYZpSeNHBsqNV7dKqxkvKtbhsqHukLKw1EQNTjcUcxUD6hUTVvNWcP",
    "SIG_R1_KvHdcwEDW8RQWEPTA3BoK9RVZqtAwKqVvYQN9Wz44XUrzjrNyRkpc7vguqc8q6FLMJBUUen59hyXM3BuLvvrp2x4S6m1o8",
    "SIG_R1_KZB6ivprUS1zwGxMxZQJ7UvWk4Tpvoo6WiFKUPJuUHj4Es39ejiFVoD7ZB6MfSJxAaRPvnAF39hnApwzFAM8Erxmj3Suvm"};

TEST(EOSTransaction, Serialization) {
    ASSERT_THROW(TransferAction("token", "eosio", "token", Asset::fromString("-20.1234 TKN"), "my first transfer"), std::invalid_argument);

    Data referenceBlockId = parse_hex("000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c");
    int32_t referenceBlockTime = 1554121728;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");

    Transaction tx{referenceBlockId, referenceBlockTime};
    tx.actions.push_back(TransferAction("token", "eosio", "token", Asset::fromString("20.1234 TKN"), "my first transfer"));
    ASSERT_EQ(tx.actions.back().serialize().dump(), "{\"account\":\"token\",\"authorizations\":[{\"actor\":\"eosio\",\"permission\":\"active\"}],\"data\":\"0000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e73666572\",\"name\":\"transfer\"}");

    Data buf;
    tx.serialize(buf);

    Signer signer{chainId};

    ASSERT_EQ(
        hex(buf),
        "1012a25cdc46a452d92c00000000010000000080a920cd000000572d3ccdcd010000000000ea305500000000a8ed3232320000000000ea30550000000080a920cd121203000000000004544b4e00000000116d79206669727374207472616e7366657200");

    ASSERT_EQ(
        hex(signer.hash(tx)),
        "acbf7e6beb6fd4e224462e87c1d70bca6a15b76f8d9e0b31782c5cdfd493b050");

    // make transaction invalid and see if signing succeeds
    tx.maxNetUsageWords = UINT32_MAX;
    ASSERT_THROW(signer.sign(PrivateKey(Hash::sha256(std::string("A"))), Type::ModernK1, tx), std::invalid_argument);

    referenceBlockId = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    referenceBlockTime = 1554209118;

    Transaction tx2{referenceBlockId, referenceBlockTime};
    tx2.actions.push_back(TransferAction("token", "token", "eosio", Asset::fromString("30.0000 TKN"), "my second transfer"));

    buf.clear();
    tx2.serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200");

    ASSERT_EQ(
        hex(signer.hash(tx2)),
        "14fc3299ee3e1113096bf1869dfa14c04a7ffdedd8ebdabf530683e4cfcd726c");

    ASSERT_NO_THROW(tx2.serialize());

    // verify k1 sigs
    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernK1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            k1Sigs[i]);
    }

    // verify r1 sigs
    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernR1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            r1Sigs[i]);
    }

    referenceBlockId = parse_hex("");
    referenceBlockTime = 0;
    EXPECT_ANY_THROW(new Transaction(referenceBlockId, referenceBlockTime));

    referenceBlockId = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    referenceBlockTime = 1554209118;
    Transaction tx3{referenceBlockId, referenceBlockTime};
    Data extBuf;
    auto ext = Extension(uint16_t(0), extBuf);
    tx3.transactionExtensions.push_back(ext);
    buf.clear();
    tx3.serialize(buf);
    ASSERT_EQ(hex(buf), "6e67a35cd6679a1f3d48000000000001000000");
}

TEST(EOSTransaction, formatDate) {
    EXPECT_EQ(Transaction::formatDate(1554209148), "2019-04-02T12:45:48");
    EXPECT_EQ(Transaction::formatDate(1654160000), "2022-06-02T08:53:20");
    EXPECT_EQ(Transaction::formatDate(0), "1970-01-01T00:00:00");
    EXPECT_EQ(Transaction::formatDate(std::numeric_limits<int32_t>::max()), "2038-01-19T03:14:07");
}

TEST(EOSTransaction, Create) {
    auto emptyData = Data{};
    EXPECT_ANY_THROW(new Transaction(emptyData, 0));
}

} // namespace TW::EOS::tests
