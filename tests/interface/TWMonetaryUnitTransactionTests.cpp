
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "MonetaryUnit/Transaction.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace MonetaryUnit;

TEST(MonetaryUnitTransaction, Encode) {
    // Test vector 3 https://github.com/zcash/zips/blob/master/zip-0243.rst
    auto transaction = MonetaryUnit::Transaction();
    transaction.lockTime = 0x0004b029;

    auto outpoint0 = Bitcoin::OutPoint(parse_hex("a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9"), 1);
    transaction.inputs.emplace_back(outpoint0, Bitcoin::Script(parse_hex("483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f")), 0xfffffffe);

    auto script0 = Bitcoin::Script(parse_hex("76a9148132712c3ff19f3a151234616777420a6d7ef22688ac"));
    transaction.outputs.emplace_back(0x02625a00, script0);

    auto script1 = Bitcoin::Script(parse_hex("76a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"));
    transaction.outputs.emplace_back(0x0098958b, script1);

    auto unsignedData = std::vector<uint8_t>();
    transaction.encode(0, unsignedData);

    ASSERT_EQ(hex(unsignedData.begin(), unsignedData.end()),
        /* header */          "04000080"
        /* versionGroupId */  "85202f89"
        /* vin */             "01""a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000""6b483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f""feffffff"
        /* vout */            "02""005a620200000000""1976a9148132712c3ff19f3a151234616777420a6d7ef22688ac"
                                  "8b95980000000000""1976a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"
        /* lockTime */        "29b00400"
        /* expiryHeight */    "48b00400"
        /* valueBalance */    "0000000000000000"
        /* vShieldedSpend */  "00"
        /* vShieldedOutput */ "00"
        /* vJoinSplit */      "00"
    );

    auto scriptCode = Bitcoin::Script(parse_hex("76a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"));
    auto preImage = transaction.getPreImage(scriptCode, 0, 1, 0x02faf080);
    ASSERT_EQ(hex(preImage.begin(), preImage.end()),
        /* header */              "04000080"
        /* versionGroupId */      "85202f89"
        /* hashPrevouts */        "fae31b8dec7b0b77e2c8d6b6eb0e7e4e55abc6574c26dd44464d9408a8e33f11"
        /* hashSequence */        "6c80d37f12d89b6f17ff198723e7db1247c4811d1a695d74d930f99e98418790"
        /* hashOutputs */         "d2b04118469b7810a0d1cc59568320aad25a84f407ecac40b4f605a4e6868454"
        /* hashJoinSplits */      "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedSpends */  "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedOutputs */ "0000000000000000000000000000000000000000000000000000000000000000"
        /* lockTime */            "29b00400"
        /* expiryHeight */        "48b00400"
        /* valueBalance */        "0000000000000000"
        /* hashType */            "01000000"
        /* prevout */             "a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000"
        /* scriptCode */          "1976a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"
        /* amount */              "80f0fa0200000000"
        /* sequence */            "feffffff"
    );

    auto sighash = transaction.getSignatureHash(scriptCode, 0, 1, 0x02faf080, BASE);
    ASSERT_EQ(hex(sighash.begin(), sighash.end()), "f3148f80dfab5e573d5edfe7a850f5fd39234f80b5429d3a57edcc11e34c585b");
}
