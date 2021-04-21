// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "THORSwap/Swap.h"

#include "Ethereum/Address.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "proto/Ethereum.pb.h"
#include "proto/Binance.pb.h"

#include "HexCoding.h"
#include "Coin.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

namespace TW::THORSwap {

// Addresses for wallet 'isolate dismiss fury ... note'
const auto Address1Btc = "bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8";
const auto Address1Eth = "0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7";
const auto Address1Bnb = "bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx";
const auto Address1Thor = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
const Data PrivateKey1Btc = parse_hex("13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c");
const Data PrivateKey1Eth = parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904");
const Data PrivateKey1Bnb = parse_hex("bcf8b072560dda05122c99390def2c385ec400e1a93df0657a85cf6b57a715da");
const auto VaultEth = "0x1091c4De6a3cF09CdA00AbDAeD42c7c3B69C83EC";
const auto VaultBnb = "bnb1n9esxuw8ca7ts8l6w66kdh800s09msvul6vlse";


TEST(THORSwap, SwapBtcEth) {
    auto res = Swap::build(Chain::BTC, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultEth, "10000000", "10000000");
    EXPECT_EQ(res.second, "Invalid from chain: 2");
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

TEST(THORSwap, SwapEthBnb) {
    auto res = Swap::build(Chain::ETH, Chain::BNB, Address1Eth, "BNB", "", Address1Bnb, VaultEth, "10000000", "10000000");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a010112010b1a0502540be40022030f42402a2a3078313039316334446536613363463039436441303041624441654434326337633342363943383345433aef0132ec010a0398968012e4011fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000098968000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000040535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3130303030303030");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), VaultEth);
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    Data vaultAddressBin = SwapTest_ethAddressStringToData(VaultEth);
    EXPECT_EQ(hex(vaultAddressBin), "1091c4de6a3cf09cda00abdaed42c7c3b69c83ec");
    auto func = Ethereum::ABI::Function("deposit", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
        std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
        std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0000000000000000000000000000000000000000")),
        std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(10000000)),
        std::make_shared<Ethereum::ABI::ParamString>("SWAP:BNB.BNB:bnb1us47wdhfx08ch97zdueh3x3u5murfrx30jecrx:10000000")
    });
    Data payload;
    func.encode(payload);
    EXPECT_EQ(hex(payload), "1fece7b4"
        "0000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000989680"
        "0000000000000000000000000000000000000000000000000000000000000080"
        "0000000000000000000000000000000000000000000000000000000000000040"
        "535741503a424e422e424e423a626e6231757334377764686678303863683937"
        "7a6475656833783375356d757266727833306a656372783a3130303030303030");

    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().amount())), "989680");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().data())), hex(payload));

    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    auto chainId = store(uint256_t(1));
    tx.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(11));
    tx.set_nonce(nonce.data(), nonce.size());
    auto gasPrice = store(uint256_t(10000000000));
    tx.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(1000000));
    tx.set_gas_limit(gasLimit.data(), gasLimit.size());
    tx.set_private_key("");
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeEthereum);
    EXPECT_EQ(hex(output.encoded()), "f9014d0b8502540be400830f4240941091c4de6a3cf09cda00abdaed42c7c3b69c83ec83989680b8e41fece7b40000000000000000000000001091c4de6a3cf09cda00abdaed42c7c3b69c83ec0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000098968000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000040535741503a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a313030303030303026a02cf193eaa4acf1a11088b1ff1efa620f06d2ed71c1c9f62d3d019e3cadad3607a01eab200380f20ab7b07c0d18fc8a541ca44d716c16cf2a2b0a15a150d99dc199");
}

TEST(THORSwap, SwapBnbBtc) {
    auto res = Swap::build(Chain::BNB, Chain::BTC, Address1Bnb, "BTC", "", Address1Btc, VaultBnb, "10000000", "10000000");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a313030303030303052480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:BTC.BTC:bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8:10000000");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // sign and encode resulting input
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
    Binance::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBinance);
    EXPECT_EQ(hex(output.encoded()), "8002f0625dee0a4c2a2c87fa0a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e421080ade20412220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e421080ade204126a0a26eb5ae9872103ea4b4bc12dc6f36a28d2c9775e01eef44def32cc70fb54f0e4177b659dbc0e1912404836ee8659caa86771281d3f104424d95977bdedf644ec8585f1674796fde525669a6d446f72da89ee90fb0e064473b0a2159a79630e081592c52948d03d67071a40535741503a4254432e4254433a62633171706a756c7433346b3973706a66796d38687373326a72776a676630786a6634307a65307070383a3130303030303030");
}

TEST(THORSwap, SwapBnbEth) {
    auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", Address1Eth, VaultBnb, "27000000", "123456");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a3b3d3a4554482e4554483a3078623966353737316332373636346266323238326439386530396437663530636563376362303161373a31323334353652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e4210c0f9ef0c12220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e4210c0f9ef0c");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "=:ETH.ETH:0xb9f5771c27664bf2282d98e09d7f50cec7cb01a7:123456");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, PrivateKey(PrivateKey1Bnb)), Address1Bnb);
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
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

TEST(THORSwap, SwapBnbRune) {
    auto res = Swap::build(Chain::BNB, Chain::THOR, Address1Bnb, "RUNE", "", Address1Thor, VaultBnb, "4000000", "121065076");
    ASSERT_EQ(res.second, "");
    EXPECT_EQ(hex(res.first), "0a1242696e616e63652d436861696e2d4e696c652a44535741503a54484f522e52554e453a74686f72317a3533777765376d64366365777a39737177717a6e306161767061756e3067773065786e32723a31323130363530373652480a220a14e42be736e933cf8b97c26f33789a3ca6f8348cd1120a0a03424e42108092f40112220a1499730371c7c77cb81ffa76b566dcef7c1e5dc19c120a0a03424e42108092f401");

    auto tx = Binance::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(res.first.data(), (int)res.first.size()));

    // check fields
    EXPECT_EQ(tx.memo(), "SWAP:THOR.RUNE:thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r:121065076");
    ASSERT_TRUE(tx.has_send_order());
    ASSERT_EQ(tx.send_order().inputs_size(), 1);
    ASSERT_EQ(tx.send_order().outputs_size(), 1);
    EXPECT_EQ(hex(tx.send_order().inputs(0).address()), "e42be736e933cf8b97c26f33789a3ca6f8348cd1");
    EXPECT_EQ(hex(tx.send_order().outputs(0).address()), "99730371c7c77cb81ffa76b566dcef7c1e5dc19c");
    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set private key and few other fields
    EXPECT_EQ(TW::deriveAddress(TWCoinTypeBinance, PrivateKey(PrivateKey1Bnb)), Address1Bnb);
    tx.set_private_key(PrivateKey1Bnb.data(), PrivateKey1Bnb.size());
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

TEST(THORSwap, Memo) {
    EXPECT_EQ(Swap::buildMemo(Chain::BTC, "BNB", "bnb123", 1234), "SWAP:BTC.BNB:bnb123:1234");
}

TEST(THORSwap, WrongFromAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, "DummyAddress", "ETH", "", Address1Eth, VaultEth, "100000", "100000");
        EXPECT_EQ(res.second, "Invalid from address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Btc, "ETH", "", Address1Eth, VaultEth, "100000", "100000");
        EXPECT_EQ(res.second, "Invalid from address");
    }
}

TEST(THORSwap, WrongToAddress) {
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", "DummyAddress", VaultEth, "100000", "100000");
        EXPECT_EQ(res.second, "Invalid to address");
    }
    {
        auto res = Swap::build(Chain::BNB, Chain::ETH, Address1Bnb, "ETH", "", Address1Btc, VaultEth, "100000", "100000");
        EXPECT_EQ(res.second, "Invalid to address");
    }
}

} // namespace
