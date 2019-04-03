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

TEST(EOSTransaction, Serialization) {

    std::string referenceBlockId = "000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c";
    uint32_t referenceBlockTime = 1554121728;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");

    Transaction tx {referenceBlockId, referenceBlockTime};
    tx.actions.push_back(TransferAction("token", "token", "token", "eosio", "20.0000 TKN", "my first transfer"));

    Data buf;
    tx.serialize(buf);

    Signer signer {chainId};

    ASSERT_EQ(
        hex(buf),
        "1e04a25cdc46a452d92c00000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232320000000080a920cd0000000000ea3055400d03000000000004544b4e00000000116d79206669727374207472616e7366657200"
    );

    ASSERT_EQ(
        hex(signer.hash(tx)),
        "1e61e7e99c340ac2936a659b507e71a7db2b5b34c7b67e5d7d4d0ab05a3cff2b"
    );

    referenceBlockId = "000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144";
    referenceBlockTime = 1554209118;

    Transaction tx2 {referenceBlockId, referenceBlockTime};
    tx2.actions.push_back(TransferAction("token", "token", "token", "eosio", "30.0000 TKN", "my second transfer"));

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

        std::cout << "Public Key: " + Address(pk.getPublicKey(PublicKeyType::secp256k1), Type::ModernK1).string() + "\n";
        std::cout << "Signature: " + tx2.signatures.back().string() + "\n";

        ASSERT_TRUE(signer.verify(pk, tx2.signatures.back(), tx2));
    }
}