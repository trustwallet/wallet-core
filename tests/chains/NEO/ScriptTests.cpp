// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEO/Script.h"
#include "NEO/Address.h"

#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOScript, Nep5TransferWithRet) {
    auto assetId = parse_hex("0d821bd7b6d53f5c2b40e217c6defc8bbe896cf5");
    std::reverse(assetId.begin(),assetId.end());
    auto from = Address("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk").toScriptHash();
    auto to = Address("AeRsDBqPiGKZhzNtL2vWhXbXGccJLCGrbJ").toScriptHash();
    auto script = Script::CreateNep5TransferScript(assetId, from, to, uint256_t(110000000), true);

    EXPECT_EQ(hex(script), "0480778e0614f88235a26e55cce0747ee827f39fd8167849672b14235a717ed7ed18a43de47499c3d05b8d4a4bcf3a53c1087472616e7366657267f56c89be8bfcdec617e2402b5c3fd5b6d71b820df166");
}

TEST(NEOScript, Nep5Transfer) {
    auto assetId = parse_hex("f46719e2d16bf50cddcef9d4bbfece901f73cbb6");
    std::reverse(assetId.begin(),assetId.end());
    auto from = Address("APqYfjvV2cCwcvFjceVcSrcouyq74qNFKS").toScriptHash();
    auto to = Address("ANeo2toNeo3MigrationAddressxwPB2Hz").toScriptHash();
    auto script = Script::CreateNep5TransferScript(assetId, from, to, uint256_t(15000000000));

    EXPECT_EQ(hex(script), "0500d6117e03144b721e06b50cc74e68b417716e3b099fb99757a8145872d3dd8741af4c8d5a94f8a1bfff5c617be01b53c1087472616e7366657267b6cb731f90cefebbd4f9cedd0cf56bd1e21967f4");
}

} // namespace TW::NEO::tests
