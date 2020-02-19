// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiEncoder.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>
#include <TrustWalletCore/TWString.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <gtest/gtest.h>
#include <string>

namespace TW::Ethereum {

TEST(TWEthereumAbi, FuncCreateEmpty) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* type = TWEthereumAbiFunctionGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz()", type2);

    // delete
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, FuncCreate1) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    int p1index = TWEthereumAbiFunctionAddParamUInt64(func, 69, false);
    EXPECT_EQ(0, p1index);
    int p2index = TWEthereumAbiFunctionAddParamUInt64(func, 9, true);
    EXPECT_EQ(0, p2index);
    // check back get value
    int p2val2 = TWEthereumAbiFunctionGetParamUInt64(func, p2index, true);
    EXPECT_EQ(9, p2val2);

    TWString* type = TWEthereumAbiFunctionGetType(func);
    std::string type2 = TWStringUTF8Bytes(type);
    EXPECT_EQ("baz(uint64)", type2);

    // delete
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, FuncCreate2) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("baz"));
    EXPECT_TRUE(func != nullptr);

    TWString* p1valStr = TWStringCreateWithUTF8Bytes("0045");
    TWData* p1val = TWDataCreateWithHexString(p1valStr);
    int p1index = TWEthereumAbiFunctionAddParamUInt256(func, p1val, false);
    EXPECT_EQ(0, p1index);
    //TWDataDelete(p1val);
    TWStringDelete(p1valStr);

    Data dummy(0);
    int p2index = TWEthereumAbiFunctionAddParamUInt256(func, &dummy, true);
    EXPECT_EQ(0, p2index);

    // check back get value
    TWData* p2val2 = TWEthereumAbiFunctionGetParamUInt256(func, p2index, true);
    Data p2val3 = data(TWDataBytes(p2val2), TWDataSize(p2val2));
    EXPECT_EQ("00", hex(p2val3));

    TWString* type = TWEthereumAbiFunctionGetType(func);
    EXPECT_EQ("baz(uint256)", std::string(TWStringUTF8Bytes(type)));

    // delete
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, EncodeFuncCase1) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("sam"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamBytes(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("64617665")), false));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddParamBool(func, true, false));
    int paramArrIdx = TWEthereumAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(2, paramArrIdx);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("01"))));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("02"))));
    EXPECT_EQ(2, TWEthereumAbiFunctionAddInArrayParamUInt256(func, paramArrIdx, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("03"))));

    TWString* type = TWEthereumAbiFunctionGetType(func);
    EXPECT_EQ("sam(bytes,bool,uint256[])", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWEthereumAbiEncoderEncode(func);
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
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, EncodeFuncCase2) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("f"));
    EXPECT_TRUE(func != nullptr);
    
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamUInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false));
    int paramArrIdx = TWEthereumAbiFunctionAddParamArray(func, false);
    EXPECT_EQ(1, paramArrIdx);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x456));
    EXPECT_EQ(1, TWEthereumAbiFunctionAddInArrayParamUInt32(func, paramArrIdx, 0x789));
    EXPECT_EQ(2, TWEthereumAbiFunctionAddParamBytesFix(func, 10, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353637383930")), false));
    EXPECT_EQ(3, TWEthereumAbiFunctionAddParamString(func, TWStringCreateWithUTF8Bytes("Hello, world!"), false));

    TWString* type = TWEthereumAbiFunctionGetType(func);
    EXPECT_EQ("f(uint256,uint32[],bytes10,string)", std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWEthereumAbiEncoderEncode(func);
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
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, EncodeFuncMonster) {
    // Monster function with all parameters types
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("monster"));
    EXPECT_TRUE(func != nullptr);
    
    TWEthereumAbiFunctionAddParamUInt8(func, 1, false);
    TWEthereumAbiFunctionAddParamUInt16(func, 2, false);
    TWEthereumAbiFunctionAddParamUInt32(func, 3, false);
    TWEthereumAbiFunctionAddParamUInt64(func, 4, false);
    TWEthereumAbiFunctionAddParamUIntN(func, 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWEthereumAbiFunctionAddParamUInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWEthereumAbiFunctionAddParamInt8(func, 1, false);
    TWEthereumAbiFunctionAddParamInt16(func, 2, false);
    TWEthereumAbiFunctionAddParamInt32(func, 3, false);
    TWEthereumAbiFunctionAddParamInt64(func, 4, false);
    TWEthereumAbiFunctionAddParamIntN(func, 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWEthereumAbiFunctionAddParamInt256(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")), false);
    TWEthereumAbiFunctionAddParamBool(func, true, false);
    TWEthereumAbiFunctionAddParamString(func, TWStringCreateWithUTF8Bytes("Hello, world!"), false);
    TWEthereumAbiFunctionAddParamAddress(func, 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")), false);
    TWEthereumAbiFunctionAddParamBytes(func, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353")), false);
    TWEthereumAbiFunctionAddParamBytesFix(func, 5, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353")), false);
    
    TWEthereumAbiFunctionAddInArrayParamUInt8(func, TWEthereumAbiFunctionAddParamArray(func, false), 1);
    TWEthereumAbiFunctionAddInArrayParamUInt16(func, TWEthereumAbiFunctionAddParamArray(func, false), 2);
    TWEthereumAbiFunctionAddInArrayParamUInt32(func, TWEthereumAbiFunctionAddParamArray(func, false), 3);
    TWEthereumAbiFunctionAddInArrayParamUInt64(func, TWEthereumAbiFunctionAddParamArray(func, false), 4);
    TWEthereumAbiFunctionAddInArrayParamUIntN(func, TWEthereumAbiFunctionAddParamArray(func, false), 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWEthereumAbiFunctionAddInArrayParamUInt256(func, TWEthereumAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWEthereumAbiFunctionAddInArrayParamInt8(func, TWEthereumAbiFunctionAddParamArray(func, false), 1);
    TWEthereumAbiFunctionAddInArrayParamInt16(func, TWEthereumAbiFunctionAddParamArray(func, false), 2);
    TWEthereumAbiFunctionAddInArrayParamInt32(func, TWEthereumAbiFunctionAddParamArray(func, false), 3);
    TWEthereumAbiFunctionAddInArrayParamInt64(func, TWEthereumAbiFunctionAddParamArray(func, false), 4);
    TWEthereumAbiFunctionAddInArrayParamIntN(func, TWEthereumAbiFunctionAddParamArray(func, false), 168, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWEthereumAbiFunctionAddInArrayParamInt256(func, TWEthereumAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0123")));
    TWEthereumAbiFunctionAddInArrayParamBool(func, TWEthereumAbiFunctionAddParamArray(func, false), true);
    TWEthereumAbiFunctionAddInArrayParamString(func, TWEthereumAbiFunctionAddParamArray(func, false), TWStringCreateWithUTF8Bytes("Hello, world!"));
    TWEthereumAbiFunctionAddInArrayParamAddress(func, TWEthereumAbiFunctionAddParamArray(func, false), 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")));
    TWEthereumAbiFunctionAddInArrayParamBytes(func, TWEthereumAbiFunctionAddParamArray(func, false), TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353")));
    TWEthereumAbiFunctionAddInArrayParamBytesFix(func, TWEthereumAbiFunctionAddParamArray(func, false), 5, TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("31323334353")));

    // check back out params
    EXPECT_EQ(1, TWEthereumAbiFunctionGetParamUInt8(func, 0, false));
    EXPECT_EQ(4, TWEthereumAbiFunctionGetParamUInt64(func, 3, false));
    EXPECT_EQ(true, TWEthereumAbiFunctionGetParamBool(func, 12, false));
    EXPECT_EQ(std::string("Hello, world!"), std::string(TWStringUTF8Bytes(TWEthereumAbiFunctionGetParamString(func, 13, false))));
    TWEthereumAbiFunctionGetParamAddress(func, 14, false);

    TWString* type = TWEthereumAbiFunctionGetType(func);
    EXPECT_EQ(
        "monster(uint8,uint16,uint32,uint64,uint168,uint256,int8,int16,int32,int64,int168,int256,bool,string,address,bytes,bytes5,"
        "uint8[],uint16[],uint32[],uint64[],uint168[],uint256[],int8[],int16[],int32[],int64[],int168[],int256[],bool[],string[],address[],bytes[],bytes5[])",
        std::string(TWStringUTF8Bytes(type)));

    TWData* encoded = TWEthereumAbiEncoderEncode(func);
    Data enc2 = data(TWDataBytes(encoded), TWDataSize(encoded));
    EXPECT_EQ(4 + 76 * 32, enc2.size());

    // delete
    TWEthereumAbiEncoderDeleteFunction(func);
}

TEST(TWEthereumAbi, DecodeOutputFuncCase1) {
    TWEthereumAbiFunction* func = TWEthereumAbiEncoderBuildFunction(TWStringCreateWithUTF8Bytes("readout"));
    EXPECT_TRUE(func != nullptr);

    TWEthereumAbiFunctionAddParamAddress(func, 
        TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("f784682c82526e245f50975190ef0fff4e4fc077")), false);
    TWEthereumAbiFunctionAddParamUInt64(func, 1000, false);
    EXPECT_EQ(0, TWEthereumAbiFunctionAddParamUInt64(func, 0, true));

    // original output value
    EXPECT_EQ(0, TWEthereumAbiFunctionGetParamUInt64(func, 0, true));

    // decode
    auto encoded = TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0000000000000000000000000000000000000000000000000000000000000045"));
    EXPECT_EQ(true, TWEthereumAbiEncoderDecodeOutput(func, encoded));

    // new output value
    EXPECT_EQ(0x45, TWEthereumAbiFunctionGetParamUInt64(func, 0, true));

    // delete
    TWEthereumAbiEncoderDeleteFunction(func);
}

} // namespace TW::Ethereum
