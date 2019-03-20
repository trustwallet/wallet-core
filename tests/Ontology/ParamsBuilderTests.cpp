// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include"PublicKey.h"
#include"HexCoding.h"

#include"Ontology/Ont.h"
#include"Ontology/Address.h"
#include"Ontology/ParamsBuilder.h"

#include <unordered_map>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ontology;


TEST(ParamsBuilder, pushInt) {
    std::vector<std::string> targetData{"00", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f", "0110", "0111",
                                        "0112", "0113", "0114", "0115", "0116", "0117", "0118", "0119", "011a", "011b", "011c", "011d", "011e", "011f",
                                        "0120", "0121", "0122", "0123", "0124", "0125", "0126", "0127", "0128", "0129", "012a", "012b", "012c", "012d",
                                        "012e", "012f", "0130", "0131", "0132", "0133", "0134", "0135", "0136", "0137", "0138", "0139", "013a", "013b",
                                        "013c", "013d", "013e", "013f", "0140", "0141", "0142", "0143", "0144", "0145", "0146", "0147", "0148", "0149",
                                        "014a", "014b", "014c", "014d", "014e", "014f", "0150", "0151", "0152", "0153", "0154", "0155", "0156", "0157",
                                        "0158", "0159", "015a", "015b", "015c", "015d", "015e", "015f", "0160", "0161", "0162", "0163", "0164", "0165",
                                        "0166", "0167", "0168", "0169", "016a", "016b", "016c", "016d", "016e", "016f", "0170", "0171", "0172", "0173",
                                        "0174", "0175", "0176", "0177", "0178", "0179", "017a", "017b", "017c", "017d", "017e", "017f", "028000", "028100",
                                        "028200", "028300", "028400", "028500", "028600", "028700", "028800", "028900", "028a00", "028b00", "028c00", "028d00",
                                        "028e00", "028f00", "029000", "029100", "029200", "029300", "029400", "029500", "029600", "029700", "029800", "029900",
                                        "029a00", "029b00", "029c00", "029d00", "029e00", "029f00", "02a000", "02a100", "02a200", "02a300", "02a400", "02a500",
                                        "02a600", "02a700", "02a800", "02a900", "02aa00", "02ab00", "02ac00", "02ad00", "02ae00", "02af00", "02b000", "02b100",
                                        "02b200", "02b300", "02b400", "02b500", "02b600", "02b700", "02b800", "02b900", "02ba00", "02bb00", "02bc00", "02bd00",
                                        "02be00", "02bf00", "02c000", "02c100", "02c200", "02c300", "02c400", "02c500", "02c600", "02c700", "02c800", "02c900",
                                        "02ca00", "02cb00", "02cc00", "02cd00", "02ce00", "02cf00", "02d000", "02d100", "02d200", "02d300", "02d400", "02d500",
                                        "02d600", "02d700", "02d800", "02d900", "02da00", "02db00", "02dc00", "02dd00", "02de00", "02df00", "02e000", "02e100",
                                        "02e200", "02e300", "02e400", "02e500", "02e600", "02e700", "02e800", "02e900", "02ea00", "02eb00", "02ec00", "02ed00",
                                        "02ee00", "02ef00", "02f000", "02f100", "02f200", "02f300", "02f400", "02f500", "02f600", "02f700", "02f800", "02f900",
                                        "02fa00", "02fb00", "02fc00", "02fd00", "02fe00", "02ff00"};

    for (uint8_t i = 0; i < 255; i++) {
        auto builder = ParamsBuilder();
        builder.push(i);
        EXPECT_EQ(targetData[i], hex(builder.getBytes()));
    }
}


TEST(ParamsBuilder, InvokeCode) {
    auto balanceParam = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD").data;
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(Ont().contractAddress(), 0x00, "balanceOf", balanceParam);
    auto hexInvokeCode = "1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode, hex(invokeCode));
    auto fromAddress = Address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD").data;
    auto toAddress = Address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn").data;
    uint64_t amount = 1;
    std::unordered_map<std::string, boost::any> transferParam{{"from", fromAddress},{"to", toAddress},{"amount", amount}};
    std::vector<boost::any> args{transferParam};
    invokeCode = ParamsBuilder::buildNativeInvokeCode(Ont().contractAddress(), 0x00, "transfer", args);
    hexInvokeCode = "00c66b1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b76a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b65";
    EXPECT_EQ(hexInvokeCode,hex(invokeCode));
}