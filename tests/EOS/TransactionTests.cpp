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
    "SIG_K1_JzCPEWQegyedcpYSvkWrcTeowKDdVhNLPdMoUuYZjBPoywhR4V852WhT1LGXoXivjocfPQiP8kUgzKKYaUZJi7QAKr13Qn",
    "SIG_K1_K2p7c6n8neJxq6gcBwsYZg8paG5ZqMoh4WeZS5EEXDNjw8vucmbAYqkMXaZB6wvcLof5BhffYGwoBDpuJWUcapv8aNQuKu",
    "SIG_K1_KmB6bRnY6Lgu4gLR9p7pt426FDVY48Zdqyw3LjfukM7hXaPLZAZokGEQ1SyrEu5MyToNEVX79784fA79tDj9nk2fHWGF59",
    "SIG_K1_KZbnL6WtEjkGE2QXvnBARTNMCjAjtTdCmnnS9r3fd5vd8YsxZTMdz8SjqSY199JtGLtfHpL1vRjJ7GgSuGiUaNvjyjD3R1",
    "SIG_K1_K4opjz6tq4HpsERPb4ZDjZSj5VLZEakbFR93saxHBbuSG8N6eGmG3ufPnJUPqErwRWQQMPtE5jrgFzvdcA1DKcGVWypxPz"
};

static std::string r1Sigs[5] { 
    "SIG_R1_KN7C1VJdLgCxo2mdmb2eRQHNNtHucA7Ry9PEvV7TYqYKwNHM9L7Y8oNXAdP8aAnqiiXGc3tG58upKA9Nhiebfcfdr78kSe",
    "SIG_R1_L1j95iuv3XJ9jP9qG2cxtxN2aXSUazG6fg9WxLQUBSJc2u36BexJYNsMXaXsSNoF9cpEGWsmKJ3nQQBrj78W6KYJybJW1p",
    "SIG_R1_Kzi1Mg5nfQUvimEcQ7hjDBtVTskUvCspEAsA7bTE7PBmY5CjaUJysAESmtMjE5d8LcyUnVha6RVqR6EhPifKqK168TjJcc",
    "SIG_R1_KGKdxNg8mDnDFXYF5GDThrU2uc2CEhywQpWQRbgZdXsLmvvo4asrX3mexMh9SwBTeditFQibwjgDq6QJPP4qdz8w8GA2hC",
    "SIG_R1_KMkPwMqz1CrtevotkkquMeSD3Qv6yz2hsEaA5bbgKBeRRNT5EdctgRFb2JrWw1JzbKsRaYTEmepQACyh1fzVtz7EdQQMLd"
};

TEST(EOSTransaction, Serialization) {

    Data referenceBlockId = parse_hex("000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c");
    int32_t referenceBlockTime = 1554121728;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");

    Transaction tx {referenceBlockId, referenceBlockTime};
    tx.actions.push_back(TransferAction("token", "eosio", "token", Bravo::Asset::fromString("20.1234 TKN"), "my first transfer"));

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

    referenceBlockId = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    referenceBlockTime = 1554209118;

    Transaction tx2 {referenceBlockId, referenceBlockTime};
    tx2.actions.push_back(TransferAction("token", "token", "eosio", Bravo::Asset::fromString("30.0000 TKN"), "my second transfer"));

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


    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(hex(std::string(i, 'A'))));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernK1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            k1Sigs[i]
        );
    }

    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(hex(std::string(i, 'A'))));
        ASSERT_NO_THROW(signer.sign(pk, Type::ModernR1, tx2));

        ASSERT_EQ(
            tx2.signatures.back().string(),
            r1Sigs[i]
        );
    }
}