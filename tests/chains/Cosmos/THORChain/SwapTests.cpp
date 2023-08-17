// Copyright © 2017-2023 Trust Wallet.
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
#include "proto/Cosmos.pb.h"
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

TEST(THORChainSwap, OverflowFixEth) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    fromAsset.set_symbol("ETH");
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    toAsset.set_symbol("BTC");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(Address1Eth)
                                         .toAddress(Address1Btc)
                                         .vault(VaultEth)
                                         .fromAmount("1234000000000000000000")
                                         .toAmountLimit("5285656144")
                                         .build();
    ASSERT_EQ(errorCode, 0);
}

TEST(THORChainSwap, SwapBtcEth) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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

TEST(THORChainSwap, SwapDogeBusd) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::DOGE));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    toAsset.set_token_id("BUSD-BD1");

    auto vaultDoge = "DExct9oTfqr7pfnbP2hkCHP1Z2eUDgqXya";
    auto fromAddressDoge = "DKftkYCtCyYxQy2TRAuAzQXoyKDdYsEBnw";
    auto toAddressBnb = "bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2";
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(fromAddressDoge)
                                         .toAddress(toAddressBnb)
                                         .vault(vaultDoge)
                                         .fromAmount("10000000000")
                                         .toAmountLimit("789627468")
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "08011080c8afa025180122224445786374396f546671723770666e625032686b434850315a3265554467715879612a22444b66746b5943744379597851793254524175417a51586f794b4464597345426e7750036a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3738393632373436383a743a30");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 10000000000);
    EXPECT_EQ(tx.to_address(), vaultDoge);
    EXPECT_EQ(tx.change_address(), fromAddressDoge);
    EXPECT_EQ(tx.output_op_return(), "=:BNB.BUSD-BD1:bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2:789627468:t:0");
    EXPECT_EQ(tx.coin_type(), TWCoinTypeDogecoin);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    //
    auto dogeKey = parse_hex("3785864c91ed408ebaeae473962a471eb4d68ce998c2957e8e5f6be7a525f2d7");
    tx.add_private_key(dogeKey.data(), dogeKey.size());
    tx.set_byte_fee(1000);
    auto& utxo = *tx.add_utxo();
    Data previousUTXOHash = parse_hex("9989c36afdd1755a679226875425b368816031186c0f1b4a363ab2ef6d0a2fe8");
    std::reverse(previousUTXOHash.begin(), previousUTXOHash.end());
    utxo.mutable_out_point()->set_hash(previousUTXOHash.data(), previousUTXOHash.size());
    utxo.mutable_out_point()->set_index(1);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX - 3);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(fromAddressDoge, TWCoinTypeDogecoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(16845776096);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeDogecoin);
    EXPECT_EQ(output.error(), 0);
    EXPECT_EQ(hex(output.encoded()), "0100000001e82f0a6defb23a364a1b0f6c1831608168b32554872692675a75d1fd6ac38999010000006b4830450221008660de3d3123a9e6831517265fb84c4fb2bfc4b98366dbfb4b63bc78a5812cce02201a0673af15edab604d9cd89f0e2842ccdd973e107ff9cd08dcf45d8c0b27c5dd0121039535d01e184b4a6d624e7ab007612e2558697fbed29274e6474f17e70d31ce5afcffffff0300e40b54020000001976a9146bb602e5e8eca75c7f6f25f766254658581db71688ac40490698010000001976a9149f64d0c07876a1dbce40cdce328bc7ecd8182b2288ac0000000000000000496a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3738393632373436383a743a3000000000");

    // similar real transaction:
    // https://viewblock.io/thorchain/tx/E7588A6A4C6B9DBA8B9AD8B0834655F9D9E5861744B5493E711623E320B981A5
    // https://dogechain.info/tx/e7588a6a4c6b9dba8b9ad8b0834655f9d9e5861744b5493e711623e320b981a5
    // https://binance.mintscan.io/txs/A5943D315BFD501DD5FC212F5A505772A20DDB154A8B5760A9897ABB8114CBDB
}

TEST(THORChainSwap, SwapLtcBusd) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::LTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    toAsset.set_token_id("BUSD-BD1");

    auto vaultLTC = "ltc1qmca5runvg3hygarulu34evdulcdfda7z7zquhn";
    auto fromAddressLTC = "ltc1qyu9qvkukx99r6yadxlk3t2x78a7dxe73s3r4x3";
    auto toAddressBnb = "bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2";
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(fromAddressLTC)
                                         .toAddress(toAddressBnb)
                                         .vault(vaultLTC)
                                         .fromAmount("15000000")
                                         .toAmountLimit("977240514")
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "080110c0c393071801222b6c746331716d63613572756e7667336879676172756c753334657664756c6364666461377a377a7175686e2a2b6c7463317179753971766b756b7839397236796164786c6b3374327837386137647865373373337234783350026a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3937373234303531343a743a30");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 15000000);
    EXPECT_EQ(tx.to_address(), vaultLTC);
    EXPECT_EQ(tx.change_address(), fromAddressLTC);
    EXPECT_EQ(tx.output_op_return(), "=:BNB.BUSD-BD1:bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2:977240514:t:0");
    EXPECT_EQ(tx.coin_type(), TWCoinTypeLitecoin);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    //
    auto ltcKey = parse_hex("6affb3d4e2c4f5a23b711e67ca94d0bd93550e203f5c8258df74cc62282d1494");
    tx.add_private_key(ltcKey.data(), ltcKey.size());
    tx.set_byte_fee(140);
    auto& utxo = *tx.add_utxo();
    Data previousUTXOHash = parse_hex("6e71e6da1898584ccf92c362db3d7c16326f9daae6687132c69abfdb043cc749");
    std::reverse(previousUTXOHash.begin(), previousUTXOHash.end());
    utxo.mutable_out_point()->set_hash(previousUTXOHash.data(), previousUTXOHash.size());
    utxo.mutable_out_point()->set_index(0);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX - 3);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(fromAddressLTC, TWCoinTypeLitecoin);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(34183600);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeLitecoin);
    EXPECT_EQ(output.error(), 0);
    EXPECT_EQ(hex(output.encoded()), "0100000000010149c73c04dbbf9ac6327168e6aa9d6f32167c3ddb62c392cf4c589818dae6716e0000000000fcffffff03c0e1e40000000000160014de3b41f26c446e44747cff235cb1bcfe1a96f7c2fc3d240100000000160014270a065b96314a3d13ad37ed15a8de3f7cd367d10000000000000000496a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3937373234303531343a743a3002483045022100fb9df5ef12c26648a50af298c5319ec52ea0287aa1405e07d817c606bb17a23502206520b087a9155a7d8c04b54b8ee3405fad9c3d22cf2c7cac06197ce555d56077012103acefb7d95b8c1da28f17400740d7e1124dbee3cfbe55646deb28198d570ea26b00000000");

    // https://viewblock.io/thorchain/tx/FBB450335ED839C5FE3DCB9CBC0999DA6E6E52B787D1B165D3FA47E6273CCF5F
    // https://blockchair.com/litecoin/transaction/fbb450335ed839c5fe3dcb9cbc0999da6e6e52b787d1b165d3fa47e6273ccf5f
    // https://binance.mintscan.io/txs/7071DF040641D9C62EAA5D7AE5CDAC0C408FE64406261EC32417BD919684707C
}

TEST(THORChainSwap, SwapBchBusd) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BCH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    toAsset.set_token_id("BUSD-BD1");

    auto vaultBCH = "qpsfh5xvk7mgf9e6kl4e045nm6awl5hmks9x7h5ad6";
    auto fromAddressBCH = "qr50u7hy3xcr3j0w9j5nfx2gevjqgfm42ykc2hqgy4";
    auto toAddressBnb = "bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2";
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(fromAddressBCH)
                                         .toAddress(toAddressBnb)
                                         .vault(vaultBCH)
                                         .fromAmount("10000000")
                                         .toAmountLimit("977240514")
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "08411080ade2041801222a71707366683578766b376d67663965366b6c34653034356e6d3661776c35686d6b7339783768356164362a2a717235307537687933786372336a3077396a356e6678326765766a7167666d3432796b633268716779345091016a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3937373234303531343a743a30");

    auto tx = Bitcoin::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.amount(), 10000000);
    EXPECT_EQ(tx.to_address(), vaultBCH);
    EXPECT_EQ(tx.change_address(), fromAddressBCH);
    EXPECT_EQ(tx.output_op_return(), "=:BNB.BUSD-BD1:bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2:977240514:t:0");
    EXPECT_EQ(tx.coin_type(), TWCoinTypeBitcoinCash);
    EXPECT_EQ(tx.private_key_size(), 0);
    EXPECT_FALSE(tx.has_plan());

    //
    auto bchKey = parse_hex("1a3b0105a08908734ed0525f4c6fadca068514cdeb732d7ebca5b0fcbe6952a7");
    tx.add_private_key(bchKey.data(), bchKey.size());
    tx.set_byte_fee(3);
    auto& utxo = *tx.add_utxo();
    Data previousUTXOHash = parse_hex("651e5d3a60f8110a6cfb745005168bdfcaf21e7f2f4371873a24b5cd894564da");
    std::reverse(previousUTXOHash.begin(), previousUTXOHash.end());
    utxo.mutable_out_point()->set_hash(previousUTXOHash.data(), previousUTXOHash.size());
    utxo.mutable_out_point()->set_index(1);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX - 3);
    auto utxoScript = Bitcoin::Script::lockScriptForAddress(fromAddressBCH, TWCoinTypeBitcoinCash);
    utxo.set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo.set_amount(14118938);
    tx.set_use_max_amount(false);

    // sign and encode resulting input
    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeBitcoinCash);
    EXPECT_EQ(output.error(), 0);
    EXPECT_EQ(hex(output.encoded()), "0100000001da644589cdb5243a8771432f7f1ef2cadf8b16055074fb6c0a11f8603a5d1e65010000006a4730440220392fab53b86e02bef19638077fd378dd713dd6b1968d07f4507e28feb022d52a02200240bb2f2e8b8eb7673c4bc69b485e28a0d56c735d84e3f794c303c1b71759e941210393dc5157b5879cd602f25529437e01b3d4892a4b9b8d9efcaa640d842b27438efcffffff0380969800000000001976a914609bd0ccb7b684973ab7eb97d693debaefd2fbb488ac8ed63e00000000001976a914e8fe7ae489b038c9ee2ca9349948cb240427755188ac0000000000000000496a473d3a424e422e425553442d4244313a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3937373234303531343a743a3000000000");

    // https://viewblock.io/thorchain/tx/B8AA6F2BFD09D7AC510BFCDA417903B2DDBEE0E9811821640D9C304B9B382B9B
    // https://blockchair.com/bitcoin-cash/transaction/b8aa6f2bfd09d7ac510bfcda417903b2ddbee0e9811821640d9c304b9b382b9b
    // https://binance.mintscan.io/txs/F4CD6554934E85D72269399607A7ADF8A92378C2287C164CC97CB57E8348B090
}

TEST(THORChainSwap, SwapBtcBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");

    auto&& [out, errorCode, error] = SwapBuilder::builder()
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

TEST(THORChainSwap, SwapAtomBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ATOM));
    fromAsset.set_symbol("ATOM");
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");

    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress("cosmos1v4e6vpehwrfez2dqepnw9g6t4fl83xzegd5ac9")
                                         .toAddress("bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2")
                                         .vault("cosmos154t5ycejlr7ax3ynmed9z05yg5a27y9u6pj5hq")
                                         .fromAmount("300000")
                                         .toAmountLimit("819391")
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "08011a0b636f736d6f736875622d342a3f3d3a424e422e424e423a626e623173346b616c6c786e67707973707a6d366e72657a6b6d6c3972677977366b78707734666872323a3831393339313a743a3042710a6f0a2d636f736d6f73317634653676706568777266657a32647165706e773967367434666c3833787a65676435616339122d636f736d6f7331353474357963656a6c7237617833796e6d6564397a303579673561323779397536706a3568711a0f0a057561746f6d1206333030303030");

    auto tx = Cosmos::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));
    ASSERT_EQ(tx.memo(), "=:BNB.BNB:bnb1s4kallxngpyspzm6nrezkml9rgyw6kxpw4fhr2:819391:t:0");

    auto& fee = *tx.mutable_fee();
    fee.set_gas(200000);
    auto& fee_amount = *fee.add_amounts();
    fee_amount.set_denom("uatom");
    fee_amount.set_amount("500");

    tx.set_account_number(1483163);
    tx.set_sequence(1);

    auto privKey = parse_hex("3eed3f32b8ba90e579ba46f37e7445fb4b34558306aa5bc32c525a93dff486e7");
    tx.set_private_key(privKey.data(), privKey.size());

    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeCosmos);
    EXPECT_EQ(output.error_message(), "");
    ASSERT_EQ(output.serialized(), "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CtMBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczF2NGU2dnBlaHdyZmV6MmRxZXBudzlnNnQ0Zmw4M3h6ZWdkNWFjORItY29zbW9zMTU0dDV5Y2VqbHI3YXgzeW5tZWQ5ejA1eWc1YTI3eTl1NnBqNWhxGg8KBXVhdG9tEgYzMDAwMDASPz06Qk5CLkJOQjpibmIxczRrYWxseG5ncHlzcHptNm5yZXprbWw5cmd5dzZreHB3NGZocjI6ODE5MzkxOnQ6MBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDmmNIYBvR9bnOloFEMOWdk9DHYIGe7naW0T19y+/k1SUSBAoCCAEYARISCgwKBXVhdG9tEgM1MDAQwJoMGkCFqUWtDu0pn1P/cnVQnIJIWF8HFJn/xkJh55Mc7ZLVPF60uXYUOg8nNkt0IQPuTFREw32/yff6lmA5w6KwPen/\"}");

    // https://viewblock.io/thorchain/tx/07F47D71A74245538E205F24ADB4BBB799B49C3A8A8875665D249EA51662AA50
    // https://www.mintscan.io/cosmos/txs/07F47D71A74245538E205F24ADB4BBB799B49C3A8A8875665D249EA51662AA50
    // https://binance.mintscan.io/txs/2C97061737B16B234990B9B18A2BF65F7C7418FF9E39A68E634C832E4E4C59CE
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

TEST(THORChainSwap, SwapErc20Rune) {
    Proto::Asset fromAsset;
    fromAsset.set_token_id("0xB97EF9Ef8734C71904D8002F8b6Bc66Dd9c48a6E");
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::AVAX));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::THOR));
    toAsset.set_symbol("RUNE");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress("0xbe6523017422A983B900b614Baeac51Ef7C1d0A3")
                                         .toAddress("thor1ad6hapypumu7su5ad9qry2d74yt9d56fssa774")
                                         .vault("0xa56f6Cb1D66cd80150b1ea79643b4C5900D6E36E")
                                         .router("0x8f66c4ae756bebc49ec8b81966dd8bba9f127549")
                                         .fromAmount("1000000")
                                         .toAmountLimit("51638857")
                                         .expirationPolicy(1775669796)
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a010012010018012201002a0100422a30783866363663346165373536626562633439656338623831393636646438626261396631323735343952ad0232aa020a010012a40244bc937b000000000000000000000000a56f6cb1d66cd80150b1ea79643b4c5900d6e36e000000000000000000000000b97ef9ef8734c71904d8002f8b6bc66dd9c48a6e00000000000000000000000000000000000000000000000000000000000f424000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000069d6922400000000000000000000000000000000000000000000000000000000000000443d3a54484f522e52554e453a74686f72316164366861707970756d753773753561643971727932643734797439643536667373613737343a35313633383835373a743a3000000000000000000000000000000000000000000000000000000000");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), "0x8f66c4ae756bebc49ec8b81966dd8bba9f127549");
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    Data vaultAddressBin = SwapTest_ethAddressStringToData("0xa56f6Cb1D66cd80150b1ea79643b4C5900D6E36E");
    EXPECT_EQ(hex(vaultAddressBin), "a56f6cb1d66cd80150b1ea79643b4c5900d6e36e");
    auto func = Ethereum::ABI::Function("depositWithExpiry", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
                                                       std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex("0xB97EF9Ef8734C71904D8002F8b6Bc66Dd9c48a6E")),
                                                       std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(1000000)),
                                                       std::make_shared<Ethereum::ABI::ParamString>("=:THOR.RUNE:thor1ad6hapypumu7su5ad9qry2d74yt9d56fssa774:51638857:t:0"),
                                                       std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(1775669796))});
    Data payload;
    func.encode(payload);
    EXPECT_EQ(hex(payload), "44bc937b000000000000000000000000a56f6cb1d66cd80150b1ea79643b4c5900d6e36e000000000000000000000000b97ef9ef8734c71904d8002f8b6bc66dd9c48a6e00000000000000000000000000000000000000000000000000000000000f424000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000069d6922400000000000000000000000000000000000000000000000000000000000000443d3a54484f522e52554e453a74686f72316164366861707970756d753773753561643971727932643734797439643536667373613737343a35313633383835373a743a3000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().amount())), "00");
    EXPECT_EQ(hex(TW::data(tx.transaction().contract_generic().data())), hex(payload));

    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    auto chainId = store(uint256_t(43114));
    tx.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(6));
    tx.set_nonce(nonce.data(), nonce.size());
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(25000000000));
    tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    auto gasLimit = store(uint256_t(108810));
    tx.set_gas_limit(gasLimit.data(), gasLimit.size());
    auto privKey = parse_hex("6649ba1d931059e7b419f97ee41c3f98b8f8054dfeb4cb57b9898bc5b9bbe318");
    tx.set_private_key(privKey.data(), privKey.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeAvalancheCChain);
    EXPECT_EQ(hex(output.encoded()), "02f9019482a86a0684773594008505d21dba008301a90a948f66c4ae756bebc49ec8b81966dd8bba9f12754980b9012444bc937b000000000000000000000000a56f6cb1d66cd80150b1ea79643b4c5900d6e36e000000000000000000000000b97ef9ef8734c71904d8002f8b6bc66dd9c48a6e00000000000000000000000000000000000000000000000000000000000f424000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000069d6922400000000000000000000000000000000000000000000000000000000000000443d3a54484f522e52554e453a74686f72316164366861707970756d753773753561643971727932643734797439643536667373613737343a35313633383835373a743a3000000000000000000000000000000000000000000000000000000000c080a04a3a01941906579f1c6888771fe0621d66ee78998bfbb87219c0b5970235fc5ca03aefe4bb0c074f90798e078270c380930f4ae75366217f85535dd9be196a4244");
    // https://viewblock.io/thorchain/tx/B5E88D61157E7073995CA8729B75DAB2C1684A7B145DB711327CA4B8FF7DBDE7
    // https://snowtrace.io/tx/0xb5e88d61157e7073995ca8729b75dab2c1684a7b145db711327ca4b8ff7dbde7
    // https://thorchain.net/tx/B5E88D61157E7073995CA8729B75DAB2C1684A7B145DB711327CA4B8FF7DBDE7
}

TEST(THORChainSwap, SwapAvaxBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::AVAX));
    fromAsset.set_token_id("0x0000000000000000000000000000000000000000");
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress("0xbB7cF2f05a01DB5394234FE1257D907059edFa66")
                                         .toAddress("bnb16gk7gczst59wy8rnxrqnt3yn6f60uw6ec0w6uv")
                                         .vault("0x3bd92906c60e5843ce01b2dc54e6dc3575b5215a")
                                         .router("0x8f66c4ae756bebc49ec8b81966dd8bba9f127549")
                                         .fromAmount("150000000000000000")
                                         .toAmountLimit("297039")
                                         .expirationPolicy(1775669796)
                                         .affFeeAddress("t")
                                         .affFeeRate("0")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a010012010018012201002a0100422a3078386636366334616537353662656263343965633862383139363664643862626139663132373534395294023291020a080214e8348c4f000012840244bc937b0000000000000000000000003bd92906c60e5843ce01b2dc54e6dc3575b5215a00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000214e8348c4f000000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000069d69224000000000000000000000000000000000000000000000000000000000000003f3d3a424e422e424e423a626e623136676b3767637a73743539777938726e7872716e7433796e36663630757736656330773675763a3239373033393a743a3000");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), "0x8f66c4ae756bebc49ec8b81966dd8bba9f127549");
    ASSERT_TRUE(tx.transaction().has_contract_generic());

    EXPECT_EQ(hex(TW::data(tx.private_key())), "");

    // set few fields before signing
    auto chainId = store(uint256_t(43114));
    tx.set_chain_id(chainId.data(), chainId.size());
    auto nonce = store(uint256_t(5));
    tx.set_nonce(nonce.data(), nonce.size());
    auto maxInclusionFeePerGas = store(uint256_t(2000000000));
    auto maxFeePerGas = store(uint256_t(25000000000));
    tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
    tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
    auto gasLimit = store(uint256_t(108810));
    tx.set_gas_limit(gasLimit.data(), gasLimit.size());
    auto privKey = parse_hex("09da019c250b7e2b140645df36fd839806c5ae8eecf4d8f35e8ff57cf3bd1e57");
    tx.set_private_key(privKey.data(), privKey.size());

    // sign and encode resulting input
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(tx, TWCoinTypeAvalancheCChain);
    EXPECT_EQ(hex(output.encoded()), "02f9017c82a86a0584773594008505d21dba008301a90a948f66c4ae756bebc49ec8b81966dd8bba9f127549880214e8348c4f0000b9010444bc937b0000000000000000000000003bd92906c60e5843ce01b2dc54e6dc3575b5215a00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000214e8348c4f000000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000069d69224000000000000000000000000000000000000000000000000000000000000003f3d3a424e422e424e423a626e623136676b3767637a73743539777938726e7872716e7433796e36663630757736656330773675763a3239373033393a743a3000c080a0a794b7cd86242df0f69bfc2555adec7841ad1f3a02e478be0d63571da8d41f20a06cd214b052d2a2aee598c2d3d57a972979e4c49a447c52828657101e9ad39737");
    // https://viewblock.io/thorchain/tx/8A29B132443BF1B0A0BD3E00F8155D10FEEEC7737BDC912C4A1AFB0A52E4FD4F
    // https://snowtrace.io/tx/0x8A29B132443BF1B0A0BD3E00F8155D10FEEEC7737BDC912C4A1AFB0A52E4FD4F
    // https://binance.mintscan.io/txs/9D250C8BAC8205B942A597AFB345045439A55CAB8DD588B75870D4E47D751C16
}

TEST(THORChainSwap, SwapEthBnb) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(Address1Eth)
                                         .toAddress(Address1Bnb)
                                         .vault(VaultEth)
                                         .fromAmount("50000000000000000")
                                         .toAmountLimit("600003")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a010012010018012201002a0100422a30783130393163344465366133634630394364413030416244416544343263376333423639433833454352480a460a07b1a2bc2ec50000123b3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), VaultEth);
    ASSERT_FALSE(tx.transaction().has_contract_generic());

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
    EXPECT_EQ(hex(output.encoded()), "02f8a60103808083013880941091c4de6a3cf09cda00abdaed42c7c3b69c83ec87b1a2bc2ec50000b83b3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a363030303033c080a00d605807f983650fafbfdcf0c33bdf0c524c7185eae8c1501ae24892faf16b1ba03b51b0a35e4754ab21d1e48fed635d8486048df50c253ba9af4cebdb6a92a450");
}

TEST(THORChainSwap, SwapBnbBtc) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    toAsset.set_symbol("BTC");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
    auto&& [out, errorCode, error] = SwapBuilder::builder()
                                         .from(fromAsset)
                                         .to(toAsset)
                                         .fromAddress(Address1Eth)
                                         .toAddress(Address1Bnb)
                                         .vault(VaultEth)
                                         .fromAmount("50000000000000000")
                                         .toAmountLimit("600003")
                                         .affFeeAddress("tthor1ql2tcqyrqsgnql2tcqyj2n8kfdmt9lh0yzql2tcqy")
                                         .affFeeRate("10")
                                         .build();
    ASSERT_EQ(errorCode, 0);
    ASSERT_EQ(error, "");
    EXPECT_EQ(hex(out), "0a010012010018012201002a0100422a307831303931633444653661336346303943644130304162444165443432633763334236394338334543527b0a790a07b1a2bc2ec50000126e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030333a7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c32746371793a3130");

    auto tx = Ethereum::Proto::SigningInput();
    ASSERT_TRUE(tx.ParseFromArray(out.data(), (int)out.size()));

    // check fields
    EXPECT_EQ(tx.to_address(), VaultEth);
    ASSERT_FALSE(tx.transaction().has_contract_generic());

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
    EXPECT_EQ(hex(output.encoded()), "02f8d90103808083013880941091c4de6a3cf09cda00abdaed42c7c3b69c83ec87b1a2bc2ec50000b86e3d3a424e422e424e423a626e62317573343777646866783038636839377a6475656833783375356d757266727833306a656372783a3630303030333a7474686f7231716c3274637179727173676e716c32746371796a326e386b66646d74396c6830797a716c32746371793a3130c001a05c16871b66fd0fa8f658d6f171310bab332d09e0533d6c97329a59ddc93a9a11a05ed2be94e6dbb640e58920c8be4fa597cd5f0a918123245acb899042dd43777f");
}

TEST(THORChainSwap, SwapBtcNegativeMemoTooLong) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::BTC));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    toAsset.set_symbol("ETH");
    auto&& [out, errorCode, error] = SwapBuilder::builder()
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
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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

TEST(THORChainSwap, EthInvalidVault) {
    Proto::Asset fromAsset;
    fromAsset.set_chain(static_cast<Proto::Chain>(Chain::ETH));
    Proto::Asset toAsset;
    toAsset.set_chain(static_cast<Proto::Chain>(Chain::BNB));
    toAsset.set_symbol("BNB");
    {
        fromAsset.set_token_id("0x53595320f158d4546677b4795cc66dff59d154db");
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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
        auto&& [_, errorCode, error] = SwapBuilder::builder()
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
