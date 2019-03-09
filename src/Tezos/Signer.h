// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OperationList.h"
#include "../Data.h"
#include "../PrivateKey.h"

#include <string>

namespace TW {
namespace Tezos {

/// Helper class that performs Tezos transaction signing.
class Signer {
public:
    /// Signs the given transaction.
    std::string signOperationList(const PrivateKey& privateKey, OperationList operationList);
    std::string signHexString(const PrivateKey& privateKey, std::string forgedBytesHex);
};

}} // namespace

/// Wrapper for C interface.
struct TWTezosSigner {
    TW::Tezos::Signer impl;
};
