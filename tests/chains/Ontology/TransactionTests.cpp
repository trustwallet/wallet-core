// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"

#include "Ontology/ParamsBuilder.h"
#include "Ontology/Signer.h"
#include "Ontology/Transaction.h"

#include <list>

#include <boost/any.hpp>
#include <gtest/gtest.h>

namespace TW::Ontology::tests {

TEST(OntologyTransaction, validity) {
    std::vector<uint8_t> ontContract{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    auto fromAddress = Address("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    auto toAddress = Address("APniYDGozkhUh8Tk7pe35aah2HGJ4fJfVd");
    uint64_t amount = 1;
    NeoVmParamValue::ParamList transferParam{fromAddress._data, toAddress._data, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(ontContract, 0x00, "transfer", {args});
    uint8_t version = 0;
    uint8_t txType = 0xd1;
    uint32_t nonce = 1552759011;
    uint64_t gasPrice = 600;
    uint64_t gasLimit = 300000;
    auto tx =
        Transaction(version, txType, nonce, gasPrice, gasLimit, toAddress.string(), invokeCode);
    std::string hexTx =
        "00d1e3388d5c5802000000000000e09304000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c6"
        "6b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81457e9d1a61f9aafa798b6c7fbeae35639681d7d"
        "f66a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f"
        "6e746f6c6f67792e4e61746976652e496e766f6b650000";
    EXPECT_EQ(hexTx, hex(tx.serialize()));
    auto signer1 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    signer1.sign(tx);
    hexTx =
        "00d1e3388d5c5802000000000000e09304000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c6"
        "6b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81457e9d1a61f9aafa798b6c7fbeae35639681d7d"
        "f66a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f"
        "6e746f6c6f67792e4e61746976652e496e766f6b6500014140e03a09d85f56d2ceb5817a1f3a430bab9bf0f469"
        "da38afe4a5b33de258a06236d8e0a59d25918a49825455c99f91de9caf8071e38a589a530519705af9081eca23"
        "21031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac";
    EXPECT_EQ(520ul, hex(tx.serialize()).length());
    EXPECT_EQ(hexTx.substr(0, 20), hex(tx.serialize()).substr(0, 20));
    auto signer2 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));
    signer2.addSign(tx);
    auto result = tx.serialize();
    auto verifyPosition1 =
        hex(result).find("21031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac");
    auto verifyPosition2 =
        hex(result).find("2103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac");
    EXPECT_EQ(450ul, verifyPosition1);
    EXPECT_EQ(654ul, verifyPosition2);
    EXPECT_EQ(724ul, hex(result).length());
}

} // namespace TW::Ontology::tests
