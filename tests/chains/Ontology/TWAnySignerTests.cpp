// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TestUtilities.h"

#include "Ontology/Oep4TxBuilder.h"
#include "Ontology/OngTxBuilder.h"
#include "Ontology/OntTxBuilder.h"

#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Ontology::tests {

TEST(TWAnySingerOntology, OntBalanceOf) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","00d1885602ec0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    // {"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"00","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("balanceOf");
    input.set_query_address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    input.set_nonce(3959576200);
    auto data = OntTxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1885602ec000000000000000000000000000000000000000000000000000000000000000000000000"
              "4d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f6614000000000000000000"
              "00000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(TWAnySingerOntology, OntDecimals) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1bdc12a48000000000000000000000000000000000000000000000000000000000000000000000000380008646563696d616c731400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    //{"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("decimals");
    input.set_nonce(1210761661);
    auto data = OntTxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1bdc12a48000000000000000000000000000000000000000000000000000000000000000000000000"
              "380008646563696d616c731400000000000000000000000000000000000000010068164f6e746f6c6f67"
              "792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(TWAnySingerOntology, OntTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/4a672ce813d3fac9042e9472cf9b470f8a5e59a2deb41fd7b23a1f7479a155d5/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Proto::SigningInput();
    input.set_contract("ONT");
    input.set_method("transfer");
    input.set_nonce(2338116610);
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeOntology);

    EXPECT_EQ("00d102d45c8bf401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
              "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
              "00000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140301766d925382a6e"
              "bb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb7cff28ddaf7f1048822c0ca21a0c4926323a"
              "2497875b963f3b8cbd3717aa6e7c2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
              "47125f927b7486ac414038466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860"
              "305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a6232103d9fd62df332403"
              "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              hex(output.encoded()));
}

TEST(TWAnySingerOntology, OngDecimals) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1e3f2e679000000000000000000000000000000000000000000000000000000000000000000000000380008646563696d616c731400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    // {"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"09","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("decimals");
    input.set_nonce(2045178595);
    auto data = OngTxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1e3f2e679000000000000000000000000000000000000000000000000000000000000000000000000"
              "380008646563696d616c731400000000000000000000000000000000000000020068164f6e746f6c6f67"
              "792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(TWAnySingerOntology, OngBalanceOf) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1ab1ad0cf0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    //{"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"27e74d240609","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("balanceOf");
    input.set_query_address("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD");
    input.set_nonce(3486522027);
    auto data = OngTxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1ab1ad0cf000000000000000000000000000000000000000000000000000000000000000000000000"
              "4d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f6614000000000000000000"
              "00000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000",
              rawTx);
}

TEST(TWAnySingerOntology, OngTransfer) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/8a1e59396dcb72d9095088f50d1023294bf9c7b79ba693bd641578f748cbd4e6/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("transfer");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    input.set_nonce(2827104669);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeOntology);

    EXPECT_EQ("00d19d3182a8f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "7100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94aba"
              "c41aaf3ead76586a7cc8516a7cc86c51c1087472616e7366657214000000000000000000000000000000"
              "00000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140e27e935b87855efa"
              "d62bb76b21c7b591f445f867eff86f888ca6ee1870ecd80f73b8ab199a4d757b4c7b9ed46c4ff8cfa8ae"
              "faa90b7fb6485e358034448cba752321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e4"
              "47125f927b7486ac4140450047b2efb384129a16ec4c707790e9379b978cc7085170071d8d7c5c037d74"
              "3b078bd4e21bb4404c0182a32ee05260e22454dffb34dacccf458dfbee6d32db232103d9fd62df332403"
              "d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              hex(output.encoded()));
}

TEST(TWAnySingerOntology, OngWithdraw) {
    // tx on polaris test net.
    // https://explorer.ont.io/transaction/433cb7ed4dec32d55be0db104aaa7ade4c7dbe0f62ef94f7b17829f7ac7cd75b/testnet
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Proto::SigningInput();
    input.set_contract("ONG");
    input.set_method("withdraw");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5");
    input.set_amount(1);
    input.set_gas_price(500);
    input.set_gas_limit(20000);
    input.set_nonce(3784713724);
    auto data = OngTxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ(
        "00d1fc2596e1f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df68b00c6"
        "6b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc81400000000000000000000000000000000000000"
        "016a7cc814fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc8516a7cc86c0c7472616e7366657246726f"
        "6d1400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f"
        "6b65000241400ef868766eeafce71b6ff2a4332aa4363980e66c55ef70aea80e3baee1daf02b43ae6d4c7c8a17"
        "8b92f523602426eaa4205ab0ae5944b0fdae0abcbabaefbc4c2321031bec1250aa8f78275f99a6663688f31085"
        "848d0ed92f1203e447125f927b7486ac4140c49c23092cd9003247a55792211d816010c7d6204c6e07a6e017da"
        "70007b25ee2ab3665103f846300cd03512040275b78ae46812d40cd611058decdff5551e1f232103d9fd62df33"
        "2403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
        rawTx);
}

TEST(TWAnySingerOntology, Oep4Decimal) {
    auto input = Proto::SigningInput();
    input.set_contract("ff31ec74d01f7b7d45ed2add930f5d2239f7de33");
    input.set_method("decimals");
    input.set_nonce(0x1234);
    auto data = Oep4TxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000002000c108646563696d616c736733def739225d0f93dd2aed457d7b1fd074ec31ff0000", rawTx);
}

TEST(TWAnySingerOntology, Oep4BalanceOf) {
    // read only method don't need signer
    auto input = Proto::SigningInput();
    input.set_contract("ff31ec74d01f7b7d45ed2add930f5d2239f7de33");
    input.set_method("balanceOf");
    input.set_query_address("AeaThtPwh5kAYnjHavzwmvxPd725nVTvbM");
    input.set_nonce(0x1234);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeOntology);
    EXPECT_EQ("00d1341200000000000000000000000000000000000000000000000000000000000000000000000000003614fa2254ffaee3c3e1172e8e98f800e4105c74988e51c10962616c616e63654f666733def739225d0f93dd2aed457d7b1fd074ec31ff0000", hex(output.encoded()));
}

TEST(TWAnySingerOntology, Oep4Transfer) {
    // https://explorer.ont.io/testnet/tx/710266b8d497e794ecd47e01e269e4aeb6f4ff2b01eaeafc4cd371e062b13757
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto input = Proto::SigningInput();
    input.set_contract("ff31ec74d01f7b7d45ed2add930f5d2239f7de33");
    input.set_method("transfer");
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("AVY6LfvxauVQAVHDV9hC3ZCv7cQqzfDotH");
    input.set_amount(233);
    input.set_gas_price(2500);
    input.set_gas_limit(50000);
    input.set_nonce(0x1234);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeOntology);

    EXPECT_EQ("00d134120000c40900000000000050c3000000000000fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a4d02e9001496f688657b95be51c11a87b51adfda4ab69e9cbb1457e9d1a61f9aafa798b6c7fbeae35639681d7df653c1087472616e736665726733def739225d0f93dd2aed457d7b1fd074ec31ff00024140bd2923854d7b84b97a107bb3cddf18c8e3dddd2f36b41a1f5f5b23366484daa22871cfb819923fe01e9cb1e9ed16baa2b05c2feb76bcbe2ec125f72701c5e965232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac41406d638653597774ce45812ea2653250806b657b32b7c6ad3e027ddeba91e9a9da4bb5dacd23dafba868cb31bacb38b4a6ff2607682a426c1dc09b05a1e158d6cd2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac", hex(output.encoded()));
}

TEST(TWAnySingerOntology, Oep4TokenBalanceOf) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","00d1a119d4f700000000000000000000000000000000000000000000000000000000000000000000000036144a03aaf03d12fd4d46bfcc260bda73ecef33b83b51c10962616c616e63654f6667e77fb36f54874c29f503d301d91d8ab98eb2342f0000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    // {"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"40922df506","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    input.set_method("balanceOf");
    input.set_query_address("ANXE3XovCwBH1ckQnPc6vKYiTwRXyrVToD");
    input.set_nonce(4157872545);
    auto data = Oep4TxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1a119d4f700000000000000000000000000000000000000000000000000000000000000000000000036144a03aaf03d12fd4d46bfcc260bda73ecef33b83b51c10962616c616e63654f6667e77fb36f54874c29f503d301d91d8ab98eb2342f0000",
              rawTx);
}

TEST(TWAnySingerOntology, Oep4TokenDecimals) {
    // curl  -H "Content-Type: application/json"  -X POST -d '{"Action":"sendrawtransaction",
    // "Version":"1.0.0","Data":"00d1b1050fb40000000000000000000000000000000000000000000000000000000000000000000000002000c108646563696d616c7367e77fb36f54874c29f503d301d91d8ab98eb2342f0000"}'
    // http://polaris2.ont.io:20334/api/v1/transaction?preExec=1
    //
    //{"Action":"sendrawtransaction","Desc":"SUCCESS","Error":0,"Result":{"State":1,"Gas":20000,"Result":"08","Notify":[]},"Version":"1.0.0"}
    auto input = Proto::SigningInput();
    input.set_contract("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    input.set_method("decimals");
    input.set_nonce(3020883377);
    auto data = Oep4TxBuilder::build(input);
    auto rawTx = hex(data);
    EXPECT_EQ("00d1b1050fb40000000000000000000000000000000000000000000000000000000000000000000000002000c108646563696d616c7367e77fb36f54874c29f503d301d91d8ab98eb2342f0000",
              rawTx);
}

TEST(TWAnySingerOntology, Oep4TokenTransfer) {
    auto ownerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    auto payerPrivateKey =
        parse_hex("4646464646464646464646464646464646464646464646464646464646464652");
    auto input = Proto::SigningInput();
    input.set_contract("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    input.set_method("transfer");
    input.set_nonce(2232822985);
    input.set_owner_private_key(ownerPrivateKey.data(), ownerPrivateKey.size());
    input.set_payer_private_key(payerPrivateKey.data(), payerPrivateKey.size());
    input.set_to_address("ARR6PsaBwRttzCmyxCMhL7NmFk1LqExD7L");
    input.set_amount(1000);
    input.set_gas_price(2500);
    input.set_gas_limit(200);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeOntology);

    EXPECT_EQ("00d1c92c1685c409000000000000c80000000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df6"
              "4d02e8031469c329fbb30a490979ea1a6f0b6a3a91235f6bd714fbacc8214765d457c8e3f2b5a1d3c498"
              "1a2e9d2a53c1087472616e7366657267e77fb36f54874c29f503d301d91d8ab98eb2342f000241402b62"
              "b4c6bc07667019e5c9a1fa1b83ca71ee23ddb763446406b1b03706bf50a6180b13e255a08ade7da376df"
              "d34faee7f51c4f0056325fa79aaf7de0ef25d64e2321031bec1250aa8f78275f99a6663688f31085848d"
              "0ed92f1203e447125f927b7486ac41408aa88ae92ea30a9e5059de8594f462af7dfa7545fffa6654e94e"
              "edfb910bcd5452a26d1554d5d980db84d00dd330aab2fc68316660c8ae5af2c806085157e8ce232103d9"
              "fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac",
              hex(output.encoded()));
}

} // namespace TW::Ontology::tests
