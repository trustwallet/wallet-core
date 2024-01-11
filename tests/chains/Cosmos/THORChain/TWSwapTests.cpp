// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "PrivateKey.h"
#include "proto/Binance.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/THORChainSwap.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWTHORChainSwap.h>

#include "HexCoding.h"
#include "uint256.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::THORChainSwap::tests {

// clang-format off
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
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::BTC);
    *input.mutable_from_asset() = fromAsset;
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
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a020801122a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070381a0708021203455448222a3078623966353737316332373636346266323238326439386530396437663530636563376362303161372a2a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a373a07313030303030304212313430303030303030303030303030303030");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 178ul);
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
    EXPECT_EQ(txInput.coin_type(), 0ul);

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
        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(hex(output.encoded()), // printed using prettyPrintTransaction
            "01000000" // version
            "0001" // marker & flag
            "01" // inputs
                "1234000000000000000000000000000000000000000000000000000000005678"  "00000000"  "00"  ""  "ffffffff"
            "03" // outputs
                "40420f0000000000"  "16"  "0014d6cbc5021c3eee72798718d447758b91d14e8c5f"
                "d49ceb0200000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
                "0000000000000000"  "49"  "6a473d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a313430303030303030303030303030303030"
            // witness
                "02"
                    "48"  "3045022100a67f84cbde5affbb46ffff2b33c1453ff2de70ef990fc974175d9a609e5a87ed0220589c57d958208f866c9477c7d6c9075dea4c58622debb02eab85032b8b6d373001"
                    "21"  "021e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee"
            "00000000" // nLockTime
        );
    }
}

TEST(TWTHORChainSwap, SwapEthBnb) {
    // prepare swap input
    Proto::SwapInput input;
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::ETH);
    *input.mutable_from_asset() = fromAsset;
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
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a020802122a3078623966353737316332373636346266323238326439386530396437663530636563376362303161371a0708031203424e42222a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372782a2a307831303931633444653661336346303943644130304162444165443432633763334236394338334543322a3078343241354564343536363530613039446331304542633633363141373438306644643631663237423a1135303030303030303030303030303030304206363030303033");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 141ul);
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
    EXPECT_EQ(hex(output.encoded()), "02f8a60103808083013880941091c4de6a3cf09cda00abdaed42c7c3b69c83ec87b1a2bc2ec50000b83b3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033c080a00d605807f983650fafbfdcf0c33bdf0c524c7185eae8c1501ae24892faf16b1ba03b51b0a35e4754ab21d1e48fed635d8486048df50c253ba9af4cebdb6a92a450");
}

TEST(TWTHORChainSwap, SwapBnbBtc) {
    // prepare swap input
    Proto::SwapInput input;
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::BNB);
    *input.mutable_from_asset() = fromAsset;
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
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a020803122a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372781a0708011203425443222a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070382a2a626e62316e396573787577386361377473386c367736366b64683830307330396d7376756c36766c73653a08313030303030303042083130303030303030");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 146ul);
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
    EXPECT_EQ(hex(output.encoded()), "fd01f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e19124086d43e9bdf12508a9a1415f5f970dfa5ff5930dee01d922f99779b63190735ba1d69694bda203b6678939a5c1eab0a52ed32bb67864ec7864de37b333533ae0c1a3d3d3a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030");
}

TEST(TWTHORChainSwap, SwapAtomBnb) {
    // prepare swap input
    Proto::SwapInput input;
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::ATOM);
    fromAsset.set_symbol("ATOM");
    *input.mutable_from_asset() = fromAsset;
    input.set_from_address("cosmos1v4e6vpehwrfez2dqepnw9g6t4fl83xzegd5ac9");
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::BNB);
    toAsset.set_symbol("BNB");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address("bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2");
    input.set_vault_address("cosmos154t5ycejlr7ax3ynmed9z05yg5a27y9u6pj5hq");
    input.set_from_amount("300000");
    input.set_to_amount_limit("819391");
    input.set_affiliate_fee_address("t");
    input.set_affiliate_fee_rate_bp("0");

    // serialize input
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a080807120441544f4d122d636f736d6f73317634653676706568777266657a32647165706e773967367434666c3833787a656764356163391a0708031203424e42222a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872322a2d636f736d6f7331353474357963656a6c7237617833796e6d6564397a303579673561323779397536706a3568713a0633303030303042063831393339314a0174520130");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 204ul);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::ATOM);
    EXPECT_EQ(outputProto.to_chain(), Proto::BNB);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
    EXPECT_TRUE(outputProto.has_cosmos());
    Cosmos::Proto::SigningInput txInput = outputProto.cosmos();

    ASSERT_EQ(txInput.memo(), "=:BNB.BNB:bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2:819391:t:0");
    auto& fee = *txInput.mutable_fee();
    fee.set_gas(200000);
    auto& fee_amount = *fee.add_amounts();
    fee_amount.set_denom("uatom");
    fee_amount.set_amount("500");

    txInput.set_account_number(1483163);
    txInput.set_sequence(1);

    auto privKey = parse_hex("3eed3f32b8ba90e579ba46f37e7445fb4b34558306aa5bc32c525a93dff486e7");
    txInput.set_private_key(privKey.data(), privKey.size());

    // sign and encode resulting input
    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(txInput, TWCoinTypeCosmos);
    EXPECT_EQ(output.error_message(), "");
    ASSERT_EQ(output.serialized(), "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CtMBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczF2NGU2dnBlaHdyZmV6MmRxZXBudzlnNnQ0Zmw4M3h6ZWdkNWFjORItY29zbW9zMTU0dDV5Y2VqbHI3YXgzeW5tZWQ5ejA1eWc1YTI3eTl1NnBqNWhxGg8KBXVhdG9tEgYzMDAwMDASPz06Qk5CLkJOQjpibmIxczRrYWxseG5ncHlzcHptNm5yZXprbWw5cmd5dzZreHB3NGZocjI6ODE5MzkxOnQ6MBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDmmNIYBvR9bnOloFEMOWdk9DHYIGe7naW0T19y+/k1SUSBAoCCAEYARISCgwKBXVhdG9tEgM1MDAQwJoMGkCFqUWtDu0pn1P/cnVQnIJIWF8HFJn/xkJh55Mc7ZLVPF60uXYUOg8nNkt0IQPuTFREw32/yff6lmA5w6KwPen/\"}");

    // https://viewblock.io/thorchain/tx/07F47D71A74245538E205F24ADB4BBB799B49C3A8A8875665D249EA51662AA50
    // https://www.mintscan.io/cosmos/txs/07F47D71A74245538E205F24ADB4BBB799B49C3A8A8875665D249EA51662AA50
    // https://binance.mintscan.io/txs/2C97061737B16B234990B9B18A2BF65F7C7418FF9E39A68E634C832E4E4C59CE
}

TEST(TWTHORChainSwap, SwapRuneDoge) {
    // prepare swap input
    Proto::SwapInput input;
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::THOR);
    fromAsset.set_symbol("RUNE");
    *input.mutable_from_asset() = fromAsset;
    input.set_from_address("thor14j5lwl8ulexrqp5x39kmkctv2937694z3jn2dz");
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::DOGE);
    toAsset.set_symbol("DOGE");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address("DNhRF1h8J4ZnB1bxp9kaqhVLYetkx1nSJ5");
    input.set_from_amount("150000000");
    input.set_to_amount_limit("10000000");
    input.set_affiliate_fee_address("tr");
    input.set_affiliate_fee_rate_bp("0");

    // serialize input
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a06120452554e45122b74686f7231346a356c776c38756c6578727170357833396b6d6b637476323933373639347a336a6e32647a1a0808041204444f47452222444e6852463168384a345a6e4231627870396b617168564c5965746b78316e534a353a09313530303030303030420831303030303030304a027472520130");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 153ul);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::THOR);
    EXPECT_EQ(outputProto.to_chain(), Proto::DOGE);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
    EXPECT_TRUE(outputProto.has_cosmos());
    Cosmos::Proto::SigningInput txInput = outputProto.cosmos();

    ASSERT_EQ(txInput.messages(0).thorchain_deposit_message().memo(), "=:DOGE.DOGE:DNhRF1h8J4ZnB1bxp9kaqhVLYetkx1nSJ5:10000000:tr:0");
    auto& fee = *txInput.mutable_fee();
    fee.set_gas(50000000);

    txInput.set_account_number(75247);
    txInput.set_sequence(8);

    auto privKey = parse_hex("2659e41d54ebd449d68b9d58510d8eeeb837ee00d6ecc760b7a731238d8c3113");
    txInput.set_private_key(privKey.data(), privKey.size());

    // sign and encode resulting input
    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(txInput, TWCoinTypeCosmos);
    EXPECT_EQ(output.error_message(), "");
    ASSERT_EQ(output.serialized(), "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Co0BCooBChEvdHlwZXMuTXNnRGVwb3NpdBJ1Ch8KEgoEVEhPUhIEUlVORRoEUlVORRIJMTUwMDAwMDAwEjw9OkRPR0UuRE9HRTpETmhSRjFoOEo0Wm5CMWJ4cDlrYXFoVkxZZXRreDFuU0o1OjEwMDAwMDAwOnRyOjAaFKyp93z8/kwwBoaJbbthbFFj7RaiElkKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQO5lUOUgVbcO1IQFrppQEnQOtAeVD7aDuUi3l6QAzblKRIECgIIARgIEgUQgOHrFxpABOMrkMdq0FFNUEvjE7DDFpDW3EudV2qPhNCD4FrYtHsiBjMefdBaN8Ddp2Fucqs6OMkoXBEoW/u1msDqnvaXdA==\"}");

    // https://viewblock.io/thorchain/tx/29C8B558051A0E0B1F44E4FFED034EDD204A7249A824DCE06C72C28D6114B5E3
    // https://dogechain.info/tx/905ce02ec3397d6d4f2cbe63ebbff2ccf8b9f16d7ea136319be5ed543cdb66f3
}

TEST(TWTHORChainSwap, SwapRuneBnbStreamParams) {
    // prepare swap input
    Proto::SwapInput input;
    Proto::Asset fromAsset;
    fromAsset.set_chain(Proto::THOR);
    fromAsset.set_symbol("RUNE");
    *input.mutable_from_asset() = fromAsset;
    input.set_from_address("thor157vzvw2chydgf8g4qu2cqhlsyhq0mydutmd0p7");
    Proto::Asset toAsset;
    toAsset.set_chain(Proto::BNB);
    toAsset.set_symbol("BNB");
    *input.mutable_to_asset() = toAsset;
    input.set_to_address("bnb1swlv73yc6rc7z4n244gcpjknqh22m7kpjpr0mw");
    input.set_from_amount("170000000");
    // Don't set `toAmountLimit`, should be 0 by default.
    auto* streamParams = input.mutable_stream_params();
    streamParams->set_interval("1");
    streamParams->set_quantity("0");
    input.set_affiliate_fee_address("tr");
    input.set_affiliate_fee_rate_bp("0");

    // serialize input
    const auto inputData_ = input.SerializeAsString();
    EXPECT_EQ(hex(inputData_), "0a06120452554e45122b74686f72313537767a7677326368796467663867347175326371686c73796871306d796475746d643070371a0708031203424e42222a626e623173776c7637337963367263377a346e3234346763706a6b6e716832326d376b706a7072306d773a093137303030303030304a0274725201306a060a0131120130");
    const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));

    // invoke swap
    const auto outputTWData_ = WRAPD(TWTHORChainSwapBuildSwap(inputTWData_.get()));
    const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
    EXPECT_EQ(outputData.size(), 156ul);
    // parse result in proto
    Proto::SwapOutput outputProto;
    EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
    EXPECT_EQ(outputProto.from_chain(), Proto::THOR);
    EXPECT_EQ(outputProto.to_chain(), Proto::BNB);
    EXPECT_EQ(outputProto.error().code(), 0);
    EXPECT_EQ(outputProto.error().message(), "");
    EXPECT_TRUE(outputProto.has_cosmos());
    Cosmos::Proto::SigningInput txInput = outputProto.cosmos();

    ASSERT_EQ(txInput.messages(0).thorchain_deposit_message().memo(), "=:BNB.BNB:bnb1swlv73yc6rc7z4n244gcpjknqh22m7kpjpr0mw:0/1/0:tr:0");
    auto& fee = *txInput.mutable_fee();
    fee.set_gas(50000000);

    txInput.set_account_number(76456);
    txInput.set_sequence(0);

    auto privKey = parse_hex("15f9be0e6c80949f3dbe24fd9614027869af1e41953a86fdced947b0b1f3efa7");
    txInput.set_private_key(privKey.data(), privKey.size());

    // sign and encode resulting input
    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(txInput, TWCoinTypeCosmos);
    EXPECT_EQ(output.error_message(), "");
    ASSERT_EQ(output.serialized(), "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpABCo0BChEvdHlwZXMuTXNnRGVwb3NpdBJ4Ch8KEgoEVEhPUhIEUlVORRoEUlVORRIJMTcwMDAwMDAwEj89OkJOQi5CTkI6Ym5iMXN3bHY3M3ljNnJjN3o0bjI0NGdjcGprbnFoMjJtN2twanByMG13OjAvMS8wOnRyOjAaFKeYJjlYuRqEnRUHFYBf8CXA/ZG8ElcKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNWwhqmW30kANTyAfdGJPa9BfZlI3xkAjqLWmhynukWThIECgIIARIFEIDh6xcaQNzvOBmgAgRriO5lsEgU4o58Gxu4mA71XZNyf5XXWBo5L9HkaJiDXE/YOlWPFj7iy86vDXVR1798pmc3n5EbkQ0=\"}");

    // https://viewblock.io/thorchain/tx/317443DD48DDEE8811D0DCCC2FCA397F8E93DA0AC9C1D5173CB42E69CD0E01B0
    // https://explorer.bnbchain.org/tx/6DE7B60C71F9FC3EEE914AAD8FE80D1A53A2EC59BE759A1C111C1B6C194740D2
}

TEST(TWTHORChainSwap, NegativeInvalidInput) {
    const auto inputData = parse_hex("00112233");
    const auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));

    const auto outputTWData = WRAPD(TWTHORChainSwapBuildSwap(inputTWData.get()));
    const auto outputData = data(TWDataBytes(outputTWData.get()), TWDataSize(outputTWData.get()));
    EXPECT_EQ(outputData.size(), 39ul);
    EXPECT_EQ(hex(outputData), "1a2508021221436f756c64206e6f7420646573657269616c697a6520696e7075742070726f746f");
    EXPECT_EQ(hex(data(std::string("Could not deserialize input proto"))), "436f756c64206e6f7420646573657269616c697a6520696e7075742070726f746f");
}
// clang-format on

} // namespace TW::ThorChainSwap::tests
