#include "EOS/Transaction.h"
#include "EOS/Action.h"
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
    tx.actions.push_back(TransferAction("token", "token", "eosio", "20.0000 TKN", "my first transfer"));

    Data buf;
    tx.serialize(buf);

    ASSERT_EQ(
        hex(buf),
        "1e04a25cdc46a452d92c00000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232320000000080a920cd0000000000ea3055400d03000000000004544b4e00000000116d79206669727374207472616e7366657200"
    );
}