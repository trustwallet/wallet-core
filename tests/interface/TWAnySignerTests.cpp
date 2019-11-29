// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include "proto/Any.pb.h"
#include "Base64.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace std;

TEST(TWAnySigner, IsEnabled) {
    ASSERT_TRUE(TWAnySignerIsSignEnabled(TWCoinTypeEthereum));
    ASSERT_TRUE(TWAnySignerIsSignEnabled(TWCoinTypeBinance));

    ASSERT_FALSE(TWAnySignerIsSignEnabled(TWCoinTypeBitcoinCash));
    ASSERT_FALSE(TWAnySignerIsSignEnabled(TWCoinTypeBitcoin));
}

bool performAnySign(TWCoinType coinType, const string& transaction, const string& privateKeyHex, TW::Any::Proto::SigningOutput& out) {
    // prepare SigningInput
    TW::Any::Proto::SigningInput signingInput;
    signingInput.set_coin_type((uint32_t)coinType);
    signingInput.set_transaction(transaction);
    signingInput.set_private_key(privateKeyHex);
    string siSerialized;
    if (!signingInput.SerializeToString(&siSerialized)) return false;

    // invoke serialization
    TW_Any_Proto_SigningOutput signerOutput = TWAnySignerSign(TWDataCreateWithBytes((const uint8_t*)siSerialized.c_str(), siSerialized.size()));

    // copy to byte string
    string outStr;
    outStr.insert(outStr.end(), TWDataBytes(signerOutput), TWDataBytes(signerOutput) + TWDataSize(signerOutput));
    // parse protobuf message
    TW::Any::Proto::SigningOutput signingOutput;
    if(!signingOutput.ParseFromString(outStr)) return false;
    out = signingOutput;
    return true;
}

TEST(TWAnySigner, SignEthereum) {
    TWCoinType coinType = TWCoinTypeEthereum;
    const string privateKeyHex = "17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543";
    // preparing transaction
    string transaction = R"({"chainId":"AQ==","gasPrice":"1pOkAA==","gasLimit":"Ugg=","toAddress":"0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1","amount":"A0i8paFgAA=="})";

    TW::Any::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(performAnySign(coinType, transaction, privateKeyHex, signingOutput));

    ASSERT_TRUE(signingOutput.success());
    ASSERT_FALSE(signingOutput.has_error());
    // OK, get output
    EXPECT_EQ(
        "f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10",
        signingOutput.output());
}

TEST(TWAnySigner, SignBinance) {
    TWCoinType coinType = TWCoinTypeBinance;
    const string privateKeyHex = "f947b3554a1c2fa70e1caa9de53fbda353348d1e856c593848f3a29737d31f11";
    // preparing transaction
    string transaction = R"({"chainId":"Binance-Chain-Tigris","accountNumber":"13186","source":"2","memo":"Testing","sendOrder":{"inputs":[{"address":"EuZU7e+eUIuDNzaph9Bp2lqJrts=","coins":[{"denom":"BNB","amount":"1345227"}]}],"outputs":[{"address":"M7vzB7mBRvE9IGk8+UbC13pMryg=","coins":[{"denom":"BNB","amount":"1345227"}]}]}})";

    TW::Any::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(performAnySign(coinType, transaction, privateKeyHex, signingOutput));

    ASSERT_TRUE(signingOutput.success());
    // OK, get output
    EXPECT_EQ(
        "ca01f0625dee0a4a2a2c87fa0a210a1412e654edef9e508b833736a987d069da5a89aedb12090a03424e4210cb8d5212210a1433bbf307b98146f13d20693cf946c2d77a4caf2812090a03424e4210cb8d52126d0a26eb5ae9872102e58176f271a9796b4288908be85094a2ac978e25535fd59a37b58626e3a84d9e1240015b4beb3d6ef366a7a92fd283f66b8f0d8cdb6b152a9189146b27f84f507f047e248517cf691a36ebc2b7f3b7f64e27585ce1c40f1928d119c31af428efcf3e1882671a0754657374696e672002",
        signingOutput.output());
}

TEST(TWAnySigner, SignNano) {
    TWCoinType coinType = TWCoinTypeNano;
    const string privateKeyHex = "173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34";
    // preparing transaction
    string transaction = R"({"link_block":")" +
        TW::Base64::encode(parse_hex("491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507")) +
        R"(","representative":"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4","balance":"96242336390000000000000000000"})";

    TW::Any::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(performAnySign(coinType, transaction, privateKeyHex, signingOutput));

    ASSERT_TRUE(signingOutput.success());
    // OK, get output
    EXPECT_EQ(
        "d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09",
        signingOutput.output());
}
