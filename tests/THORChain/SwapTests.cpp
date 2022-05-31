// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "THORChain/Swap.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "Ethereum/Address.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "Binance/Address.h"
#include "proto/THORChainSwap.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/Binance.pb.h"

#include "HexCoding.h"
#include "Coin.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include "uint256.h"
#include "../interface/TWTestUtilities.h"

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
    auto res = Swap::build(Chain::BTC, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultBtc, "", "1000000", "140000000000000000");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "080110c0843d1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a473d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a313430303030303030303030303030303030");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.amount(), 1000000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:140000000000000000");
    EXPECT_EQ(tx.coin_type(), 0);
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
            "609deb0200000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
            "0000000000000000"  "42"  "6a403d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a3134303030303030303030"
        // witness
            "02"
                "47"  "304402205de19c68b5ea683b9d701d45b09f96658088db76e59ad27bd7b8383ee5d484ec0220245459a4d6d679d8b457564fccc7ecc5831c7ebed49e0366c65ac031e8a5b49201"
                "21"  "021e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee"
        "00000000" // nLockTime
    );
}

TEST(THORChainSwap, SwapBtcBnb) {
    auto res = Swap::build(Chain::BTC, Chain::BNB, Address1Btc, "BNB", "", Address1Bnb, VaultBtc, "", "200000", "140000000");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "080110c09a0c1801222a62633171366d397532717375386d68387937763872723279776176746a38673561727a6c796863656a372a2a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070386a41535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a313430303030303030");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.amount(), 200000);
    EXPECT_EQ(tx.to_address(), VaultBtc);
    EXPECT_EQ(tx.change_address(), Address1Btc);
    EXPECT_EQ(tx.output_op_return(), "SWAP:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:140000000");
    EXPECT_EQ(tx.coin_type(), 0);
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
            "108d030000000000"  "16"  "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"
            "0000000000000000"  "42"  "6a40535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3134303030303030"
        // witness
            "02"
                "48"  "30450221008427ac07af830abbf9f2e1b182096d9faefc9e5b4324786ec68386579b05d02102204fd062817a59255d62aba24b1b0c66bc070d0ddbb70bf130a6159cc057e7f6c801210"
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
    auto res = Swap::build(Chain::ETH, Chain::BNB, Address1Eth, "BNB", "", Address1Bnb, VaultEth, RouterEth, "50000000000000000", "600003");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "0a01001201002201002a0100422a30783432413545643435363635306130394463313045426336333631413734383066446436316632374252f30132f0010a07b1a2bc2ec5000012e4011fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003e535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030330000");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.to_address(), RouterEth);
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    Data vaultAddressBin = SwapTest_ethAddressStringToData(VaultEth);
    EXPECT_EQ(hex(vaultAddressBin), "1091c4de6a3cf09cda00abdaed42c7c3b69c83ec");
    auto func = Ethereum::ABI::Function("deposit", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
        std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
        std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0000000000000000000000000000000000000000")),
        std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(50000000000000000)),
        std::make_shared<Ethereum::ABI::ParamString>("SWAP:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:600003")
    });
    Data payload;
    func.encode(payload);
    EXPECT_EQ(hex(payload), "1fece7b4"
        "0000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000b1a2bc2ec50000"
        "0000000000000000000000000000000000000000000000000000000000000080"
        "000000000000000000000000000000000000000000000000000000000000003e"
        "535741503a424e422e424e423a626e6231757334377764686678303863683937"
        "7a6475656833783375356d757266727833306a656372783a3630303030330000");
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
    EXPECT_EQ(hex(output.encoded()), "f90151038506fc23ac00830138809442a5ed456650a09dc10ebc6361a7480fdd61f27b87b1a2bc2ec50000b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000003e535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033000025a06ae104be3201baca38315352f81fac70ca4dd47339981914e64e91149813e780a066a3f0b2c44ddf5a96a38481274f623f552a593d723237d6742185f4885c0064");
}

TEST(THORChainSwap, SwapBnbBtc) {
    auto res = Swap::build(Chain::BNB, Chain::BTC, Address1Bnb, "BTC", "", Address1Btc, VaultBnb, "", "10000000", "10000000");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "2a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a313030303030303052480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:BTC.BTC:bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8:10000000");
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
    EXPECT_EQ(hex(output.encoded()), "8002f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e191240af2117ebd42e31a9562738e9f8933b3b54b59e6305b5675956525e4edb6a6ac65abea614e90959ae388664e2b36bf720024879b6047e174e3cff95f8f364a4e71a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030");
}

TEST(THORChainSwap, SwapBnbEth) {
    auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", Address1Eth, VaultBnb, "", "27000000", "123456");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "2a3b3d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a31323334353652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e4210c0f9ef0c12220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e4210c0f9ef0c");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

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
    auto res = Swap::build(Chain::BNB, Chain::THOR, Address1Bnb, "RUNE", "", Address1Thor, VaultBnb, "", "4000000", "121065076");
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "2a44535741503a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a31323130363530373652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e42108092f40112220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e42108092f401");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:THOR.RUNE:thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r:121065076");
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
    EXPECT_EQ(hex(output.encoded()), "8a02f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e42108092f40112220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e42108092f40112700a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e191240d91b6655ea4ade62a90cc9b28e43ccd2887dcf1c563e42bbd0d6ae4e825c2c6a1ba7784866810f36b6e098b0c877d1daa48016d0558f7b796b3f0b410107ba2f18ea8f7420041a44535741503a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a313231303635303736");

    // real transaction:
    // https://explorer.binance.org/tx/84EE429B35945F0568097527A084532A9DE7BBAB0E6A5562E511CEEFB188DE69
    // https://viewblock.io/thorchain/tx/D582E1473FE229F02F162055833C64F49FB4FF515989A4785ED7898560A448FC
}

TEST(THORChainSwap, SwapBnbBnbToken) {
    auto res = Swap::build(
        Chain::BNB,
        Chain::BNB,
        "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx",
        "BNB",
        "TWT-8C2",
        "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx",
        "bnb1qefsjm654cdw94ejj8g4s49w7z8te75veslusz",
        "",
        "10000000", // 0.1 bnb
        "5400000000" // 54.0 twt
    ); 
    ASSERT_EQ(std::get<1>(res), 0);
    ASSERT_EQ(std::get<2>(res), "");
    EXPECT_EQ(hex(std::get<0>(res)), "2a46535741503a424e422e5457542d3843323a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3534303030303030303052480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a140653096f54ae1ae2d73291d15854aef08ebcfa8c120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(std::get<0>(res).data(), (int)std::get<0>(res).size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:BNB.TWT-8C2:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:5400000000");
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
    EXPECT_EQ(hex(output.encoded()), "8c02f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a140653096f54ae1ae2d73291d15854aef08ebcfa8c120a0a03424e421080ade20412700a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e1912405fd64a0ed5777f5ea4556624bd096f8b20b6d2b510655e4c928db1ec967e6c7025453882ce7e10138ac92f5d6a949acc5382a5539f81347856c67c4bb678d3c418ea8f7420121a46535741503a424e422e5457542d3843323a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a35343030303030303030");

    // real transaction:
    // curl -X GET "http://dataseed1.binance.org/broadcast_tx_sync?tx=0x8c02...3030"
    // https://viewblock.io/thorchain/tx/6D1EDC9BD9BFAFEF0F88F95A164191262EA02A0413BF3D9773110AD5676E1523
    // https://explorer.binance.org/tx/6D1EDC9BD9BFAFEF0F88F95A164191262EA02A0413BF3D9773110AD5676E1523
    // https://explorer.binance.org/tx/60C54C9F253B89C36A2788AB66951045E8AC5F5729597CB6C64A13013A7A54CC
}

TEST(THORChainSwap, Memo) {
    EXPECT_EQ(Swap::buildMemo(Chain::BTC, "BTC", "", "btc123", 1234), "SWAP:BTC.BTC:btc123:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::BNB, "BNB", "", "bnb123", 1234), "SWAP:BNB.BNB:bnb123:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::ETH, "ETH", "", "0xaabbccdd", 1234), "=:ETH.ETH:0xaabbccdd:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::ETH, "ETH", "", "0xaabbccdd", 1234), "=:ETH.ETH:0xaabbccdd:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::ETH, "ETH", "0x0000000000000000000000000000000000000000", "0xaabbccdd", 1234), "=:ETH.ETH:0xaabbccdd:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::ETH, "ETH", "0x4B0F1812e5Df2A09796481Ff14017e6005508003", "0xaabbccdd", 1234), "=:ETH.0x4B0F1812e5Df2A09796481Ff14017e6005508003:0xaabbccdd:1234");
    EXPECT_EQ(Swap::buildMemo(Chain::BNB, "BNB", "TWT-8C2", "bnb123", 1234), "SWAP:BNB.TWT-8C2:bnb123:1234");
}

TEST(THORChainSwap, WrongFromAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, "DummyAddress", "ETH", "", Address1Eth, VaultEth, "", "100000", "100000");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_from_address);
        EXPECT_EQ(std::get<2>(res), "Invalid from address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultEth, "", "100000", "100000");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_from_address);
        EXPECT_EQ(std::get<2>(res), "Invalid from address");
    }
}

TEST(THORChainSwap, WrongToAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", "DummyAddress", VaultEth, "", "100000", "100000");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_to_address);
        EXPECT_EQ(std::get<2>(res), "Invalid to address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", Address1Btc, VaultEth, "", "100000", "100000");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_to_address);
        EXPECT_EQ(std::get<2>(res), "Invalid to address");
    }
}

TEST(THORChainSwap, FromRuneNotSupported) {
    auto res = Swap::build(Chain::THOR, Chain::BNB, Address1Thor, "BNB", "", Address1Bnb, "", "", "1000", "1000");
    EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Unsupported_from_chain);
    EXPECT_EQ(std::get<2>(res), "Unsupported from chain: 3");
}

TEST(THORChainSwap, EthInvalidVault) {
    {
        auto res = Swap::build(Chain::ETH, Chain::BNB, Address1Eth, "BNB", "", Address1Bnb, "_INVALID_ADDRESS_", RouterEth, "50000000000000000", "600003");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_vault_address);
        EXPECT_EQ(std::get<2>(res), "Invalid vault address: _INVALID_ADDRESS_");
    }
    {
        auto res = Swap::build(Chain::ETH, Chain::BNB, Address1Eth, "BNB", "", Address1Bnb, VaultEth, "_INVALID_ADDRESS_", "50000000000000000", "600003");
        EXPECT_EQ(std::get<1>(res), Proto::ErrorCode::Error_Invalid_router_address);
        EXPECT_EQ(std::get<2>(res), "Invalid router address: _INVALID_ADDRESS_");
    }
}

} // namespace
