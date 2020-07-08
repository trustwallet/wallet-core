// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyAbiEncoder.h>
#include <TrustWalletCore/TWHarmonyAbiFunction.h>
#include <TrustWalletCore/TWString.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <gtest/gtest.h>
#include <string>

namespace TW::Harmony {

TEST(TWHarmonyAbi, FuncCreateEmpty) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz()", type2);

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, FuncCreate1) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    auto p1index = TWHarmonyAbiFunctionAddParamUInt64(func, 69, false);
    EXPECT_EQ(0, p1index);
    auto p2index = TWHarmonyAbiFunctionAddParamUInt64(func, 9, true);
    EXPECT_EQ(0, p2index);
    // check back get value
    auto p2val2 = TWHarmonyAbiFunctionGetParamUInt64(func, p2index, true);
    EXPECT_EQ(9, p2val2);

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz(uint64)", type2);

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, FuncCreate2) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* p1valStr = TWStringCreateWithUTF8Bytes("0045");
    TWData* p1val = TWDataCreateWithHexString(p1valStr);
    auto p1index = TWHarmonyAbiFunctionAddParamUInt256(func, p1val, false);
    EXPECT_EQ(0, p1index);
    //TWDataDelete(p1val);
    TWStringDelete(p1valStr);

    Data dummy(0);
    auto p2index = TWHarmonyAbiFunctionAddParamUInt256(func, &dummy, true);
    EXPECT_EQ(0, p2index);

    // check back get value
    TWData* p2val2 = TWHarmonyAbiFunctionGetParamUInt256(func, p2index, true);
    Data p2val3 = data(TWDataBytes(p2val2), TWDataSize(p2val2));
    EXPECT_EQ("00", hex(p2val3));

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    EXPECT_EQ("baz(uint256)", std::string(TWStringUTF8Bytes(type)));

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, EncodeFuncCase1) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("sam"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddParamBytes(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("64617665")), false));
    EXPECT_EQ(1, TWHarmonyAbiFunctionAddParamBool(func, true, false));
    auto paramArrIdx = TWHarmonyAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(2, paramArrIdx);
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("01"))));
    EXPECT_EQ(1, TWHarmonyAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("02"))));
    EXPECT_EQ(2, TWHarmonyAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("03"))));

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    EXPECT_EQ("sam(bytes,bool,uint256[])", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWHarmonyAbiEncoderEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
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
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, EncodeFuncCase2) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("f"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddParamUInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false));
    auto paramArrIdx = TWHarmonyAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(1, paramArrIdx);
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x456));
    EXPECT_EQ(1, TWHarmonyAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x789));
    EXPECT_EQ(2, TWHarmonyAbiFunctionAddParamBytesFix(func, 10, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353637383930")), false));
    EXPECT_EQ(3, TWHarmonyAbiFunctionAddParamString(func, TWStringCreateWithUTF8Bytes("Hello, world!"), false));

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    EXPECT_EQ("f(uint256,uint32[],bytes10,string)", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWHarmonyAbiEncoderEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
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
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, EncodeFuncMonster) {
    // Monster function with all parameters types
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("monster"));
    EXPECT_TRUE(func != nullptr);
    
    TWHarmonyAbiFunctionAddParamUInt8(func, 1, false);
    TWHarmonyAbiFunctionAddParamUInt16(func, 2, false);
    TWHarmonyAbiFunctionAddParamUInt32(func, 3, false);
    TWHarmonyAbiFunctionAddParamUInt64(func, 4, false);
    TWHarmonyAbiFunctionAddParamUIntN(func, 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWHarmonyAbiFunctionAddParamUInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWHarmonyAbiFunctionAddParamInt8(func, 1, false);
    TWHarmonyAbiFunctionAddParamInt16(func, 2, false);
    TWHarmonyAbiFunctionAddParamInt32(func, 3, false);
    TWHarmonyAbiFunctionAddParamInt64(func, 4, false);
    TWHarmonyAbiFunctionAddParamIntN(func, 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWHarmonyAbiFunctionAddParamInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWHarmonyAbiFunctionAddParamBool(func, true, false);
    TWHarmonyAbiFunctionAddParamString(func, TWStringCreateWithUTF8Bytes("Hello, world!"), false);
    TWHarmonyAbiFunctionAddParamAddress(func, 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")), false);
    TWHarmonyAbiFunctionAddParamBytes(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("3132333435")), false);
    TWHarmonyAbiFunctionAddParamBytesFix(func, 5, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("3132333435")), false);
    
    TWHarmonyAbiFunctionAddInArrayParamUInt8(func, TWHarmonyAbiFunctionAddParamArray(func, false), 1);
    TWHarmonyAbiFunctionAddInArrayParamUInt16(func, TWHarmonyAbiFunctionAddParamArray(func, false), 2);
    TWHarmonyAbiFunctionAddInArrayParamUInt32(func, TWHarmonyAbiFunctionAddParamArray(func, false), 3);
    TWHarmonyAbiFunctionAddInArrayParamUInt64(func, TWHarmonyAbiFunctionAddParamArray(func, false), 4);
    TWHarmonyAbiFunctionAddInArrayParamUIntN(func, TWHarmonyAbiFunctionAddParamArray(func, false), 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWHarmonyAbiFunctionAddInArrayParamUInt256(func, TWHarmonyAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWHarmonyAbiFunctionAddInArrayParamInt8(func, TWHarmonyAbiFunctionAddParamArray(func, false), 1);
    TWHarmonyAbiFunctionAddInArrayParamInt16(func, TWHarmonyAbiFunctionAddParamArray(func, false), 2);
    TWHarmonyAbiFunctionAddInArrayParamInt32(func, TWHarmonyAbiFunctionAddParamArray(func, false), 3);
    TWHarmonyAbiFunctionAddInArrayParamInt64(func, TWHarmonyAbiFunctionAddParamArray(func, false), 4);
    TWHarmonyAbiFunctionAddInArrayParamIntN(func, TWHarmonyAbiFunctionAddParamArray(func, false), 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWHarmonyAbiFunctionAddInArrayParamInt256(func, TWHarmonyAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWHarmonyAbiFunctionAddInArrayParamBool(func, TWHarmonyAbiFunctionAddParamArray(func, false), true);
    TWHarmonyAbiFunctionAddInArrayParamString(func, TWHarmonyAbiFunctionAddParamArray(func, false), TWStringCreateWithUTF8Bytes("Hello, world!"));
    TWHarmonyAbiFunctionAddInArrayParamAddress(func, TWHarmonyAbiFunctionAddParamArray(func, false), 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")));
    TWHarmonyAbiFunctionAddInArrayParamBytes(func, TWHarmonyAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("3132333435")));
    TWHarmonyAbiFunctionAddInArrayParamBytesFix(func, TWHarmonyAbiFunctionAddParamArray(func, false), 5, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("3132333435")));

    // check back out params
    EXPECT_EQ(1, TWHarmonyAbiFunctionGetParamUInt8(func, 0, false));
    EXPECT_EQ(4, TWHarmonyAbiFunctionGetParamUInt64(func, 3, false));
    EXPECT_EQ(true, TWHarmonyAbiFunctionGetParamBool(func, 12, false));
    EXPECT_EQ(std::string("Hello, world!"), std::string(TWStringUTF8Bytes(TWHarmonyAbiFunctionGetParamString(func, 13, false))));
    TWHarmonyAbiFunctionGetParamAddress(func, 14, false);

    TWString* type = TWHarmonyAbiFunctionGetType(func);
    EXPECT_EQ(
        "monster(uint8,uint16,uint32,uint64,uint168,uint256,int8,int16,int32,int64,int168,int256,bool,string,address,bytes,bytes5,"
        "uint8[],uint16[],uint32[],uint64[],uint168[],uint256[],int8[],int16[],int32[],int64[],int168[],int256[],bool[],string[],address[],bytes[],bytes5[])",
        std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWHarmonyAbiEncoderEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
    EXPECT_EQ(4 + 76 * 32, enc2.size());

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, DecodeOutputFuncCase1) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("readout"));
    EXPECT_TRUE(func != nullptr);

    TWHarmonyAbiFunctionAddParamAddress(func, 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")), false);
    TWHarmonyAbiFunctionAddParamUInt64(func, 1000, false);
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddParamUInt64(func, 0, true));

    // original output value
    EXPECT_EQ(0, TWHarmonyAbiFunctionGetParamUInt64(func, 0, true));

    // decode
    auto encoded = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0000000000000000000000000000000000000000000000000000000000000045"));
    EXPECT_EQ(true, TWHarmonyAbiEncoderDecodeOutput(func, encoded));

    // new output value
    EXPECT_EQ(0x45, TWHarmonyAbiFunctionGetParamUInt64(func, 0, true));

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

TEST(TWHarmonyAbi, GetParamWrongType) {
    TWHarmonyAbiFunction* func = TWHarmonyAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("func"));
    ASSERT_TRUE(func != nullptr);
    // add parameters
    EXPECT_EQ(0, TWHarmonyAbiFunctionAddParamUInt8(func, 1, true));
    EXPECT_EQ(1, TWHarmonyAbiFunctionAddParamUInt64(func, 2, true));

    // GetParameter with correct types
    EXPECT_EQ(1, TWHarmonyAbiFunctionGetParamUInt8(func, 0, true));
    EXPECT_EQ(2, TWHarmonyAbiFunctionGetParamUInt64(func, 1, true));

    // GetParameter with wrong type, default value (0) is returned
    EXPECT_EQ(0, TWHarmonyAbiFunctionGetParamUInt8(func, 1, true));
    EXPECT_EQ(0, TWHarmonyAbiFunctionGetParamUInt64(func, 0, true));
    EXPECT_EQ("00", hex(*(static_cast<const Data*>(TWHarmonyAbiFunctionGetParamUInt256(func, 0, true)))));
    EXPECT_EQ(false, TWHarmonyAbiFunctionGetParamBool(func, 0, true));
    EXPECT_EQ("", std::string(TWStringUTF8Bytes(TWHarmonyAbiFunctionGetParamString(func, 0, true))));
    EXPECT_EQ("", hex(*(static_cast<const Data*>(TWHarmonyAbiFunctionGetParamAddress(func, 0, true)))));

    // GetParameter with wrong index, default value (0) is returned
    EXPECT_EQ(0, TWHarmonyAbiFunctionGetParamUInt8(func, 99, true));
    EXPECT_EQ(0, TWHarmonyAbiFunctionGetParamUInt64(func, 99, true));
    EXPECT_EQ("00", hex(*(static_cast<const Data*>(TWHarmonyAbiFunctionGetParamUInt256(func, 99, true)))));
    EXPECT_EQ(false, TWHarmonyAbiFunctionGetParamBool(func, 99, true));
    EXPECT_EQ("", std::string(TWStringUTF8Bytes(TWHarmonyAbiFunctionGetParamString(func, 99, true))));
    EXPECT_EQ("", hex(*(static_cast<const Data*>(TWHarmonyAbiFunctionGetParamAddress(func, 99, true)))));

    // delete
    TWHarmonyAbiEncoderDeleteFunction(func);
}

} // namespace TW::Harmony
