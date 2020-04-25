// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Script.h"
#include "../interface/TWTestUtilities.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

const Script PayToScriptHash(parse_hex("a914" "4733f37cf4db86fbc2efed2500b4f4e49f312023" "87"));
const Script PayToWitnessScriptHash22(parse_hex("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1"));
const Script PayToWitnessScriptHash34(parse_hex("0020ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db"));
const Script PayToWitnessPublicKeyHash(parse_hex("001479091972186c449eb1ded22b78e40d009bdf0089"));
const Script PayToPubKeySecp256k1(parse_hex("2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac"));
const Script PayToPubKeySecp256k1Extended(parse_hex("410499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91ac"));
const Script PayToPubKeyHash(parse_hex("76a91479091972186c449eb1ded22b78e40d009bdf008988ac"));

TEST(BitcoinScript, PayToPubKey) {
    Data res;
    EXPECT_EQ(PayToPubKeySecp256k1.matchPayToPubkey(res), true);
    EXPECT_EQ(hex(res), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(PayToPubKeySecp256k1Extended.matchPayToPubkey(res), true);
    EXPECT_EQ(hex(res), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");

    EXPECT_EQ(PayToScriptHash.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessScriptHash22.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessScriptHash34.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToPubKeyHash.matchPayToPubkey(res), false);
}

TEST(BitcoinScript, PayToPubKeyHash) {
    Data res;
    EXPECT_EQ(PayToPubKeyHash.matchPayToPubkeyHash(res), true);
    EXPECT_EQ(hex(res), "79091972186c449eb1ded22b78e40d009bdf0089");
    EXPECT_EQ(PayToPubKeySecp256k1.matchPayToPubkey(res), true);
    EXPECT_EQ(hex(res), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(PayToPubKeySecp256k1Extended.matchPayToPubkey(res), true);
    EXPECT_EQ(hex(res), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");

    EXPECT_EQ(PayToScriptHash.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessScriptHash22.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessScriptHash34.matchPayToPubkey(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToPubkey(res), false);
}

TEST(BitcoinScript, PayToScriptHash) {
    EXPECT_EQ(PayToScriptHash.isPayToScriptHash(), true);

    EXPECT_EQ(PayToWitnessScriptHash22.isPayToScriptHash(), false);
    EXPECT_EQ(PayToWitnessScriptHash34.isPayToScriptHash(), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPubKeySecp256k1.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPubKeySecp256k1Extended.isPayToScriptHash(), false);
    EXPECT_EQ(PayToPubKeyHash.isPayToScriptHash(), false);

    Data res;
    EXPECT_EQ(PayToScriptHash.matchPayToScriptHash(res), true);
    EXPECT_EQ(hex(res), "4733f37cf4db86fbc2efed2500b4f4e49f312023");

    EXPECT_EQ(PayToWitnessScriptHash22.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToWitnessScriptHash34.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPubKeySecp256k1.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPubKeySecp256k1Extended.matchPayToScriptHash(res), false);
    EXPECT_EQ(PayToPubKeyHash.matchPayToScriptHash(res), false);
}

TEST(BitcoinScript, PayToWitnessScriptHash) {
    EXPECT_EQ(PayToWitnessScriptHash22.isPayToWitnessScriptHash(), true);
    EXPECT_EQ(PayToWitnessPublicKeyHash.isPayToWitnessScriptHash(), true);

    EXPECT_EQ(PayToScriptHash.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToWitnessScriptHash34.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPubKeySecp256k1.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPubKeySecp256k1Extended.isPayToWitnessScriptHash(), false);
    EXPECT_EQ(PayToPubKeyHash.isPayToWitnessScriptHash(), false);

    Data res;
    EXPECT_EQ(PayToWitnessScriptHash34.matchPayToWitnessScriptHash(res), true);
    EXPECT_EQ(hex(res), "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db");

    EXPECT_EQ(PayToScriptHash.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToWitnessScriptHash22.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPubKeySecp256k1.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPubKeySecp256k1Extended.matchPayToWitnessScriptHash(res), false);
    EXPECT_EQ(PayToPubKeyHash.matchPayToWitnessScriptHash(res), false);
}

TEST(BitcoinScript, PayToWitnessPublicKeyHash) {
    Data res;
    EXPECT_EQ(PayToWitnessPublicKeyHash.matchPayToWitnessPublicKeyHash(res), true);
    EXPECT_EQ(hex(res), "79091972186c449eb1ded22b78e40d009bdf0089");
    EXPECT_EQ(PayToWitnessScriptHash22.matchPayToWitnessPublicKeyHash(res), true);
    EXPECT_EQ(hex(res), "1d0f172a0ecb48aee1be1f2687d2963ae33f71a1");
    EXPECT_EQ(PayToPubKeyHash.matchPayToWitnessPublicKeyHash(res), false);
}

TEST(BitcoinScript, WitnessProgram) {
    EXPECT_EQ(PayToWitnessScriptHash22.isWitnessProgram(), true);
    EXPECT_EQ(PayToScriptHash.isWitnessProgram(), false);
}

TEST(BitcoinScript, EncodeNumber) {
    EXPECT_EQ(Script::encodeNumber(0), OP_0);
    EXPECT_EQ(Script::encodeNumber(1), OP_1);
    EXPECT_EQ(Script::encodeNumber(3), OP_3);
    EXPECT_EQ(Script::encodeNumber(9), OP_9);
}

TEST(BitcoinScript, DecodeNumber) {
    EXPECT_EQ(Script::decodeNumber(OP_0), 0);
    EXPECT_EQ(Script::decodeNumber(OP_1), 1);
    EXPECT_EQ(Script::decodeNumber(OP_3), 3);
    EXPECT_EQ(Script::decodeNumber(OP_9), 9);
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
    ASSERT_EQ(keys.size(), 1);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    EXPECT_EQ(Script(parse_hex("51" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "51" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("52" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "51" "ae")).matchMultisig(keys, required), false);

    // valid two keys
    EXPECT_EQ(Script(parse_hex("52" "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "21" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" "52" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 2);
    ASSERT_EQ(keys.size(), 2);
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
    ASSERT_EQ(keys.size(), 1);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    // OP_PUSHDATA2
    EXPECT_EQ(Script(parse_hex("514dae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "0500" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "0500" "0102030405" "ae")).matchMultisig(keys, required), false);
    EXPECT_EQ(Script(parse_hex("514d" "2100" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ae")).matchMultisig(keys, required), false);

    // valid one key, OP_PUSHDATA2
    EXPECT_EQ(Script(parse_hex("514d" "2100" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "51" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 1);
    ASSERT_EQ(keys.size(), 1);
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
    ASSERT_EQ(keys.size(), 1);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    // valid three keys, mixed
    EXPECT_EQ(Script(parse_hex("53" 
        "21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" 
        "4d" "2100" "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1" 
        "4e" "21000000" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" 
        "53" "ae")).matchMultisig(keys, required), true);
    EXPECT_EQ(required, 3);
    ASSERT_EQ(keys.size(), 3);
    EXPECT_EQ(hex(keys[0]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    EXPECT_EQ(hex(keys[1]), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_EQ(hex(keys[2]), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
}
