// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
#pragma once

#include "Data.h"
#include "uint256.h"

namespace TW::NEO {

class Script {
public:
    static Data CreateSignatureRedeemScript(const Data& publicKey);
    static Data CreateInvocationScript(const Data& signature);
    // nep5 assetId has only 20 bytes, different with gas & neo that are 32 bytes.
    static Data CreateNep5TransferScript(const Data& assetId, const Data& from, const Data& to, uint256_t value, bool withRet = false);
};
} // namespace TW::NEO
