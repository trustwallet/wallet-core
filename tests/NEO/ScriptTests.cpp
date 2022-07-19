// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEO/Script.h"
#include "NEO/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOScript, Nep5Transfer) {
    auto assetId = parse_hex("0d821bd7b6d53f5c2b40e217c6defc8bbe896cf5");
    std::reverse(assetId.begin(),assetId.end());
    auto from = Address("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk").toScriptHash();
    auto to = Address("AeRsDBqPiGKZhzNtL2vWhXbXGccJLCGrbJ").toScriptHash();
    auto script = Script::CreateNep5TransferScript(assetId, from, to, uint256_t(110000000));

    EXPECT_EQ(hex(script), "0480778e0614f88235a26e55cce0747ee827f39fd8167849672b14235a717ed7ed18a43de47499c3d05b8d4a4bcf3a53c1087472616e7366657267f56c89be8bfcdec617e2402b5c3fd5b6d71b820df166");
}