// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/Script.h"
#include "Decred/OutPoint.h"
#include "Decred/Transaction.h"
#include "Decred/TransactionInput.h"
#include "Decred/TransactionOutput.h"
#include "HexCoding.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

#include <TrustWalletCore/TWBitcoinSigHashType.h>

using namespace TW;
using namespace TW::Decred;

TEST(DecredTransaction, SignatureHash) {
    Decred::Transaction transaction;

    auto po0 = OutPoint(
        parse_hex("5897de6bd6027a475eadd57019d4e6872c396d0716c4875a5f1a6fcfdf385c1f"), 0, 0);
    transaction.inputs.emplace_back(po0, Bitcoin::Script(), 4294967295);

    auto po1 = OutPoint(
        parse_hex("bf829c6bcf84579331337659d31f89dfd138f7f7785802d5501c92333145ca7c"), 18, 0);
    transaction.inputs.emplace_back(po1, Bitcoin::Script(), 4294967295);

    auto po2 = OutPoint(
        parse_hex("22a6f904655d53ae2ff70e701a0bbd90aa3975c0f40bfc6cc996a9049e31cdfc"), 1, 0);
    transaction.inputs.emplace_back(po2, Bitcoin::Script(), 4294967295);

    auto oscript0 =
        Bitcoin::Script(parse_hex("76a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac"));
    transaction.outputs.emplace_back(18000000, 0, oscript0);

    auto oscript1 =
        Bitcoin::Script(parse_hex("0x76a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac"));
    transaction.outputs.emplace_back(400000000, 0, oscript1);

    auto preOutScript =
        Bitcoin::Script(parse_hex("a914f5916158e3e2c4551c1796708db8367207ed13bb87"));

    // throw exception
    EXPECT_EXCEPTION(transaction.computeSignatureHash(preOutScript, transaction.outputs.size(),
                                                      TWBitcoinSigHashTypeSingle),
                     "attempt to sign single input at index larger than the number of outputs");

    // All
    auto hash = transaction.computeSignatureHash(preOutScript, 1, TWBitcoinSigHashTypeAll);
    EXPECT_EQ(hex(hash), "05b01b517f41112e279b1a9da89d7847a64e5143dba799d7355b1c6c97b4b397");

    // AnyoneCanPay|Single
    hash = transaction.computeSignatureHash(
        preOutScript, 1,
        TWBitcoinSigHashType(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeSingle));
    EXPECT_EQ(hex(hash), "fa2a276cd2c4d9f56e05ccae6022ca8c201dccffda36b45c39a031711135bc58");

    // AnyoneCanPay|None
    hash = transaction.computeSignatureHash(
        preOutScript, 1,
        TWBitcoinSigHashType(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeNone));
    EXPECT_EQ(hex(hash), "82338ab38b4d154c72de55c4700909ad97c0f9bb10d8858759d0c90acb220edb");

    // All & noWitness
    Data result;
    transaction.serializeType = SerializeType::noWitness;
    transaction.encode(result);
    EXPECT_EQ(hex(result),
              "01000100035897de6bd6027a475eadd57019d4e6872c396d0716c4875a5f1a6fcfdf385c1f0000000000"
              "ffffffffbf829c6bcf84579331337659d31f89dfd138f7f7785802d5501c92333145ca7c1200000000ff"
              "ffffff22a6f904655d53ae2ff70e701a0bbd90aa3975c0f40bfc6cc996a9049e31cdfc0100000000ffff"
              "ffff0280a812010000000000001976a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac0084d7"
              "170000000000001976a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac0000000000000000");

    // All & onlyWitness
    result.clear();
    transaction.serializeType = SerializeType::onlyWitness;
    transaction.encode(result);
    EXPECT_EQ(hex(result), "0100020003000000000000000000000000ffffffff00000000000000000000000000fff"
                           "fffff00000000000000000000000000ffffffff00");
}