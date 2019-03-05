// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OperationList.h"
#include <string>

#include "proto/Tezos.pb.h"

namespace TW {
namespace Tezos {

/// Helper class that performs Tezos transaction signing.
class Signer {
public:
    /// Signs the given transaction.
    std::string signOperation(OperationList);
};

}} // namespace

/// Wrapper for C interface.
struct TWTezosSigner {
    TW::Tezos::Signer impl;
};
