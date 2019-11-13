// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Any/Signer.h"
#include "Coin.h"

#include <stdexcept>
#include <gtest/gtest.h>
#include "Base64.h"
#include "Tezos/BinaryCoding.h"

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
    auto transaction = R"({"operationList": {"branch": "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp","operations": [{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30738,"gasLimit": 10100,"storageLimit": 257,"kind": 107,"revealOperationData": {"publicKey": "QpqYbIBypAofOj4qtaWBm7Gy+2mZPFAEg3gVudxVkj4="}},{"source": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","fee": 1272,"counter": 30739,"gasLimit": 10100,"storageLimit": 257,"kind": 108,"transactionOperationData": {"destination": "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW","amount": 1}}]}})";
    auto input = Proto::SigningInput();

    input.set_private_key("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeTezos);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b957210001b86398d5b9be737dca8e4106ea18d70e69b75e92f892fb283546a99152b8d7794b919c0fbf1c31de386069a60014491c0e7505adef5781cead1cfe6608030b",
              output.output());
}

TEST(Signer, IoTeXTransactionSign) {
    auto transaction = R"({"version": 1,"nonce": 123,"gasLimit": 888,"gasPrice": "999","transfer": {"amount": "456","recipient": "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j","payload": "aGVsbG8gd29ybGQh"}})";
    auto input = Proto::SigningInput();
    input.set_private_key("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeIoTeX);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("0a4c0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e723937716c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c64211241044e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34fbeb71270d4bad3d648d91a41555cc8af4181bf85c044c3201462eeeb95374f78aa48c67b87510ee63d5e502372e53082f03e9a11c1e351de539cedf85d8dff87de9d003cb9f92243541541a000",
              output.output());
}

TEST(Signer, WanchainTransactionSign) {
    auto transaction = R"({"chainId": "MQ==","toAddress": "0x3535353535353535353535353535353535353535","nonce": "OQ==","gasPrice": "MjAwMDAwMDAwMDA=","gasLimit": "MjEwMDA=","amount": "MTAwMDAwMDAwMDAwMDAwMDAwMA=="})";
    auto input = Proto::SigningInput();
    input.set_private_key("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeWanchain);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("f88201398b32303030303030303030308532313030309435353535353535353535353535353535353535359331303030303030303030303030303030303030808185a0f7bca5c00884bcba5c068f507a33559df775e09785fc55956c9bbe4276259a4ca06679fc4f853b7f224ac8096d15005a3d1070c81a0a13aee8e60a72654273fe5e",
              output.output());
}

TEST(Signer, WavesTransactionSign) {
    
    auto transaction = R"({"timestamp": 1526641218066, "transferMessage": {"amount": 100000000,"asset": "WAVES","fee": 100000000,"fee_asset": "WAVES","to": "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds","attachment": "ZmFsYWZlbA=="} })";
    auto input = Proto::SigningInput();
    input.set_private_key("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeWaves);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ(R"({"amount":100000000,"attachment":"4t2Xazb2SX","fee":100000000,"proofs":["4WUvsbgA2EsyCd5C7jHS57SgvPNczXYnxyKSvAzm18vtRhvbjjH9Dq2vUDgqm8h4GcDzzueQnHg3WH4xTyxDJ1Hg"],"recipient":"3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds","senderPublicKey":"6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU","timestamp":1526641218066,"type":4,"version":2})",
              output.output());
}

TEST(Signer, NebulasTransactionSign) {
    auto transaction = R"({"fromAddress": "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY","chainId": "MQ==","nonce": "Nw==","gasPrice": "MTAwMDAwMA==","gasLimit": "MjAwMDAw","toAddress": "n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17","amount": "MTEwMDAwMDAwMDAwMDAwMDAwMDA=","timestamp": "MTU2MDA1MjkzOA==","payload": ""})";
    auto input = Proto::SigningInput();
    input.set_private_key("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeNebulas);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("17aca84f1c31f2065123ff14d53f10e2c2a8cd6e99f75fb9b11a2bc7bacb14e05c29f2267c700f9f1c5b9df26bddc5972993c71be651c5c7872683e23cdd80c301",
              output.output());
}

TEST(Signer, TronTransactionSign) {
    auto transaction = R"({"transaction": {"timestamp": 1539295479000,"expiration": 1539331479000,"blockHeader": {"timestamp": 1539295479000,"txTrieRoot": "ZCiMLbBkExZ2KpnbsC73yQ+Wi2D58uQQg1mAYUMy+G0=","parentHash": "AAAAAAAvezr09fi54jowxTD3GfFlt0LnNYU2soDurS0=","number": 3111739,"witnessAddress": "QVhj9gkbjnF2bagIsd0xWXkPYd59","version": 3},"transfer": {"ownerAddress": "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC","toAddress": "THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV","amount": 2000000}}})";
    auto input = Proto::SigningInput();
    input.set_private_key("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeTron);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ(R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TransferContract","value":{"amount":2000000,"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","to_address":"41521ea197907927725ef36d70f25f850d1659c7c7"}},"type":"TransferContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["ede769f6df28aefe6a846be169958c155e23e7e5c9621d2e8dce1719b4d952b63e8a8bf9f00e41204ac1bf69b1a663dacdf764367e48e4a5afcd6b055a747fb200"],"txID":"dc6f6d9325ee44ab3c00528472be16e1572ab076aa161ccd12515029869d0451"})",
              output.output());
}

TEST(Signer, VeChhainTransactionSign) {
    auto transaction = R"({"chainTag": 1,"blockRef": 1,"expiration": 1,"clauses": [{"to": "0x3535353535353535353535353535353535353535","value": "MTAwMA==","data": ""}],"gasPriceCoef": 0,"gas": 21000,"dependsOn": "","nonce": 1})";
    auto input = Proto::SigningInput();
    input.set_private_key("0x4646464646464646464646464646464646464646464646464646464646464646");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeVeChain);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());;
    ASSERT_EQ("f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b841bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601",
              output.output());
}

TEST(Signer, HarmonyTransactionSign) {
    auto transaction =
        R"({"chainId": "AQ==","transactionMessage": {"nonce": "Cg==","gasPrice": "","gasLimit": "Ugg=","toAddress": "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc","amount": "TFPs3BimAAA=","payload": "","fromShardId": "AQ==","toShardId": "AA=="}})";
    auto input = Proto::SigningInput();
    input.set_private_key("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e");
    input.set_transaction(transaction);
    input.set_coin_type(TWCoinTypeHarmony);

    auto signer = Signer(input);
    auto output = signer.sign();

    ASSERT_TRUE(output.success());
    ;
    ASSERT_EQ("f8690a808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008026a0"
              "74acbc63a58e7861e54ca24babf1cb800c5b694da25c3ae2b154304505366708a0616ab8262ee6f6fb30"
              "ffcab3e9e8261479c7469ce97010a70b3d3f962842c61a",
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
