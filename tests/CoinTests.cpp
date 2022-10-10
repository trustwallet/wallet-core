// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>

#include "Coin.h"
#include <nlohmann/json.hpp>

namespace TW::Coin::tests {

TEST(Coin, RuntimeRegistryFromJson) {
    nlohmann::json j = R"([
                        {
                            "id": "juno",
                            "name": "Juno",
                            "displayName": "Juno",
                            "coinId": 20000118,
                            "symbol": "JUNO",
                            "decimals": 6,
                            "blockchain": "Cosmos",
                            "derivation": [
                              {
                                "path": "m/44'/118'/0'/0/0"
                              }
                            ],
                            "curve": "secp256k1",
                            "publicKeyType": "secp256k1",
                            "hrp": "juno",
                            "chainId": "juno-1",
                            "addressHasher": "sha256ripemd",
                            "explorer": {
                              "url": "https://mintscan.io/juno",
                              "txPath": "/txs/",
                              "accountPath": "/account/",
                              "sampleTx": "91A78E04C7CB2CA4E9FA884EB3A3BE151E85EC977CE8B0FF0C8565BAA2FD518A",
                              "sampleAccount": "juno1h4zffvsfnydkptg2mgpm0hquxv2p9zjyxl2pxd"
                            },
                            "info": {
                              "url": "https://www.junonetwork.io/",
                              "client": "https://github.com/CosmosContracts/juno",
                              "clientPublic": "https://juno-rpc.polkachu.com/",
                              "clientDocs": ""
                            }
                          }
                        ])"_json;
    jsonToCoinRegistry(j);
    CoinInfo junoCfg = gRuntimeCoinInfoRegistry.at("juno");
    ASSERT_EQ(std::string(junoCfg.id), "juno");
    ASSERT_EQ(std::string(junoCfg.chainId), "juno-1");
    ASSERT_EQ(std::string(junoCfg.symbol), "JUNO");
    ASSERT_EQ(std::string(junoCfg.name), "Juno");
    ASSERT_EQ(junoCfg.slip44, 20000118u);
    ASSERT_EQ(junoCfg.blockchain, TWBlockchainCosmos);
    ASSERT_EQ(junoCfg.curve, TWCurveSECP256k1);
    ASSERT_EQ(junoCfg.decimals, 6);
    ASSERT_EQ(std::string(junoCfg.hrpStr), "juno");
    ASSERT_EQ(junoCfg.hrp, TWHRPUnknown);
}

} // namespace TW::Coin::tests
