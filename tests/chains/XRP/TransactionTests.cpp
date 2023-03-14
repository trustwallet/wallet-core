// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "XRP/Address.h"
#include "XRP/Transaction.h"
#include "XRP/BinaryCoding.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace std;

namespace TW::Ripple::tests {

TEST(RippleTransaction, serializeAmount) {
    /// From https://github.com/trezor/trezor-core/blob/master/tests/test_apps.ripple.serializer.py
    auto data0 = Transaction::serializeAmount(0);
    auto data1 = Transaction::serializeAmount(1);
    auto data2 = Transaction::serializeAmount(93493429243);
    auto data3 = Transaction::serializeAmount(25000000);
    auto data4 = Transaction::serializeAmount(100000000000);
    /// more than max supply
    auto data5 = Transaction::serializeAmount(200000000000000000);
    /// negative value
    auto data6 = Transaction::serializeAmount(-1);

    ASSERT_EQ(hex(data0), "4000000000000000");
    ASSERT_EQ(hex(data1), "4000000000000001");
    ASSERT_EQ(hex(data2), "40000015c4a483fb");
    ASSERT_EQ(hex(data3), "40000000017d7840");
    ASSERT_EQ(hex(data4), "400000174876e800");
    ASSERT_EQ(hex(data5), "42c68af0bb140000");
    ASSERT_EQ(hex(data6), "");
}

TEST(RippleTransaction, serialize) {
    /// From https://github.com/trezor/trezor-core/blob/master/tests/test_apps.ripple.serializer.py
    auto account = Address("r9TeThyi5xiuUUrFjtPKZiHcDxs7K9H6Rb");
    auto destination = "r4BPgS7DHebQiU31xWELvZawwSG2fSPJ7C";
    auto tx1 = Transaction(
       /* fee */10,
       /* flags */0,
       /* sequence */2,
       /* last_ledger_sequence */0,
       /* account */account
    );
    tx1.createXrpPayment(
       /* amount */25000000,
       /* destination */destination,
       /* destination_tag*/0
    );
    auto serialized1 = tx1.serialize();
    ASSERT_EQ(hex(serialized1), "120000220000000024000000026140000000017d784068400000000000000a81145ccb151f6e9d603f394ae778acf10d3bece874f68314e851bbbe79e328e43d68f43445368133df5fba5a");

    auto tx2 = Transaction(
       /* fee */15,
       /* flags */0,
       /* sequence */144,
       /* last_ledger_sequence */0,
       /* account */Address("rGWTUVmm1fB5QUjMYn8KfnyrFNgDiD9H9e")
    );
    tx2.createXrpPayment(
       /* amount */200000,
       /* destination */"rw71Qs1UYQrSQ9hSgRohqNNQcyjCCfffkQ",
       /* destination_tag*/0
    );
    auto serialized2 = tx2.serialize();
    ASSERT_EQ(hex(serialized2), "12000022000000002400000090614000000000030d4068400000000000000f8114aa1bd19d9e87be8069fdbf6843653c43837c03c6831467fe6ec28e0464dd24fb2d62a492aac697cfad02");

    auto tx3 = Transaction(
       /* fee */12,
       /* flags */0,
       /* sequence */1,
       /* last_ledger_sequence */0,
       /* account */Address("r4BPgS7DHebQiU31xWELvZawwSG2fSPJ7C")
    );
    tx3.createXrpPayment(
       /* amount */25000000,
       /* destination */"rBqSFEFg2B6GBMobtxnU1eLA1zbNC9NDGM",
       /* destination_tag*/4146942154
    );
    auto serialized3 = tx3.serialize();
    ASSERT_EQ(hex(serialized3), "120000220000000024000000012ef72d50ca6140000000017d784068400000000000000c8114e851bbbe79e328e43d68f43445368133df5fba5a831476dac5e814cd4aa74142c3ab45e69a900e637aa2");

    auto tx4 = Transaction(
       /* fee */12,
       /* flags */0,
       /* sequence */1,
       /* last_ledger_sequence */0,
       /* account */Address("r4BPgS7DHebQiU31xWELvZawwSG2fSPJ7C")
    );
    tx4.createXrpPayment(
       /* amount */25000000,
       /* destination */"XVhidoXkozM5DTZFdDnJ5nYC8FPrTuJiyGh1VxSGS6RNJJ5",
       /* ignore destination_tag*/12345
    );
    auto serialized4 = tx4.serialize();
    ASSERT_EQ(hex(serialized4), hex(serialized3));
}

TEST(RippleTransaction, preImage) {
    auto account = Address("r9LqNeG6qHxjeUocjvVki2XR35weJ9mZgQ");
    auto destination = "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh";
    auto tx1 = Transaction(
        /* fee */10,
        /* flags */2147483648,
        /* sequence */1,
        /* last_ledger_sequence */0,
        /* account */account
    );
    tx1.createXrpPayment(
        /* amount */1000,
        /* destination */destination,
        /* destination_tag*/0
    );
    tx1.pub_key = parse_hex("ed5f5ac8b98974a3ca843326d9b88cebd0560177b973ee0b149f782cfaa06dc66a");
    auto unsignedTx = tx1.getPreImage();

    ASSERT_EQ(hex(unsignedTx),
          /* prefix */      "53545800"
          /* tx type */     "120000"
          /* flags */       "2280000000"
          /* sequence */    "2400000001"
          /* amount */      "6140000000000003e8"
          /* fee */         "68400000000000000a"
          /* pub key */     "7321ed5f5ac8b98974a3ca843326d9b88cebd0560177b973ee0b149f782cfaa06dc66a"
          /* account */     "81145b812c9d57731e27a2da8b1830195f88ef32a3b6"
          /* destination */ "8314b5f762798a53d543a014caf8b297cff8f2f937e8"
    );
    ASSERT_EQ(unsignedTx.size(), 114ul);
}

} // namespace TW::Ripple::tests
