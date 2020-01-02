// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OperationList.h"
#include "../Data.h"
#include "../PrivateKey.h"

#include <string>

namespace TW::Tezos {

/// Helper class that performs Tezos transaction signing.
class Signer {
  public:
    /// Signs the given transaction.
    Data signOperationList(const PrivateKey& privateKey, const OperationList& operationList);
    Data signData(const PrivateKey& privateKey, Data data);
};

} // namespace TW::Tezos

/// Wrapper for C interface.
struct TWTezosSigner {
    TW::Tezos::Signer impl;
};
