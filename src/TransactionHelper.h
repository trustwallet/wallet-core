// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include "Data.h"

#include <string>

namespace TW {

/// Non-core transaction utility methods
class TransactionHelper {
public:
    static Data buildInput(TWCoinType coinType, const std::string& from, const std::string& to, const std::string& amount, const std::string& asset, const std::string& memo);

    /// Obtain pre-signing hash of a transaction.
    static Data preImageHash(TWCoinType coinType, const Data& txInputData);

    /// Compile a complete transation with provided signature, put together from transaction input and provided public key and signature
    static Data compileWithSignature(TWCoinType coinType, const Data& txInputData, const Data& signature, const Data& publicKey);
};

} // namespace TW
