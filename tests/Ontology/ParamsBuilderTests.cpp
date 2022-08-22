// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PublicKey.h"

#include "Ontology/Address.h"
#include "Ontology/Ont.h"
#include "Ontology/ParamsBuilder.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>

using namespace TW;
using namespace TW::Ontology;

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
                                                           "balanceOf", balanceParam);
    auto hexInvokeCode =
        "1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000"
        "000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}

TEST(ParamsBuilder, transferInvokeCode) {
    auto fromAddress = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD")._data;
    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn")._data;
    uint64_t amount = 1;
    std::list<boost::any> transferParam{fromAddress, toAddress, amount};
    std::vector<boost::any> args{transferParam};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(Ont().contractAddress(), 0x00, "transfer", args);
    auto hexInvokeCode =
        "00c66b1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b76a7cc814feec06b79ed299ea06fcb94abac41aaf3e"
        "ad76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068"
        "164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}

TEST(ParamsBuilder, balanceOep4TokenInvokeCode) {
    auto contractAddr = parse_hex("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    std::reverse(std::begin(contractAddr), std::end(contractAddr));
    auto balanceAddr = Address("ANXE3XovCwBH1ckQnPc6vKYiTwRXyrVToD")._data;
    std::string method = "balanceOf";
    std::vector<boost::any> args{balanceAddr};
    std::vector<boost::any> params{args, method};

    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddr, params);
    auto hexInvokeCode = "144a03aaf03d12fd4d46bfcc260bda73ecef33b83b51c10962616c616e63654f6667e77fb36f54874c29f503d301d91d8ab98eb2342f";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}

TEST(ParamsBuilder, transferOep4TokenInvokeCode) {
    auto contractAddr = parse_hex("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    std::reverse(std::begin(contractAddr), std::end(contractAddr));
    auto fromAddr = Address("AYxk7SzhSP34QbeYPeskHt9TCZq2JYjp8e")._data;
    auto toAddr = Address("ARR6PsaBwRttzCmyxCMhL7NmFk1LqExD7L")._data;
    uint64_t amount = 1000;
    std::string method = "transfer";
    std::vector<boost::any> args{amount, toAddr, fromAddr};
    std::vector<boost::any> params{args, method};

    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddr, params);
    auto hexInvokeCode = "02e8031469c329fbb30a490979ea1a6f0b6a3a91235f6bd714bc886c7935e7b8a44cc677a059ab41d10c9382cf53c1087472616e7366657267e77fb36f54874c29f503d301d91d8ab98eb2342f";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
}