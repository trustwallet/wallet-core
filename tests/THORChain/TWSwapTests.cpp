// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTHORChainSwap.h>
#include "proto/THORChainSwap.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/Binance.pb.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/Script.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include "PrivateKey.h"

#include "HexCoding.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW::THORChainSwap;
using namespace TW;

const auto Address1Bnb = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx";
const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";
const auto Address1Eth = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7";
const auto VaultBnb = "bnb1n9esxuw8ca7ts8l6w66kdh800s09msvul6vlse";
const auto VaultBtc = "bc1q6m9u2qsu8mh8y7v8rr2ywavtj8g5arzlyhcej7";
const auto VaultEth = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC";
const auto RouterEth = "0x42A5Ed456650a09Dc10EBc6361A7480fDd61f27B";
const Data TestKey1Bnb = parse_hex("bcf8b072560dda05122c99390def2c385ec400e1a93df0657a85cf6b57a715da");
const Data TestKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
const Data TestKey1Eth = parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904");

TEST(TWTHORChainSwap, SwapBtcToEth) {
    // prepare swap input
    Proto::SwapInput input;
    input.set_from_chain(Proto::BTC);
    input.set_from_address(Address1Btc);
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::ETH);
    toAsset.set_symbol("ETH");
    toAsset.set_token_id("");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address(Address1Eth);
    input.set_vault_address(VaultBtc);
    input.set_router_address("");
    input.set_from_amount("1000000");
    input.set_to_amount_limit("140000000000000000");

    // serialize input
    const auto inputData = input.SerializeAsString();
    EXPECT_EQ(hex(inputData), "0801122a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070381a0708021203455448222a3078623966353737316332373636346266323238326439386530396437663530636563376362303161372a2a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a373a07313030303030304212313430303030303030303030303030303030");
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    // invoke swap
    const auto outputTWData = WRAPD(TWTHORChainSwapBuildSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 178);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::BTC);
    EXPECT_EQ(outputProto.to_chain(), Proto::ETH);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
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
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), "bc").string(), Address1Btc);
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

TEST(TWTHORChainSwap, SwapEthBnb) {
    // prepare swap input
    Proto::SwapInput input;
    input.set_from_chain(Proto::ETH);
    input.set_from_address(Address1Eth);
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::BNB);
    toAsset.set_symbol("BNB");
    toAsset.set_token_id("");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address(Address1Bnb);
    input.set_vault_address(VaultEth);
    input.set_router_address(RouterEth);
    input.set_from_amount("50000000000000000");
    input.set_to_amount_limit("600003");

    // serialize input
    const auto inputData = input.SerializeAsString();
    EXPECT_EQ(hex(inputData), "0802122a3078623966353737316332373636346266323238326439386530396437663530636563376362303161371a0708031203424e42222a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372782a2a307831303931633444653661336346303943644130304162444165443432633763334236394338334543322a3078343241354564343536363530613039446331304542633633363141373438306644643631663237423a1135303030303030303030303030303030304206363030303033");
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    // invoke swap
    const auto outputTWData = WRAPD(TWTHORChainSwapBuildSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 311);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::ETH);
    EXPECT_EQ(outputProto.to_chain(), Proto::BNB);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
    EXPECT_TRUE(outputProto.has_ethereum());
    Ethereum::Proto::SigningInput txInput = outputProto.ethereum();
    
    // sign tx input for signed full tx
    // set few fields before signing
    auto chainId = store(uint256_t(1));
    txInput.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(3));
    txInput.set_nonce(nonce.data(), nonce.size());
    auto gasPrice = store(uint256_t(30000000000));
    txInput.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(80000));
    txInput.set_gas_limit(gasLimit.data(), gasLimit.size());
    txInput.set_private_key("");
    txInput.set_private_key(TestKey1Eth.data(), TestKey1Eth.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(txInput, TWCoinTypeEthereum);
    EXPECT_EQ(hex(output.encoded()), "f90151038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003e535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033000025a06ae104be3201baca38315352f81fac70ca4dd47339981914e64e91149813e780a066a3f0b2c44ddf5a96a38481274f623f552a593d723237d6742185f4885c0064");
}

TEST(TWTHORChainSwap, SwapBnbBtc) {
    // prepare swap input
    Proto::SwapInput input;
    input.set_from_chain(Proto::BNB);
    input.set_from_address(Address1Bnb);
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::BTC);
    toAsset.set_symbol("BTC");
    toAsset.set_token_id("");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address(Address1Btc);
    input.set_vault_address(VaultBnb);
    input.set_router_address("");
    input.set_from_amount("10000000");
    input.set_to_amount_limit("10000000");

    // serialize input
    const auto inputData = input.SerializeAsString();
    EXPECT_EQ(hex(inputData), "0803122a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372781a0708011203425443222a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070382a2a626e62316e396573787577386361377473386c367736366b64683830307330396d7376756c36766c73653a08313030303030303042083130303030303030");
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    // invoke swap
    const auto outputTWData = WRAPD(TWTHORChainSwapBuildSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 149);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::BNB);
    EXPECT_EQ(outputProto.to_chain(), Proto::BTC);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
    EXPECT_TRUE(outputProto.has_binance());
    Binance::Proto::SigningInput txInput = outputProto.binance();
    
    // set few fields before signing
    txInput.set_chain_id("Binance-Chain-Tigris");
    txInput.set_private_key(TestKey1Bnb.data(), TestKey1Bnb.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(txInput, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8002f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e191240af2117ebd42e31a9562738e9f8933b3b54b59e6305b5675956525e4edb6a6ac65abea614e90959ae388664e2b36bf720024879b6047e174e3cff95f8f364a4e71a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030");
}

TEST(TWTHORChainSwap, NegativeInvalidInput) {
    const auto inputData = parse_hex("00112233");
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    const auto outputTWData = WRAPD(TWTHORChainSwapBuildSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 39);
    EXPECT_EQ(hex(outputData), "1a2508021221436f756c64206e6f7420646573657269616c697a6520696e7075742070726f746f");
    EXPECT_EQ(hex(data(std::string("Could not deserialize input proto"))), "436f756c64206e6f7420646573657269616c697a6520696e7075742070726f746f");
}
