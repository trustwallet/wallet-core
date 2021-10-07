// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTHORSwap.h>
#include "proto/THORSwap.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/Binance.pb.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/Script.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include "PrivateKey.h"

#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW::THORSwap;
using namespace TW;

TEST(TWTHORSwap, SwapBtcToEth) {
    const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";

    // prepare swap input
    Proto::SwapInput input;
    input.set_from_chain(Proto::BTC);
    input.set_to_chain(Proto::ETH);
    input.set_from_address(Address1Btc);
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::ETH);
    toAsset.set_symbol("ETH");
    toAsset.set_token_id("");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address("0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7");
    input.set_vault_address("bc1q6m9u2qsu8mh8y7v8rr2ywavtj8g5arzlyhcej7");
    input.set_router_address("");
    input.set_from_amount("1000000");
    input.set_to_amount_limit("140000000000000000");

    // serialize input
    const auto inputData = input.SerializeAsString();
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    // invoke swap
    const auto outputTWData = WRAPD(TWTHORSwapSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 176);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::BTC);
    EXPECT_EQ(outputProto.to_chain(), Proto::ETH);
    EXPECT_EQ(outputProto.error(), "");
    EXPECT_TRUE(outputProto.has_bitcoin());
    Bitcoin::Proto::SigningInput txInput = outputProto.bitcoin();
    
    // tx input: check some fields
    EXPECT_EQ(txInput.amount(), 1000000);
    EXPECT_EQ(txInput.to_address(), "bc1q6m9u2qsu8mh8y7v8rr2ywavtj8g5arzlyhcej7");
    EXPECT_EQ(txInput.change_address(), "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8");
    EXPECT_EQ(txInput.output_op_return(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:140000000000000000");
    EXPECT_EQ(txInput.coin_type(), 0);

    // sign tx input for signed full tx
    // set few fields before signing
    txInput.set_byte_fee(20);
    const Data TestKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), 0, "bc").string(), Address1Btc);
    txInput.add_private_key(TestKey1Btc.data(), TestKey1Btc.size());
    auto& utxo = *txInput.add_utxo();
    Data utxoHash = parse_hex("1234000000000000000000000000000000000000000000000000000000005678");
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(Address1Btc, TWCoinTypeBitcoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(50000000);
    txInput.set_use_max_amount(false);

    // sign and encode resulting input
    {
        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(txInput, TWCoinTypeBitcoin);
        EXPECT_EQ(output.error(), 0);
        EXPECT_EQ(hex(output.encoded()), // printed using prettyPrintTransaction
            "01000000" // version
            "0001" // marker & flag
            "01" // inputs
                "1234000000000000000000000000000000000000000000000000000000005678"  "00000000"  "00"  ""  "ffffffff"
            "03" // outputs
                "40420f0000000000"  "16"  "0014d6cbc5021c3eee72798718d447758b91d14e8c5f"
                "609deb0200000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
                "0000000000000000"  "42"  "6a403d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a3134303030303030303030"
            // witness
                "02"
                    "47"  "304402205de19c68b5ea683b9d701d45b09f96658088db76e59ad27bd7b8383ee5d484ec0220245459a4d6d679d8b457564fccc7ecc5831c7ebed49e0366c65ac031e8a5b49201"
                    "21"  "021e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee"
            "00000000" // nLockTime
        );
    }
}
