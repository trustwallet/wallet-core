// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Any/Signer.h"
#include "Coin.h"

#include <stdexcept>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Any;

TEST(Signer, CosmosTransactionSign) {
    auto transaction = R"({"accountNumber":"8733","chainId":"cosmoshub-2","fee":{"amounts":[{"denom":"uatom","amount":"5000"}],"gas":"200000"},"memo":"Testing","sendCoinsMessage":{"fromAddress":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","toAddress":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0","amounts":[{"denom":"uatom","amount":"995000"}]}})";
    auto input = Proto::SigningInput();
    input.set_private_key("c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeCosmos);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());
    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"5000\",\"denom\":\"uatom\"}],\"gas\":\"200000\"},\"memo\":\"Testing\",\"msg\":[{\"type\":\"cosmos-sdk/MsgSend\",\"value\":{\"amount\":[{\"amount\":\"995000\",\"denom\":\"uatom\"}],\"from_address\":\"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575\",\"to_address\":\"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm\"},\"signature\":\"ULEpUqNzoAnYEx2x22F3ANAiPXquAU9+mqLWoAA/ZOUGTMsdb6vryzsW6AKX2Kqj1pGNdrTcQ58Z09JPyjpgEA==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}",
            output.output());
}

TEST(Signer, BinanceTransactionSign) {
    auto transaction = R"({"chainId":"Binance-Chain-Tigris","accountNumber":"13186","source":"2","memo":"Testing","sendOrder":{"inputs":[{"address":"EuZU7e+eUIuDNzaph9Bp2lqJrts=","coins":[{"denom":"BNB","amount":"1345227"}]}],"outputs":[{"address":"M7vzB7mBRvE9IGk8+UbC13pMryg=","coins":[{"denom":"BNB","amount":"1345227"}]}]}})";
    auto input = Proto::SigningInput();
    input.set_private_key("f947b3554a1c2fa70e1caa9de53fbda353348d1e856c593848f3a29737d31f11");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeBinance);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());
    ASSERT_EQ("ca01f0625dee0a4a2a2c87fa0a210a1412e654edef9e508b833736a987d069da5a89aedb12090a03424e4210cb8d5212210a1433bbf307b98146f13d20693cf946c2d77a4caf2812090a03424e4210cb8d52126d0a26eb5ae9872102e58176f271a9796b4288908be85094a2ac978e25535fd59a37b58626e3a84d9e1240015b4beb3d6ef366a7a92fd283f66b8f0d8cdb6b152a9189146b27f84f507f047e248517cf691a36ebc2b7f3b7f64e27585ce1c40f1928d119c31af428efcf3e1882671a0754657374696e672002",
            output.output());
}

TEST(Signer, EthereumTransactionSign) {
    auto transaction = R"({"chainId":"AQ==","gasPrice":"1pOkAA==","gasLimit":"Ugg=","toAddress":"0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1","amount":"A0i8paFgAA=="})";
    auto input = Proto::SigningInput();
    input.set_private_key("17209af590a86462395d5881e60d11c7fa7d482cfb02b5a01b93c2eeef243543");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeEthereum);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("f86a8084d693a400825208947d8bf18c7ce84b3e175b339c4ca93aed1dd166f1870348bca5a160008025a0fe5802b49e04c6b1705088310e133605ed8b549811a18968ad409ea02ad79f21a05bf845646fb1e1b9365f63a7fd5eb5e984094e3ed35c3bed7361aebbcbf41f10",
              output.output());
}

TEST(Signer, TezosTransactionSign) {
    auto transaction = R"({"operationList": {"branch": "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp","operations": [{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30738,"gasLimit": 10100,"storageLimit": 257,"kind": 7,"revealOperationData": {"publicKey": "QpqYbIBypAofOj4qtaWBm7Gy+2mZPFAEg3gVudxVkj4="}},{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30739,"gasLimit": 10100,"storageLimit": 257,"kind": 8,"transactionOperationData": {"destination": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","amount": 1}}]}})";
    auto input = Proto::SigningInput();
    input.set_private_key("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeTezos);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0107000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b9572100d924cb3e56c4b9f55e50735e461899a2f616a26bfb0aa05d0b356b66f517b023df330ad3621f0bf39d518131a1becd6a7b2e226ed291483af3682535d1f4530f",
              output.output());
}

TEST(Signer, NetworkNotSupported) {
    auto transaction = R"({"accountNumber":"8733","chainId":"cosmoshub-2","fee":{"amounts":[{"denom":"uatom","amount":"5000"}],"gas":"200000"},"memo":"Testing","sendCoinsMessage":{"fromAddress":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","toAddress":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0","amounts":[{"denom":"uatom","amount":"995000"}]}})";
    auto input = Proto::SigningInput();
    input.set_private_key("c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeBitcoinCash);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_FALSE(output.success());
    ASSERT_EQ(SignerErrorCodeNotSupported, output.error().code());
    ASSERT_EQ("Network not supported", output.error().description());
}

TEST(Signer, InvalidJsonFormat) {
    auto transaction = R"({"accountNumber":"8733""chainId":"cosmoshub-2"})";
    auto input = Proto::SigningInput();
    input.set_private_key("c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeCosmos);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_FALSE(output.success());
    ASSERT_EQ(SignerErrorCodeInvalidJson, output.error().code());
}
