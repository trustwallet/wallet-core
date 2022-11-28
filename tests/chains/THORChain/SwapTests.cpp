// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Binance/Address.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/Address.h"
#include "THORChain/Swap.h"
#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/THORChainSwap.pb.h"

#include "Coin.h"
#include "HexCoding.h"
#include "TestUtilities.h"
#include "uint256.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

namespace TW::THORChainSwap {

// Addresses for wallet 'isolate dismiss fury ... note'
const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";
const auto Address1Eth = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7";
const auto Address1Bnb = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx";
const auto Address1Thor = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
const Data TestKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
const Data TestKey1Eth = parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904");
const Data TestKey1Bnb = parse_hex("bcf8b072560dda05122c99390def2c385ec400e1a93df0657a85cf6b57a715da");
const auto VaultBtc = "bc1q6m9u2qsu8mh8y7v8rr2ywavtj8g5arzlyhcej7";
const auto VaultEth = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC";
const auto VaultBnb = "bnb1n9esxuw8ca7ts8l6w66kdh800s09msvul6vlse";
const auto RouterEth = "0x42A5Ed456650a09Dc10EBc6361A7480fDd61f27B";

TEST(THORChainSwap, SwapBtcEth) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Btc)
                   .toAddress(Address1Eth)
                   .vault(VaultBtc)
                   .fromAmount("1000000")
                   .toAmountLimit("140000000000000000")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "080110c0843d1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a473d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a313430303030303030303030303030303030");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 1000000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:140000000000000000");
    EXPECT_EQ(tx.coin_type(), 0ul);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    // set few fields before signing
    tx.set_byte_fee(20);
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), "bc").string(), Address1Btc);
    tx.add_private_key(TestKey1Btc.data(), TestKey1Btc.size());
    auto& utxo = *tx.add_utxo();
    Data utxoHash = parse_hex("1234000000000000000000000000000000000000000000000000000000005678");
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(Address1Btc, TWCoinTypeBitcoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(50000000);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBitcoin);
    EXPECT_EQ(output.error(), 0);
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

TEST(THORChainSwap, SwapBtcBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");

    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Btc)
                   .toAddress(Address1Bnb)
                   .vault(VaultBtc)
                   .fromAmount("200000")
                   .toAmountLimit("140000000")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "080110c09a0c1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a3e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a313430303030303030");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 200000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "=:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:140000000");
    EXPECT_EQ(tx.coin_type(), 0ul);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    // set few fields before signing
    tx.set_byte_fee(80);
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), "bc").string(), Address1Btc);
    tx.add_private_key(TestKey1Btc.data(), TestKey1Btc.size());
    auto& utxo = *tx.add_utxo();
    Data utxoHash = parse_hex("8eae5c3a4c75058d4e3facd5d72f18a40672bcd3d1f35ebf3094bd6c78da48eb");
    std::reverse(utxoHash.begin(), utxoHash.end());
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX - 3);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(Address1Btc, TWCoinTypeBitcoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(450000);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBitcoin);
    EXPECT_EQ(output.error(), 0);
    EXPECT_EQ(hex(output.encoded()), // printed using prettyPrintTransaction
        "01000000" // version
        "0001" // marker & flag
        "01" // inputs
            "eb48da786cbd9430bf5ef3d1d3bc7206a4182fd7d5ac3f4e8d05754c3a5cae8e"  "00000000"  "00"  ""  "fcffffff"
        "03" // outputs
            "400d030000000000"  "16"  "0014d6cbc5021c3eee72798718d447758b91d14e8c5f"
            "b08d030000000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
            "0000000000000000"  "40"  "6a3e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a313430303030303030"
        // witness
            "02"
                "48"  "3045022100e17d8cf207c79edfb7afa16102842b434e1f908bd9858553fd54970f1a8b4334022059583f89c3a126df0da46d92947bcbe7c265a1bb838b696c0e7ea7fc8761c2bf01210"
                "21"  "e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee"
        "00000000" // nLockTime
    );

    // similar real transaction:
    // https://blockchair.com/bitcoin/transaction/1cd9056b212b85d9d7d34d0795a746dd8691b8cd34ef56df0aa9622fbdec5f88
    // https://viewblock.io/thorchain/tx/1CD9056B212B85D9D7D34D0795A746DD8691B8CD34EF56DF0AA9622FBDEC5F88
    // https://explorer.binance.org/tx/8D78469069118E9B9546696214CCD46E63D3FA0D7E854C094D63C8F6061278B7
}

Data SwapTest_ethAddressStringToData(const std::string& asString) {
    if (asString.empty()) {
        return Data();
    }
    auto address = Ethereum::Address(asString);
    Data asData;
    asData.resize(20);
    std::copy(address.bytes.begin(), address.bytes.end(), asData.data());
    return asData;
}

TEST(THORChainSwap, SwapEthBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Eth)
                   .toAddress(Address1Bnb)
                   .vault(VaultEth)
                   .router(RouterEth)
                   .fromAmount("50000000000000000")
                   .toAmountLimit("600003")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a01001201002201002a0100422a30783432413545643435363635306130394463313045426336333631413734383066446436316632374252f30132f0010a07b1a2bc2ec5000012e4011fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003b3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030330000000000");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), RouterEth);
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    Data vaultAddressBin = SwapTest_ethAddressStringToData(VaultEth);
    EXPECT_EQ(hex(vaultAddressBin), "1091c4de6a3cf09cda00abdaed42c7c3b69c83ec");
    auto func = Ethereum::ABI::Function("deposit", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0000000000000000000000000000000000000000")),
                                                       std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(50000000000000000)),
                                                       std::make_shared<Ethereum::ABI::ParamString>("=:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:600003")});
    Data payload;
    func.encode(payload);
    EXPECT_EQ(hex(payload), "1fece7b4"
                            "0000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec"
                            "0000000000000000000000000000000000000000000000000000000000000000"
                            "00000000000000000000000000000000000000000000000000b1a2bc2ec50000"
                            "0000000000000000000000000000000000000000000000000000000000000080"
                            "000000000000000000000000000000000000000000000000000000000000003b"
                            "3d3a424e422e424e423a626e62317573343777646866783038636839377a6475"
                            "656833783375356d757266727833306a656372783a3630303030330000000000");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().amount())), "b1a2bc2ec50000");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().data())), hex(payload));

    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    auto chainId = store(uint256_t(1));
    tx.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(3));
    tx.set_nonce(nonce.data(), nonce.size());
    auto gasPrice = store(uint256_t(30000000000));
    tx.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(80000));
    tx.set_gas_limit(gasLimit.data(), gasLimit.size());
    tx.set_private_key("");
    tx.set_private_key(TestKey1Eth.data(), TestKey1Eth.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeEthereum);
    EXPECT_EQ(hex(output.encoded()), "f90151038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003b3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033000000000026a0669563be8a0022fcd32fdf82ccca7dc66012ea28c57e95a2d9348dbf37afc377a03505f5eb041038c565d2f2888207c9dbcad8ca12f10ce5c5bd2ca41de01a9e89");
}

TEST(THORChainSwap, SwapBnbBtc) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    toAsset.set_symbol("BTC");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Bnb)
                   .toAddress(Address1Btc)
                   .vault(VaultBnb)
                   .fromAmount("10000000")
                   .toAmountLimit("10000000")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "2a3d3d3a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a313030303030303052480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "=:BTC.BTC:bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8:10000000");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    tx.set_chain_id("Binance-Chain-Tigris");
    tx.set_private_key(TestKey1Bnb.data(), TestKey1Bnb.size());

    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "fd01f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e19124086d43e9bdf12508a9a1415f5f970dfa5ff5930dee01d922f99779b63190735ba1d69694bda203b6678939a5c1eab0a52ed32bb67864ec7864de37b333533ae0c1a3d3d3a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030");
}

TEST(THORChainSwap, SwapBnbEth) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Bnb)
                   .toAddress(Address1Eth)
                   .vault(VaultBnb)
                   .fromAmount("27000000")
                   .toAmountLimit("123456")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "2a3b3d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a31323334353652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e4210c0f9ef0c12220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e4210c0f9ef0c");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:123456");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, PrivateKey(TestKey1Bnb)), Address1Bnb);
    tx.set_private_key(TestKey1Bnb.data(), TestKey1Bnb.size());
    tx.set_chain_id("Binance-Chain-Tigris");
    tx.set_account_number(1902570);
    tx.set_sequence(12);
    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8102f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e4210c0f9ef0c12220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e4210c0f9ef0c12700a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e1912409ad3d44f3cc8d5dd2701b0bf3758ef674683533fb63e3e94d39728688c0279f8410395d631075dac62dee74b972c320f5a58e88ab81be6f1bb6a9564468ae1b618ea8f74200c1a3b3d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a313233343536");

    // real transaction:
    // https://explorer.binance.org/tx/F0CFDB0D9467E83B5BBF6DF92E4E2D04FE9EFF9B0A1C71D88DCEF566233DCAA2
    // https://viewblock.io/thorchain/tx/F0CFDB0D9467E83B5BBF6DF92E4E2D04FE9EFF9B0A1C71D88DCEF566233DCAA2
    // https://etherscan.io/tx/0x8e5bb7d87e17af86e649e402bc5c182ea8c32ddaca153804679de1184e0d9747
}

TEST(THORChainSwap, SwapBnbRune) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::THOR));
    toAsset.set_symbol("RUNE");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Bnb)
                   .toAddress(Address1Thor)
                   .vault(VaultBnb)
                   .fromAmount("4000000")
                   .toAmountLimit("121065076")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "2a413d3a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a31323130363530373652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e42108092f40112220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e42108092f401");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "=:THOR.RUNE:thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r:121065076");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, PrivateKey(TestKey1Bnb)), Address1Bnb);
    tx.set_private_key(TestKey1Bnb.data(), TestKey1Bnb.size());
    tx.set_chain_id("Binance-Chain-Tigris");
    tx.set_account_number(1902570);
    tx.set_sequence(4);
    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8702f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e42108092f40112220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e42108092f40112700a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e191240f0bd5a0b4936ce73b1564f737a22cb7cfa3c171a3598b1fe42f6c926c516777042673f3b30148d54b591dcfcb88c2aa04bb87b4b492e8d17c72e4d263f57159018ea8f7420041a413d3a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a313231303635303736");

    // real transaction:
    // https://explorer.binance.org/tx/84EE429B35945F0568097527A084532A9DE7BBAB0E6A5562E511CEEFB188DE69
    // https://viewblock.io/thorchain/tx/D582E1473FE229F02F162055833C64F49FB4FF515989A4785ED7898560A448FC
}

TEST(THORChainSwap, SwapBusdTokenBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_symbol("BNB");
    fromAsset.set_token_id("BUSD-BD1");
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress("bnb1gddl87crh47wzynjx3c6pmcclzk7txlkm74x28")
                                         .toAddress("bnb1gddl87crh47wzynjx3c6pmcclzk7txlkm74x28")
                                         .vault("bnb17e9qd0ffrkxsy9pehx7q6hjer730pzq5z4tv82")
                                         .fromAmount("500000000")
                                         .toAmountLimit("719019")
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build(false);
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "2a42535741503a424e422e424e423a626e62316764646c38376372683437777a796e6a78336336706d63636c7a6b3774786c6b6d37347832383a3731393031393a743a3052540a280a14435bf3fb03bd7ce112723471a0ef18f8ade59bf612100a08425553442d4244311080cab5ee0112280a14f64a06bd291d8d021439b9bc0d5e591fa2f0881412100a08425553442d4244311080cab5ee01");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:BNB.BNB:bnb1gddl87crh47wzynjx3c6pmcclzk7txlkm74x28:719019:t:0");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "435bf3fb03bd7ce112723471a0ef18f8ade59bf6");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "f64a06bd291d8d021439b9bc0d5e591fa2f08814");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    const Data privateKey = parse_hex("412c379cccf9d792238f0a8bd923604e00c2be11ea1de715945f6a849796362a");
    EXPECT_EQ(Binance::Address(PrivateKey(privateKey).getPublicKey(TWPublicKeyTypeSECP256k1)).string(), "bnb1gddl87crh47wzynjx3c6pmcclzk7txlkm74x28");
    tx.set_private_key(privateKey.data(), privateKey.size());
    tx.set_chain_id("Binance-Chain-Tigris");
    tx.set_account_number(7320332);
    tx.set_sequence(2);

    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "9502f0625dee0a582a2c87fa0a280a14435bf3fb03bd7ce112723471a0ef18f8ade59bf612100a08425553442d4244311080cab5ee0112280a14f64a06bd291d8d021439b9bc0d5e591fa2f0881412100a08425553442d4244311080cab5ee0112710a26eb5ae98721039aa92707d6789692628099f288de219c9c9a0dd179df4e8b1b717191c75fbbfb1240fb41cf3eaaf1286de4be633682c120886b39dcc41690b583f4f08561d660a1677ebda2323e0f22c440c6fe8855d21f1153557b94066ce956363f0a82d1ab3c92188ce6be0320021a42535741503a424e422e424e423a626e62316764646c38376372683437777a796e6a78336336706d63636c7a6b3774786c6b6d37347832383a3731393031393a743a30");

    // https://viewblock.io/thorchain/tx/1B7E472C7C8D60176FCFD83CAD7DA970EB12B45145C553CD37BD34CABE276C59
    // https://explorer.bnbchain.org/tx/1B7E472C7C8D60176FCFD83CAD7DA970EB12B45145C553CD37BD34CABE276C59
    // https://explorer.bnbchain.org/tx/79D2194584F498CA2D4C391FBD7B158FC94B670703B629CA6F46852BB24234A6
}

TEST(THORChainSwap, SwapBnbBnbToken) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    toAsset.set_token_id("TWT-8C2");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress("bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx")
                   .toAddress("bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx")
                   .vault("bnb1qefsjm654cdw94ejj8g4s49w7z8te75veslusz")
                   .fromAmount("10000000")
                   .toAmountLimit("5400000000")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "2a433d3a424e422e5457542d3843323a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3534303030303030303052480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a140653096f54ae1ae2d73291d15854aef08ebcfa8c120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "=:BNB.TWT-8C2:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:5400000000");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "0653096f54ae1ae2d73291d15854aef08ebcfa8c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    const Data privateKey = parse_hex("bcf8b072560dda05122c99390def2c385ec400e1a93df0657a85cf6b57a715da");
    EXPECT_EQ(Binance::Address(PrivateKey(privateKey).getPublicKey(TWPublicKeyTypeSECP256k1)).string(), "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx");
    tx.set_private_key(privateKey.data(), privateKey.size());
    tx.set_chain_id("Binance-Chain-Tigris");
    tx.set_account_number(1902570);
    tx.set_sequence(18);

    // sign and encode resulting input
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8902f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a140653096f54ae1ae2d73291d15854aef08ebcfa8c120a0a03424e421080ade20412700a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e191240918963970aedc528e3a9ba34f37fb544ec18e7d2caade2ebf7b8371928c93e6e0eca072313ddfda393c1340766d5fef00e6b0cb7147ef3382b6303f3a6ca01a318ea8f7420121a433d3a424e422e5457542d3843323a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a35343030303030303030");

    // real transaction:
    // curl -X GET "http://dataseed1.binance.org/broadcast_tx_sync?tx=0x8c02...3030"
    // https://viewblock.io/thorchain/tx/6D1EDC9BD9BFAFEF0F88F95A164191262EA02A0413BF3D9773110AD5676E1523
    // https://explorer.binance.org/tx/6D1EDC9BD9BFAFEF0F88F95A164191262EA02A0413BF3D9773110AD5676E1523
    // https://explorer.binance.org/tx/60C54C9F253B89C36A2788AB66951045E8AC5F5729597CB6C64A13013A7A54CC
}

TEST(THORChainSwap, SwapBtcEthWithAffFee) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Btc)
                   .toAddress(Address1Eth)
                   .vault(VaultBtc)
                   .fromAmount("1000000")
                   .toAmountLimit("140000000000000000")
                   .affFeeAddress("thrnm")
                   .affFeeRate("10")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "080110c0843d1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a503d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a3134303030303030303030303030303030303a7468726e6d3a3130");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 1000000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:140000000000000000:thrnm:10");
    EXPECT_EQ(tx.coin_type(), 0ul);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    // set few fields before signing
    tx.set_byte_fee(20);
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), "bc").string(), Address1Btc);
    tx.add_private_key(TestKey1Btc.data(), TestKey1Btc.size());
    auto& utxo = *tx.add_utxo();
    Data utxoHash = parse_hex("1234000000000000000000000000000000000000000000000000000000005678");
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(Address1Btc, TWCoinTypeBitcoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(50000000);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBitcoin);
    EXPECT_EQ(output.error(), 0);
    EXPECT_EQ(hex(output.encoded()), // printed using prettyPrintTransaction
        "01000000" // version
        "0001" // marker & flag
        "01" // inputs
            "1234000000000000000000000000000000000000000000000000000000005678"  "00000000"  "00"  ""  "ffffffff"
        "03" // outputs
            "40420f0000000000"  "16"  "0014d6cbc5021c3eee72798718d447758b91d14e8c5f"
            "0c9ceb0200000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
            "0000000000000000"  "53"  "6a4c503d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a3134303030303030303030303030303030303a7468726e6d3a3130"
        // witness
            "02"
                "47"  "3044022056e918d8dea9431057b7b8b7f7c990ff72d653aef296eda9a85e546537e1eaa4022050b64766ea4ce56ecd3325f184d67b20924fd4539cb40bbad916ede1cc26017f01"
                "21"  "021e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee"
        "00000000" // nLockTime
    );
}

TEST(THORChainSwap, SwapEthBnbWithAffFee) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Eth)
                   .toAddress(Address1Bnb)
                   .vault(VaultEth)
                   .router(RouterEth)
                   .fromAmount("50000000000000000")
                   .toAmountLimit("600003")
                   .affFeeAddress("tthor1ql2tcqyrqsgnql2tcqyj2n8kfdmt9lh0yzql2tcqy")
                   .affFeeRate("10")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a01001201002201002a0100422a30783432413545643435363635306130394463313045426336333631413734383066446436316632374252b30232b0020a07b1a2bc2ec5000012a4021fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000006e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030333a7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c32746371793a3130000000000000000000000000000000000000");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), RouterEth);
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    Data vaultAddressBin = SwapTest_ethAddressStringToData(VaultEth);
    EXPECT_EQ(hex(vaultAddressBin), "1091c4de6a3cf09cda00abdaed42c7c3b69c83ec");
    auto func = Ethereum::ABI::Function("deposit", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0000000000000000000000000000000000000000")),
                                                       std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(50000000000000000)),
                                                       std::make_shared<Ethereum::ABI::ParamString>("=:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:600003:tthor1ql2tcqyrqsgnql2tcqyj2n8kfdmt9lh0yzql2tcqy:10")});
    Data payload;
    func.encode(payload);
    EXPECT_EQ(hex(payload), "1fece7b4"
                            "0000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec"
                            "0000000000000000000000000000000000000000000000000000000000000000"
                            "00000000000000000000000000000000000000000000000000b1a2bc2ec50000"
                            "0000000000000000000000000000000000000000000000000000000000000080"
                            "000000000000000000000000000000000000000000000000000000000000006e"
                            "3d3a424e422e424e423a626e62317573343777646866783038636839377a6475"
                            "656833783375356d757266727833306a656372783a3630303030333a7474686f"
                            "7231716c3274637179727173676e716c32746371796a326e386b66646d74396c"
                            "6830797a716c32746371793a3130000000000000000000000000000000000000");

    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().amount())), "b1a2bc2ec50000");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().data())), hex(payload));

    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    auto chainId = store(uint256_t(1));
    tx.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(3));
    tx.set_nonce(nonce.data(), nonce.size());
    auto gasPrice = store(uint256_t(30000000000));
    tx.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(80000));
    tx.set_gas_limit(gasLimit.data(), gasLimit.size());
    tx.set_private_key("");
    tx.set_private_key(TestKey1Eth.data(), TestKey1Eth.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeEthereum);
    EXPECT_EQ(hex(output.encoded()), "f90192038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b901241fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000006e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030333a7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c32746371793a313000000000000000000000000000000000000026a0ee68bd41da9a9b1ad87fd547e83e4b8022460de024839f4f5f528abc6aecf2aea0402205812d62a075138743f6048ba2a1c073f4a3a14224009a34ee74d3dccef1");
}

TEST(THORChainSwap, SwapBtcNegativeMemoTooLong) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto && [out, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Btc)
                   .toAddress(Address1Eth)
                   .vault(VaultBtc)
                   .fromAmount("1000000")
                   .toAmountLimit("140000000000000000")
                   .affFeeAddress("affiliate_address")
                   .affFeeRate("10")
                   .extraMemo("extra_memo_very_loooooooooooooong")
                   .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "080110c0843d1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a7e3d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a3134303030303030303030303030303030303a616666696c696174655f616464726573733a31303a65787472615f6d656d6f5f766572795f6c6f6f6f6f6f6f6f6f6f6f6f6f6f6f6e67");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 1000000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:140000000000000000:affiliate_address:10:extra_memo_very_loooooooooooooong");
    EXPECT_EQ(tx.output_op_return().length(), 126ul);
    EXPECT_EQ(tx.coin_type(), 0ul);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    // set few fields before signing
    tx.set_byte_fee(20);
    EXPECT_EQ(Bitcoin::SegwitAddress(PrivateKey(TestKey1Btc).getPublicKey(TWPublicKeyTypeSECP256k1), "bc").string(), Address1Btc);
    tx.add_private_key(TestKey1Btc.data(), TestKey1Btc.size());
    auto& utxo = *tx.add_utxo();
    Data utxoHash = parse_hex("1234000000000000000000000000000000000000000000000000000000005678");
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(Address1Btc, TWCoinTypeBitcoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(50000000);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBitcoin);
    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_memo);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(THORChainSwap, Memo) {
    Proto::Asset toAssetBTC;
    toAssetBTC.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    toAssetBTC.set_symbol("BTC");
    auto builder = SwapBuilder::builder().to(toAssetBTC).toAddress("btc123").toAmountLimit("1234");
    EXPECT_EQ(builder.buildMemo(), "=:BTC.BTC:btc123:1234");
    EXPECT_EQ(builder.affFeeAddress("feeaddr").buildMemo(), "=:BTC.BTC:btc123:1234:feeaddr");
    EXPECT_EQ(builder.affFeeRate("10").buildMemo(), "=:BTC.BTC:btc123:1234:feeaddr:10");
    EXPECT_EQ(builder.extraMemo("extramemo").buildMemo(), "=:BTC.BTC:btc123:1234:feeaddr:10:extramemo");
    EXPECT_EQ(builder.extraMemo("").affFeeRate("0").buildMemo(), "=:BTC.BTC:btc123:1234:feeaddr:0");
    EXPECT_EQ(builder.affFeeAddress("").affFeeRate("10").buildMemo(), "=:BTC.BTC:btc123:1234::10");
    EXPECT_EQ(builder.extraMemo("extramemo").affFeeRate("").buildMemo(), "=:BTC.BTC:btc123:1234:::extramemo");

    Proto::Asset toAssetETH;
    toAssetETH.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAssetETH.set_symbol("ETH");
    builder = SwapBuilder::builder().to(toAssetETH).toAddress("0xaabbccdd").toAmountLimit("1234");
    EXPECT_EQ(builder.buildMemo(), "=:ETH.ETH:0xaabbccdd:1234");
    toAssetETH.set_token_id("0x0000000000000000000000000000000000000000");
    EXPECT_EQ(builder.to(toAssetETH).buildMemo(), "=:ETH.ETH:0xaabbccdd:1234");
    toAssetETH.set_token_id("0x4B0F1812e5Df2A09796481Ff14017e6005508003");
    EXPECT_EQ(builder.to(toAssetETH).buildMemo(), "=:ETH.0x4B0F1812e5Df2A09796481Ff14017e6005508003:0xaabbccdd:1234");

    builder = SwapBuilder::builder().to(toAssetETH).toAddress("bnb123").toAmountLimit("1234");
    Proto::Asset toAssetBNB;
    toAssetBNB.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAssetBNB.set_symbol("BNB");
    EXPECT_EQ(builder.to(toAssetBNB).buildMemo(), "=:BNB.BNB:bnb123:1234");
    toAssetBNB.set_token_id("TWT-8C2");
    EXPECT_EQ(builder.to(toAssetBNB).buildMemo(), "=:BNB.TWT-8C2:bnb123:1234");
}

TEST(THORChainSwap, WrongFromAddress) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    {
        auto && [_, errorCode, error] = SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress("DummyAddress")
                       .toAddress(Address1Eth)
                       .vault(VaultEth)
                       .fromAmount("1000000")
                       .toAmountLimit("100000")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_from_address);
        EXPECT_EQ(error, "Invalid from address");
    }
    {
        auto && [_, errorCode, error] = SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress(Address1Btc)
                       .toAddress(Address1Eth)
                       .vault(VaultEth)
                       .fromAmount("1000000")
                       .toAmountLimit("100000")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_from_address);
        EXPECT_EQ(error, "Invalid from address");
    }
}

TEST(THORChainSwap, WrongToAddress) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    {
        auto && [_, errorCode, error] = SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress(Address1Bnb)
                       .toAddress("DummyAddress")
                       .vault(VaultEth)
                       .fromAmount("100000")
                       .toAmountLimit("100000")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_to_address);
        EXPECT_EQ(error, "Invalid to address");
    }
    {
        auto && [_, errorCode, error] = SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress(Address1Bnb)
                       .toAddress(Address1Btc)
                       .vault(VaultEth)
                       .fromAmount("100000")
                       .toAmountLimit("100000")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_to_address);
        EXPECT_EQ(error, "Invalid to address");
    }
}

TEST(THORChainSwap, FromRuneNotSupported) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::THOR));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto && [_, errorCode, error] = SwapBuilder::builder()
                   .from(fromAsset)
                   .to(toAsset)
                   .fromAddress(Address1Thor)
                   .toAddress(Address1Bnb)
                   .fromAmount("1000")
                   .toAmountLimit("1000")
                   .build();
    EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Unsupported_from_chain);
    EXPECT_EQ(error, "Unsupported from chain: 0");
}

TEST(THORChainSwap, EthInvalidVault) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    {
        auto && [_, errorCode, error] = SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress(Address1Eth)
                       .toAddress(Address1Bnb)
                       .vault("_INVALID_ADDRESS_")
                       .router(RouterEth)
                       .fromAmount("50000000000000000")
                       .toAmountLimit("600003")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_vault_address);
        EXPECT_EQ(error, "Invalid vault address: _INVALID_ADDRESS_");
    }
    {
        auto && [_, errorCode, error] =  SwapBuilder::builder()
                       .from(fromAsset)
                       .to(toAsset)
                       .fromAddress(Address1Eth)
                       .toAddress(Address1Bnb)
                       .vault(VaultEth)
                       .router("_INVALID_ADDRESS_")
                       .fromAmount("50000000000000000")
                       .toAmountLimit("600003")
                       .build();
        EXPECT_EQ(errorCode, Proto::ErrorCode::Error_Invalid_router_address);
        EXPECT_EQ(error, "Invalid router address: _INVALID_ADDRESS_");
    }
}

} // namespace TW::THORChainSwap
