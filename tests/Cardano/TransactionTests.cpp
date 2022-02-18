// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Transaction.h"
#include "Cardano/AddressV3.h"

#include "HexCoding.h"
#include "Cbor.h"

#include <gtest/gtest.h>


using namespace TW::Cardano;
using namespace TW;
using namespace std;


TEST(CardanoTransaction, Encode) {
    Transaction tx;
    tx.inputs.push_back(OutPoint{"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767", 1});
    tx.inputs.push_back(OutPoint{"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0", 0});
    tx.outputs.push_back(TxOutput{
        AddressV3("addr1q9068st87h22h3l6w6t5evnlm067rag94llqya2hkjrsd3wvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qpmxzjt").data(),
        2000000
    });
    tx.outputs.push_back(TxOutput{
        AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq").data(),
        16749189
    });
    tx.fee = 165555;
    tx.ttl = 53333345;

    const auto encoded = tx.encode();
    EXPECT_EQ(hex(encoded), "a40082827840663037343133346161626266623133623861656337636635343635623165356138363262646535636238383533326363376536343631393137396233653736370182784035353466326664393432613233643036383335643236626264373866303130366661393463386135353131313461306265663831393237663636343637616630000182825839015fa3c167f5d4abc7fa76974cb27fdbf5e1f505affe027557b48706c5cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34681a001e8480825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34681a00ff9285021a000286b3031a032dcd61");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.getMapElements().size(), 4);
        EXPECT_EQ(decode.dumpToString(), "{0: [[\"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767\", 1], [\"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0\", 0]], 1: [[h\"015fa3c167f5d4abc7fa76974cb27fdbf5e1f505affe027557b48706c5cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468\", 2000000], [h\"018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468\", 16749189]], 2: 165555, 3: 53333345}");
    }
}
