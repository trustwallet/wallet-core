// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Script.h"
#include "Bitcoin/SignatureBuilder.h"
#include "TestUtilities.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Bitcoin::tests {

const Script PayToScriptHash(parse_hex("a914" "4733f37cf4db86fbc2efed2500b4f4e49f312023" "87"));
const Script PayToWitnessScriptHash(parse_hex("0020" "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db"));
const Script PayToWitnessPublicKeyHash(parse_hex("0014" "79091972186c449eb1ded22b78e40d009bdf0089"));
const Script PayToPublicKeySecp256k1(parse_hex("21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ac"));
const Script PayToPublicKeySecp256k1Extended(parse_hex("41" "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "66b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91ac"));
const Script PayToPublicKeyHash(parse_hex("76a914" "79091972186c449eb1ded22b78e40d009bdf0089" "88ac"));

TEST(BitcoinScript, PayToPublicKey) {
    const Script script = Script::buildPayToPublicKey(parse_hex("03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432"));
    EXPECT_EQ(hex(script.bytes), hex(PayToPublicKeySecp256k1.bytes));
    const Script scriptExtended = Script::buildPayToPublicKey(parse_hex("0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91"));
    EXPECT_EQ(hex(scriptExtended.bytes), hex(PayToPublicKeySecp256k1Extended.bytes));

    Data res;
    EXPECT_EQ(PayToPublicKeySecp256k1.matchPayToPublicKey(res), true);
    EXPECT_EQ(hex(res), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.matchPayToPublicKey(res), true);
    EXPECT_EQ(hex(res), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");

    EXPECT_EQ(PayToScriptHash.matchPayToPublicKey(res), false);
    EXPECT_EQ(PayToWitnessScriptHash.matchPayToPublicKey(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToPublicKey(res), false);
    EXPECT_EQ(PayToPublicKeyHash.matchPayToPublicKey(res), false);
}

TEST(BitcoinScript, PayToPublicKeyHash) {
    const Script script = Script::buildPayToPublicKeyHash(parse_hex("79091972186c449eb1ded22b78e40d009bdf0089"));
    EXPECT_EQ(hex(script.bytes), hex(PayToPublicKeyHash.bytes));

    Data res;
    EXPECT_EQ(PayToPublicKeyHash.matchPayToPublicKeyHash(res), true);
    EXPECT_EQ(hex(res), "79091972186c449eb1ded22b78e40d009bdf0089");
    EXPECT_EQ(PayToPublicKeySecp256k1.matchPayToPublicKey(res), true);
    EXPECT_EQ(hex(res), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.matchPayToPublicKey(res), true);
    EXPECT_EQ(hex(res), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");

    EXPECT_EQ(PayToScriptHash.matchPayToPublicKeyHash(res), false);
    EXPECT_EQ(PayToWitnessScriptHash.matchPayToPublicKeyHash(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToPublicKeyHash(res), false);
}

TEST(BitcoinScript, PayToScriptHash) {
    const Script script = Script::buildPayToScriptHash(parse_hex("4733f37cf4db86fbc2efed2500b4f4e49f312023"));
    EXPECT_EQ(hex(script.bytes), hex(PayToScriptHash.bytes));

    EXPECT_EQ(PayToScriptHash.isPayToScriptHash(), true);
    EXPECT_EQ(PayToScriptHash.bytes.size(), 23ul);

    EXPECT_EQ(PayToWitnessScriptHash.isPayToScriptHash(), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPublicKeyHash.isPayToScriptHash(), false);

    Data res;
    EXPECT_EQ(PayToScriptHash.matchPayToScriptHash(res), true);
    EXPECT_EQ(hex(res), "4733f37cf4db86fbc2efed2500b4f4e49f312023");

    EXPECT_EQ(PayToWitnessScriptHash.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeyHash.matchPayToScriptHash(res), false);
}

TEST(BitcoinScript, PayToScriptHashReplay) {
    const Script script = Script::buildPayToScriptHashReplay(
        parse_hex("2cda89c2f217517108d55ffdf3d90e111d450be9"),
        parse_hex("f1bf729948789aef5801fd91a3bf4e014ffcf4fcbd4f685de2b6990100000000"), 1190791);
    EXPECT_EQ(hex(script.bytes), "a9142cda89c2f217517108d55ffdf3d90e111d450be98720f1bf729948789aef5801fd91a3bf4e014ffcf4fcbd4f685de2b699010000000003872b12b4");

    const Script script2 = Script::lockScriptForAddress(
        "zsiZvKaCW9bSVt7Fy6C79CE5rFR6SEiJxAw", TWCoinTypeZen,
        parse_hex("f1bf729948789aef5801fd91a3bf4e014ffcf4fcbd4f685de2b6990100000000"), 1190791);
    EXPECT_EQ(script.bytes, script2.bytes);

    Data res;
    EXPECT_EQ(script.matchPayToScriptHashReplay(res), true);
    EXPECT_EQ(hex(res), "2cda89c2f217517108d55ffdf3d90e111d450be9");
}

TEST(BitcoinScript, PayToWitnessScriptHash) {
    const Script script = Script::buildPayToWitnessScriptHash(parse_hex("ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db"));
    EXPECT_EQ(hex(script.bytes), hex(PayToWitnessScriptHash.bytes));

    EXPECT_EQ(PayToWitnessScriptHash.isPayToWitnessScriptHash(), true);
    EXPECT_EQ(PayToWitnessScriptHash.bytes.size(), 34ul);

    EXPECT_EQ(PayToScriptHash.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPublicKeyHash.isPayToWitnessScriptHash(), false);

    Data res;
    EXPECT_EQ(PayToWitnessScriptHash.matchPayToWitnessScriptHash(res), true);
    EXPECT_EQ(hex(res), "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db");

    EXPECT_EQ(PayToScriptHash.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPublicKeyHash.matchPayToWitnessScriptHash(res), false);
}

TEST(BitcoinScript, PayToWitnessPublicKeyHash) {
    const Script script = Script::buildPayToWitnessPublicKeyHash(parse_hex("79091972186c449eb1ded22b78e40d009bdf0089"));
    EXPECT_EQ(hex(script.bytes), hex(PayToWitnessPublicKeyHash.bytes));

    EXPECT_EQ(PayToWitnessPublicKeyHash.isPayToWitnessPublicKeyHash(), true);
    EXPECT_EQ(PayToWitnessPublicKeyHash.bytes.size(), 22ul);

    EXPECT_EQ(PayToScriptHash.isPayToWitnessPublicKeyHash(), false);
    EXPECT_EQ(PayToWitnessScriptHash.isPayToWitnessPublicKeyHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.isPayToWitnessPublicKeyHash(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.isPayToWitnessPublicKeyHash(), false);
    EXPECT_EQ(PayToPublicKeyHash.isPayToWitnessPublicKeyHash(), false);

    Data res;
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToWitnessPublicKeyHash(res), true);
    EXPECT_EQ(hex(res), "79091972186c449eb1ded22b78e40d009bdf0089");

    EXPECT_EQ(PayToScriptHash.matchPayToWitnessPublicKeyHash(res), false);
    EXPECT_EQ(PayToWitnessScriptHash.matchPayToWitnessPublicKeyHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.matchPayToWitnessPublicKeyHash(res), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.matchPayToWitnessPublicKeyHash(res), false);
    EXPECT_EQ(PayToPublicKeyHash.matchPayToWitnessPublicKeyHash(res), false);
}

TEST(BitcoinScript, WitnessProgram) {
    EXPECT_EQ(PayToWitnessScriptHash.isWitnessProgram(), true);
    EXPECT_EQ(PayToWitnessPublicKeyHash.isWitnessProgram(), true);

    EXPECT_EQ(PayToScriptHash.isWitnessProgram(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1.isWitnessProgram(), false);
    EXPECT_EQ(PayToPublicKeySecp256k1Extended.isWitnessProgram(), false);
    EXPECT_EQ(PayToPublicKeyHash.isWitnessProgram(), false);
}

TEST(BitcoinScript, EncodeNumber) {
    EXPECT_EQ(Script::encodeNumber(0), OP_0);
    EXPECT_EQ(Script::encodeNumber(1), OP_1);
    EXPECT_EQ(Script::encodeNumber(3), OP_3);
    EXPECT_EQ(Script::encodeNumber(9), OP_9);

    EXPECT_EQ(hex(Script::encodeNumber(int64_t(0))), "00");
    EXPECT_EQ(hex(Script::encodeNumber(int64_t(1))), "51");
    EXPECT_EQ(hex(Script::encodeNumber(int64_t(10000000))), "80969800");
    EXPECT_EQ(hex(Script::encodeNumber(int64_t(-10000000))), "80969880");
}

TEST(BitcoinScript, DecodeNumber) {
    EXPECT_EQ(Script::decodeNumber(OP_0), 0);
    EXPECT_EQ(Script::decodeNumber(OP_1), 1);
    EXPECT_EQ(Script::decodeNumber(OP_3), 3);
    EXPECT_EQ(Script::decodeNumber(OP_9), 9);
}

TEST(BitcoinScript, GetScriptOp) {
    {
        size_t index = 5; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("")).getScriptOp(index, opcode, operand), false);
    }
    {
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4f")).getScriptOp(index, opcode, operand), true);
        EXPECT_EQ(index, 1ul);
        EXPECT_EQ(opcode, 0x4f);
        EXPECT_EQ(hex(operand), "");
    }
    {
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("05" "0102030405")).getScriptOp(index, opcode, operand), true);
        EXPECT_EQ(index, 6ul);
        EXPECT_EQ(opcode, 0x05);
        EXPECT_EQ(hex(operand), "0102030405");
    }
    {   // OP_PUSHDATA1
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4c" "05" "0102030405")).getScriptOp(index, opcode, operand), true);
        EXPECT_EQ(index, 7ul);
        EXPECT_EQ(opcode, 0x4c);
        EXPECT_EQ(hex(operand), "0102030405");
    }
    {   // OP_PUSHDATA1 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4c")).getScriptOp(index, opcode, operand), false);
    }
    {   // OP_PUSHDATA1 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4c" "05" "010203")).getScriptOp(index, opcode, operand), false);
    }
    {   // OP_PUSHDATA2
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4d" "0500" "0102030405")).getScriptOp(index, opcode, operand), true);
        EXPECT_EQ(index, 8ul);
        EXPECT_EQ(opcode, 0x4d);
        EXPECT_EQ(hex(operand), "0102030405");
    }
    {   // OP_PUSHDATA2 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4d" "05")).getScriptOp(index, opcode, operand), false);
    }
    {   // OP_PUSHDATA2 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4d" "0500" "010203")).getScriptOp(index, opcode, operand), false);
    }
    {   // OP_PUSHDATA4
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4e" "05000000" "0102030405")).getScriptOp(index, opcode, operand), true);
        EXPECT_EQ(index, 10ul);
        EXPECT_EQ(opcode, 0x4e);
        EXPECT_EQ(hex(operand), "0102030405");
    }
    {   // OP_PUSHDATA4 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4e" "0500")).getScriptOp(index, opcode, operand), false);
    }
    {   // OP_PUSHDATA4 too short
        size_t index = 0; uint8_t opcode; Data operand;
        EXPECT_EQ(Script(parse_hex("4e" "05000000" "010203")).getScriptOp(index, opcode, operand), false);
    }
}

TEST(BitcoinScript, MatchMultiSig) {
    std::vector<Data> keys;
    int required;
    EXPECT_EQ(Script(parse_hex("")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("20")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("00ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("4fae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("20ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514cae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514c05ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51" "05" "0102030405" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "00ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "52ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51aeae")).matchMultisig(keys, required), false);

    // valid one key
    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 1);
    ASSERT_EQ(keys.size(), 1ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "51" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("52" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "51" "ae")).matchMultisig(keys, required), false);

    // valid two keys
    EXPECT_EQ(Script(parse_hex("52" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "52" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 2);
    ASSERT_EQ(keys.size(), 2ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(hex(keys[1]), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");

    // OP_PUSHDATA1
    EXPECT_EQ(Script(parse_hex("514cae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514c" "05" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514c" "05" "0102030405" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514c" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ae")).matchMultisig(keys, required), false);

    // valid one key, OP_PUSHDATA1
    EXPECT_EQ(Script(parse_hex("514c" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 1);
    ASSERT_EQ(keys.size(), 1ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    // OP_PUSHDATA2
    EXPECT_EQ(Script(parse_hex("514dae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "0500" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "0500" "0102030405" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "2100" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ae")).matchMultisig(keys, required), false);

    // valid one key, OP_PUSHDATA2
    EXPECT_EQ(Script(parse_hex("514d" "2100" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 1);
    ASSERT_EQ(keys.size(), 1ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    // OP_PUSHDATA4
    EXPECT_EQ(Script(parse_hex("514eae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514e" "0500" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514e" "05000000" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514e" "05000000" "0102030405" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514e" "21000000" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ae")).matchMultisig(keys, required), false);

    // valid one key, OP_PUSHDATA2
    EXPECT_EQ(Script(parse_hex("514e" "21000000" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 1);
    ASSERT_EQ(keys.size(), 1ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    // valid three keys, mixed
    EXPECT_EQ(Script(parse_hex("53" 
        "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" 
        "4d" "2100" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" 
        "4e" "21000000" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" 
        "53" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 3);
    ASSERT_EQ(keys.size(), 3ul);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(hex(keys[1]), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_EQ(hex(keys[2]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
}

TEST(BitcoinScript, OpReturn) {
    {
        Data data = parse_hex("00010203");
        Script script = Script::buildOpReturnScript(data);
        EXPECT_EQ(script.bytes.size(), 2 + data.size());
        EXPECT_EQ(hex(script.bytes), "6a0400010203");
    }
    {
        Data data = parse_hex("535741503a54484f522e52554e453a74686f72317470657263616d6b6b7865633071306a6b366c74646e6c7176737732396775617038776d636c3a");
        Script script = Script::buildOpReturnScript(data);
        EXPECT_EQ(script.bytes.size(), 2 + data.size());
        EXPECT_EQ(hex(script.bytes), "6a3b535741503a54484f522e52554e453a74686f72317470657263616d6b6b7865633071306a6b366c74646e6c7176737732396775617038776d636c3a");
    }
    {
        Data data = Data(69);
        data.push_back(0xab);
        Script script = Script::buildOpReturnScript(data);
        EXPECT_EQ(script.bytes.size(), 2 + data.size());
        EXPECT_EQ(hex(script.bytes), "6a46000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ab");
    }
    {
        Data data = Data(74);
        data.push_back(0xab);
        Script script = Script::buildOpReturnScript(data);
        EXPECT_EQ(script.bytes.size(), 2 + data.size());
        EXPECT_EQ(hex(script.bytes), 
            "6a4b"
            "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ab");
    }
    {
        // >75 bytes, with OP_PUSHDATA1
        Data data = Data(79);
        data.push_back(0xab);
        Script script = Script::buildOpReturnScript(data);
        EXPECT_EQ(script.bytes.size(), 3 + data.size());
        EXPECT_EQ(hex(script.bytes), 
            "6a4c50"
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ab");
    }
    {
        // >80 bytes, fails
        EXPECT_EQ(hex(Script::buildOpReturnScript(Data(81)).bytes), "");
        EXPECT_EQ(hex(Script::buildOpReturnScript(Data(255)).bytes), "");
    }
}

TEST(BitcoinScript, OpReturnTooLong) {
    // too long, truncated
    Data data = Data(89);
    data.push_back(0xab);
    Script script = Script::buildOpReturnScript(data);
    EXPECT_EQ(hex(script.bytes), "");
}

TEST(BitcoinTransactionSigner, PushAllEmpty) {
    {
        std::vector<Data> input = {};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), "");
    }
    {
        std::vector<Data> input = {parse_hex("")};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), "00");
    }
    {
        std::vector<Data> input = {parse_hex("09")};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), "59" "09");
    }
    {
        std::vector<Data> input = {parse_hex("00010203040506070809")};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), "0a" "00010203040506070809");
    }
    {
        std::vector<Data> input = {parse_hex("0001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809")};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), "4c50" "0001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809000102030405060708090001020304050607080900010203040506070809");
    }
    {
        // 2-byte len
        Data in1 = Data(256 + 10);
        Data expected = parse_hex("4d" "0a01");
        TW::append(expected, in1);
        std::vector<Data> input = {in1};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), hex(expected));
    }
    {
        // 4-byte len
        Data in1 = Data(65536 + 256 + 10);
        Data expected = parse_hex("4e" "0a010100");
        TW::append(expected, in1);
        std::vector<Data> input = {in1};
        Data res = SignatureBuilder<Transaction>::pushAll(input);
        EXPECT_EQ(hex(res), hex(expected));
    }
}
} // namespace TW::Bitcoin::tests
