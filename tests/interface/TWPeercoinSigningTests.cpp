// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "TWTestUtilities.h"

#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinTransactionSigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(PeercoinSigning, SignP2PKH) {
    auto emptyScript = Script();
    auto unsignedTx = Transaction(1, 0, TW::Hash::sha256d, 0x5DC5448A);

    auto outpoint0 = OutPoint(parse_hex("3d7d3531b0881f244d1f353c208fd00cb18bd152a054460aa4eed815d69ab241"), 0);
    unsignedTx.inputs.emplace_back(outpoint0, emptyScript, 0xffffffff);

    auto outScript0 = Script(parse_hex("76a914ff9a05654150fdc92b1655f49d7f2a8aaf6a3a2a88ac"));
    unsignedTx.outputs.emplace_back(90000, outScript0);

    // Setup input
    Proto::SigningInput input;
    input.set_timestamp(1579853035);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(90'000);
    input.set_byte_fee(1);
    input.set_to_address("PXtfyTjzgXSgTwK5AbszdHQSSxyQN3BLM5");
    input.set_change_address("PXtfyTjzgXSgTwK5AbszdHQSSxyQN3BLM5");

    auto utxoKey0 = parse_hex("83e907a4a861fb919c29b39bbc4e18ed6714f3e349b4439c54a998ccfc79199e");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    //auto utxoKey1 = parse_hex("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9");
    //input.add_private_key(utxoKey1.data(), utxoKey1.size());

    auto scriptPub1 = Script(parse_hex("76a914ff9a05654150fdc92b1655f49d7f2a8aaf6a3a2a88ac"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToPubkeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash.begin(), scriptHash.end());
    ASSERT_EQ(scriptHashHex, "ff9a05654150fdc92b1655f49d7f2a8aaf6a3a2a");

    auto redeemScript = Script::buildPayToPublicKeyHash(scriptHash);
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHashHex] = scriptString;

    auto utxo0 = input.add_utxo();
    auto hash0 = parse_hex("41b29ad615d8eea40a4654a052d18bb10cd08f203c351f4d241f88b031357d3d");
    auto utxo0Script = parse_hex("76a914d68a96304b1fb73aadfea0f44c17061f5e353e1b88ac");
    utxo0->set_script(utxo0Script.data(), utxo0Script.size());
    utxo0->set_amount(100'000);
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    // Sign
    auto signer = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    signer.transaction = unsignedTx;
    signer.plan.utxos = {*utxo0};
    auto result = signer.sign();
    ASSERT_TRUE(result) << result.error();;
    auto signedTx = result.payload();

    // txid = "f7e3624c143b6a170cc44f9337d0fa8ea8564a211de9c077c6889d8c78f80909"

    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(hex(serialized),
        "01000000"
        "8a44c55d"
        "01"
            "3d7d3531b0881f244d1f353c208fd00cb18bd152a054460aa4eed815d69ab241" "00000000" "6a473044022025c0ea702390c702c7ae8b5ea469820bea8d942c8c16439f8f0ba2e91e699efc02200db9b0a48fa2861695fa91df4831a4c7306587e5d2dc85419647f462717bc8f001210274cb0ee652d9457fbb0f3872d43155a6bc16f77bd5749d8826b53db443b1b278" "ffffffff"
        "01"
            "905f010000000000" "1976a914ff9a05654150fdc92b1655f49d7f2a8aaf6a3a2a88ac"
        "00000000"
    );
}
