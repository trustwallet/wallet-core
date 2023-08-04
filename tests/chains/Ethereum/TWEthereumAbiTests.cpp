// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbi.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>
#include <TrustWalletCore/TWString.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <string>

namespace TW::Ethereum {

TEST(TWEthereumAbi, FuncCreateEmpty) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("baz")).get());
    EXPECT_TRUE(func != nullptr);

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    std::string type2 = TWStringUTF8Bytes(type.get());
    EXPECT_EQ("baz()", type2);

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, FuncCreate1) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("baz")).get());
    EXPECT_TRUE(func != nullptr);

    auto p1index = TWEthereumAbiFunctionAddParamUInt64(func, 69, false);
    EXPECT_EQ(0, p1index);
    auto p2index = TWEthereumAbiFunctionAddParamUInt64(func, 9, true);
    EXPECT_EQ(0, p2index);
    // check back get value
    auto p2val2 = TWEthereumAbiFunctionGetParamUInt64(func, p2index, true);
    EXPECT_EQ(9ul, p2val2);

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    std::string type2 = TWStringUTF8Bytes(type.get());
    EXPECT_EQ("baz(uint64)", type2);

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, FuncCreate2) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("baz")).get());
    EXPECT_TRUE(func != nullptr);

    auto p1valStr = WRAPS(TWStringCreateWithUTF8Bytes("0045"));
    auto p1val = WRAPD(TWDataCreateWithHexString(p1valStr.get()));
    auto p1index = TWEthereumAbiFunctionAddParamUInt256(func, p1val.get(), false);
    EXPECT_EQ(0, p1index);

    Data dummy(0);
    auto p2index = TWEthereumAbiFunctionAddParamUInt256(func, &dummy, true);
    EXPECT_EQ(0, p2index);

    // check back get value
    auto p2val2 = WRAPD(TWEthereumAbiFunctionGetParamUInt256(func, p2index, true));
    Data p2val3 = data(TWDataBytes(p2val2.get()), TWDataSize(p2val2.get()));
    EXPECT_EQ("00", hex(p2val3));

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    EXPECT_EQ("baz(uint256)", std::string(TWStringUTF8Bytes(type.get())));

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, EncodeFuncCase1) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("sam")).get());
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamBytes(func, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("64617665")).get())).get(), false));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddParamBool(func, true, false));
    auto paramArrIdx = TWEthereumAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(2, paramArrIdx);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("01")).get())).get()));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("02")).get())).get()));
    EXPECT_EQ(2, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("03")).get())).get()));

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    EXPECT_EQ("sam(bytes,bool,uint256[])", std::string(TWStringUTF8Bytes(type.get())));

    auto encoded = WRAPD(TWEthereumAbiEncode(func));
    Data enc2 = data(TWDataBytes(encoded.get()), TWDataSize(encoded.get()));
    EXPECT_EQ("a5643bf2"
        "0000000000000000000000000000000000000000000000000000000000000060"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "00000000000000000000000000000000000000000000000000000000000000a0"
        "0000000000000000000000000000000000000000000000000000000000000004"
        "6461766500000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000003"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000003",
        hex(enc2));

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, EncodeFuncCase2) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("f")).get());
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamUInt256(func, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get(), false));
    auto paramArrIdx = TWEthereumAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(1, paramArrIdx);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x456));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x789));
    EXPECT_EQ(2, TWEthereumAbiFunctionAddParamBytesFix(func, 10, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("31323334353637383930")).get())).get(), false));
    EXPECT_EQ(3, TWEthereumAbiFunctionAddParamString(func, WRAPS(TWStringCreateWithUTF8Bytes("Hello, world!")).get(), false));

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    EXPECT_EQ("f(uint256,uint32[],bytes10,string)", std::string(TWStringUTF8Bytes(type.get())));

    auto encoded = WRAPD(TWEthereumAbiEncode(func));
    Data enc2 = data(TWDataBytes(encoded.get()), TWDataSize(encoded.get()));
    EXPECT_EQ("47b941bf"
        "0000000000000000000000000000000000000000000000000000000000000123"
        "0000000000000000000000000000000000000000000000000000000000000080"
        "3132333435363738393000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000e0"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000456"
        "0000000000000000000000000000000000000000000000000000000000000789"
        "000000000000000000000000000000000000000000000000000000000000000d"
        "48656c6c6f2c20776f726c642100000000000000000000000000000000000000",
        hex(enc2));

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, EncodeFuncMonster) {
    // Monster function with all parameters types
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("monster")).get());
    EXPECT_TRUE(func != nullptr);
    
    TWEthereumAbiFunctionAddParamUInt8(func, 1, false);
    TWEthereumAbiFunctionAddParamUInt16(func, 2, false);
    TWEthereumAbiFunctionAddParamUInt32(func, 3, false);
    TWEthereumAbiFunctionAddParamUInt64(func, 4, false);
    TWEthereumAbiFunctionAddParamUIntN(func, 168, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get(), false);
    TWEthereumAbiFunctionAddParamUInt256(func, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get(), false);
    TWEthereumAbiFunctionAddParamInt8(func, 1, false);
    TWEthereumAbiFunctionAddParamInt16(func, 2, false);
    TWEthereumAbiFunctionAddParamInt32(func, 3, false);
    TWEthereumAbiFunctionAddParamInt64(func, 4, false);
    TWEthereumAbiFunctionAddParamIntN(func, 168, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get(), false);
    TWEthereumAbiFunctionAddParamInt256(func, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get(), false);
    TWEthereumAbiFunctionAddParamBool(func, true, false);
    TWEthereumAbiFunctionAddParamString(func, WRAPS(TWStringCreateWithUTF8Bytes("Hello, world!")).get(), false);
    TWEthereumAbiFunctionAddParamAddress(func,
        WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")).get())).get(), false);
    TWEthereumAbiFunctionAddParamBytes(func, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("3132333435")).get())).get(), false);
    TWEthereumAbiFunctionAddParamBytesFix(func, 5, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("3132333435")).get())).get(), false);
    
    TWEthereumAbiFunctionAddInArrayParamUInt8(func, TWEthereumAbiFunctionAddParamArray(func, false), 1);
    TWEthereumAbiFunctionAddInArrayParamUInt16(func, TWEthereumAbiFunctionAddParamArray(func, false), 2);
    TWEthereumAbiFunctionAddInArrayParamUInt32(func, TWEthereumAbiFunctionAddParamArray(func, false), 3);
    TWEthereumAbiFunctionAddInArrayParamUInt64(func, TWEthereumAbiFunctionAddParamArray(func, false), 4);
    TWEthereumAbiFunctionAddInArrayParamUIntN(func, TWEthereumAbiFunctionAddParamArray(func, false), 168, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamUInt256(func, TWEthereumAbiFunctionAddParamArray(func, false), WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamInt8(func, TWEthereumAbiFunctionAddParamArray(func, false), 1);
    TWEthereumAbiFunctionAddInArrayParamInt16(func, TWEthereumAbiFunctionAddParamArray(func, false), 2);
    TWEthereumAbiFunctionAddInArrayParamInt32(func, TWEthereumAbiFunctionAddParamArray(func, false), 3);
    TWEthereumAbiFunctionAddInArrayParamInt64(func, TWEthereumAbiFunctionAddParamArray(func, false), 4);
    TWEthereumAbiFunctionAddInArrayParamIntN(func, TWEthereumAbiFunctionAddParamArray(func, false), 168, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamInt256(func, TWEthereumAbiFunctionAddParamArray(func, false), WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0123")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamBool(func, TWEthereumAbiFunctionAddParamArray(func, false), true);
    TWEthereumAbiFunctionAddInArrayParamString(func, TWEthereumAbiFunctionAddParamArray(func, false), WRAPS(TWStringCreateWithUTF8Bytes("Hello, world!")).get());
    TWEthereumAbiFunctionAddInArrayParamAddress(func, TWEthereumAbiFunctionAddParamArray(func, false), 
        WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamBytes(func, TWEthereumAbiFunctionAddParamArray(func, false), WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("3132333435")).get())).get());
    TWEthereumAbiFunctionAddInArrayParamBytesFix(func, TWEthereumAbiFunctionAddParamArray(func, false), 5, WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("3132333435")).get())).get());

    // check back out params
    EXPECT_EQ(1, TWEthereumAbiFunctionGetParamUInt8(func, 0, false));
    EXPECT_EQ(4ul, TWEthereumAbiFunctionGetParamUInt64(func, 3, false));
    EXPECT_EQ(true, TWEthereumAbiFunctionGetParamBool(func, 12, false));
    EXPECT_EQ(std::string("Hello, world!"), std::string(TWStringUTF8Bytes(WRAPS(TWEthereumAbiFunctionGetParamString(func, 13, false)).get())));
    WRAPD(TWEthereumAbiFunctionGetParamAddress(func, 14, false));

    auto type = WRAPS(TWEthereumAbiFunctionGetType(func));
    EXPECT_EQ(
        "monster(uint8,uint16,uint32,uint64,uint168,uint256,int8,int16,int32,int64,int168,int256,bool,string,address,bytes,bytes5,"
        "uint8[],uint16[],uint32[],uint64[],uint168[],uint256[],int8[],int16[],int32[],int64[],int168[],int256[],bool[],string[],address[],bytes[],bytes5[])",
        std::string(TWStringUTF8Bytes(type.get())));

    auto encoded = WRAPD(TWEthereumAbiEncode(func));
    Data enc2 = data(TWDataBytes(encoded.get()), TWDataSize(encoded.get()));
    EXPECT_EQ(4ul + 76 * 32, enc2.size());

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, DecodeOutputFuncCase1) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("readout")).get());
    EXPECT_TRUE(func != nullptr);

    TWEthereumAbiFunctionAddParamAddress(func, 
        WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")).get())).get(), false);
    TWEthereumAbiFunctionAddParamUInt64(func, 1000, false);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamUInt64(func, 0, true));

    // original output value
    EXPECT_EQ(0ul, TWEthereumAbiFunctionGetParamUInt64(func, 0, true));

    // decode
    auto encoded = WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0000000000000000000000000000000000000000000000000000000000000045")).get()));
    EXPECT_EQ(true, TWEthereumAbiDecodeOutput(func, encoded.get()));

    // new output value
    EXPECT_EQ(0x45ul, TWEthereumAbiFunctionGetParamUInt64(func, 0, true));

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, GetParamWrongType) {
    TWEthereumAbiFunction* func = TWEthereumAbiFunctionCreateWithString(WRAPS(TWStringCreateWithUTF8Bytes("func")).get());
    ASSERT_TRUE(func != nullptr);
    // add parameters
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamUInt8(func, 1, true));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddParamUInt64(func, 2, true));

    // GetParameter with correct types
    EXPECT_EQ(1, TWEthereumAbiFunctionGetParamUInt8(func, 0, true));
    EXPECT_EQ(2ul, TWEthereumAbiFunctionGetParamUInt64(func, 1, true));

    // GetParameter with wrong type, default value (0) is returned
    EXPECT_EQ(0, TWEthereumAbiFunctionGetParamUInt8(func, 1, true));
    EXPECT_EQ(0ul, TWEthereumAbiFunctionGetParamUInt64(func, 0, true));
    EXPECT_EQ("00", hex(*(static_cast<const Data*>(WRAPD(TWEthereumAbiFunctionGetParamUInt256(func, 0, true)).get()))));
    EXPECT_EQ(false, TWEthereumAbiFunctionGetParamBool(func, 0, true));
    EXPECT_EQ("", std::string(TWStringUTF8Bytes(WRAPS(TWEthereumAbiFunctionGetParamString(func, 0, true)).get())));
    EXPECT_EQ("", hex(*(static_cast<const Data*>(WRAPD(TWEthereumAbiFunctionGetParamAddress(func, 0, true)).get()))));

    // GetParameter with wrong index, default value (0) is returned
    EXPECT_EQ(0, TWEthereumAbiFunctionGetParamUInt8(func, 99, true));
    EXPECT_EQ(0ul, TWEthereumAbiFunctionGetParamUInt64(func, 99, true));
    EXPECT_EQ("00", hex(*(static_cast<const Data*>(WRAPD(TWEthereumAbiFunctionGetParamUInt256(func, 99, true)).get()))));
    EXPECT_EQ(false, TWEthereumAbiFunctionGetParamBool(func, 99, true));
    EXPECT_EQ("", std::string(TWStringUTF8Bytes(WRAPS(TWEthereumAbiFunctionGetParamString(func, 99, true)).get())));
    EXPECT_EQ("", hex(*(static_cast<const Data*>(WRAPD(TWEthereumAbiFunctionGetParamAddress(func, 99, true)).get()))));

    // delete
    TWEthereumAbiFunctionDelete(func);
}

TEST(TWEthereumAbi, DecodeCall) {
    auto callHex = STRING("c47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000086465616462656566000000000000000000000000000000000000000000000000");
    auto call = WRAPD(TWDataCreateWithHexString(callHex.get()));
    auto abi = STRING(R"|({"c47f0027":{"constant":false,"inputs":[{"name":"name","type":"string"}],"name":"setName","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"}})|");
    auto decoded = WRAPS(TWEthereumAbiDecodeCall(call.get(), abi.get()));
    auto expected = R"|({"function":"setName(string)","inputs":[{"name":"name","type":"string","value":"deadbeef"}]})|";

    assertStringsEqual(decoded, expected);
}

TEST(TWEthereumAbi, DecodeCallTupleArray) {
    auto callHex = STRING("0x846a1bc6000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec700000000000000000000000000000000000000000000000000470de4df82000000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000740000000000000000000000000000000000000000000000000000000000000078000000000000000000000000000000000000000000000000000000000000007c00000000000000000000000000000000000000000000000000000000000000820000000000000000000000000a140f413c63fbda84e9008607e678258fffbc76b00000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec7000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000044095ea7b300000000000000000000000099a58482bd75cbab83b27ec03ca68ff489b5788f00000000000000000000000000000000000000000000000000470de4df820000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000099a58482bd75cbab83b27ec03ca68ff489b5788f000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000046000000000000000000000000000000000000000000000000000000000000003840651cb35000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec7000000000000000000000000bebc44782c7db0a1a60cb6fe97d0b483032ff1c7000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb4800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000470de4df8200000000000000000000000000000000000000000000000000000000298ce42936ed0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ce16f69375520ab01377ce7b88f5ba8c48f8d66600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000045553444300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000762696e616e636500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002a307863653136463639333735353230616230313337376365374238386635424138433438463844363636000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000000040000000000000000000000000a140f413c63fbda84e9008607e678258fffbc76b000000000000000000000000000000000000000000000000000000000000000700000000000000000000000000000000000000000000000000000000000000e000000000000000000000000000000000000000000000000000000000000001e0000000000000000000000000000000000000000000000000000000000000036000000000000000000000000000000000000000000000000000000000000005a0000000000000000000000000000000000000000000000000000000000000072000000000000000000000000000000000000000000000000000000000000009600000000000000000000000000000000000000000000000000000000000000ac000000000000000000000000000000000000000000000000000000000000000030000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000200000000000000000000000004268b8f0b87b6eae5d897996e6b845ddbd99adf300000000000000000000000000000000000000000000000000000000000000010000000000000000000000004268b8f0b87b6eae5d897996e6b845ddbd99adf3000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000044095ea7b30000000000000000000000001b81d678ffb9c0263b24a97847620c99d213eb1400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000000000000000000000004268b8f0b87b6eae5d897996e6b845ddbd99adf3000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000001b81d678ffb9c0263b24a97847620c99d213eb14000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000001e00000000000000000000000000000000000000000000000000000000000000104414bf3890000000000000000000000004268b8f0b87b6eae5d897996e6b845ddbd99adf300000000000000000000000055d398326f99059ff775485246999027b319795500000000000000000000000000000000000000000000000000000000000000640000000000000000000000004fd39c9e151e50580779bd04b1f7ecc310079fd300000000000000000000000000000000000000000000000000000189c04a7044000000000000000000000000000000000000000000000000000029a23529cf68000000000000000000000000000000000000000000005af4f3f913bd553d03b900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000000000000000000000004268b8f0b87b6eae5d897996e6b845ddbd99adf30000000000000000000000000000000000000000000000000000000000000005000000000000000000000000000000000000000000000000000000000000000100000000000000000000000055d398326f99059ff775485246999027b3197955000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000044095ea7b30000000000000000000000001b81d678ffb9c0263b24a97847620c99d213eb14000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000055d398326f99059ff775485246999027b3197955000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000001b81d678ffb9c0263b24a97847620c99d213eb14000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000001e00000000000000000000000000000000000000000000000000000000000000104414bf38900000000000000000000000055d398326f99059ff775485246999027b3197955000000000000000000000000bb4cdb9cbd36b01bd1cbaebf2de08d9173bc095c00000000000000000000000000000000000000000000000000000000000001f40000000000000000000000004fd39c9e151e50580779bd04b1f7ecc310079fd300000000000000000000000000000000000000000000000000000189c04a7045000000000000000000000000000000000000000000005b527785e694f805bdd300000000000000000000000000000000000000000000005f935a1fa5c4a6ec61000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000055d398326f99059ff775485246999027b319795500000000000000000000000000000000000000000000000000000000000000050000000000000000000000000000000000000000000000000000000000000001000000000000000000000000bb4cdb9cbd36b01bd1cbaebf2de08d9173bc095c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000242e1a7d4d0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040000000000000000000000000bb4cdb9cbd36b01bd1cbaebf2de08d9173bc095c00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002000000000000000000000000a140f413c63fbda84e9008607e678258fffbc76b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    auto call = WRAPD(TWDataCreateWithHexString(callHex.get()));
    auto abi = STRING(R"|({
    "846a1bc6": {
        "inputs": [
            {
                "name": "token",
                "type": "address"
            },
            {
                "name": "amount",
                "type": "uint256"
            },
            {
                "components": [
                    {
                        "name": "callType",
                        "type": "uint8"
                    },
                    {
                        "name": "target",
                        "type": "address"
                    },
                    {
                        "name": "value",
                        "type": "uint256"
                    },
                    {
                        "name": "callData",
                        "type": "bytes"
                    },
                    {
                        "name": "payload",
                        "type": "bytes"
                    }
                ],
                "name": "calls",
                "type": "tuple[]"
            },
            {
                "name": "bridgedTokenSymbol",
                "type": "string"
            },
            {
                "name": "destinationChain",
                "type": "string"
            },
            {
                "name": "destinationAddress",
                "type": "string"
            },
            {
                "name": "payload",
                "type": "bytes"
            },
            {
                "name": "gasRefundRecipient",
                "type": "address"
            },
            {
                "name": "enableExpress",
                "type": "bool"
            }
        ],
        "name": "callBridgeCall",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    }
})|");
    auto decoded = WRAPS(TWEthereumAbiDecodeCall(call.get(), abi.get()));
    auto expected = R"|({"TOOD"})|";

    assertStringsEqual(decoded, expected);
}

TEST(TWEthereumAbi, DecodeInvalidCall) {
    auto callHex = STRING("c47f002700");
    auto call = WRAPD(TWDataCreateWithHexString(callHex.get()));

    auto decoded1 = TWEthereumAbiDecodeCall(call.get(), STRING(",,").get());
    auto decoded2 = TWEthereumAbiDecodeCall(call.get(), STRING("{}").get());

    EXPECT_TRUE(decoded1 == nullptr);
    EXPECT_TRUE(decoded2 == nullptr);
}

TEST(TWEthereumAbi, encodeTyped) {
    auto message = WRAPS(TWStringCreateWithUTF8Bytes(
        R"({
            "types": {
                "EIP712Domain": [
                    {"name": "name", "type": "string"},
                    {"name": "version", "type": "string"},
                    {"name": "chainId", "type": "uint256"},
                    {"name": "verifyingContract", "type": "address"}
                ],
                "Person": [
                    {"name": "name", "type": "string"},
                    {"name": "wallets", "type": "address[]"}
                ],
                "Mail": [
                    {"name": "from", "type": "Person"},
                    {"name": "to", "type": "Person[]"},
                    {"name": "contents", "type": "string"}
                ]
            },
            "primaryType": "Mail",
            "domain": {
                "name": "Ether Mail",
                "version": "1",
                "chainId": 1,
                "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
            },
            "message": {
                "from": {
                    "name": "Cow",
                    "wallets": [
                        "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
                        "DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
                    ]
                },
                "to": [
                    {
                        "name": "Bob",
                        "wallets": [
                            "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
                            "B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57",
                            "B0B0b0b0b0b0B000000000000000000000000000"
                        ]
                    }
                ],
                "contents": "Hello, Bob!"
            }
        })"));
    auto hash = WRAPD(TWEthereumAbiEncodeTyped(message.get()));

    EXPECT_EQ(
        hex(TW::data(TWDataBytes(hash.get()), TWDataSize(hash.get()))),
        "a85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2"
    );
}

} // namespace TW::Ethereum
