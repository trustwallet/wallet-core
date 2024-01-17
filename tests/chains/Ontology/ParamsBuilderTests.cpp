// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PublicKey.h"

#include "Ontology/Address.h"
#include "Ontology/Ont.h"
#include "Ontology/ParamsBuilder.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

namespace TW::Ontology::tests {

TEST(ParamsBuilder, pushInt) {
    std::vector<uint64_t> numVector{0,
                                    1,
                                    2,
                                    127,
                                    128,
                                    129,
                                    65534,
                                    65535,
                                    65536,
                                    65537,
                                    4294967294,
                                    4294967295,
                                    4294967296,
                                    68719476735,
                                    68719476736,
                                    281474976710655,
                                    72057594037927935,
                                    1152921504606846975};
    std::vector<std::string> codeVector{"00",
                                        "51",
                                        "52",
                                        "017f",
                                        "028000",
                                        "028100",
                                        "03feff00",
                                        "03ffff00",
                                        "03000001",
                                        "03010001",
                                        "05feffffff00",
                                        "05ffffffff00",
                                        "050000000001",
                                        "05ffffffff0f",
                                        "050000000010",
                                        "07ffffffffffff00",
                                        "08ffffffffffffff00",
                                        "08ffffffffffffff0f"};
    for (auto index = 0ul; index < numVector.size(); index++) {
        auto builder = ParamsBuilder();
        builder.push(numVector[index]);
        EXPECT_EQ(codeVector[index], hex(builder.getBytes()));
    }
}

TEST(ParamsBuilder, balanceInvokeCode) {
    auto balanceParam = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")._data;
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(Ont().contractAddress(), 0x00,
                                                           "balanceOf", {balanceParam});
    auto hexInvokeCode =
        "1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000"
        "000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}

TEST(ParamsBuilder, transferInvokeCode) {
    auto fromAddress = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")._data;
    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn")._data;
    uint64_t amount = 1;
    NeoVmParamValue::ParamList transferParam{fromAddress, toAddress, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(Ont().contractAddress(), 0x00, "transfer", {args});
    auto hexInvokeCode =
        "00c66b1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b76a7cc814feec06b79ed299ea06fcb94abac41aaf3e"
        "ad76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068"
        "164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}

TEST(ParamsBuilder, invokeOep4Code) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));

    NeoVmParamValue::ParamArray args{};
    std::string method{"name"};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(wing_addr, method, {args});

    auto expectCode = "00c1046e616d656733def739225d0f93dd2aed457d7b1fd074ec31ff";
    EXPECT_EQ(expectCode, hex(invokeCode));
}

TEST(ParamsBuilder, invokeOep4CodeBalanceOf) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    auto user_addr = Address("AeaThtPwh5kAYnjHavzwmvxPd725nVTvbM");
    Data d(std::begin(user_addr._data), std::end(user_addr._data));

    NeoVmParamValue::ParamArray args{d};
    std::string method{"balanceOf"};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(wing_addr, method, {args});

    auto expectCode = "14fa2254ffaee3c3e1172e8e98f800e4105c74988e51c10962616c616e63654f666733def739225d0f93dd2aed457d7b1fd074ec31ff";
    EXPECT_EQ(expectCode, hex(invokeCode));
}

TEST(OntologyOep4, invokeOep4CodeTransfer) {
    std::string wing_hex{"ff31ec74d01f7b7d45ed2add930f5d2239f7de33"};
    auto wing_addr = Address(parse_hex(wing_hex));
    auto from = Address("APniYDGozkhUh8Tk7pe35aah2HGJ4fJfVd");
    auto to = Address("AVY6LfvxauVQAVHDV9hC3ZCv7cQqzfDotH");
    uint64_t amount = 253;

    NeoVmParamValue::ParamArray args{from._data, to._data, amount};
    std::reverse(args.begin(), args.end());
    std::string method{"transfer"};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(wing_addr, method, {args});

    auto expectCode = "02fd001496f688657b95be51c11a87b51adfda4ab69e9cbb1457e9d1a61f9aafa798b6c7fbeae35639681d7df653c1087472616e736665726733def739225d0f93dd2aed457d7b1fd074ec31ff";
    EXPECT_EQ(expectCode, hex(invokeCode));
}

} // namespace TW::Ontology::tests
