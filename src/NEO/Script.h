// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
